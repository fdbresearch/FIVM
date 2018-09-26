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
#define RELATION_TWITTER2_DYNAMIC
#define RELATION_TWITTER3_DYNAMIC

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
    DOUBLE_TYPE b; DOUBLE_TYPE c; long __av; TWITTER2_entry* nxt; TWITTER2_entry* prv;
    explicit TWITTER2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER2_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { b = c0; c = c1; __av = c2;  }
    TWITTER2_entry(const TWITTER2_entry& other) : b(other.b), c(other.c), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER2_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ b = std::stod(f[0]); c = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER2_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { b = c0; c = c1;  return *this; }
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
  
  typedef MultiHashMap<TWITTER2_entry, long, 
    PrimaryHashIndex<TWITTER2_entry, TWITTER2_mapkey01_idxfn>
  > TWITTER2_map;
  typedef TWITTER2_entry DELTA_TWITTER2_entry;
  typedef TWITTER2_map DELTA_TWITTER2_map;
  struct TWITTER3_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE c; long __av; TWITTER3_entry* nxt; TWITTER3_entry* prv;
    explicit TWITTER3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER3_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; c = c1; __av = c2;  }
    TWITTER3_entry(const TWITTER3_entry& other) : a(other.a), c(other.c), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER3_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); c = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER3_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; c = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, a);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, c);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TWITTER3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const TWITTER3_entry& x, const TWITTER3_entry& y) {
      return x.a == y.a && x.c == y.c;
    }
  };
  
  typedef MultiHashMap<TWITTER3_entry, long, 
    PrimaryHashIndex<TWITTER3_entry, TWITTER3_mapkey01_idxfn>
  > TWITTER3_map;
  typedef TWITTER3_entry DELTA_TWITTER3_entry;
  typedef TWITTER3_map DELTA_TWITTER3_map;

  struct BASE_TWITTER1_entry {
    DOUBLE_TYPE A; DOUBLE_TYPE B; long __av; BASE_TWITTER1_entry* nxt; BASE_TWITTER1_entry* prv;
    explicit BASE_TWITTER1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER1_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { A = c0; B = c1; __av = c2;  }
    BASE_TWITTER1_entry(const BASE_TWITTER1_entry& other) : A(other.A), B(other.B), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER1_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ A = std::stod(f[0]); B = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER1_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { A = c0; B = c1;  return *this; }
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
  
  struct BASE_TWITTER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      hash_combine(h, e.B);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER1_entry& x, const BASE_TWITTER1_entry& y) {
      return x.A == y.A && x.B == y.B;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER1_entry, long, 
    PrimaryHashIndex<BASE_TWITTER1_entry, BASE_TWITTER1_mapkey01_idxfn>
  > BASE_TWITTER1_map;
  struct BASE_TWITTER2_entry {
    DOUBLE_TYPE B; DOUBLE_TYPE C; long __av; BASE_TWITTER2_entry* nxt; BASE_TWITTER2_entry* prv;
    explicit BASE_TWITTER2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER2_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { B = c0; C = c1; __av = c2;  }
    BASE_TWITTER2_entry(const BASE_TWITTER2_entry& other) : B(other.B), C(other.C), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER2_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ B = std::stod(f[0]); C = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER2_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { B = c0; C = c1;  return *this; }FORCE_INLINE BASE_TWITTER2_entry& modify1(const DOUBLE_TYPE c1) { C = c1;  return *this; }
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
  
  struct BASE_TWITTER2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.B);
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER2_entry& x, const BASE_TWITTER2_entry& y) {
      return x.B == y.B && x.C == y.C;
    }
  };
  
  struct BASE_TWITTER2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER2_entry& x, const BASE_TWITTER2_entry& y) {
      return x.C == y.C;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER2_entry, long, 
    PrimaryHashIndex<BASE_TWITTER2_entry, BASE_TWITTER2_mapkey01_idxfn>, 
    SecondaryHashIndex<BASE_TWITTER2_entry, BASE_TWITTER2_mapkey1_idxfn>
  > BASE_TWITTER2_map;
  struct BASE_TWITTER3_entry {
    DOUBLE_TYPE A; DOUBLE_TYPE C; long __av; BASE_TWITTER3_entry* nxt; BASE_TWITTER3_entry* prv;
    explicit BASE_TWITTER3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER3_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { A = c0; C = c1; __av = c2;  }
    BASE_TWITTER3_entry(const BASE_TWITTER3_entry& other) : A(other.A), C(other.C), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER3_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ A = std::stod(f[0]); C = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER3_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { A = c0; C = c1;  return *this; }FORCE_INLINE BASE_TWITTER3_entry& modify1(const DOUBLE_TYPE c1) { C = c1;  return *this; }
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
  
  struct BASE_TWITTER3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.A);
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER3_entry& x, const BASE_TWITTER3_entry& y) {
      return x.A == y.A && x.C == y.C;
    }
  };
  
  struct BASE_TWITTER3_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER3_entry& x, const BASE_TWITTER3_entry& y) {
      return x.C == y.C;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER3_entry, long, 
    PrimaryHashIndex<BASE_TWITTER3_entry, BASE_TWITTER3_mapkey01_idxfn>, 
    SecondaryHashIndex<BASE_TWITTER3_entry, BASE_TWITTER3_mapkey1_idxfn>
  > BASE_TWITTER3_map;


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
  typedef AGG_C_entry DELTA_AGG_C_entry;
  typedef AGG_C_value DELTA_AGG_C_value;
  typedef AGG_C_map DELTA_AGG_C_map;

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
    
    /* Trigger functions for stream relations */
    void on_batch_update_TWITTER1(const std::vector<TWITTER1_entry>::iterator &begin, const std::vector<TWITTER1_entry>::iterator &end) {
      tN += std::distance(begin, end);
      { //foreach
        for (std::vector<TWITTER1_entry>::iterator e1 = begin; e1 != end; e1++) {          
          DOUBLE_TYPE a = e1->a;
          DOUBLE_TYPE b = e1->b;
          long count1 = e1->__av;

          entry_AGG_C.A = a;
          entry_AGG_C.B = b;
          const AGG_C_value &vAGG_C = AGG_C.getValueOrDefault(entry_AGG_C);
          long count2 = vAGG_C.count;

          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count1 * a * vAGG_C.sums[0];
            AGG.cofactors[3] += count * b * b;
            AGG.cofactors[4] += count1 * b * vAGG_C.sums[0];
            AGG.cofactors[5] += count1 * vAGG_C.cofactors[0];
          }

          BASE_TWITTER1.addOrDelOnZero(entry_BASE_TWITTER1.modify(a, b), count1);
        }
      }
    }

    void on_batch_update_TWITTER2(const std::vector<TWITTER2_entry>::iterator &begin, const std::vector<TWITTER2_entry>::iterator &end) {
      tN += std::distance(begin, end);
      DELTA_AGG_C.clear();
      { //foreach
        for (std::vector<TWITTER2_entry>::iterator e7 = begin; e7 != end; e7++) {
          DOUBLE_TYPE b = e7->b;
          DOUBLE_TYPE c = e7->c;
          long count1 = e7->__av;

          { //slice
            const SecondaryIdxNode<BASE_TWITTER3_entry>* n2_head = static_cast<const SecondaryIdxNode<BASE_TWITTER3_entry>*>(BASE_TWITTER3.slice(entry_BASE_TWITTER3.modify1(c), 0));
            const SecondaryIdxNode<BASE_TWITTER3_entry>* n2 = n2_head;
            BASE_TWITTER3_entry* e2;
            while (n2) {
              e2 = n2->obj;
              DOUBLE_TYPE a = e2->A;
              long count2 = e2->__av;
              long count = count1 * count2;

              if (count > 0) {
                // KEY
                entry_DELTA_AGG_C.A = a;
                entry_DELTA_AGG_C.B = b;

                // VALUE
                value_DELTA_AGG_C.count = count;
                value_DELTA_AGG_C.sums[0] = count * c;
                value_DELTA_AGG_C.cofactors[0] = count * c * c;

                DELTA_AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, value_DELTA_AGG_C);
              }

              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }

          BASE_TWITTER2.addOrDelOnZero(entry_BASE_TWITTER2.modify(b, c), count1);
        }
      }

      { //foreach
        DELTA_AGG_C_entry* e10 = DELTA_AGG_C.head;
        while (e10) {
          DOUBLE_TYPE a = e10->A;
          DOUBLE_TYPE b = e10->B;
          const DELTA_AGG_C_value &vDELTA_AGG_C = e10->__av;

          long count1 = vDELTA_AGG_C.count;
          const long &count2 = BASE_TWITTER1.getValueOrDefault(entry_BASE_TWITTER1.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vDELTA_AGG_C.sums[0];
            AGG.cofactors[3] += count * b * b;
            AGG.cofactors[4] += count2 * b * vDELTA_AGG_C.sums[0];
            AGG.cofactors[5] += count2 * vDELTA_AGG_C.cofactors[0];
          }
         
          entry_AGG_C.A = a;
          entry_AGG_C.B = b;
          AGG_C.addOrDelOnZero(entry_AGG_C, vDELTA_AGG_C);

          e10 = e10->nxt;
        }
      }
    }
    
    void on_batch_update_TWITTER3(const std::vector<TWITTER3_entry>::iterator &begin, const std::vector<TWITTER3_entry>::iterator &end) {
      tN += std::distance(begin, end);

      DELTA_AGG_C.clear();
      { //foreach
        for (std::vector<TWITTER3_entry>::iterator e16 = begin; e16 != end; e16++) {
          DOUBLE_TYPE a = e16->a;
          DOUBLE_TYPE c = e16->c;
          long count1 = e16->__av;

          { //slice
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n2_head = static_cast<const SecondaryIdxNode<BASE_TWITTER2_entry>*>(BASE_TWITTER2.slice(entry_BASE_TWITTER2.modify1(c), 0));
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n2 = n2_head;
            BASE_TWITTER2_entry* e2;
            while (n2) {
              e2 = n2->obj;
              DOUBLE_TYPE b = e2->B;
              long count2 = e2->__av;
              long count = count1 * count2;

              if (count > 0) {
                // KEY
                entry_DELTA_AGG_C.A = a;
                entry_DELTA_AGG_C.B = b;

                // VALUE
                value_DELTA_AGG_C.count = count;
                value_DELTA_AGG_C.sums[0] = count * c;
                value_DELTA_AGG_C.cofactors[0] = count * c * c;

                DELTA_AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, value_DELTA_AGG_C);
              }

              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }

          BASE_TWITTER3.addOrDelOnZero(entry_BASE_TWITTER3.modify(a, c), count1);
        }
      }

      { //foreach
        DELTA_AGG_C_entry* e10 = DELTA_AGG_C.head;
        while (e10) {
          DOUBLE_TYPE a = e10->A;
          DOUBLE_TYPE b = e10->B;
          const DELTA_AGG_C_value &vDELTA_AGG_C = e10->__av;

          long count1 = vDELTA_AGG_C.count;
          const long &count2 = BASE_TWITTER1.getValueOrDefault(entry_BASE_TWITTER1.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vDELTA_AGG_C.sums[0];
            AGG.cofactors[3] += count * b * b;
            AGG.cofactors[4] += count2 * b * vDELTA_AGG_C.sums[0];
            AGG.cofactors[5] += count2 * vDELTA_AGG_C.cofactors[0];
          }

          entry_DELTA_AGG_C.A = a;
          entry_DELTA_AGG_C.B = b;
          AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, vDELTA_AGG_C);

          e10 = e10->nxt;
        }
      }
    }
    
    void on_system_ready_event() {
    }
    
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    AGG_C_entry entry_AGG_C;
    AGG_C_value value_AGG_C;
    DELTA_AGG_C_entry entry_DELTA_AGG_C;
    DELTA_AGG_C_value value_DELTA_AGG_C;
    BASE_TWITTER1_entry entry_BASE_TWITTER1;
    BASE_TWITTER2_entry entry_BASE_TWITTER2;
    BASE_TWITTER3_entry entry_BASE_TWITTER3;


    /* Data structures used for storing materialized views */
    AGG_C_map AGG_C;
    DELTA_AGG_C_map DELTA_AGG_C;
    BASE_TWITTER1_map BASE_TWITTER1;
    BASE_TWITTER2_map BASE_TWITTER2;
    BASE_TWITTER3_map BASE_TWITTER3;
  };
}
        