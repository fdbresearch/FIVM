#ifndef DBTOASTER_POOL_HPP
#define DBTOASTER_POOL_HPP

#include <cassert>
#include <iostream>
#include <mutex>

#include "macro.hpp"

namespace dbtoaster {
#define DEFAULT_POOL_CHUNK_SIZE 32
#define MAX_POOL_CHUNK_SIZE 1048576  // 2^20

template <typename T>
class MemoryPool {
 public:
  MemoryPool(size_t sz = DEFAULT_POOL_CHUNK_SIZE)
      : lastChunk_(nullptr),
        currentSlot_(nullptr),
        lastSlot_(nullptr),
        freeSlots_(nullptr) {
    assert(sz > 1);
    allocateChunk(sz);
  }

  ~MemoryPool() {
    Slot_* curr = lastChunk_;
    while (curr != nullptr) {
      Slot_* next = curr->next;
      free(curr);
      curr = next;
    }
  }

  FORCE_INLINE T* allocate() {
    if (freeSlots_ != nullptr) {
      T* result = reinterpret_cast<T*>(freeSlots_);
      freeSlots_ = freeSlots_->next;
      return result;
    } else {
      if (currentSlot_ > lastSlot_) {
        allocateChunk((lastSlot_ - lastChunk_ + 1) << 1);
      }
      return reinterpret_cast<T*>(currentSlot_++);
    }
  }

  FORCE_INLINE void deallocate(T* obj) {
    if (obj != nullptr) {
      reinterpret_cast<Slot_*>(obj)->next = freeSlots_;
      freeSlots_ = reinterpret_cast<Slot_*>(obj);
    }
  }

  template <class... Args>
  FORCE_INLINE T* newElement(Args&&... args) {
    T* result = allocate();
    new (result) T(std::forward<Args>(args)...);
    return result;
  }

  FORCE_INLINE void deleteElement(T* obj) {
    if (obj != nullptr) {
      obj->~T();
      deallocate(obj);
    }
  }

 private:
  union Slot_ {
    T obj;
    Slot_* next;
  };

  Slot_* lastChunk_;
  Slot_* currentSlot_;
  Slot_* lastSlot_;
  Slot_* freeSlots_;

  void allocateChunk(size_t sz) {
    // precondition: no available elements
    assert(freeSlots_ == nullptr);

    size_t size = (sz < MAX_POOL_CHUNK_SIZE ? sz : MAX_POOL_CHUNK_SIZE);
    Slot_* chunk = reinterpret_cast<Slot_*>(malloc(size * sizeof(Slot_)));
    assert(chunk != nullptr);

    chunk->next = lastChunk_;
    lastChunk_ = chunk;
    currentSlot_ = chunk + 1;
    lastSlot_ = chunk + size - 1;
  }
};

template <typename T>
struct Elem {
  union {
    T obj;
    struct Elem* next;
  };
  bool used;

  Elem() : next(nullptr), used(false) {}

  ~Elem() { deactivate(); }

  FORCE_INLINE void deactivate() {
    if (used) {
      obj.~T();
      used = false;
    }
    next = nullptr;
  }
};

template <typename T>
struct ValueElem {
  union {
    T obj;
    struct ValueElem* next;
  };

  ValueElem() { next = nullptr; }
  ~ValueElem() { next = nullptr; }
};

template <typename T>
class Pool {
 private:
  Elem<T>* free_;
  Elem<T>* data_;
  size_t size_;

  void add_chunk(size_t new_size) {
    // precondition: no available elements
    assert(free_ == nullptr);

    size_ = new_size;
    Elem<T>* chunk = new Elem<T>[size_ + 1];
    for (size_t i = 0; i < size_ - 1; i++) {
      chunk[i].next = &chunk[i + 1];
    }
    chunk[size_ - 1].next = nullptr;
    chunk[size_].next = data_;
    data_ = chunk;
    free_ = chunk;
  }

 public:
  Pool(size_t chunk_size = DEFAULT_POOL_CHUNK_SIZE)
      : free_(nullptr), data_(nullptr) {
    add_chunk(chunk_size);
  }

  ~Pool() {
    size_t sz = size_;
    while (data_ != nullptr) {
      Elem<T>* el = data_[sz].next;
      delete[] data_;
      data_ = el;
      sz = sz >> 1;
    }
  }

  FORCE_INLINE T* add() {
    if (free_ == nullptr) {
      add_chunk(size_ << 1);
    }

    Elem<T>* el = free_;
    free_ = free_->next;
    el->used = true;
    // el->next = nullptr;
    return &(el->obj);
  }

  FORCE_INLINE void del(T* obj) {
    if (obj == nullptr) {
      return;
    }

    Elem<T>* el = reinterpret_cast<Elem<T>*>(obj);
    el->deactivate();
    el->next = free_;
    free_ = el;
  }

  FORCE_INLINE void delete_all(T* current_data) {
    if (current_data == nullptr) {
      return;
    }

    T* tmp = current_data;
    do {
      T* tmpNext = tmp->nxt;
      Elem<T>* tmpEl = reinterpret_cast<Elem<T>*>(tmp);
      tmpEl->deactivate();
      tmpEl->next =
          (tmpNext != nullptr ? reinterpret_cast<Elem<T>*>(tmpNext) : free_);
      tmp = tmpNext;
    } while (tmp != nullptr);
    free_ = reinterpret_cast<Elem<T>*>(current_data);
  }

