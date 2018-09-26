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
#define RELATION_TWITTER4_STATIC

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
  struct TWITTER2_entry {
    DOUBLE_TYPE b; DOUBLE_TYPE c; long __av; TWITTER2_entry* nxt; TWITTER2_entry* prv;
    explicit TWITTER2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER2_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { b = c0; c = c1; __av = c2;  }
    TWITTER2_entry(const TWITTER2_entry& other) : b(other.b), c(other.c), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER2_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ b = std::stod(f[0]); c = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER2_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { b = c0; c = c1;  return *this; }FORCE_INLINE TWITTER2_entry& modify1(const DOUBLE_TYPE c1) { c = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, b);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, c);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.b);
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER2_entry& x, const TWITTER2_entry& y) {
      return x.b == y.b && x.c == y.c;
    }
  };
  
  struct TWITTER2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER2_entry& x, const TWITTER2_entry& y) {
      return x.c == y.c;
    }
  };
  
  typedef MultiHashMap<TWITTER2_entry, long, 
    PrimaryHashIndex<TWITTER2_entry, TWITTER2_mapkey01_idxfn>, 
    SecondaryHashIndex<TWITTER2_entry, TWITTER2_mapkey1_idxfn>
  > TWITTER2_map;
  struct TWITTER3_entry {
    DOUBLE_TYPE c; DOUBLE_TYPE d; long __av; TWITTER3_entry* nxt; TWITTER3_entry* prv;
    explicit TWITTER3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER3_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { c = c0; d = c1; __av = c2;  }
    TWITTER3_entry(const TWITTER3_entry& other) : c(other.c), d(other.d), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER3_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ c = std::stod(f[0]); d = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER3_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { c = c0; d = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, c);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.c);
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER3_entry& x, const TWITTER3_entry& y) {
      return x.c == y.c && x.d == y.d;
    }
  };
  
  typedef MultiHashMap<TWITTER3_entry, long, 
    PrimaryHashIndex<TWITTER3_entry, TWITTER3_mapkey01_idxfn>
  > TWITTER3_map;
  struct TWITTER4_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE d; long __av; TWITTER4_entry* nxt; TWITTER4_entry* prv;
    explicit TWITTER4_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER4_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; d = c1; __av = c2;  }
    TWITTER4_entry(const TWITTER4_entry& other) : a(other.a), d(other.d), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER4_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); d = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER4_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; d = c1;  return *this; }FORCE_INLINE TWITTER4_entry& modify1(const DOUBLE_TYPE c1) { d = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER4_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER4_entry& x, const TWITTER4_entry& y) {
      return x.a == y.a && x.d == y.d;
    }
  };
  
  struct TWITTER4_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER4_entry& x, const TWITTER4_entry& y) {
      return x.d == y.d;
    }
  };
  
  typedef MultiHashMap<TWITTER4_entry, long, 
    PrimaryHashIndex<TWITTER4_entry, TWITTER4_mapkey01_idxfn>, 
    SecondaryHashIndex<TWITTER4_entry, TWITTER4_mapkey1_idxfn>
  > TWITTER4_map;

  // CUSTOM DATA STRUCTURES
  typedef AGG_value<1> AGG_D_value;
  struct AGG_D_entry {
    DOUBLE_TYPE a; 
    DOUBLE_TYPE c; 
    AGG_D_value __av;  
    AGG_D_entry* nxt; 
    AGG_D_entry* prv;
    explicit AGG_D_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, c);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_D_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_D_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_D_entry& x, const AGG_D_entry& y) {
      return x.a == y.a && x.c == y.c;
    }
  };
  
  typedef MultiHashMap<AGG_D_entry, AGG_D_value, 
    PrimaryHashIndex<AGG_D_entry, AGG_D_mapkey012_idxfn>
  > AGG_D_map;


  typedef AGG_value<2> AGG_C_value;
  struct AGG_C_entry {
    DOUBLE_TYPE a; 
    DOUBLE_TYPE b; 
    AGG_C_value __av;  
    AGG_C_entry* nxt; 
    AGG_C_entry* prv;
    explicit AGG_C_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, b);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_C_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_C_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.b);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_C_entry& x, const AGG_C_entry& y) {
      return x.a == y.a && x.b == y.b;
    }
  };
  
  typedef MultiHashMap<AGG_C_entry, AGG_C_value, 
    PrimaryHashIndex<AGG_C_entry, AGG_C_mapkey012_idxfn>
  > AGG_C_map;


  typedef AGG_value<3> AGG_B_value;
  struct AGG_B_entry {
    DOUBLE_TYPE a;
    AGG_B_value __av;
    AGG_B_entry* nxt; 
    AGG_B_entry* prv;
    explicit AGG_B_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_B_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_B_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_B_entry& x, const AGG_B_entry& y) {
      return x.a == y.a;
    }
  };
  
  typedef MultiHashMap<AGG_B_entry, AGG_B_value, 
    PrimaryHashIndex<AGG_B_entry, AGG_B_mapkey012_idxfn>
  > AGG_B_map;
  
  typedef AGG_value<4> AGG_value;
 
  
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
    void on_insert_TWITTER3(const DOUBLE_TYPE c, const DOUBLE_TYPE d) {
      TWITTER3_entry e(c, d, 1L);
      TWITTER3.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER3(TWITTER3_entry &e) {
      e.__av = 1L;
      TWITTER3.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER4(const DOUBLE_TYPE a, const DOUBLE_TYPE d) {
      TWITTER4_entry e(a, d, 1L);
      TWITTER4.addOrDelOnZero(e, 1L);
    }
    void on_insert_TWITTER4(TWITTER4_entry &e) {
      e.__av = 1L;
      TWITTER4.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_system_ready_event() {
      AGG_D.clear();
      { //foreach
        TWITTER3_entry* e1 = TWITTER3.head;
        while (e1) {
          DOUBLE_TYPE c = e1->c;
          DOUBLE_TYPE d = e1->d;
          long count1 = e1->__av;
          { //slice
            const SecondaryIdxNode<TWITTER4_entry>* n2_head = static_cast<const SecondaryIdxNode<TWITTER4_entry>*>(TWITTER4.slice(se2.modify1(d), 0));
            const SecondaryIdxNode<TWITTER4_entry>* n2 = n2_head;
            TWITTER4_entry* e2;
            while (n2) {
              e2 = n2->obj;
              DOUBLE_TYPE a = e2->a;
              long count2 = e2->__av;

              long count = count1 * count2;

              if (count > 0) {
                // KEY
                entry_AGG_D.a = a;
                entry_AGG_D.c = c;

                // VALUE
                value_AGG_D.count = count;
                value_AGG_D.sums[0] = count * d;
                value_AGG_D.cofactors[0] = count * d * d;

                AGG_D.addOrDelOnZero(entry_AGG_D, value_AGG_D);
              }

              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }
          e1 = e1->nxt;
        }
      }
      AGG_C.clear();
      { //foreach
        AGG_D_entry* e4 = AGG_D.head;
        while (e4) {
          DOUBLE_TYPE a = e4->a;
          DOUBLE_TYPE c = e4->c;
          const AGG_D_value &vAGG_D = e4->__av;
          long count1 = vAGG_D.count;
          
          { //slice
            const SecondaryIdxNode<TWITTER2_entry>* n5_head = static_cast<const SecondaryIdxNode<TWITTER2_entry>*>(TWITTER2.slice(se4.modify1(c), 0));
            const SecondaryIdxNode<TWITTER2_entry>* n5 = n5_head;
            TWITTER2_entry* e5;
            while (n5) {
              e5 = n5->obj;
              DOUBLE_TYPE b = e5->b;
              long count2 = e5->__av;

              long count = count1 * count2;
              if (count > 0) {
                // KEY
                entry_AGG_C.a = a;
                entry_AGG_C.b = b;

                // VALUE
                long count = count1 * count2;
                value_AGG_C.count = count;
                value_AGG_C.sums[0] = count * c;
                value_AGG_C.sums[1] = count2 * vAGG_D.sums[0];
                value_AGG_C.cofactors[0] = count * c * c;
                value_AGG_C.cofactors[1] = count2 * c * vAGG_D.sums[0];
                value_AGG_C.cofactors[2] = count2 * vAGG_D.cofactors[0];

                AGG_C.addOrDelOnZero(entry_AGG_C, value_AGG_C);
              }

              n5 = (n5 != n5_head ? n5->nxt : n5->child);
            }
          }
          e4 = e4->nxt;
        }
      }

      AGG.clear();
      { //foreach
        AGG_C_entry* e4 = AGG_C.head;
        while (e4) {
          DOUBLE_TYPE a = e4->a;
          DOUBLE_TYPE b = e4->b;
          const AGG_C_value &vAGG_C = e4->__av;
          
          long count1 = vAGG_C.count;
          long count2 = TWITTER1.getValueOrDefault(se6.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vAGG_C.sums[0];
            AGG.cofactors[3] += count2 * a * vAGG_C.sums[1];
            AGG.cofactors[4] += count * b * b;
            AGG.cofactors[5] += count2 * b * vAGG_C.sums[0];
            AGG.cofactors[6] += count2 * b * vAGG_C.sums[1];
            AGG.cofactors[7] += count2 * vAGG_C.cofactors[0];
            AGG.cofactors[8] += count2 * vAGG_C.cofactors[1];
            AGG.cofactors[9] += count2 * vAGG_C.cofactors[2];
          }

          e4 = e4->nxt;
        }
      }
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    AGG_D_entry entry_AGG_D;
    AGG_C_entry entry_AGG_C;
    AGG_B_entry entry_AGG_B;
    AGG_D_value value_AGG_D;
    AGG_C_value value_AGG_C;
    AGG_B_value value_AGG_B;

    TWITTER4_entry se2;
    TWITTER2_entry se4;
    TWITTER1_entry se6;
  
    /* Data structures used for storing materialized views */
    TWITTER1_map TWITTER1;
    TWITTER2_map TWITTER2;
    TWITTER3_map TWITTER3;
    TWITTER4_map TWITTER4;
    
    AGG_D_map AGG_D;
    AGG_C_map AGG_C;
    AGG_B_map AGG_B;
    
    
  
  };
}
        