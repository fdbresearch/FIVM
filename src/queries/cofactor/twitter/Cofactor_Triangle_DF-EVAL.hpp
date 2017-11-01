#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"
#include "../templates/cofactor_value.cpp"

#define RELATION_TWITTER1_STATIC
#define RELATION_TWITTER2_STATIC
#define RELATION_TWITTER3_STATIC

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct TWITTER1_entry {
    DOUBLE_TYPE A; DOUBLE_TYPE B; long __av; TWITTER1_entry* nxt; TWITTER1_entry* prv;
    explicit TWITTER1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER1_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { A = c0; B = c1; __av = c2;  }
    TWITTER1_entry(const TWITTER1_entry& other) : A(other.A), B(other.B), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER1_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ A = std::stod(f[0]); B = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER1_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { A = c0; B = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, A);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, B);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      hash_combine(h, e.B);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER1_entry& x, const TWITTER1_entry& y) {
      return x.A == y.A && x.B == y.B;
    }
  };
  
  typedef MultiHashMap<TWITTER1_entry, long, 
    PrimaryHashIndex<TWITTER1_entry, TWITTER1_mapkey01_idxfn>
  > TWITTER1_map;
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
    void on_insert_TWITTER1(const DOUBLE_TYPE a, const DOUBLE_TYPE b) {
      TWITTER1_entry e(a, b, 1L);
      TWITTER1.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER1(TWITTER1_entry &e) {
      e.__av = 1L;
      TWITTER1.addOrDelOnZero(e, 1L);
    }
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
              long count = count1 * count2;

              if (count > 0) {
                // KEY
                entry_AGG_C.A = a;
                entry_AGG_C.B = b;

                // VALUE
                value_AGG_C.count = count;
                value_AGG_C.sums[0] = count * c;
                value_AGG_C.cofactors[0] = count * c * c;

                AGG_C.addOrDelOnZero(entry_AGG_C, value_AGG_C);
              }

              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }
          e1 = e1->nxt;
        }
      }

      AGG.clear();
      { //foreach
        AGG_C_entry* e4 = AGG_C.head;
        while (e4) {
          DOUBLE_TYPE a = e4->A;
          DOUBLE_TYPE b = e4->B;
          const AGG_C_value &vAGG_C = e4->__av;
          
          long count1 = vAGG_C.count;
          long count2 = TWITTER1.getValueOrDefault(se4.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vAGG_C.sums[0];
            AGG.cofactors[3] += count * b * b;
            AGG.cofactors[4] += count2 * b * vAGG_C.sums[0];
            AGG.cofactors[5] += count2 * vAGG_C.cofactors[0];
          }

          e4 = e4->nxt;
        }
      }

      // AGG_B.clear();
      // { //foreach
      //   AGG_C_entry* e4 = AGG_C.head;
      //   while (e4) {
      //     DOUBLE_TYPE a = e4->A;
      //     DOUBLE_TYPE b = e4->B;
      //     const AGG_C_value &vAGG_C = e4->__av;
          
      //     long count1 = vAGG_C.count;
      //     long count2 = TWITTER1.getValueOrDefault(se4.modify(a,b));
      //     long count = count1 * count2;

      //     // KEY
      //     entry_AGG_B.A = a;

      //     // VALUE
      //     long count = count1 * count2;
      //     value_AGG_B.count = count;
      //     value_AGG_B.sums[0] = count * b;
      //     value_AGG_B.sums[1] = count2 * vAGG_C.sums[0];
      //     value_AGG_B.cofactors[0] = count * b * b;
      //     value_AGG_B.cofactors[1] = count2 * b * vAGG_C.sums[0];
      //     value_AGG_B.cofactors[2] = count2 * vAGG_C.cofactors[0];

      //     AGG_B.addOrDelOnZero(entry_AGG_B, value_AGG_B);

      //     e4 = e4->nxt;
      //   }
      // }
      // // AGG.clear();
      // { //foreach
      //   AGG_B_entry* e6 = AGG_B.head;
      //   while (e6) {
      //     DOUBLE_TYPE a = e6->A;
      //     const AGG_B_value &vAGG_B = e6->__av;
      //     long count = vAGG_B.count;

      //     AGG.cofactors[0] += count * a * a;
      //     AGG.cofactors[1] += a * vAGG_B.sums[0];
      //     AGG.cofactors[2] += a * vAGG_B.sums[1];
      //     AGG.cofactors[3] += vAGG_B.cofactors[0];
      //     AGG.cofactors[4] += vAGG_B.cofactors[1];
      //     AGG.cofactors[5] += vAGG_B.cofactors[2];

      //     e6 = e6->nxt;
      //   }
      // }
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    AGG_C_entry entry_AGG_C;
    AGG_C_value value_AGG_C;
    AGG_B_entry entry_AGG_B;
    AGG_B_value value_AGG_B;
    TWITTER3_entry se2;
    TWITTER1_entry se4;
  
    /* Data structures used for storing materialized views */
    TWITTER1_map TWITTER1;
    TWITTER2_map TWITTER2;
    TWITTER3_map TWITTER3;
    AGG_C_map AGG_C;
    AGG_B_map AGG_B;
    
    
  
  };
}
        