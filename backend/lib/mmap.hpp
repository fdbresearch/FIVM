//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_MMAP_HPP
#define DBTOASTER_MMAP_HPP

#include <iostream>
#include <functional>
#include <string>

#include "types.hpp"
#include "macro.hpp"
#include "hash.hpp"
#include "pool.hpp"
#include "serialization.hpp"

namespace dbtoaster {

#define DEFAULT_CHUNK_SIZE 32   // 2^N

template <typename T>
class Singleton {
 private:
  static T* instance_;
  static size_t counter_;

 public:
  T* acquire() {
    ++counter_;
    if (instance_ == nullptr) { 
      instance_ = new T(); 
    }
    return instance_;
  }

  void release(T* obj) {
    if (instance_ == obj && --counter_ == 0) {
      delete instance_;
      instance_ = nullptr;
    }
  }
};

template <typename T>
T* Singleton<T>::instance_ = nullptr;

template <typename T>
size_t Singleton<T>::counter_ = 0;

template <typename T>
struct PrimaryIdxNode {
  T *obj;
  HASH_RES_t hash;
  struct PrimaryIdxNode *next;
};

template <typename T>
struct LinkedNode {
  T *obj;
  struct LinkedNode *next;
};

template <typename T>
struct SecondaryIdxNode {
  LinkedNode<T> node;
  HASH_RES_t hash;
  struct SecondaryIdxNode *next;
};

template <typename T, typename IDX_FN = T>
class PrimaryHashIndex { 
 private:
  typedef PrimaryIdxNode<T> IdxNode;
    
  MemoryPool<IdxNode>* pool_;

  IdxNode *buckets_;
  size_t size_;
  size_t count_;  
  size_t index_mask_;   // derived value
  size_t threshold_;    // derived value
  double load_factor_;
  
  void resize_(size_t new_size) {
    IdxNode* old_buckets = buckets_;
    size_t old_size = size_;

    int r = posix_memalign((void **) &buckets_, 32, new_size * sizeof(IdxNode));
    assert(r == 0);
    memset(buckets_, 0, sizeof(IdxNode) * new_size);
    size_ = new_size;
    index_mask_ = size_ - 1;
    threshold_ = size_ * load_factor_;

    for (size_t i = 0; i < old_size; i++) {
      IdxNode* src = old_buckets + i;

      if (src->obj != nullptr) {
        IdxNode* dst = buckets_ + (src->hash & index_mask_);

        if (dst->obj != nullptr) {
          IdxNode* new_node = pool_->allocate();
          new_node->obj = src->obj;
          new_node->hash = src->hash;
          new_node->next = dst->next;
          dst->next = new_node;
        }
        else {
          dst->obj = src->obj;
          dst->hash = src->hash;
          assert(dst->next == nullptr);
        }

        src = src->next;
        while (src != nullptr) {
          IdxNode* dst = buckets_ + (src->hash & index_mask_);
          IdxNode* nxt = src->next;

          if (dst->obj != nullptr) {
            src->next = dst->next;
            dst->next = src;
          }
          else {
            dst->obj = src->obj;
            dst->hash = src->hash;
            assert(dst->next == nullptr);
            pool_->deallocate(src);
          }
          src = nxt;
        }
      }
    }

    if (old_buckets != nullptr) delete[] old_buckets;
  }

 public:

  PrimaryHashIndex(size_t size = DEFAULT_CHUNK_SIZE, double load_factor = 0.75) 
      : pool_(Singleton<MemoryPool<IdxNode>>().acquire()),
        buckets_(nullptr),
        size_(0), 
        count_(0), 
        index_mask_(0), 
        threshold_(0), 
        load_factor_(load_factor) {
    resize_(size);
  }

  ~PrimaryHashIndex() {
    clear();    
    if (buckets_ != nullptr) {
        delete[] buckets_;
        buckets_ = nullptr;
    }
    Singleton<MemoryPool<IdxNode>>().release(pool_);
    pool_ = nullptr;
  }

  void clear() {
    if (count_ == 0) return;

    for (size_t i = 0; i < size_; i++) {
      buckets_[i].obj = nullptr;
      buckets_[i].hash = 0;
      // delete chain
      IdxNode *curr = buckets_[i].next;
      while (curr != nullptr) {
        IdxNode *nxt = curr->next;
        pool_->deallocate(curr);
        curr = nxt;
      }
      buckets_[i].next = nullptr;
    }
    count_ = 0;
  }