  FORCE_INLINE void clear() {
    Elem<T>* prevChunk = nullptr;
    Elem<T>* chunk = data_;
    size_t sz = size_;
    size_t doubleSz = sz << 1;

    while (chunk != nullptr) {
      if (prevChunk != nullptr) {
        prevChunk[doubleSz - 1].next = chunk;
      }
      for (size_t i = 0; i < sz - 1; i++) {
        chunk[i].deactivate();
        chunk[i].next = &chunk[i + 1];
      }
      chunk[sz - 1].next = nullptr;
      prevChunk = chunk;
      chunk = chunk[sz].next;
      doubleSz = sz;
      sz = sz >> 1;
    }
    free_ = data_;
  }
};

template <typename T>
class ValuePool {
 private:
  ValueElem<T>* free_;
  ValueElem<T>* data_;
  size_t size_;

  void add_chunk(size_t new_size) {
    // precondition: no available elements
    assert(free_ == nullptr);

    size_ = new_size;
    ValueElem<T>* chunk = new ValueElem<T>[size_ + 1];
    for (size_t i = 0; i < size_ - 1; i++) {
      chunk[i].next = &chunk[i + 1];
    }
    chunk[size_ - 1].next = nullptr;
    chunk[size_].next = data_;
    data_ = chunk;
    free_ = chunk;
  }

 public:
  ValuePool(size_t chunk_size = DEFAULT_POOL_CHUNK_SIZE)
      : free_(nullptr), data_(nullptr) {
    add_chunk(chunk_size);
  }

  ~ValuePool() {
    size_t sz = size_;
    while (data_ != nullptr) {
      ValueElem<T>* el = data_[sz].next;
      delete[] data_;
      data_ = el;
      sz = sz >> 1;
    }
  }

  FORCE_INLINE T* add() {
    if (free_ == nullptr) {
      add_chunk(size_ << 1);
    }

    ValueElem<T>* el = free_;
    free_ = free_->next;
    // el->next = nullptr;
    return &(el->obj);
  }

  FORCE_INLINE void del(T* obj) {
    if (obj == nullptr) {
      return;
    }

    ValueElem<T>* el = reinterpret_cast<ValueElem<T>*>(obj);
    el->next = free_;
    free_ = el;
  }

  FORCE_INLINE void delete_all(T* current_data) {
    if (current_data == nullptr) {
      return;
    }

    T* tmp = current_data;
    do {
      T* tmpNext = tmp->nxt;
      ValueElem<T>* tmpEl = reinterpret_cast<ValueElem<T>*>(tmp);
      tmpEl->next =
          (tmpNext != nullptr ? reinterpret_cast<ValueElem<T>*>(tmpNext)
                              : free_);
      tmp = tmpNext;
    } while (tmp != nullptr);
    free_ = reinterpret_cast<ValueElem<T>*>(current_data);
  }

  FORCE_INLINE void clear() {
    ValueElem<T>* prevChunk = nullptr;
    ValueElem<T>* chunk = data_;
    size_t sz = size_;
    size_t doubleSz = sz << 1;

    while (chunk != nullptr) {
      if (prevChunk != nullptr) {
        prevChunk[doubleSz - 1].next = chunk;
      }
      for (size_t i = 0; i < sz - 1; i++) {
        chunk[i].next = &chunk[i + 1];
      }
      chunk[sz - 1].next = nullptr;
      prevChunk = chunk;
      chunk = chunk[sz].next;
      doubleSz = sz;
      sz = sz >> 1;
    }
    free_ = data_;
  }
};

template <typename T>
class ThreadSafeValuePool {
 private:
  ValueElem<T>* free_;
  ValueElem<T>* data_;
  size_t size_;
  std::mutex mtx_;

  void add_chunk(size_t new_size) {
    // precondition: no available elements
    assert(free_ == nullptr);

    size_ = new_size;
    ValueElem<T>* chunk = new ValueElem<T>[size_ + 1];
    for (size_t i = 0; i < size_ - 1; i++) {
      chunk[i].next = &chunk[i + 1];
    }
    chunk[size_ - 1].next = nullptr;

    chunk[size_].next = data_;
    data_ = chunk;
    free_ = chunk;
  }

 public:
  explicit ThreadSafeValuePool(size_t chunk_size = DEFAULT_POOL_CHUNK_SIZE)
      : free_(nullptr), data_(nullptr), size_(0) {
    add_chunk(chunk_size);
  }

  ~ThreadSafeValuePool() {
    std::lock_guard<std::mutex> lock(mtx_);

    size_t sz = size_;
    while (data_ != nullptr) {
      ValueElem<T>* next = data_[sz].next;
      delete[] data_;
      data_ = next;
      sz >>= 1;
    }
  }

  FORCE_INLINE T* add() {
    std::lock_guard<std::mutex> lock(mtx_);

    if (free_ == nullptr) {
      add_chunk(size_ << 1);
    }

    ValueElem<T>* el = free_;
    free_ = free_->next;
    return &(el->obj);
  }

  FORCE_INLINE void del(T* obj) {
    if (obj == nullptr) return;

    std::lock_guard<std::mutex> lock(mtx_);

    ValueElem<T>* el = reinterpret_cast<ValueElem<T>*>(obj);
    el->next = free_;
    free_ = el;
  }

  FORCE_INLINE void clear() {
    std::lock_guard<std::mutex> lock(mtx_);

    ValueElem<T>* prevChunk = nullptr;
    ValueElem<T>* chunk = data_;
    size_t sz = size_;
    size_t doubleSz = sz << 1;

    while (chunk != nullptr) {
      if (prevChunk != nullptr) {
        prevChunk[doubleSz - 1].next = chunk;
      }
      for (size_t i = 0; i < sz - 1; i++) {
        chunk[i].next = &chunk[i + 1];
      }
      chunk[sz - 1].next = nullptr;

      prevChunk = chunk;
      chunk = chunk[sz].next;
      doubleSz = sz;
      sz >>= 1;
    }

    free_ = data_;
  }
};

}  // namespace dbtoaster
#endif /* DBTOASTER_POOL_HPP */