#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"
#include "../templates/cofactor_value.cpp"

#define RELATION_TWITTER1_DYNAMIC
#define RELATION_TWITTER2_STATIC
#define RELATION_TWITTER3_STATIC

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct TWITTER1_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE b; long __av; TWITTER1_entry* nxt; TWITTER1_entry* prv;
    explicit TWITTER1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER1_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; b = c1; __av = c2;  }
    TWITTER1_entry(const TWITTER1_entry& other) : a(other.a), b(other.b), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER1_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); b = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER1_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; b = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, b);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.b);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER1_entry& x, const TWITTER1_entry& y) {
      return x.a == y.a && x.b == y.b;
    }
  };
  
  typedef MultiHashMap<TWITTER1_entry, long, 
    PrimaryHashIndex<TWITTER1_entry, TWITTER1_mapkey01_idxfn>
  > TWITTER1_map;
  typedef TWITTER1_entry DELTA_TWITTER1_entry;
  typedef TWITTER1_map DELTA_TWITTER1_map;
  struct TWITTER2_entry {
    DOUBLE_TYPE B; DOUBLE_TYPE C; long __av; TWITTER2_entry* nxt; TWITTER2_entry* prv;
    explicit TWITTER2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER2_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { B = c0; C = c1; __av = c2;  }
    TWITTER2_entry(const TWITTER2_entry& other) : B(other.B), C(other.C), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER2_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ B = std::stod(f[0]); C = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER2_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { B = c0; C = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.B);
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER2_entry& x, const TWITTER2_entry& y) {
      return x.B == y.B && x.C == y.C;
    }
  };
  
  typedef MultiHashMap<TWITTER2_entry, long, 
    PrimaryHashIndex<TWITTER2_entry, TWITTER2_mapkey01_idxfn>
  > TWITTER2_map;
  struct TWITTER3_entry {
    DOUBLE_TYPE A; DOUBLE_TYPE C; long __av; TWITTER3_entry* nxt; TWITTER3_entry* prv;
    explicit TWITTER3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER3_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { A = c0; C = c1; __av = c2;  }
    TWITTER3_entry(const TWITTER3_entry& other) : A(other.A), C(other.C), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER3_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ A = std::stod(f[0]); C = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER3_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { A = c0; C = c1;  return *this; }FORCE_INLINE TWITTER3_entry& modify1(const DOUBLE_TYPE c1) { C = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER3_entry& x, const TWITTER3_entry& y) {
      return x.A == y.A && x.C == y.C;
    }
  };
  
  struct TWITTER3_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER3_entry& x, const TWITTER3_entry& y) {
      return x.C == y.C;
    }
  };
  
  typedef MultiHashMap<TWITTER3_entry, long, 
    PrimaryHashIndex<TWITTER3_entry, TWITTER3_mapkey01_idxfn>, 
    SecondaryHashIndex<TWITTER3_entry, TWITTER3_mapkey1_idxfn>
  > TWITTER3_map;

  // CUSTOM DATA STRUCTURES
  typedef AGG_value<1> AGG_C_value;
  struct AGG_C_entry {
    DOUBLE_TYPE A; 
    DOUBLE_TYPE B; 
    AGG_C_value __av;  
    AGG_C_entry* nxt; 
    AGG_C_entry* prv;
    explicit AGG_C_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
   struct AGG_C_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_C_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      hash_combine(h, e.B);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_C_entry& x, const AGG_C_entry& y) {
      return x.A == y.A && x.B == y.B;
    }
  };
  
  typedef MultiHashMap<AGG_C_entry, AGG_C_value, 
    PrimaryHashIndex<AGG_C_entry, AGG_C_mapkey012_idxfn>
  > AGG_C_map;


  typedef AGG_value<2> AGG_B_value;
  struct AGG_B_entry {
    DOUBLE_TYPE A;
    AGG_B_value __av;
    AGG_B_entry* nxt; 
    AGG_B_entry* prv;
    explicit AGG_B_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_B_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_B_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_B_entry& x, const AGG_B_entry& y) {
      return x.A == y.A;
    }
  };
  
  typedef MultiHashMap<AGG_B_entry, AGG_B_value, 
    PrimaryHashIndex<AGG_B_entry, AGG_B_mapkey012_idxfn>
  > AGG_B_map;
  
  typedef AGG_value<3> AGG_value;

  
  // struct DELTA_AGG_B_entry {
  //   DOUBLE_TYPE A; long Deg_b; long Deg_c; DOUBLE_TYPE __av; DELTA_AGG_B_entry* nxt; DELTA_AGG_B_entry* prv;
  //   explicit DELTA_AGG_B_entry() : nxt(nullptr), prv(nullptr) { }
  //   explicit DELTA_AGG_B_entry(const DOUBLE_TYPE c0, const long c1, const long c2, const DOUBLE_TYPE c3) { A = c0; Deg_b = c1; Deg_c = c2; __av = c3;  }
  //   DELTA_AGG_B_entry(const DELTA_AGG_B_entry& other) : A(other.A), Deg_b(other.Deg_b), Deg_c(other.Deg_c), __av(other.__av), nxt(nullptr), prv(nullptr) { }
  //   DELTA_AGG_B_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) { if (f.size() < 3) return; A = std::stod(f[0]); Deg_b = std::stol(f[1]); Deg_c = std::stol(f[2]); __av = v; nxt = nullptr; prv = nullptr; }
  //   FORCE_INLINE DELTA_AGG_B_entry& modify(const DOUBLE_TYPE c0, const long c1, const long c2) { A = c0; Deg_b = c1; Deg_c = c2;  return *this; }
  //   template<class Archive>
  //   void serialize(Archive& ar, const unsigned int version) const {
  //     ar << ELEM_SEPARATOR;
  //     DBT_SERIALIZATION_NVP(ar, A);
  //     ar << ELEM_SEPARATOR;
  //     DBT_SERIALIZATION_NVP(ar, Deg_b);
  //     ar << ELEM_SEPARATOR;
  //     DBT_SERIALIZATION_NVP(ar, Deg_c);
  //     ar << ELEM_SEPARATOR;
  //     DBT_SERIALIZATION_NVP(ar, __av);
  //   }
  // };
  
  // struct DELTA_AGG_B_mapkey012_idxfn {
  //   FORCE_INLINE static size_t hash(const DELTA_AGG_B_entry& e) {
  //     size_t h = 0;
  //     hash_combine(h, e.A);
  //     hash_combine(h, e.Deg_b);
  //     hash_combine(h, e.Deg_c);
  //     return h;
  //   }
  //   FORCE_INLINE static bool equals(const DELTA_AGG_B_entry& x, const DELTA_AGG_B_entry& y) {
  //     return x.A == y.A && x.Deg_b == y.Deg_b && x.Deg_c == y.Deg_c;
  //   }
  // };
  
  // typedef MultiHashMap<DELTA_AGG_B_entry, DOUBLE_TYPE, 
  //   PrimaryHashIndex<DELTA_AGG_B_entry, DELTA_AGG_B_mapkey012_idxfn>
  // > DELTA_AGG_B_map;
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0)  { gettimeofday(&t0, NULL); }
  
    /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG), AGG, "\t");
    }
  
  protected:
    /* Data structures used for storing / computing top level queries */
    AGG_value AGG;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t() {
      
    }
  
  
    /* Trigger functions for table relations */
    void on_insert_TWITTER2(const DOUBLE_TYPE b, const DOUBLE_TYPE c) {
      TWITTER2_entry e(b, c, 1L);
      TWITTER2.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER2(TWITTER2_entry &e) {
      e.__av = 1L;
      TWITTER2.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER3(const DOUBLE_TYPE a, const DOUBLE_TYPE c) {
      TWITTER3_entry e(a, c, 1L);
      TWITTER3.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER3(TWITTER3_entry &e) {
      e.__av = 1L;
      TWITTER3.addOrDelOnZero(e, 1L);
    }
    
    /* Trigger functions for stream relations */
    void on_batch_update_TWITTER1(const std::vector<TWITTER1_entry>::iterator &begin, const std::vector<TWITTER1_entry>::iterator &end) {
      tN += std::distance(begin, end);
      { //foreach
        for (std::vector<DELTA_TWITTER1_entry>::iterator e1 = begin; e1 != end; e1++) {          
          DOUBLE_TYPE a = e1->a;
          DOUBLE_TYPE b = e1->b;
          long count1 = e1->__av;

          entry_AGG_C.A = a;
          entry_AGG_C.B = b;
          const AGG_C_value &vAGG_C = AGG_C.getValueOrDefault(entry_AGG_C);
          long count2 = vAGG_C.count;

          long count = count1 * count2;

          AGG.cofactors[0] += count * a * a;
          AGG.cofactors[1] += count * a * b;
          AGG.cofactors[2] += count1 * a * vAGG_C.sums[0];
          AGG.cofactors[3] += count * b * b;
          AGG.cofactors[4] += count1 * b * vAGG_C.sums[0];
          AGG.cofactors[5] += count1 * vAGG_C.cofactors[0];
        }
      }
    }
    
    void on_system_ready_event() {
      AGG_C.clear();
      { //foreach
        TWITTER2_entry* e1 = TWITTER2.head;
        while (e1) {
          DOUBLE_TYPE b = e1->B;
          DOUBLE_TYPE c = e1->C;
          long count1 = e1->__av;
          { //slice
            const SecondaryIdxNode<TWITTER3_entry>* n2_head = static_cast<const SecondaryIdxNode<TWITTER3_entry>*>(TWITTER3.slice(se2.modify1(c), 0));
            const SecondaryIdxNode<TWITTER3_entry>* n2 = n2_head;
            TWITTER3_entry* e2;
            while (n2) {
              e2 = n2->obj;
              DOUBLE_TYPE a = e2->A;
              long count2 = e2->__av;

              // KEY
              entry_AGG_C.A = a;
              entry_AGG_C.B = b;

              // VALUE
              long count = count1 * count2;
              value_AGG_C.count = count;
              value_AGG_C.sums[0] = count * c;
              value_AGG_C.cofactors[0] = count * c * c;

              AGG_C.addOrDelOnZero(entry_AGG_C, value_AGG_C);

              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }
          e1 = e1->nxt;
        }
      }
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    AGG_C_entry entry_AGG_C;
    AGG_C_value value_AGG_C;
    TWITTER3_entry se2;
  
    /* Data structures used for storing materialized views */
    TWITTER2_map TWITTER2;
    TWITTER3_map TWITTER3;
    AGG_C_map AGG_C;  
  };
}
        