  inline size_t count() const { 
    return count_; 
  }

  inline size_t size() const { 
    return count_; 
  }

  inline HASH_RES_t computeHash(const T& key) { 
    return IDX_FN::hash(key); 
  }

  inline T* get(const T& key) const { 
    return get(key, IDX_FN::hash(key)); 
  }

  // Returns the first matching element or null if not found
  inline T* get(const T& key, const HASH_RES_t h) const {
    IdxNode* n = buckets_ + (h & index_mask_);
    do {
      if (n->obj && h == n->hash && IDX_FN::equals(key, *n->obj)) {
        return n->obj;
      }
    } while ((n = n->next));
    return nullptr;
  }

  inline void insert(T *obj) {
    if (obj != nullptr) insert(obj, IDX_FN::hash(*obj));
  }

  // Inserts regardless of whether element already exists
  inline void insert(T* obj, const HASH_RES_t h) {
    assert(obj != nullptr);

    if (count_ > threshold_) { resize_(size_ << 1); }

    IdxNode* dst = buckets_ + (h & index_mask_);
    if (dst->obj != nullptr) {
      IdxNode* new_node = pool_->allocate();
      new_node->obj = obj;
      new_node->hash = h;
      new_node->next = dst->next;
      dst->next = new_node;
    }
    else {
      dst->obj = obj;
      dst->hash = h;
      assert(dst->next == nullptr);
    }
    count_++;
  }

  void erase(const T* obj) {
    if (obj != nullptr) erase(obj, IDX_FN::hash(*obj));
  }

  // Deletes an existing elements (equality by pointer comparison)
  void erase(const T* obj, const HASH_RES_t h) {
    assert(obj != nullptr);

    IdxNode *dst = buckets_ + (h & index_mask_);
    IdxNode *prv = nullptr, *nxt = nullptr;

    do {
      nxt = dst->next;
      // pointer comparison is sufficient as all objects are stored in pool
      if (dst->obj == obj) {
        if (prv != nullptr) {  // element in linked list (and not bucket)
          prv->next = nxt;
          pool_->deallocate(dst);
        }
        else if (nxt != nullptr) {  // element in bucket, and there are other elements in linked list
          dst->obj = nxt->obj;
          dst->hash = nxt->hash;
          dst->next = nxt->next;
          pool_->deallocate(nxt);
        } 
        else {   // only element in bucket
          dst->obj = nullptr;
          dst->hash = 0;
          dst->next = nullptr;
        }
        count_--;
        return;
      }
      prv = dst;
    } while ((dst = nxt));
  }

  template <typename U, typename V, typename PRIMARY_INDEX, typename... SECONDARY_INDEXES> 
  friend class MultiHashMap;
};

template <typename T> 
class SecondaryIndex {
 public:
  virtual const LinkedNode<T>* slice(const T& key) const = 0;

  virtual void insert(T* obj) = 0;

  virtual void erase(const T* obj) = 0;

  virtual void clear() = 0;

  virtual ~SecondaryIndex() { }
};


template <typename T, typename IDX_FN = T>
class SecondaryHashIndex : public SecondaryIndex<T> {
 private:
  typedef SecondaryIdxNode<T> IdxNode;
  typedef LinkedNode<T> Node;

  MemoryPool<IdxNode>* idxNodePool_;
  MemoryPool<Node>* nodePool_;

  IdxNode* buckets_;
  size_t size_;
  size_t count_;
  size_t index_mask_;   // derived value
  size_t threshold_;    // derived value
  double load_factor_;
  
  void resize_(size_t new_size) {
    IdxNode* old_buckets = buckets_;
    size_t old_size = size_;

    int r = posix_memalign((void **) &buckets_, 32, new_size * sizeof(IdxNode));
    assert(r == 0);
    memset(buckets_, 0, sizeof(IdxNode) * new_size);
    size_ = new_size;
    index_mask_ = size_ - 1;
    threshold_ = size_ * load_factor_;

    for (size_t i = 0; i < old_size; i++) {
      IdxNode* src = old_buckets + i;

      if (src->node.obj != nullptr) {
        IdxNode* dst = buckets_ + (src->hash & index_mask_);
                
        if (dst->node.obj != nullptr) {
          IdxNode* new_node = idxNodePool_->allocate();
          new_node->node = src->node;
          new_node->hash = src->hash;
          new_node->next = dst->next;
          dst->next = new_node;
        }
        else {
          dst->node = src->node;
          dst->hash = src->hash;
          assert(dst->next == nullptr);
        }

        src = src->next;
        while (src != nullptr) {
          IdxNode* dst = buckets_ + (src->hash & index_mask_);
          IdxNode* nxt = src->next;

          if (dst->node.obj != nullptr) {
            src->next = dst->next;
            dst->next = src;
          }
          else {
            dst->node = src->node;
            dst->hash = src->hash;
            assert(dst->next == nullptr);
            idxNodePool_->deallocate(src);
          }
          src = nxt;
        }
      }
    }

    if (old_buckets != nullptr) delete[] old_buckets;
  }    

