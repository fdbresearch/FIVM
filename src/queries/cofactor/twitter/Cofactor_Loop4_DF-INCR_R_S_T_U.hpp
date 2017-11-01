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
#define RELATION_TWITTER4_DYNAMIC

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
  
  typedef TWITTER3_entry DELTA_TWITTER3_entry;
  typedef TWITTER3_map DELTA_TWITTER3_map;
  struct TWITTER4_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE d; long __av; TWITTER4_entry* nxt; TWITTER4_entry* prv;
    explicit TWITTER4_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TWITTER4_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; d = c1; __av = c2;  }
    TWITTER4_entry(const TWITTER4_entry& other) : a(other.a), d(other.d), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TWITTER4_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); d = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TWITTER4_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; d = c1;  return *this; }
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
  
  typedef MultiHashMap<TWITTER4_entry, long, 
    PrimaryHashIndex<TWITTER4_entry, TWITTER4_mapkey01_idxfn>
  > TWITTER4_map;
  
  typedef TWITTER4_entry DELTA_TWITTER4_entry;
  typedef TWITTER4_map DELTA_TWITTER4_map;

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
  
  struct AGG_D_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const AGG_D_entry& e) {
      size_t h = 0;
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_D_entry& x, const AGG_D_entry& y) {
      return x.c == y.c;
    }
  };

  typedef MultiHashMap<AGG_D_entry, AGG_D_value, 
    PrimaryHashIndex<AGG_D_entry, AGG_D_mapkey012_idxfn>,
    SecondaryHashIndex<AGG_D_entry, AGG_D_mapkey1_idxfn>
  > AGG_D_map;

  typedef MultiHashMap<AGG_D_entry, AGG_D_value, 
    PrimaryHashIndex<AGG_D_entry, AGG_D_mapkey012_idxfn>
  > DELTA_AGG_D_map;
  typedef AGG_D_entry DELTA_AGG_D_entry;
  typedef AGG_D_value DELTA_AGG_D_value;


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
  
  struct AGG_C_mapkey01_idxfn {
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
    PrimaryHashIndex<AGG_C_entry, AGG_C_mapkey012_idxfn>,
    SecondaryHashIndex<AGG_C_entry, AGG_C_mapkey01_idxfn>
  > AGG_C_map;
  typedef MultiHashMap<AGG_C_entry, AGG_C_value, 
    PrimaryHashIndex<AGG_C_entry, AGG_C_mapkey012_idxfn>
  > DELTA_AGG_C_map;
  typedef AGG_C_entry DELTA_AGG_C_entry;
  typedef AGG_C_value DELTA_AGG_C_value;


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

  typedef AGG_B_map DELTA_AGG_B_map;
  typedef AGG_B_entry DELTA_AGG_B_entry;
  typedef AGG_B_value DELTA_AGG_B_value;

  typedef AGG_value<4> AGG_value;

  
  struct BASE_TWITTER1_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE b; long __av; BASE_TWITTER1_entry* nxt; BASE_TWITTER1_entry* prv;
    explicit BASE_TWITTER1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER1_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; b = c1; __av = c2;  }
    BASE_TWITTER1_entry(const BASE_TWITTER1_entry& other) : a(other.a), b(other.b), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER1_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); b = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER1_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; b = c1;  return *this; }
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
  
  struct BASE_TWITTER1_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.b);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER1_entry& x, const BASE_TWITTER1_entry& y) {
      return x.a == y.a && x.b == y.b;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER1_entry, long, 
    PrimaryHashIndex<BASE_TWITTER1_entry, BASE_TWITTER1_mapkey01_idxfn>
  > BASE_TWITTER1_map;
  
  struct BASE_TWITTER2_entry {
    DOUBLE_TYPE b; DOUBLE_TYPE c; long __av; BASE_TWITTER2_entry* nxt; BASE_TWITTER2_entry* prv;
    explicit BASE_TWITTER2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER2_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { b = c0; c = c1; __av = c2;  }
    BASE_TWITTER2_entry(const BASE_TWITTER2_entry& other) : b(other.b), c(other.c), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER2_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ b = std::stod(f[0]); c = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER2_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { b = c0; c = c1;  return *this; }
    FORCE_INLINE BASE_TWITTER2_entry& modify1(const DOUBLE_TYPE c1) { c = c1;  return *this; }
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
  
  struct BASE_TWITTER2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.b);
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER2_entry& x, const BASE_TWITTER2_entry& y) {
      return x.b == y.b && x.c == y.c;
    }
  };
  
  struct BASE_TWITTER2_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.c);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER2_entry& x, const BASE_TWITTER2_entry& y) {
      return x.c == y.c;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER2_entry, long, 
    PrimaryHashIndex<BASE_TWITTER2_entry, BASE_TWITTER2_mapkey01_idxfn>, 
    SecondaryHashIndex<BASE_TWITTER2_entry, BASE_TWITTER2_mapkey1_idxfn>
  > BASE_TWITTER2_map;
  
  struct BASE_TWITTER3_entry {
    DOUBLE_TYPE c; DOUBLE_TYPE d; long __av; BASE_TWITTER3_entry* nxt; BASE_TWITTER3_entry* prv;
    explicit BASE_TWITTER3_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER3_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { c = c0; d = c1; __av = c2;  }
    BASE_TWITTER3_entry(const BASE_TWITTER3_entry& other) : c(other.c), d(other.d), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER3_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ c = std::stod(f[0]); d = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER3_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { c = c0; d = c1;  return *this; }
    FORCE_INLINE BASE_TWITTER3_entry& modify1(const DOUBLE_TYPE c1) { d = c1;  return *this; }
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
  
  struct BASE_TWITTER3_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.c);
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER3_entry& x, const BASE_TWITTER3_entry& y) {
      return x.c == y.c && x.d == y.d;
    }
  };
  
  struct BASE_TWITTER3_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER3_entry& e) {
      size_t h = 0;
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER3_entry& x, const BASE_TWITTER3_entry& y) {
      return x.d == y.d;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER3_entry, long, 
    PrimaryHashIndex<BASE_TWITTER3_entry, BASE_TWITTER3_mapkey01_idxfn>, 
    SecondaryHashIndex<BASE_TWITTER3_entry, BASE_TWITTER3_mapkey1_idxfn>
  > BASE_TWITTER3_map;
  
  struct BASE_TWITTER4_entry {
    DOUBLE_TYPE a; DOUBLE_TYPE d; long __av; BASE_TWITTER4_entry* nxt; BASE_TWITTER4_entry* prv;
    explicit BASE_TWITTER4_entry() : nxt(nullptr), prv(nullptr) { }
    explicit BASE_TWITTER4_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const long c2) { a = c0; d = c1; __av = c2;  }
    BASE_TWITTER4_entry(const BASE_TWITTER4_entry& other) : a(other.a), d(other.d), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    BASE_TWITTER4_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 2) return; */ a = std::stod(f[0]); d = std::stod(f[1]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE BASE_TWITTER4_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1) { a = c0; d = c1;  return *this; }
    FORCE_INLINE BASE_TWITTER4_entry& modify1(const DOUBLE_TYPE c1) { d = c1;  return *this; }
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
  
  struct BASE_TWITTER4_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.a);
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER4_entry& x, const BASE_TWITTER4_entry& y) {
      return x.a == y.a && x.d == y.d;
    }
  };
  
  struct BASE_TWITTER4_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const BASE_TWITTER4_entry& e) {
      size_t h = 0;
      hash_combine(h, e.d);
      return h;
    }
    FORCE_INLINE static bool equals(const BASE_TWITTER4_entry& x, const BASE_TWITTER4_entry& y) {
      return x.d == y.d;
    }
  };
  
  typedef MultiHashMap<BASE_TWITTER4_entry, long, 
    PrimaryHashIndex<BASE_TWITTER4_entry, BASE_TWITTER4_mapkey01_idxfn>, 
    SecondaryHashIndex<BASE_TWITTER4_entry, BASE_TWITTER4_mapkey1_idxfn>
  > BASE_TWITTER4_map;
  
  
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
    

    /* Trigger functions for stream relations */
    void on_batch_update_TWITTER1(const std::vector<TWITTER1_entry>::iterator &begin, const std::vector<TWITTER1_entry>::iterator &end) {
      tN += std::distance(begin, end);
      { //foreach
        for (std::vector<DELTA_TWITTER1_entry>::iterator e1 = begin; e1 != end; e1++) {
          DOUBLE_TYPE a = e1->a;
          DOUBLE_TYPE b = e1->b;
          long count1 = e1->__av;

          entry_AGG_C.a = a;
          entry_AGG_C.b = b;
          const AGG_C_value &vAGG_C = AGG_C.getValueOrDefault(entry_AGG_C);
          long count2 = vAGG_C.count;
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count1 * a * vAGG_C.sums[0];
            AGG.cofactors[3] += count1 * a * vAGG_C.sums[1];
            AGG.cofactors[4] += count * b * b;
            AGG.cofactors[5] += count1 * b * vAGG_C.sums[0];
            AGG.cofactors[6] += count1 * b * vAGG_C.sums[1];
            AGG.cofactors[7] += count1 * vAGG_C.cofactors[0];
            AGG.cofactors[8] += count1 * vAGG_C.cofactors[1];
            AGG.cofactors[9] += count1 * vAGG_C.cofactors[2];
          }

          BASE_TWITTER1.addOrDelOnZero(entry_BASE_TWITTER1.modify(a, b), count1);
        }
      }
    }
    
    void on_batch_update_TWITTER2(const std::vector<TWITTER2_entry>::iterator &begin, const std::vector<TWITTER2_entry>::iterator &end) {
      tN += std::distance(begin, end);
      DELTA_AGG_C.clear();
      { //foreach
        for (std::vector<DELTA_TWITTER2_entry>::iterator e7 = begin; e7 != end; e7++) {
          DOUBLE_TYPE b = e7->b;
          DOUBLE_TYPE c = e7->c;
          long count1 = e7->__av;
          { //slice
            entry_AGG_D.c = c;
            const SecondaryIdxNode<AGG_D_entry>* n8_head = static_cast<const SecondaryIdxNode<AGG_D_entry>*>(AGG_D.slice(entry_AGG_D, 0));
            const SecondaryIdxNode<AGG_D_entry>* n8 = n8_head;
            AGG_D_entry* e8;
            while (n8) {
              e8 = n8->obj;
              DOUBLE_TYPE a = e8->a;
              
              const AGG_D_value &vADD_D = e8->__av;
              long count2 = vADD_D.count;
              long count = count1 * count2;

              if (count > 0) {

                // KEY
                entry_DELTA_AGG_C.a = a;
                entry_DELTA_AGG_C.b = b;

                // VALUE
                value_DELTA_AGG_C.count = count;
                value_DELTA_AGG_C.sums[0] = count * c;
                value_DELTA_AGG_C.sums[1] = count1 * vADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[0] = count * c * c;
                value_DELTA_AGG_C.cofactors[1] = count1 * c * vADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[2] = count1 * vADD_D.cofactors[0];


                DELTA_AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, value_DELTA_AGG_C);
              }

              n8 = (n8 != n8_head ? n8->nxt : n8->child);
            }
          }

          BASE_TWITTER2.addOrDelOnZero(entry_BASE_TWITTER2.modify(b, c), count1);
        }
      }
      { //foreach
        DELTA_AGG_C_entry* e10 = DELTA_AGG_C.head;
        while (e10) {
          DOUBLE_TYPE a = e10->a;
          DOUBLE_TYPE b = e10->b;
          const DELTA_AGG_C_value &vDELTA_AGG_C = e10->__av;

          long count1 = vDELTA_AGG_C.count;
          const long &count2 = BASE_TWITTER1.getValueOrDefault(entry_BASE_TWITTER1.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vDELTA_AGG_C.sums[0];
            AGG.cofactors[3] += count2 * a * vDELTA_AGG_C.sums[1];
            AGG.cofactors[4] += count * b * b;
            AGG.cofactors[5] += count2 * b * vDELTA_AGG_C.sums[0];
            AGG.cofactors[6] += count2 * b * vDELTA_AGG_C.sums[1];
            AGG.cofactors[7] += count2 * vDELTA_AGG_C.cofactors[0];
            AGG.cofactors[8] += count2 * vDELTA_AGG_C.cofactors[1];
            AGG.cofactors[9] += count2 * vDELTA_AGG_C.cofactors[2];
          }
         
          entry_AGG_C.a = a;
          entry_AGG_C.b = b;
          AGG_C.addOrDelOnZero(entry_AGG_C, vDELTA_AGG_C);

          e10 = e10->nxt;
        }
      }
    }
    
    void on_batch_update_TWITTER3(const std::vector<TWITTER3_entry>::iterator &begin, const std::vector<TWITTER3_entry>::iterator &end) {
      tN += std::distance(begin, end);
      DELTA_AGG_D.clear();
      { //foreach
        for (std::vector<DELTA_TWITTER3_entry>::iterator e16 = begin; e16 != end; e16++) {
          DOUBLE_TYPE c = e16->c;
          DOUBLE_TYPE d = e16->d;
          long count1 = e16->__av;
          { //slice
            const SecondaryIdxNode<BASE_TWITTER4_entry>* n17_head = static_cast<const SecondaryIdxNode<BASE_TWITTER4_entry>*>(BASE_TWITTER4.slice(entry_BASE_TWITTER4.modify1(d), 0));
            const SecondaryIdxNode<BASE_TWITTER4_entry>* n17 = n17_head;
            BASE_TWITTER4_entry* e17;
            while (n17) {
              e17 = n17->obj;
              DOUBLE_TYPE a = e17->a;
              long count2 = e17->__av;

              long count = count1 * count2;
              if (count > 0) {
                // KEY
                entry_DELTA_AGG_D.a = a;
                entry_DELTA_AGG_D.c = c;

                // VALUE
                value_DELTA_AGG_D.count = count;
                value_DELTA_AGG_D.sums[0] = count * d;
                value_DELTA_AGG_D.cofactors[0] = count * d * d;

                DELTA_AGG_D.addOrDelOnZero(entry_DELTA_AGG_D, value_DELTA_AGG_D);
              }

              n17 = (n17 != n17_head ? n17->nxt : n17->child);
            }
          }

          BASE_TWITTER3.addOrDelOnZero(entry_BASE_TWITTER3.modify(c, d), count1);
        }
      }

      DELTA_AGG_C.clear();
      { //foreach
        DELTA_AGG_D_entry* e19 = DELTA_AGG_D.head;
        while (e19) {
          DOUBLE_TYPE a = e19->a;
          DOUBLE_TYPE c = e19->c;
          const DELTA_AGG_D_value &vDELTA_ADD_D = e19->__av;
          long count1 = vDELTA_ADD_D.count;
          
          { //slice
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n20_head = static_cast<const SecondaryIdxNode<BASE_TWITTER2_entry>*>(BASE_TWITTER2.slice(entry_BASE_TWITTER2.modify1(c), 0));
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n20 = n20_head;
            BASE_TWITTER2_entry* e20;
            while (n20) {
              e20 = n20->obj;
              DOUBLE_TYPE b = e20->b;
              long count2 = e20->__av;

              long count = count1 * count2;
              if (count > 0) {

                // KEY
                entry_DELTA_AGG_C.a = a;
                entry_DELTA_AGG_C.b = b;

                // VALUE
                value_DELTA_AGG_C.count = count;
                value_DELTA_AGG_C.sums[0] = count * c;
                value_DELTA_AGG_C.sums[1] = count2 * vDELTA_ADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[0] = count * c * c;
                value_DELTA_AGG_C.cofactors[1] = count2 * c * vDELTA_ADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[2] = count2 * vDELTA_ADD_D.cofactors[0];

                DELTA_AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, value_DELTA_AGG_C);
              }

              n20 = (n20 != n20_head ? n20->nxt : n20->child);
            }
          }

          entry_DELTA_AGG_D.a = a;
          entry_DELTA_AGG_D.c = c;
          AGG_D.addOrDelOnZero(entry_DELTA_AGG_D, vDELTA_ADD_D);

          e19 = e19->nxt;
        }
      }

      { //foreach
        DELTA_AGG_C_entry* e4 = DELTA_AGG_C.head;
        while (e4) {
          DOUBLE_TYPE a = e4->a;
          DOUBLE_TYPE b = e4->b;
          const DELTA_AGG_C_value &vDELTA_AGG_C = e4->__av;
          
          long count1 = vDELTA_AGG_C.count;
          long count2 = BASE_TWITTER1.getValueOrDefault(entry_BASE_TWITTER1.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vDELTA_AGG_C.sums[0];
            AGG.cofactors[3] += count2 * a * vDELTA_AGG_C.sums[1];
            AGG.cofactors[4] += count * b * b;
            AGG.cofactors[5] += count2 * b * vDELTA_AGG_C.sums[0];
            AGG.cofactors[6] += count2 * b * vDELTA_AGG_C.sums[1];
            AGG.cofactors[7] += count2 * vDELTA_AGG_C.cofactors[0];
            AGG.cofactors[8] += count2 * vDELTA_AGG_C.cofactors[1];
            AGG.cofactors[9] += count2 * vDELTA_AGG_C.cofactors[2];
          }

          entry_DELTA_AGG_C.a = a;
          entry_DELTA_AGG_C.b = b;
          AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, vDELTA_AGG_C);

          e4 = e4->nxt;
        }
      }
    }
    
    void on_batch_update_TWITTER4(const std::vector<TWITTER4_entry>::iterator &begin, const std::vector<TWITTER4_entry>::iterator &end) {
      tN += std::distance(begin, end);
      DELTA_AGG_D.clear();
      { //foreach
        for (std::vector<DELTA_TWITTER4_entry>::iterator e29 = begin; e29 != end; e29++) {
          DOUBLE_TYPE a = e29->a;
          DOUBLE_TYPE d = e29->d;
          long count1 = e29->__av;
          { //slice
            const SecondaryIdxNode<BASE_TWITTER3_entry>* n30_head = static_cast<const SecondaryIdxNode<BASE_TWITTER3_entry>*>(BASE_TWITTER3.slice(entry_BASE_TWITTER3.modify1(d), 0));
            const SecondaryIdxNode<BASE_TWITTER3_entry>* n30 = n30_head;
            BASE_TWITTER3_entry* e30;
            while (n30) {
              e30 = n30->obj;
              DOUBLE_TYPE c = e30->c;
              long count2 = e30->__av;

              long count = count1 * count2;
              if (count > 0) {
                // KEY
                entry_DELTA_AGG_D.a = a;
                entry_DELTA_AGG_D.c = c;

                // VALUE
                value_DELTA_AGG_D.count = count;
                value_DELTA_AGG_D.sums[0] = count * d;
                value_DELTA_AGG_D.cofactors[0] = count * d * d;

                DELTA_AGG_D.addOrDelOnZero(entry_DELTA_AGG_D, value_DELTA_AGG_D);
              }

              n30 = (n30 != n30_head ? n30->nxt : n30->child);
            }
          }

          BASE_TWITTER4.addOrDelOnZero(entry_BASE_TWITTER4.modify(a, d), count1);
        }
      }

      DELTA_AGG_C.clear();
      { //foreach
        DELTA_AGG_D_entry* e19 = DELTA_AGG_D.head;
        while (e19) {
          DOUBLE_TYPE a = e19->a;
          DOUBLE_TYPE c = e19->c;
          const DELTA_AGG_D_value &vDELTA_ADD_D = e19->__av;
          long count1 = vDELTA_ADD_D.count;
          
          { //slice
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n20_head = static_cast<const SecondaryIdxNode<BASE_TWITTER2_entry>*>(BASE_TWITTER2.slice(entry_BASE_TWITTER2.modify1(c), 0));
            const SecondaryIdxNode<BASE_TWITTER2_entry>* n20 = n20_head;
            BASE_TWITTER2_entry* e20;
            while (n20) {
              e20 = n20->obj;
              DOUBLE_TYPE b = e20->b;
              long count2 = e20->__av;

              long count = count1 * count2;
              if (count > 0) {

                // KEY
                entry_DELTA_AGG_C.a = a;
                entry_DELTA_AGG_C.b = b;

                // VALUE
                value_DELTA_AGG_C.count = count;
                value_DELTA_AGG_C.sums[0] = count * c;
                value_DELTA_AGG_C.sums[1] = count2 * vDELTA_ADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[0] = count * c * c;
                value_DELTA_AGG_C.cofactors[1] = count2 * c * vDELTA_ADD_D.sums[0];
                value_DELTA_AGG_C.cofactors[2] = count2 * vDELTA_ADD_D.cofactors[0];

                DELTA_AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, value_DELTA_AGG_C);
              }

              n20 = (n20 != n20_head ? n20->nxt : n20->child);
            }
          }

          entry_DELTA_AGG_D.a = a;
          entry_DELTA_AGG_D.c = c;
          AGG_D.addOrDelOnZero(entry_DELTA_AGG_D, vDELTA_ADD_D);

          e19 = e19->nxt;
        }
      }

      { //foreach
        DELTA_AGG_C_entry* e4 = DELTA_AGG_C.head;
        while (e4) {
          DOUBLE_TYPE a = e4->a;
          DOUBLE_TYPE b = e4->b;
          const DELTA_AGG_C_value &vDELTA_AGG_C = e4->__av;
          
          long count1 = vDELTA_AGG_C.count;
          long count2 = BASE_TWITTER1.getValueOrDefault(entry_BASE_TWITTER1.modify(a,b));
          long count = count1 * count2;

          if (count > 0) {
            AGG.cofactors[0] += count * a * a;
            AGG.cofactors[1] += count * a * b;
            AGG.cofactors[2] += count2 * a * vDELTA_AGG_C.sums[0];
            AGG.cofactors[3] += count2 * a * vDELTA_AGG_C.sums[1];
            AGG.cofactors[4] += count * b * b;
            AGG.cofactors[5] += count2 * b * vDELTA_AGG_C.sums[0];
            AGG.cofactors[6] += count2 * b * vDELTA_AGG_C.sums[1];
            AGG.cofactors[7] += count2 * vDELTA_AGG_C.cofactors[0];
            AGG.cofactors[8] += count2 * vDELTA_AGG_C.cofactors[1];
            AGG.cofactors[9] += count2 * vDELTA_AGG_C.cofactors[2];
          }

          entry_DELTA_AGG_C.a = a;
          entry_DELTA_AGG_C.b = b;
          AGG_C.addOrDelOnZero(entry_DELTA_AGG_C, vDELTA_AGG_C);

          e4 = e4->nxt;
        }
      }
    }
    

    
    void on_system_ready_event() {
    }

  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    BASE_TWITTER1_entry entry_BASE_TWITTER1;
    BASE_TWITTER2_entry entry_BASE_TWITTER2;
    BASE_TWITTER3_entry entry_BASE_TWITTER3;
    BASE_TWITTER4_entry entry_BASE_TWITTER4;

    AGG_C_entry entry_AGG_C;
    AGG_C_value value_AGG_C;
    AGG_D_entry entry_AGG_D;
    AGG_D_value value_AGG_D;
    DELTA_AGG_C_entry entry_DELTA_AGG_C;
    DELTA_AGG_C_value value_DELTA_AGG_C;
    DELTA_AGG_D_entry entry_DELTA_AGG_D;
    DELTA_AGG_D_value value_DELTA_AGG_D;

    /* Data structures used for storing materialized views */
    BASE_TWITTER1_map BASE_TWITTER1;
    BASE_TWITTER2_map BASE_TWITTER2;
    BASE_TWITTER3_map BASE_TWITTER3;
    BASE_TWITTER4_map BASE_TWITTER4;
    AGG_D_map AGG_D;
    AGG_C_map AGG_C;
    DELTA_AGG_D_map DELTA_AGG_D;
    DELTA_AGG_C_map DELTA_AGG_C;
  };
}
        