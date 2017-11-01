#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"
#include "../templates/cofactor_value.cpp"

#define RELATION_HOUSE_STATIC
#define RELATION_SHOP_STATIC
#define RELATION_INSTITUTION_STATIC
#define RELATION_RESTAURANT_STATIC
#define RELATION_DEMOGRAPHICS_STATIC
#define RELATION_TRANSPORT_STATIC

#define AGG_HOUSE_SIZE 10
#define AGG_SHOP_SIZE 5
#define AGG_INSTITUTION_SIZE 2
#define AGG_RESTAURANT_SIZE 2
#define AGG_DEMOGRAPHICS_SIZE 4
#define AGG_TRANSPORT_SIZE 3
#define AGG_SIZE 27

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct HOUSE_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE h2; DOUBLE_TYPE h3; DOUBLE_TYPE h4; DOUBLE_TYPE h5; DOUBLE_TYPE h6; DOUBLE_TYPE h7; DOUBLE_TYPE h8; DOUBLE_TYPE h9; DOUBLE_TYPE h10; DOUBLE_TYPE h11; long __av; HOUSE_entry* nxt; HOUSE_entry* prv;
    explicit HOUSE_entry() : nxt(nullptr), prv(nullptr) { }
    explicit HOUSE_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const long c11) { postcode = c0; h2 = c1; h3 = c2; h4 = c3; h5 = c4; h6 = c5; h7 = c6; h8 = c7; h9 = c8; h10 = c9; h11 = c10; __av = c11;  }
    HOUSE_entry(const HOUSE_entry& other) : postcode(other.postcode), h2(other.h2), h3(other.h3), h4(other.h4), h5(other.h5), h6(other.h6), h7(other.h7), h8(other.h8), h9(other.h9), h10(other.h10), h11(other.h11), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    HOUSE_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 11) return; */ postcode = std::stod(f[0]); h2 = std::stod(f[1]); h3 = std::stod(f[2]); h4 = std::stod(f[3]); h5 = std::stod(f[4]); h6 = std::stod(f[5]); h7 = std::stod(f[6]); h8 = std::stod(f[7]); h9 = std::stod(f[8]); h10 = std::stod(f[9]); h11 = std::stod(f[10]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE HOUSE_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10) { postcode = c0; h2 = c1; h3 = c2; h4 = c3; h5 = c4; h6 = c5; h7 = c6; h8 = c7; h9 = c8; h10 = c9; h11 = c10;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h4);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h5);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h6);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h7);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h8);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h9);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h10);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, h11);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct HOUSE_mapkey012345678910_idxfn {
    FORCE_INLINE static size_t hash(const HOUSE_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.h2);
      hash_combine(h, e.h3);
      hash_combine(h, e.h4);
      hash_combine(h, e.h5);
      hash_combine(h, e.h6);
      hash_combine(h, e.h7);
      hash_combine(h, e.h8);
      hash_combine(h, e.h9);
      hash_combine(h, e.h10);
      hash_combine(h, e.h11);
      return h;
    }
    FORCE_INLINE static bool equals(const HOUSE_entry& x, const HOUSE_entry& y) {
      return x.postcode == y.postcode && x.h2 == y.h2 && x.h3 == y.h3 && x.h4 == y.h4 && x.h5 == y.h5 && x.h6 == y.h6 && x.h7 == y.h7 && x.h8 == y.h8 && x.h9 == y.h9 && x.h10 == y.h10 && x.h11 == y.h11;
    }
  };
  
  typedef MultiHashMap<HOUSE_entry, long, 
    PrimaryHashIndex<HOUSE_entry, HOUSE_mapkey012345678910_idxfn>
  > HOUSE_map;
  struct SHOP_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE s2; DOUBLE_TYPE s3; DOUBLE_TYPE s4; DOUBLE_TYPE s5; DOUBLE_TYPE s6; long __av; SHOP_entry* nxt; SHOP_entry* prv;
    explicit SHOP_entry() : nxt(nullptr), prv(nullptr) { }
    explicit SHOP_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const long c6) { postcode = c0; s2 = c1; s3 = c2; s4 = c3; s5 = c4; s6 = c5; __av = c6;  }
    SHOP_entry(const SHOP_entry& other) : postcode(other.postcode), s2(other.s2), s3(other.s3), s4(other.s4), s5(other.s5), s6(other.s6), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    SHOP_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 6) return; */ postcode = std::stod(f[0]); s2 = std::stod(f[1]); s3 = std::stod(f[2]); s4 = std::stod(f[3]); s5 = std::stod(f[4]); s6 = std::stod(f[5]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE SHOP_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5) { postcode = c0; s2 = c1; s3 = c2; s4 = c3; s5 = c4; s6 = c5;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, s2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, s3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, s4);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, s5);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, s6);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct SHOP_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const SHOP_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.s2);
      hash_combine(h, e.s3);
      hash_combine(h, e.s4);
      hash_combine(h, e.s5);
      hash_combine(h, e.s6);
      return h;
    }
    FORCE_INLINE static bool equals(const SHOP_entry& x, const SHOP_entry& y) {
      return x.postcode == y.postcode && x.s2 == y.s2 && x.s3 == y.s3 && x.s4 == y.s4 && x.s5 == y.s5 && x.s6 == y.s6;
    }
  };
  
  typedef MultiHashMap<SHOP_entry, long, 
    PrimaryHashIndex<SHOP_entry, SHOP_mapkey012345_idxfn>
  > SHOP_map;
  struct INSTITUTION_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE i2; DOUBLE_TYPE i3; long __av; INSTITUTION_entry* nxt; INSTITUTION_entry* prv;
    explicit INSTITUTION_entry() : nxt(nullptr), prv(nullptr) { }
    explicit INSTITUTION_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const long c3) { postcode = c0; i2 = c1; i3 = c2; __av = c3;  }
    INSTITUTION_entry(const INSTITUTION_entry& other) : postcode(other.postcode), i2(other.i2), i3(other.i3), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    INSTITUTION_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 3) return; */ postcode = std::stod(f[0]); i2 = std::stod(f[1]); i3 = std::stod(f[2]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE INSTITUTION_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2) { postcode = c0; i2 = c1; i3 = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct INSTITUTION_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const INSTITUTION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.i2);
      hash_combine(h, e.i3);
      return h;
    }
    FORCE_INLINE static bool equals(const INSTITUTION_entry& x, const INSTITUTION_entry& y) {
      return x.postcode == y.postcode && x.i2 == y.i2 && x.i3 == y.i3;
    }
  };
  
  typedef MultiHashMap<INSTITUTION_entry, long, 
    PrimaryHashIndex<INSTITUTION_entry, INSTITUTION_mapkey012_idxfn>
  > INSTITUTION_map;
  struct RESTAURANT_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE r2; DOUBLE_TYPE r3; long __av; RESTAURANT_entry* nxt; RESTAURANT_entry* prv;
    explicit RESTAURANT_entry() : nxt(nullptr), prv(nullptr) { }
    explicit RESTAURANT_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const long c3) { postcode = c0; r2 = c1; r3 = c2; __av = c3;  }
    RESTAURANT_entry(const RESTAURANT_entry& other) : postcode(other.postcode), r2(other.r2), r3(other.r3), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    RESTAURANT_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 3) return; */ postcode = std::stod(f[0]); r2 = std::stod(f[1]); r3 = std::stod(f[2]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE RESTAURANT_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2) { postcode = c0; r2 = c1; r3 = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct RESTAURANT_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const RESTAURANT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.r2);
      hash_combine(h, e.r3);
      return h;
    }
    FORCE_INLINE static bool equals(const RESTAURANT_entry& x, const RESTAURANT_entry& y) {
      return x.postcode == y.postcode && x.r2 == y.r2 && x.r3 == y.r3;
    }
  };
  
  typedef MultiHashMap<RESTAURANT_entry, long, 
    PrimaryHashIndex<RESTAURANT_entry, RESTAURANT_mapkey012_idxfn>
  > RESTAURANT_map;
  struct DEMOGRAPHICS_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE d2; DOUBLE_TYPE d3; DOUBLE_TYPE d4; DOUBLE_TYPE d5; long __av; DEMOGRAPHICS_entry* nxt; DEMOGRAPHICS_entry* prv;
    explicit DEMOGRAPHICS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DEMOGRAPHICS_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const long c5) { postcode = c0; d2 = c1; d3 = c2; d4 = c3; d5 = c4; __av = c5;  }
    DEMOGRAPHICS_entry(const DEMOGRAPHICS_entry& other) : postcode(other.postcode), d2(other.d2), d3(other.d3), d4(other.d4), d5(other.d5), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DEMOGRAPHICS_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 5) return; */ postcode = std::stod(f[0]); d2 = std::stod(f[1]); d3 = std::stod(f[2]); d4 = std::stod(f[3]); d5 = std::stod(f[4]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE DEMOGRAPHICS_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4) { postcode = c0; d2 = c1; d3 = c2; d4 = c3; d5 = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d4);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d5);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DEMOGRAPHICS_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const DEMOGRAPHICS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.d2);
      hash_combine(h, e.d3);
      hash_combine(h, e.d4);
      hash_combine(h, e.d5);
      return h;
    }
    FORCE_INLINE static bool equals(const DEMOGRAPHICS_entry& x, const DEMOGRAPHICS_entry& y) {
      return x.postcode == y.postcode && x.d2 == y.d2 && x.d3 == y.d3 && x.d4 == y.d4 && x.d5 == y.d5;
    }
  };
  
  typedef MultiHashMap<DEMOGRAPHICS_entry, long, 
    PrimaryHashIndex<DEMOGRAPHICS_entry, DEMOGRAPHICS_mapkey01234_idxfn>
  > DEMOGRAPHICS_map;
  struct TRANSPORT_entry {
    DOUBLE_TYPE postcode; DOUBLE_TYPE t2; DOUBLE_TYPE t3; DOUBLE_TYPE t4; long __av; TRANSPORT_entry* nxt; TRANSPORT_entry* prv;
    explicit TRANSPORT_entry() : nxt(nullptr), prv(nullptr) { }
    explicit TRANSPORT_entry(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const long c4) { postcode = c0; t2 = c1; t3 = c2; t4 = c3; __av = c4;  }
    TRANSPORT_entry(const TRANSPORT_entry& other) : postcode(other.postcode), t2(other.t2), t3(other.t3), t4(other.t4), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    TRANSPORT_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 4) return; */ postcode = std::stod(f[0]); t2 = std::stod(f[1]); t3 = std::stod(f[2]); t4 = std::stod(f[3]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE TRANSPORT_entry& modify(const DOUBLE_TYPE c0, const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3) { postcode = c0; t2 = c1; t3 = c2; t4 = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, t2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, t3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, t4);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct TRANSPORT_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const TRANSPORT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      hash_combine(h, e.t2);
      hash_combine(h, e.t3);
      hash_combine(h, e.t4);
      return h;
    }
    FORCE_INLINE static bool equals(const TRANSPORT_entry& x, const TRANSPORT_entry& y) {
      return x.postcode == y.postcode && x.t2 == y.t2 && x.t3 == y.t3 && x.t4 == y.t4;
    }
  };
  
  typedef MultiHashMap<TRANSPORT_entry, long, 
    PrimaryHashIndex<TRANSPORT_entry, TRANSPORT_mapkey0123_idxfn>
  > TRANSPORT_map;
  
  // CUSTOM DATA STRUCTURES
  typedef AGG_value<AGG_HOUSE_SIZE> AGG_HOUSE_value;
  struct AGG_HOUSE_entry {
    DOUBLE_TYPE postcode; 
    AGG_HOUSE_value __av;  
    AGG_HOUSE_entry* nxt; 
    AGG_HOUSE_entry* prv;
    explicit AGG_HOUSE_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_HOUSE_mapkey012345678910_idxfn {
    FORCE_INLINE static size_t hash(const AGG_HOUSE_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_HOUSE_entry& x, const AGG_HOUSE_entry& y) {
      return x.postcode == y.postcode;
    }
  };
  
  typedef MultiHashMap<AGG_HOUSE_entry, AGG_HOUSE_value, 
    PrimaryHashIndex<AGG_HOUSE_entry, AGG_HOUSE_mapkey012345678910_idxfn>
  > AGG_HOUSE_map;
  
  typedef AGG_value<AGG_SHOP_SIZE> AGG_SHOP_value;
  struct AGG_SHOP_entry {
    DOUBLE_TYPE postcode; 
    AGG_SHOP_value __av;  
    AGG_SHOP_entry* nxt; 
    AGG_SHOP_entry* prv;
    explicit AGG_SHOP_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_SHOP_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const AGG_SHOP_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_SHOP_entry& x, const AGG_SHOP_entry& y) {
      return x.postcode == y.postcode;
    }
  };
  
  typedef MultiHashMap<AGG_SHOP_entry, AGG_SHOP_value, 
    PrimaryHashIndex<AGG_SHOP_entry, AGG_SHOP_mapkey012345_idxfn>
  > AGG_SHOP_map;
  
  typedef AGG_value<AGG_INSTITUTION_SIZE> AGG_INSTITUTION_value;
  struct AGG_INSTITUTION_entry {
    DOUBLE_TYPE postcode; 
    AGG_INSTITUTION_value __av; 
    AGG_INSTITUTION_entry* nxt; 
    AGG_INSTITUTION_entry* prv;
    explicit AGG_INSTITUTION_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct AGG_INSTITUTION_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_INSTITUTION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_INSTITUTION_entry& x, const AGG_INSTITUTION_entry& y) {
      return x.postcode == y.postcode;
    }
  };
   
  typedef MultiHashMap<AGG_INSTITUTION_entry, AGG_INSTITUTION_value, 
    PrimaryHashIndex<AGG_INSTITUTION_entry, AGG_INSTITUTION_mapkey012_idxfn>
  > AGG_INSTITUTION_map;
  
  typedef AGG_value<AGG_RESTAURANT_SIZE> AGG_RESTAURANT_value;
  struct AGG_RESTAURANT_entry {
    DOUBLE_TYPE postcode; 
    AGG_RESTAURANT_value __av;  
    AGG_RESTAURANT_entry* nxt; 
    AGG_RESTAURANT_entry* prv;

    explicit AGG_RESTAURANT_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_RESTAURANT_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_RESTAURANT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_RESTAURANT_entry& x, const AGG_RESTAURANT_entry& y) {
      return x.postcode == y.postcode;
    }
  };
    
  typedef MultiHashMap<AGG_RESTAURANT_entry, AGG_RESTAURANT_value, 
    PrimaryHashIndex<AGG_RESTAURANT_entry, AGG_RESTAURANT_mapkey012_idxfn>
  > AGG_RESTAURANT_map;
  

  typedef AGG_value<AGG_DEMOGRAPHICS_SIZE> AGG_DEMOGRAPHICS_value;
  struct AGG_DEMOGRAPHICS_entry {
    DOUBLE_TYPE postcode; 
    AGG_DEMOGRAPHICS_value __av;  
    AGG_DEMOGRAPHICS_entry* nxt; 
    AGG_DEMOGRAPHICS_entry* prv;
    explicit AGG_DEMOGRAPHICS_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_DEMOGRAPHICS_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const AGG_DEMOGRAPHICS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_DEMOGRAPHICS_entry& x, const AGG_DEMOGRAPHICS_entry& y) {
      return x.postcode == y.postcode;
    }
  };
  
  typedef MultiHashMap<AGG_DEMOGRAPHICS_entry, AGG_DEMOGRAPHICS_value, 
    PrimaryHashIndex<AGG_DEMOGRAPHICS_entry, AGG_DEMOGRAPHICS_mapkey01234_idxfn>
  > AGG_DEMOGRAPHICS_map;
  
  typedef AGG_value<AGG_TRANSPORT_SIZE> AGG_TRANSPORT_value;  
  struct AGG_TRANSPORT_entry {
    DOUBLE_TYPE postcode; 
    AGG_TRANSPORT_value __av;  
    AGG_TRANSPORT_entry* nxt; 
    AGG_TRANSPORT_entry* prv;
    explicit AGG_TRANSPORT_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, postcode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_TRANSPORT_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const AGG_TRANSPORT_entry& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const AGG_TRANSPORT_entry& x, const AGG_TRANSPORT_entry& y) {
      return x.postcode == y.postcode;
    }
  };
   
  typedef MultiHashMap<AGG_TRANSPORT_entry, AGG_TRANSPORT_value, 
    PrimaryHashIndex<AGG_TRANSPORT_entry, AGG_TRANSPORT_mapkey0123_idxfn>
  > AGG_TRANSPORT_map;
  
  typedef AGG_value<AGG_SIZE> AGG_Output_value; 
  
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
  
    /* Functions returning / computing the results of top level queries */
  
  protected:
    /* Data structures used for storing / computing top level queries */
    AGG_Output_value AGG;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t() {
      
    }
  
    long counter = 0;
  
    /* Trigger functions for table relations */
    void on_insert_HOUSE(const DOUBLE_TYPE house_postcode, const DOUBLE_TYPE house_h2, const DOUBLE_TYPE house_h3, const DOUBLE_TYPE house_h4, const DOUBLE_TYPE house_h5, const DOUBLE_TYPE house_h6, const DOUBLE_TYPE house_h7, const DOUBLE_TYPE house_h8, const DOUBLE_TYPE house_h9, const DOUBLE_TYPE house_h10, const DOUBLE_TYPE house_h11) {
      HOUSE_entry e(house_postcode, house_h2, house_h3, house_h4, house_h5, house_h6, house_h7, house_h8, house_h9, house_h10, house_h11, 1L);
      HOUSE.addOrDelOnZero(e, 1L);
    }
    void on_insert_HOUSE(HOUSE_entry &e) {
      e.__av = 1L;
      HOUSE.addOrDelOnZero(e, 1L);
    }
    void on_insert_SHOP(const DOUBLE_TYPE shop_postcode, const DOUBLE_TYPE shop_s2, const DOUBLE_TYPE shop_s3, const DOUBLE_TYPE shop_s4, const DOUBLE_TYPE shop_s5, const DOUBLE_TYPE shop_s6) {
      SHOP_entry e(shop_postcode, shop_s2, shop_s3, shop_s4, shop_s5, shop_s6, 1L);
      SHOP.addOrDelOnZero(e, 1L);
    }
    void on_insert_SHOP(SHOP_entry &e) {
      e.__av = 1L;
      SHOP.addOrDelOnZero(e, 1L);
    }
    void on_insert_INSTITUTION(const DOUBLE_TYPE institution_postcode, const DOUBLE_TYPE institution_i2, const DOUBLE_TYPE institution_i3) {
      INSTITUTION_entry e(institution_postcode, institution_i2, institution_i3, 1L);
      INSTITUTION.addOrDelOnZero(e, 1L);
    }
    void on_insert_INSTITUTION(INSTITUTION_entry &e) {
      e.__av = 1L;
      INSTITUTION.addOrDelOnZero(e, 1L);
    }
    void on_insert_RESTAURANT(const DOUBLE_TYPE restaurant_postcode, const DOUBLE_TYPE restaurant_r2, const DOUBLE_TYPE restaurant_r3) {
      RESTAURANT_entry e(restaurant_postcode, restaurant_r2, restaurant_r3, 1L);
      RESTAURANT.addOrDelOnZero(e, 1L);
    }
    void on_insert_RESTAURANT(RESTAURANT_entry &e) {
      e.__av = 1L;
      RESTAURANT.addOrDelOnZero(e, 1L);
    }
    void on_insert_DEMOGRAPHICS(const DOUBLE_TYPE demographics_postcode, const DOUBLE_TYPE demographics_d2, const DOUBLE_TYPE demographics_d3, const DOUBLE_TYPE demographics_d4, const DOUBLE_TYPE demographics_d5) {
      DEMOGRAPHICS_entry e(demographics_postcode, demographics_d2, demographics_d3, demographics_d4, demographics_d5, 1L);
      DEMOGRAPHICS.addOrDelOnZero(e, 1L);
    }
    void on_insert_DEMOGRAPHICS(DEMOGRAPHICS_entry &e) {
      e.__av = 1L;
      DEMOGRAPHICS.addOrDelOnZero(e, 1L);
    }
    void on_insert_TRANSPORT(const DOUBLE_TYPE transport_postcode, const DOUBLE_TYPE transport_t2, const DOUBLE_TYPE transport_t3, const DOUBLE_TYPE transport_t4) {
      TRANSPORT_entry e(transport_postcode, transport_t2, transport_t3, transport_t4, 1L);
      TRANSPORT.addOrDelOnZero(e, 1L);
    }
    void on_insert_TRANSPORT(TRANSPORT_entry &e) {
      e.__av = 1L;
      TRANSPORT.addOrDelOnZero(e, 1L);
    }

    
    /* Trigger functions for stream relations */
    void on_system_ready_event() {
      
      AGG_HOUSE.clear();
      { //foreach
        HOUSE_entry* e1 = HOUSE.head;
        while (e1) {
          DOUBLE_TYPE postcode = e1->postcode;
          DOUBLE_TYPE h2 = e1->h2;
          DOUBLE_TYPE h3 = e1->h3;
          DOUBLE_TYPE h4 = e1->h4;
          DOUBLE_TYPE h5 = e1->h5;
          DOUBLE_TYPE h6 = e1->h6;
          DOUBLE_TYPE h7 = e1->h7;
          DOUBLE_TYPE h8 = e1->h8;
          DOUBLE_TYPE h9 = e1->h9;
          DOUBLE_TYPE h10 = e1->h10;
          DOUBLE_TYPE h11 = e1->h11;
          long count = e1->__av;

          houseFields[0] = h2;
          houseFields[1] = h3;
          houseFields[2] = h4;
          houseFields[3] = h5;
          houseFields[4] = h6;
          houseFields[5] = h7;
          houseFields[6] = h8;
          houseFields[7] = h9;
          houseFields[8] = h10;
          houseFields[9] = h11;   

          // KEY
          entry_AGG_HOUSE.postcode = postcode;

          // VALUE
          value_AGG_HOUSE.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_HOUSE_SIZE; i++) {
            value_AGG_HOUSE.sums[indexOutput++] = count * houseFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_HOUSE_SIZE; i++) {
            for (size_t j = i; j < AGG_HOUSE_SIZE; j++) {
              value_AGG_HOUSE.cofactors[indexOutput++] = count * houseFields[i] * houseFields[j];
            }
          }

          AGG_HOUSE.addOrDelOnZero(entry_AGG_HOUSE, value_AGG_HOUSE);

          e1 = e1->nxt;
        }
      }

      AGG_SHOP.clear();
      { //foreach
        SHOP_entry* e3 = SHOP.head;
        while (e3) {
          DOUBLE_TYPE postcode = e3->postcode;
          DOUBLE_TYPE s2 = e3->s2;
          DOUBLE_TYPE s3 = e3->s3;
          DOUBLE_TYPE s4 = e3->s4;
          DOUBLE_TYPE s5 = e3->s5;
          DOUBLE_TYPE s6 = e3->s6;
          long count = e3->__av;

          shopFields[0] = s2;
          shopFields[1] = s3;
          shopFields[2] = s4;
          shopFields[3] = s5;
          shopFields[4] = s6;

          // KEY
          entry_AGG_SHOP.postcode = postcode;

          // VALUE
          value_AGG_SHOP.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_SHOP_SIZE; i++) {
            value_AGG_SHOP.sums[indexOutput++] = count * shopFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_SHOP_SIZE; i++) {
            for (size_t j = i; j < AGG_SHOP_SIZE; j++) {
              value_AGG_SHOP.cofactors[indexOutput++] = count * shopFields[i] * shopFields[j];
            }
          }

          AGG_SHOP.addOrDelOnZero(entry_AGG_SHOP, value_AGG_SHOP);

          e3 = e3->nxt;
        }
      }

      AGG_INSTITUTION.clear();
      { //foreach
        INSTITUTION_entry* e5 = INSTITUTION.head;
        while (e5) {
          DOUBLE_TYPE postcode = e5->postcode;
          DOUBLE_TYPE i2 = e5->i2;
          DOUBLE_TYPE i3 = e5->i3;
          long count = e5->__av;

          institutionFields[0] = i2;
          institutionFields[1] = i3;

          // KEY
          entry_AGG_INSTITUTION.postcode = postcode;

          // VALUE
          value_AGG_INSTITUTION.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_INSTITUTION_SIZE; i++) {
            value_AGG_INSTITUTION.sums[indexOutput++] = count * institutionFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_INSTITUTION_SIZE; i++) {
            for (size_t j = i; j < AGG_INSTITUTION_SIZE; j++) {
              value_AGG_INSTITUTION.cofactors[indexOutput++] = count * institutionFields[i] * institutionFields[j];
            }
          }

          AGG_INSTITUTION.addOrDelOnZero(entry_AGG_INSTITUTION, value_AGG_INSTITUTION);

          e5 = e5->nxt;
        }
      }
      AGG_RESTAURANT.clear();
      { //foreach
        RESTAURANT_entry* e7 = RESTAURANT.head;
        while (e7) {
          DOUBLE_TYPE postcode = e7->postcode;
          DOUBLE_TYPE r2 = e7->r2;
          DOUBLE_TYPE r3 = e7->r3;
          long count = e7->__av;

          restaurantFields[0] = r2;
          restaurantFields[1] = r3;

          // KEY
          entry_AGG_RESTAURANT.postcode = postcode;

          // VALUE
          value_AGG_RESTAURANT.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_RESTAURANT_SIZE; i++) {
            value_AGG_RESTAURANT.sums[indexOutput++] = count * restaurantFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_RESTAURANT_SIZE; i++) {
            for (size_t j = i; j < AGG_RESTAURANT_SIZE; j++) {
              value_AGG_RESTAURANT.cofactors[indexOutput++] = count * restaurantFields[i] * restaurantFields[j];
            }
          }

          AGG_RESTAURANT.addOrDelOnZero(entry_AGG_RESTAURANT, value_AGG_RESTAURANT);

          e7 = e7->nxt;
        }
      }
      AGG_DEMOGRAPHICS.clear();
      { //foreach
        DEMOGRAPHICS_entry* e9 = DEMOGRAPHICS.head;
        while (e9) {
          DOUBLE_TYPE postcode = e9->postcode;
          DOUBLE_TYPE d2 = e9->d2;
          DOUBLE_TYPE d3 = e9->d3;
          DOUBLE_TYPE d4 = e9->d4;
          DOUBLE_TYPE d5 = e9->d5;
          long count = e9->__av;

          demographicsFields[0] = d2;
          demographicsFields[1] = d3;
          demographicsFields[2] = d4;
          demographicsFields[3] = d5;

          // KEY
          entry_AGG_DEMOGRAPHICS.postcode = postcode;

          // VALUE
          value_AGG_DEMOGRAPHICS.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_DEMOGRAPHICS_SIZE; i++) {
            value_AGG_DEMOGRAPHICS.sums[indexOutput++] = count * demographicsFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_DEMOGRAPHICS_SIZE; i++) {
            for (size_t j = i; j < AGG_DEMOGRAPHICS_SIZE; j++) {
              value_AGG_DEMOGRAPHICS.cofactors[indexOutput++] = count * demographicsFields[i] * demographicsFields[j];
            }
          }

          AGG_DEMOGRAPHICS.addOrDelOnZero(entry_AGG_DEMOGRAPHICS, value_AGG_DEMOGRAPHICS);

          e9 = e9->nxt;
        }
      }
      AGG_TRANSPORT.clear();
      { //foreach
        TRANSPORT_entry* e11 = TRANSPORT.head;
        while (e11) {
          DOUBLE_TYPE postcode = e11->postcode;
          DOUBLE_TYPE t2 = e11->t2;
          DOUBLE_TYPE t3 = e11->t3;
          DOUBLE_TYPE t4 = e11->t4;
          long count = e11->__av;

          transportFields[0] = t2;
          transportFields[1] = t3;
          transportFields[2] = t4;

          // KEY
          entry_AGG_TRANSPORT.postcode = postcode;

          // VALUE
          value_AGG_TRANSPORT.count = count;
          size_t indexOutput = 0;
          for (size_t i = 0; i < AGG_TRANSPORT_SIZE; i++) {
            value_AGG_TRANSPORT.sums[indexOutput++] = count * transportFields[i];
          }
          indexOutput = 0;
          for (size_t i = 0; i < AGG_TRANSPORT_SIZE; i++) {
            for (size_t j = i; j < AGG_TRANSPORT_SIZE; j++) {
              value_AGG_TRANSPORT.cofactors[indexOutput++] = count * transportFields[i] * transportFields[j];
            }
          }

          AGG_TRANSPORT.addOrDelOnZero(entry_AGG_TRANSPORT, value_AGG_TRANSPORT);

          e11 = e11->nxt;
        }
      }

     AGG.clear();
     { //foreach
       AGG_HOUSE_entry* e13 = AGG_HOUSE.head;
       while (e13) {
         DOUBLE_TYPE postcode = e13->postcode;
         const AGG_HOUSE_value &vHouse = e13->__av;

         entry_AGG_SHOP.postcode = postcode;
         const AGG_SHOP_value &vShop = AGG_SHOP.getValueOrDefault(entry_AGG_SHOP);

         entry_AGG_INSTITUTION.postcode = postcode;
         const AGG_INSTITUTION_value &vInstitution = AGG_INSTITUTION.getValueOrDefault(entry_AGG_INSTITUTION);

         entry_AGG_RESTAURANT.postcode = postcode;
         const AGG_RESTAURANT_value &vRestaurant = AGG_RESTAURANT.getValueOrDefault(entry_AGG_RESTAURANT);

         entry_AGG_DEMOGRAPHICS.postcode = postcode;
         const AGG_DEMOGRAPHICS_value &vDemographics = AGG_DEMOGRAPHICS.getValueOrDefault(entry_AGG_DEMOGRAPHICS);

         entry_AGG_TRANSPORT.postcode = postcode;
         const AGG_TRANSPORT_value &vTransport = AGG_TRANSPORT.getValueOrDefault(entry_AGG_TRANSPORT);

         long count1 = vHouse.count;
         long count2 = vShop.count;
         long count3 = vInstitution.count;
         long count4 = vRestaurant.count;
         long count5 = vDemographics.count;
         long count6 = vTransport.count;

         AGG.count += count1 * count2 * count3 * count4 * count5 * count6;

         size_t indexOutput = 0;
         AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * count5 * count6 * postcode * postcode;

         for (size_t i = 0; i < AGG_HOUSE_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count2 * count3 * count4 * count5 * count6 * postcode * vHouse.sums[i];
         }
         for (size_t i = 0; i < AGG_SHOP_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count1 * count3 * count4 * count5 * count6 * postcode * vShop.sums[i];
         }
         for (size_t i = 0; i < AGG_INSTITUTION_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count1 * count2 * count4 * count5 * count6 * postcode * vInstitution.sums[i];
         }
         for (size_t i = 0; i < AGG_RESTAURANT_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count5 * count6 * postcode * vRestaurant.sums[i];
         }
         for (size_t i = 0; i < AGG_DEMOGRAPHICS_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * count6 * postcode * vDemographics.sums[i];
         }
         for (size_t i = 0; i < AGG_TRANSPORT_SIZE; i++) {
           AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * count5 * postcode * vTransport.sums[i];
         }

         size_t indexInput = 0;
         for (size_t i = 0; i < AGG_HOUSE_SIZE; i++) {
           for (size_t j = i; j < AGG_HOUSE_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count2 * count3 * count4 * count5 * count6 * vHouse.cofactors[indexInput++];
           }
           for (size_t j = 0; j < AGG_SHOP_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count3 * count4 * count5 * count6 * vHouse.sums[i] * vShop.sums[j]; 
           }
           for (size_t j = 0; j < AGG_INSTITUTION_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count2 * count4 * count5 * count6 * vHouse.sums[i] * vInstitution.sums[j]; 
           }
           for (size_t j = 0; j < AGG_RESTAURANT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count2 * count3 * count5 * count6 * vHouse.sums[i] * vRestaurant.sums[j]; 
           }
           for (size_t j = 0; j < AGG_DEMOGRAPHICS_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count2 * count3 * count4 * count6 * vHouse.sums[i] * vDemographics.sums[j]; 
           }
           for (size_t j = 0; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count2 * count3 * count4 * count5 * vHouse.sums[i] * vTransport.sums[j]; 
           }
         }

         indexInput = 0;
         for (size_t i = 0; i < AGG_SHOP_SIZE; i++) {
           for (size_t j = i; j < AGG_SHOP_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count3 * count4 * count5 * count6 * vShop.cofactors[indexInput++];
           }
           for (size_t j = 0; j < AGG_INSTITUTION_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count4 * count5 * count6 * vShop.sums[i] * vInstitution.sums[j]; 
           }
           for (size_t j = 0; j < AGG_RESTAURANT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count3 * count5 * count6 * vShop.sums[i] * vRestaurant.sums[j]; 
           }
           for (size_t j = 0; j < AGG_DEMOGRAPHICS_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count3 * count4 * count6 * vShop.sums[i] * vDemographics.sums[j]; 
           }
           for (size_t j = 0; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count3 * count4 * count5 * vShop.sums[i] * vTransport.sums[j]; 
           }
         }

         indexInput = 0;
         for (size_t i = 0; i < AGG_INSTITUTION_SIZE; i++) {
           for (size_t j = i; j < AGG_INSTITUTION_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count4 * count5 * count6 * vInstitution.cofactors[indexInput++];
           }
           for (size_t j = 0; j < AGG_RESTAURANT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count5 * count6 * vInstitution.sums[i] * vRestaurant.sums[j]; 
           }
           for (size_t j = 0; j < AGG_DEMOGRAPHICS_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count4 * count6 * vInstitution.sums[i] * vDemographics.sums[j]; 
           }
           for (size_t j = 0; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count4 * count5 * vInstitution.sums[i] * vTransport.sums[j]; 
           }
         }

         indexInput = 0;
         for (size_t i = 0; i < AGG_RESTAURANT_SIZE; i++) {
           for (size_t j = i; j < AGG_RESTAURANT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count5 * count6 * vRestaurant.cofactors[indexInput++];
           }
           for (size_t j = 0; j < AGG_DEMOGRAPHICS_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count6 * vRestaurant.sums[i] * vDemographics.sums[j]; 
           }
           for (size_t j = 0; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count5 * vRestaurant.sums[i] * vTransport.sums[j]; 
           }
         }


         indexInput = 0;
         for (size_t i = 0; i < AGG_DEMOGRAPHICS_SIZE; i++) {
           for (size_t j = i; j < AGG_DEMOGRAPHICS_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * count6 * vDemographics.cofactors[indexInput++];
           }
           for (size_t j = 0; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * vDemographics.sums[i] * vTransport.sums[j]; 
           }
         }

         indexInput = 0;
         for (size_t i = 0; i < AGG_TRANSPORT_SIZE; i++) {
           for (size_t j = i; j < AGG_TRANSPORT_SIZE; j++) {
             AGG.cofactors[indexOutput++] += count1 * count2 * count3 * count4 * count5 * vTransport.cofactors[indexInput++];
           }
         }

         e13 = e13->nxt;
       }
     }
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    AGG_HOUSE_entry entry_AGG_HOUSE;
    AGG_SHOP_entry entry_AGG_SHOP;
    AGG_INSTITUTION_entry entry_AGG_INSTITUTION;
    AGG_RESTAURANT_entry entry_AGG_RESTAURANT;
    AGG_DEMOGRAPHICS_entry entry_AGG_DEMOGRAPHICS;
    AGG_TRANSPORT_entry entry_AGG_TRANSPORT;

    AGG_HOUSE_value value_AGG_HOUSE;
    AGG_SHOP_value value_AGG_SHOP;
    AGG_INSTITUTION_value value_AGG_INSTITUTION;
    AGG_RESTAURANT_value value_AGG_RESTAURANT;
    AGG_DEMOGRAPHICS_value value_AGG_DEMOGRAPHICS;
    AGG_TRANSPORT_value value_AGG_TRANSPORT;    

    DOUBLE_TYPE houseFields[AGG_HOUSE_SIZE];
    DOUBLE_TYPE shopFields[AGG_SHOP_SIZE];
    DOUBLE_TYPE institutionFields[AGG_INSTITUTION_SIZE];
    DOUBLE_TYPE restaurantFields[AGG_RESTAURANT_SIZE];
    DOUBLE_TYPE demographicsFields[AGG_DEMOGRAPHICS_SIZE];
    DOUBLE_TYPE transportFields[AGG_TRANSPORT_SIZE];

  
    /* Data structures used for storing materialized views */
    HOUSE_map HOUSE;
    SHOP_map SHOP;
    INSTITUTION_map INSTITUTION;
    RESTAURANT_map RESTAURANT;
    DEMOGRAPHICS_map DEMOGRAPHICS;
    TRANSPORT_map TRANSPORT;

    AGG_HOUSE_map AGG_HOUSE;
    AGG_SHOP_map AGG_SHOP;
    AGG_INSTITUTION_map AGG_INSTITUTION;
    AGG_RESTAURANT_map AGG_RESTAURANT;
    AGG_DEMOGRAPHICS_map AGG_DEMOGRAPHICS;
    AGG_TRANSPORT_map AGG_TRANSPORT;
  };
}
        