 public:
  SecondaryHashIndex(size_t size = DEFAULT_CHUNK_SIZE, double load_factor = 0.75) 
      : idxNodePool_(Singleton<MemoryPool<IdxNode>>().acquire()),
        nodePool_(Singleton<MemoryPool<Node>>().acquire()),
        buckets_(nullptr), 
        size_(0), 
        count_(0), 
        index_mask_(0), 
        threshold_(0), 
        load_factor_(load_factor) {    
    resize_(size);
  }

  ~SecondaryHashIndex() {
    clear();
    if (buckets_ != nullptr) {
      delete[] buckets_;
      buckets_ = nullptr;
    }
    Singleton<MemoryPool<IdxNode>>().release(idxNodePool_);
    idxNodePool_ = nullptr;
    Singleton<MemoryPool<Node>>().release(nodePool_);
    nodePool_ = nullptr;
  }

  void clear() {
    if (count_ == 0) return;

    for (size_t i = 0; i < size_; i++) {
      IdxNode *curr = buckets_ + i;
      curr->node.obj = nullptr;
      delete_chain(curr->node.next);    // delete slice elements
      curr->node.next = nullptr;
      curr->hash = 0;

      IdxNode *nxt = curr->next;
      curr->next = nullptr;
      curr = nxt;
      while (curr != nullptr) {
        IdxNode *nxt = curr->next;
        delete_chain(curr->node.next);    // delete slice elements
        idxNodePool_->deallocate(curr);        
        curr = nxt;
      }
    }
    count_ = 0;
  }

  void delete_chain(Node* curr) {
    while (curr != nullptr) {
      Node* next = curr->next;
      nodePool_->deallocate(curr);
      curr = next;
    }
  }

  inline size_t count() const {
    return count_;
  }    

  inline const Node* slice(const T& key) const {
    return slice(key, IDX_FN::hash(key));
  }

  // Returns the first matching node or nullptr if not found
  inline Node* slice(const T& key, const HASH_RES_t h) const {
    IdxNode* n = buckets_ + (h & index_mask_);
    do {
      if (n->node.obj && h == n->hash && IDX_FN::equals(key, *n->node.obj)) {
        return &(n->node);
      }
    } while ((n = n->next));
    return nullptr;
  }

  inline void insert(T* obj) {
    if (obj != nullptr) insert(obj, IDX_FN::hash(*obj));
  }

  // Inserts regardless of whether element already exists
  inline void insert(T* obj, const HASH_RES_t h) {
    assert(obj != nullptr);

    if (count_ > threshold_) resize_(size_ << 1);

    IdxNode* dst = buckets_ + (h & index_mask_);
    
    if (dst->node.obj == nullptr) {
      dst->node.obj = obj;
      assert(dst->node.next == nullptr);
      dst->hash = h;
      assert(dst->next == nullptr);
      count_++;
    }
    else {
      Node* idx_node = slice(*obj, h);
      if (idx_node != nullptr) {
        Node* new_node = nodePool_->allocate();
        new_node->obj = obj;
        new_node->next = idx_node->next;
        idx_node->next = new_node;
      }
      else {
        IdxNode* new_node = idxNodePool_->allocate();
        new_node->node.obj = obj;
        new_node->node.next = nullptr;
        new_node->hash = h;
        new_node->next = dst->next;
        dst->next = new_node;
        count_++;       // count only distinct elements for non-unique index
      }
    }
  }

  void erase(const T* obj) {
    if (obj != nullptr) erase(obj, IDX_FN::hash(*obj));
  }

