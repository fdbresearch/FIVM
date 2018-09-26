#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_A1_STATIC
#define RELATION_A2_DYNAMIC
#define RELATION_A3_STATIC

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct A2_entry {
    long i; DOUBLE_TYPE __av; A2_entry* nxt; A2_entry* prv;
    explicit A2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit A2_entry(const long c0, const DOUBLE_TYPE c1) { i = c0; __av = c1;  }
    A2_entry(const A2_entry& other) : i(other.i), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    A2_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { /* if (f.size() < 1) return; */ i = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE A2_entry& modify(const long c0) { i = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct A2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const A2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.i);
      return h;
    }
    FORCE_INLINE static bool equals(const A2_entry& x, const A2_entry& y) {
      return x.i == y.i;
    }
  };
  
  typedef MultiHashMap<A2_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<A2_entry, A2_mapkey0_idxfn>
  > A2_map;
  
  typedef A2_entry DELTA_A2_entry;
  typedef A2_map DELTA_A2_map;
  struct A1_entry {
    long i; long j; DOUBLE_TYPE __av; A1_entry* nxt; A1_entry* prv;
    explicit A1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit A1_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { i = c0; j = c1; __av = c2;  }
    A1_entry(const A1_entry& other) : i(other.i), j(other.j), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    A1_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { /* if (f.size() < 2) return; */ i = std::stol(f[0]); j = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE A1_entry& modify(const long c0, const long c1) { i = c0; j = c1;  return *this; }
    FORCE_INLINE A1_entry& modify1(const long c1) { j = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, j);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct A1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const A1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.i);
      hash_combine(h, e.j);
      return h;
    }
    FORCE_INLINE static bool equals(const A1_entry& x, const A1_entry& y) {
      return x.i == y.i && x.j == y.j;
    }
  };
  
  struct A1_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const A1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.j);
      return h;
    }
    FORCE_INLINE static bool equals(const A1_entry& x, const A1_entry& y) {
      return x.j == y.j;
    }
  };
  
  typedef MultiHashMap<A1_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<A1_entry, A1_mapkey01_idxfn>, 
    SecondaryHashIndex<A1_entry, A1_mapkey1_idxfn>
  > A1_map;
  
  struct A3_entry {
    long i; long j; DOUBLE_TYPE __av; A3_entry* nxt; A3_entry* prv;
    explicit A3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit A3_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { i = c0; j = c1; __av = c2;  }
    A3_entry(const A3_entry& other) : i(other.i), j(other.j), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    A3_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { /* if (f.size() < 2) return; */ i = std::stol(f[0]); j = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE A3_entry& modify(const long c0, const long c1) { i = c0; j = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, j);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct A3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const A3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.i);
      hash_combine(h, e.j);
      return h;
    }
    FORCE_INLINE static bool equals(const A3_entry& x, const A3_entry& y) {
      return x.i == y.i && x.j == y.j;
    }
  };
  
  typedef MultiHashMap<A3_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<A3_entry, A3_mapkey01_idxfn>
  > A3_map;
  
  struct BASE_A2_entry {
    long i; long j; DOUBLE_TYPE __av; BASE_A2_entry* nxt; BASE_A2_entry* prv;
    explicit BASE_A2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_A2_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { i = c0; j = c1; __av = c2;  }
    BASE_A2_entry(const BASE_A2_entry& other) : i(other.i), j(other.j), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_A2_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { /* if (f.size() < 2) return; */ i = std::stol(f[0]); j = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_A2_entry& modify(const long c0, const long c1) { i = c0; j = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, j);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct BASE_A2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_A2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.i);
      hash_combine(h, e.j);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_A2_entry& x, const BASE_A2_entry& y) {
      return x.i == y.i && x.j == y.j;
    }
  };
  
  typedef MultiHashMap<BASE_A2_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<BASE_A2_entry, BASE_A2_mapkey01_idxfn>
  > BASE_A2_map;
  
  struct Q_entry {
    long i; long j; DOUBLE_TYPE __av; Q_entry* nxt; Q_entry* prv;
    explicit Q_entry() : nxt(nullptr), prv(nullptr) { }
    explicit Q_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { i = c0; j = c1; __av = c2;  }
    Q_entry(const Q_entry& other) : i(other.i), j(other.j), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    Q_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { /* if (f.size() < 2) return; */ i = std::stol(f[0]); j = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE Q_entry& modify(const long c0, const long c1) { i = c0; j = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, j);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct Q_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const Q_entry& e) {
      size_t h = 0;
      hash_combine(h, e.i);
      hash_combine(h, e.j);
      return h;
    }
    FORCE_INLINE static bool equals(const Q_entry& x, const Q_entry& y) {
      return x.i == y.i && x.j == y.j;
    }
  };
  
  typedef MultiHashMap<Q_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<Q_entry, Q_mapkey01_idxfn>
  > Q_map;
  
  struct tuple2_L_L {
    long _1;  DOUBLE_TYPE __av; tuple2_L_L* nxt; tuple2_L_L* prv;
    explicit tuple2_L_L() : nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE tuple2_L_L& modify(const long c0) { _1 = c0;  return *this; }
    static bool equals(const tuple2_L_L &x, const tuple2_L_L &y) {
      return ((x._1==y._1));
    }
    static long hash(const tuple2_L_L &e) {
      size_t h = 0;
          hash_combine(h, e._1);
      return h;
    }
  };
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0)  { gettimeofday(&t0, NULL); }
  
    /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      const Q_map& _Q = get_Q();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(Q), _Q, "\t");
    }
  
    /* Functions returning / computing the results of top level queries */
    const Q_map& get_Q() const {
      return Q;
    }
  
  protected:
    /* Data structures used for storing / computing top level queries */
    Q_map Q;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t(), agg2(16U), agg1(16U) {
      
    }
  
  
    /* Trigger functions for table relations */
    void on_insert_A1(const long i, const long j, const DOUBLE_TYPE value) {
      A1_entry e(i, j, value);
      A1.insert(e);
    }
    void on_insert_A1(A1_entry &e, const DOUBLE_TYPE value) {
      e.__av = value;
      A1.insert(e);
    }
    void on_insert_A3(const long i, const long j, const DOUBLE_TYPE value) {
      A3_entry e(i, j, value);
      A3.insert(e);
    }
    void on_insert_A3(A3_entry &e, const DOUBLE_TYPE value) {
      e.__av = value;
      A3.insert(e);
    }
    
    
    /* Trigger functions for stream relations */
    void on_batch_update_A2(const std::vector<A2_entry>::iterator &begin, const std::vector<A2_entry>::iterator &end) {
      DELTA_A2.clear();
      int i = rand() % matrixSize;
      for (int j = 0; j < matrixSize; j++) {
        se3.i = j;
        se3.__av = (DOUBLE_TYPE) rand() / RAND_MAX;
        DELTA_A2.insert(se3);
      }
      tN += DELTA_A2.count();

      // cout << "i = " << i << endl;
      // cout << "v = [ ";
      // for (int i = 0; i < matrixSize; i++) {
      //   cout << DELTA_A2.getValueOrDefault(se3.modify(i)) << " ";
      // }
      // cout << "];" << endl;

      agg1.clear();
      long l1 = i;
      { //slice
        const SecondaryIdxNode<A1_entry>* n1_head = static_cast<const SecondaryIdxNode<A1_entry>*>(A1.slice(se2.modify1(l1), 0));
        const SecondaryIdxNode<A1_entry>* n1 = n1_head;
        A1_entry* e1;
        while (n1) {
          e1 = n1->obj;
          long i = e1->i;
          DOUBLE_TYPE v1 = e1->__av;
          agg1.add(st1.modify(i), v1);
          n1 = (n1 != n1_head ? n1->nxt : n1->child);
        }
      }

      agg2.clear();     
      { //foreach
        A3_entry* e3 = A3.head;
        while (e3) {
          long k = e3->i;
          long l = e3->j;
          DOUBLE_TYPE v3 = e3->__av;
          agg2.add(st2.modify(l), (v3 * DELTA_A2.getValueOrDefault(se3.modify(k))));
          e3 = e3->nxt;
        }
      }

      { //temp foreach
        tuple2_L_L* e2 = agg1.head;
        while(e2) {
          long i = e2->_1;
          DOUBLE_TYPE v2 = e2->__av;

          { //temp foreach
            tuple2_L_L* e4 = agg2.head;
            while(e4) {
              long l = e4->_1;
              DOUBLE_TYPE v4 = e4->__av;
              Q.addOrDelOnZero(se1.modify(i, l), (v2 * v4));
              e4 = e4->nxt;            
            }
          }
          e2 = e2->nxt;        
        }
      }

      long l2 = i;
      { //temp foreach
        DELTA_A2_entry* e2 = DELTA_A2.head;
        while(e2) {
          long k = e2->i;
          DOUBLE_TYPE v5 = e2->__av;
          BASE_A2.addOrDelOnZero(se4.modify(l2, k), v5);
          e2 = e2->nxt;
        }
      }

      // cout << "Q = [ ";
      // for (int i = 0; i < matrixSize; i++) {
      //   for (int j = 0; j < matrixSize; j++) {
      //     cout << Q.getValueOrDefault(se1.modify(i,j)) << " ";
      //   }
      //   cout << "; ";
      // }
      // cout << "];" << endl;
    }
    
    void on_system_ready_event() {
      /* initialize random seed: */
      srand (time(NULL));

      for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {       
          on_insert_A1(i, j, (DOUBLE_TYPE) rand() / RAND_MAX);
          on_insert_A3(i, j, (DOUBLE_TYPE) rand() / RAND_MAX);
          se4.i = i;
          se4.j = j;
          se4.__av = (DOUBLE_TYPE) rand() / RAND_MAX; 
          BASE_A2.insert(se4);          
        }
      }
      cout << endl << "Matrix size: " << matrixSize << "x" << matrixSize  << endl;
      cout << "Matrix A1 count: " << A1.count() <<  "    Matrix A2 count: " << BASE_A2.count() <<  "    Matrix A3 count: " << A3.count() << endl;
      
      // cout << "A1 = [ ";
      // for (int i = 0; i < matrixSize; i++) {
      //   for (int j = 0; j < matrixSize; j++) {
      //     cout << A1.getValueOrDefault(se2.modify(i,j)) << " ";
      //   }
      //   cout << "; ";
      // }
      // cout << "];" << endl;

      // cout << "A3 = [ ";
      // for (int i = 0; i < matrixSize; i++) {
      //   for (int j = 0; j < matrixSize; j++) {
      //     cout << A3.getValueOrDefault(se5.modify(i,j)) << " ";
      //   }
      //   cout << "; ";
      // }
      // cout << "];" << endl;

    }
  
  private:
  
#ifdef MATRIX_SIZE
    const int matrixSize = MATRIX_SIZE;
#endif

    /* Sample entries for avoiding recreation of temporary objects */
    Q_entry se1;
    tuple2_L_L st1;
    A1_entry se2;    
    DELTA_A2_entry se3;
    tuple2_L_L st2;
    BASE_A2_entry se4;
    A3_entry se5;

    /* Data structures used for storing materialized views */
    A1_map A1;
    A3_map A3;
    BASE_A2_map BASE_A2;
    DELTA_A2_map DELTA_A2;
    MultiHashMap<tuple2_L_L, DOUBLE_TYPE, PrimaryHashIndex<tuple2_L_L> > agg2;
    MultiHashMap<tuple2_L_L, DOUBLE_TYPE, PrimaryHashIndex<tuple2_L_L> > agg1;
    
  
  };
}
        