  // Deletes an existing elements (equality by pointer comparison)
  void erase(const T* obj, const HASH_RES_t h) {
    assert(obj != nullptr);

    IdxNode *dst = buckets_ + (h & index_mask_);
    IdxNode *prv = nullptr, *nxt = nullptr;

    do {
        nxt = dst->next;
            
        if (dst->node.obj && h == dst->hash && IDX_FN::equals(*obj, *dst->node.obj)) {
          
          // pointer comparison is sufficient as all objects are stored in pool
          if (dst->node.obj == obj) {

            // delete the first element of slice
            Node* child = dst->node.next;
            if (child != nullptr) {
              dst->node.obj = child->obj;
              dst->node.next = child->next;
              // dst->hash remains unchanged
              // dst->nxt remains unchanged
              nodePool_->deallocate(child);
            }
            else {
              if (prv != nullptr) {  // element in linked list (and not in bucket itself)
                prv->next = nxt;
                idxNodePool_->deallocate(dst);
              }
              else if (nxt != nullptr) {  // element in bucket, and there are other elements in linked list
                dst->node.obj = nxt->node.obj;
                dst->node.next = nxt->node.next;
                dst->hash = nxt->hash;
                dst->next = nxt->next;
                idxNodePool_->deallocate(nxt);
              }
              else {   // the only element in bucket
                dst->node.obj = nullptr;
                dst->node.next = nullptr;
                dst->hash = 0;
                dst->next = nullptr;
              }
              count_--;
            }
          }
          else {
            // look for a matching child of slice
            Node* prev = &dst->node;
            Node* curr = dst->node.next;
            while (curr != nullptr) {
              if (curr->obj == obj) {
                prev->next = curr->next;
                nodePool_->deallocate(curr);
                return;
              }
              prev = curr;
              curr = curr->next;
            }
          }
          return;
        }
        prv = dst;
    } while ((dst = nxt));
  }

  template <typename U, typename V, typename PRIMARY_INDEX, typename... SECONDARY_INDEXES> 
  friend class MultiHashMap;  
};

template <typename T, typename V, typename PRIMARY_INDEX, typename... SECONDARY_INDEXES>
class MultiHashMap {
 public:
  MemoryPool<T>* pool;
  PRIMARY_INDEX* primary_index;
  SecondaryIndex<T>** secondary_indexes;

  inline void insert(const T& elem, HASH_RES_t h) { // the element is not in the map
    T* curr = pool->newElement(elem);
    curr->prv = nullptr;
    curr->nxt = head;
    if (head != nullptr) { head->prv = curr; }
    head = curr;

    primary_index->insert(curr, h);
    for (size_t i = 0; i < sizeof...(SECONDARY_INDEXES); i++)
      secondary_indexes[i]->insert(curr);
  }

  void erase(T* elem, HASH_RES_t h) { // the element is already in the map
    // assume the element is already in the map and mainIdx=0
    assert(elem != nullptr);

    T* elemPrv = elem->prv;
    T* elemNxt = elem->nxt;

    if (elemPrv != nullptr) { elemPrv->nxt = elemNxt; }
    if (elemNxt != nullptr) { elemNxt->prv = elemPrv; }
    if (elem == head) { head = elemNxt; }
    elem->nxt = nullptr;
    elem->prv = nullptr;

    primary_index->erase(elem, h);
    for (size_t i = 0; i < sizeof...(SECONDARY_INDEXES); i++)
      secondary_indexes[i]->erase(elem);

    pool->deleteElement(elem);
  }

 public:
  T* head;

  explicit MultiHashMap() : head(nullptr) { 
    pool = Singleton<MemoryPool<T>>().acquire();
    primary_index = new PRIMARY_INDEX();
    secondary_indexes = new SecondaryIndex<T>*[sizeof...(SECONDARY_INDEXES)] { 
      new SECONDARY_INDEXES()...
    };
  }

  explicit MultiHashMap(size_t init_capacity) : head(nullptr) {
    pool = Singleton<MemoryPool<T>>().acquire();
    primary_index = new PRIMARY_INDEX(init_capacity);
    secondary_indexes = new SecondaryIndex<T>*[sizeof...(SECONDARY_INDEXES)] { 
      new SECONDARY_INDEXES(init_capacity)...
    };
  }

  MultiHashMap(const MultiHashMap &) = delete;

  ~MultiHashMap() {
    clear();

    if (primary_index != nullptr) {
      delete primary_index;
      primary_index = nullptr;
    }
    for (size_t i = 0; i < sizeof...(SECONDARY_INDEXES); i++) {
      if (secondary_indexes[i] != nullptr) {
        delete secondary_indexes[i];
      }            
    }
    delete[] secondary_indexes;
    secondary_indexes = nullptr;

    Singleton<MemoryPool<T>>().release(pool);
    pool = nullptr;
  }

  void clear() {
    if (primary_index->count() == 0) return;

    primary_index->clear();
    for (size_t i = 0; i < sizeof...(SECONDARY_INDEXES); i++)
      secondary_indexes[i]->clear();
        
    // delete all elements
    T *curr = head;
    while (curr != nullptr) {
      T *next = curr->nxt;
      pool->deleteElement(curr);
      curr = next;
    }
    head = nullptr;
  }

  MultiHashMap& operator=(const MultiHashMap &other) {
      if (this != &other) {
          clear();
          T *tmp = other.head;
          while (tmp) {
              insert(*tmp);
              tmp = tmp->nxt;
          }
      }
      return *this;
  }

  FORCE_INLINE MultiHashMap<T, V, PRIMARY_INDEX, SECONDARY_INDEXES...>& operator+=(const MultiHashMap<T, V, PRIMARY_INDEX, SECONDARY_INDEXES...> &other) {
      for (size_t b = 0; b < other.primary_index->size_; b++) {
          PrimaryIdxNode<T> *n = &other.primary_index->buckets_[b];
          do {
              if (n->obj) insert(*n->obj, n->hash);
          } while ((n = n->nxt));
      }
      return *this;
  }

  inline size_t count() const {
      return primary_index->count();
  }

  inline bool isZero() const {
      return (count() == 0);
  }

  inline const T* get(const T& key) const {
    return primary_index->get(key);
  }

  inline const V& getValueOrDefault(const T& key) const {
     T* elem = primary_index->get(key);
     if (elem != nullptr) return elem->__av;
     return Value<V>::zero;
  }

  inline const LinkedNode<T>* slice(const T& k, size_t idx) {
    return secondary_indexes[idx]->slice(k);
  }    

  void erase(const T& k) {
    HASH_RES_t h = primary_index->computeHash(k);
    T* elem = get(k, h);
    if (elem != nullptr) erase(elem, h);
  }

  template <typename V2>
  FORCE_INLINE void insertOrAssign(const T& k, const V2& v) {
      HASH_RES_t h = primary_index->computeHash(k);
      T* elem = primary_index->get(k, h);
      if (elem != nullptr) {
          elem->__av = v;
      }
      else {
          k.__av = v;
          insert(k, h);
      }
  }

  template <typename V2>
  FORCE_INLINE void insertOrAssignNonZero(const T& k, const V2& v) {
      HASH_RES_t h = primary_index->computeHash(k);
      T* elem = primary_index->get(k, h);
      if (elem != nullptr) {
          if (Value<V2>::isZero(v)) erase(elem, h);
          else elem->__av = v;
      }
      else if (!Value<V2>::isZero(v)) {
          k.__av = v;
          insert(k, h);
      }
  }

  template <typename V2>
  FORCE_INLINE void incrementOrAssign(T& k, const V2& v) {
      HASH_RES_t h = primary_index->computeHash(k);
      T* elem = primary_index->get(k, h);
      if (elem != nullptr) {
          elem->__av += v;
      }
      else {
          k.__av = v;
          insert(k, h);
      }
  }

  template <typename V2>
  FORCE_INLINE void incrementOrAssignNonZero(T& k, const V2& v) {
      if (Value<V2>::isZero(v)) return;
      HASH_RES_t h = primary_index->computeHash(k);
      T* elem = primary_index->get(k, h);
      if (elem != nullptr) {
          elem->__av += v;
          if (Value<V>::isZero(elem->__av)) erase(elem, h);
      }
      else {
          k.__av = v;
          insert(k, h);
      }
  }

  template <typename V2>
  FORCE_INLINE void addOrDelOnZero(T& k, const V2& v) {
      incrementOrAssignNonZero(k, v);
  }

  template <typename V2>
  FORCE_INLINE void add(T& k, const V2& v) {
      incrementOrAssign(k, v);
  }

  template <typename V2>
  FORCE_INLINE void setOrDelOnZero(T& k, const V2& v) {
      insertOrAssignNonZero(k, v);
  }

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) const {
    ar << "\n\t\t";
    dbtoaster::serialize_nvp(ar, "count", count());
    T* elem = head;
    while (elem) {
      ar << "\n";
      dbtoaster::serialize_nvp_tabbed(ar, "item", *elem, "\t\t");
      elem = elem->nxt;
    }
  }

};

}

#endif /* DBTOASTER_MMAP_HPP */