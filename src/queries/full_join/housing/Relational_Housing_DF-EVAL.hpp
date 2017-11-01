#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_HOUSE_STATIC
#define RELATION_SHOP_STATIC
#define RELATION_INSTITUTION_STATIC
#define RELATION_RESTAURANT_STATIC
#define RELATION_DEMOGRAPHICS_STATIC
#define RELATION_TRANSPORT_STATIC

namespace dbtoaster {
  
  struct PAYLOAD_H2_HOUSE {
    DOUBLE_TYPE h2; DOUBLE_TYPE h3; DOUBLE_TYPE h4; DOUBLE_TYPE h5; DOUBLE_TYPE h6; DOUBLE_TYPE h7; DOUBLE_TYPE h8; DOUBLE_TYPE h9; DOUBLE_TYPE h10; DOUBLE_TYPE h11; long __av; PAYLOAD_H2_HOUSE* nxt; PAYLOAD_H2_HOUSE* prv;
    explicit PAYLOAD_H2_HOUSE() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_H2_HOUSE(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const long c11) { h2 = c1; h3 = c2; h4 = c3; h5 = c4; h6 = c5; h7 = c6; h8 = c7; h9 = c8; h10 = c9; h11 = c10; __av = c11;  }
    PAYLOAD_H2_HOUSE(const PAYLOAD_H2_HOUSE& other) : h2(other.h2), h3(other.h3), h4(other.h4), h5(other.h5), h6(other.h6), h7(other.h7), h8(other.h8), h9(other.h9), h10(other.h10), h11(other.h11), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_H2_HOUSE& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10) { h2 = c1; h3 = c2; h4 = c3; h5 = c4; h6 = c5; h7 = c6; h8 = c7; h9 = c8; h10 = c9; h11 = c10;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
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
  
  struct PAYLOAD_H2_HOUSE_mapkey012345678910_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_H2_HOUSE& e) {
      size_t h = 0;
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
    FORCE_INLINE static bool equals(const PAYLOAD_H2_HOUSE& x, const PAYLOAD_H2_HOUSE& y) {
      return x.h2 == y.h2 && x.h3 == y.h3 && x.h4 == y.h4 && x.h5 == y.h5 && x.h6 == y.h6 && x.h7 == y.h7 && x.h8 == y.h8 && x.h9 == y.h9 && x.h10 == y.h10 && x.h11 == y.h11;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_H2_HOUSE, long, 
    PrimaryHashIndex<PAYLOAD_H2_HOUSE, PAYLOAD_H2_HOUSE_mapkey012345678910_idxfn>
  > PAYLOAD_H2_HOUSE_map;
    

  struct PAYLOAD_S2_SHOP {
    DOUBLE_TYPE s2; DOUBLE_TYPE s3; DOUBLE_TYPE s4; DOUBLE_TYPE s5; DOUBLE_TYPE s6; long __av; PAYLOAD_S2_SHOP* nxt; PAYLOAD_S2_SHOP* prv;
    explicit PAYLOAD_S2_SHOP() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_S2_SHOP(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const long c6) { s2 = c1; s3 = c2; s4 = c3; s5 = c4; s6 = c5; __av = c6;  }
    PAYLOAD_S2_SHOP(const PAYLOAD_S2_SHOP& other) : s2(other.s2), s3(other.s3), s4(other.s4), s5(other.s5), s6(other.s6), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_S2_SHOP& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5) { s2 = c1; s3 = c2; s4 = c3; s5 = c4; s6 = c5;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
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
  
  struct PAYLOAD_S2_SHOP_mapkey012345_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_S2_SHOP& e) {
      size_t h = 0;
      hash_combine(h, e.s2);
      hash_combine(h, e.s3);
      hash_combine(h, e.s4);
      hash_combine(h, e.s5);
      hash_combine(h, e.s6);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_S2_SHOP& x, const PAYLOAD_S2_SHOP& y) {
      return x.s2 == y.s2 && x.s3 == y.s3 && x.s4 == y.s4 && x.s5 == y.s5 && x.s6 == y.s6;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_S2_SHOP, long, 
    PrimaryHashIndex<PAYLOAD_S2_SHOP, PAYLOAD_S2_SHOP_mapkey012345_idxfn>
  > PAYLOAD_S2_SHOP_map;


  struct PAYLOAD_I2_INSTITUTION {
    DOUBLE_TYPE i2; DOUBLE_TYPE i3; long __av; PAYLOAD_I2_INSTITUTION* nxt; PAYLOAD_I2_INSTITUTION* prv;
    explicit PAYLOAD_I2_INSTITUTION() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_I2_INSTITUTION(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const long c3) { i2 = c1; i3 = c2; __av = c3;  }
    PAYLOAD_I2_INSTITUTION(const PAYLOAD_I2_INSTITUTION& other) : i2(other.i2), i3(other.i3), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_I2_INSTITUTION& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2) { i2 = c1; i3 = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_I2_INSTITUTION_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_I2_INSTITUTION& e) {
      size_t h = 0;
      hash_combine(h, e.i2);
      hash_combine(h, e.i3);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_I2_INSTITUTION& x, const PAYLOAD_I2_INSTITUTION& y) {
      return x.i2 == y.i2 && x.i3 == y.i3;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_I2_INSTITUTION, long, 
    PrimaryHashIndex<PAYLOAD_I2_INSTITUTION, PAYLOAD_I2_INSTITUTION_mapkey012_idxfn>
  > PAYLOAD_I2_INSTITUTION_map;

  struct PAYLOAD_R2_RESTAURANT {
    DOUBLE_TYPE r2; DOUBLE_TYPE r3; long __av; PAYLOAD_R2_RESTAURANT* nxt; PAYLOAD_R2_RESTAURANT* prv;
    explicit PAYLOAD_R2_RESTAURANT() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_R2_RESTAURANT(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const long c3) { r2 = c1; r3 = c2; __av = c3;  }
    PAYLOAD_R2_RESTAURANT(const PAYLOAD_R2_RESTAURANT& other) : r2(other.r2), r3(other.r3), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_R2_RESTAURANT& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2) { r2 = c1; r3 = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_R2_RESTAURANT_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_R2_RESTAURANT& e) {
      size_t h = 0;
      hash_combine(h, e.r2);
      hash_combine(h, e.r3);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_R2_RESTAURANT& x, const PAYLOAD_R2_RESTAURANT& y) {
      return x.r2 == y.r2 && x.r3 == y.r3;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_R2_RESTAURANT, long, 
    PrimaryHashIndex<PAYLOAD_R2_RESTAURANT, PAYLOAD_R2_RESTAURANT_mapkey012_idxfn>
  > PAYLOAD_R2_RESTAURANT_map;
 
  struct PAYLOAD_D2_DEMOGRAPHICS {
    DOUBLE_TYPE d2; DOUBLE_TYPE d3; DOUBLE_TYPE d4; DOUBLE_TYPE d5; long __av; PAYLOAD_D2_DEMOGRAPHICS* nxt; PAYLOAD_D2_DEMOGRAPHICS* prv;
    explicit PAYLOAD_D2_DEMOGRAPHICS() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_D2_DEMOGRAPHICS(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const long c5) { d2 = c1; d3 = c2; d4 = c3; d5 = c4; __av = c5;  }
    PAYLOAD_D2_DEMOGRAPHICS(const PAYLOAD_D2_DEMOGRAPHICS& other) : d2(other.d2), d3(other.d3), d4(other.d4), d5(other.d5), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_D2_DEMOGRAPHICS& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4) { d2 = c1; d3 = c2; d4 = c3; d5 = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
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
  
  struct PAYLOAD_D2_DEMOGRAPHICS_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_D2_DEMOGRAPHICS& e) {
      size_t h = 0;
      hash_combine(h, e.d2);
      hash_combine(h, e.d3);
      hash_combine(h, e.d4);
      hash_combine(h, e.d5);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_D2_DEMOGRAPHICS& x, const PAYLOAD_D2_DEMOGRAPHICS& y) {
      return x.d2 == y.d2 && x.d3 == y.d3 && x.d4 == y.d4 && x.d5 == y.d5;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_D2_DEMOGRAPHICS, long, 
    PrimaryHashIndex<PAYLOAD_D2_DEMOGRAPHICS, PAYLOAD_D2_DEMOGRAPHICS_mapkey01234_idxfn>
  > PAYLOAD_D2_DEMOGRAPHICS_map;


  struct PAYLOAD_T2_TRANSPORT {
    DOUBLE_TYPE t2; DOUBLE_TYPE t3; DOUBLE_TYPE t4; long __av; PAYLOAD_T2_TRANSPORT* nxt; PAYLOAD_T2_TRANSPORT* prv;
    explicit PAYLOAD_T2_TRANSPORT() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_T2_TRANSPORT(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const long c4) { t2 = c1; t3 = c2; t4 = c3; __av = c4;  }
    PAYLOAD_T2_TRANSPORT(const PAYLOAD_T2_TRANSPORT& other) : t2(other.t2), t3(other.t3), t4(other.t4), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_T2_TRANSPORT& modify(const DOUBLE_TYPE c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3) { t2 = c1; t3 = c2; t4 = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
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
  
  struct PAYLOAD_T2_TRANSPORT_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_T2_TRANSPORT& e) {
      size_t h = 0;
      hash_combine(h, e.t2);
      hash_combine(h, e.t3);
      hash_combine(h, e.t4);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_T2_TRANSPORT& x, const PAYLOAD_T2_TRANSPORT& y) {
      return x.t2 == y.t2 && x.t3 == y.t3 && x.t4 == y.t4;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_T2_TRANSPORT, long, 
    PrimaryHashIndex<PAYLOAD_T2_TRANSPORT, PAYLOAD_T2_TRANSPORT_mapkey0123_idxfn>
  > PAYLOAD_T2_TRANSPORT_map;


  template<typename V>
  struct KEY_Generic {
    DOUBLE_TYPE postcode;  V __av; KEY_Generic* nxt; KEY_Generic* prv;
    explicit KEY_Generic() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_Generic& modify(const DOUBLE_TYPE c0) { postcode = c0;  return *this; }
    static bool equals(const KEY_Generic &x, const KEY_Generic &y) {
      return ((x.postcode==y.postcode));
    }
    static long hash(const KEY_Generic &e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
  };  

  template<typename V>
  struct KEY_Generic_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const KEY_Generic<V>& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_Generic<V>& x, const KEY_Generic<V>& y) {
      return x.postcode == y.postcode;
    }
  };

  typedef KEY_Generic<PAYLOAD_H2_HOUSE_map> KEY_H2_HOUSE;
  typedef KEY_Generic<PAYLOAD_S2_SHOP_map> KEY_S2_SHOP;
  typedef KEY_Generic<PAYLOAD_I2_INSTITUTION_map> KEY_I2_INSTITUTION;
  typedef KEY_Generic<PAYLOAD_R2_RESTAURANT_map> KEY_R2_RESTAURANT;
  typedef KEY_Generic<PAYLOAD_D2_DEMOGRAPHICS_map> KEY_D2_DEMOGRAPHICS;
  typedef KEY_Generic<PAYLOAD_T2_TRANSPORT_map> KEY_T2_TRANSPORT;

  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_H2_HOUSE_map> KEY_H2_HOUSE_mapkey0_idxfn;
  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_S2_SHOP_map> KEY_S2_SHOP_mapkey0_idxfn;
  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_I2_INSTITUTION_map> KEY_I2_INSTITUTION_mapkey0_idxfn;
  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_R2_RESTAURANT_map> KEY_R2_RESTAURANT_mapkey0_idxfn;
  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_D2_DEMOGRAPHICS_map> KEY_D2_DEMOGRAPHICS_mapkey0_idxfn;
  typedef KEY_Generic_mapkey0_idxfn<PAYLOAD_T2_TRANSPORT_map> KEY_T2_TRANSPORT_mapkey0_idxfn;


  struct PAYLOAD_POSTCODE {
    DOUBLE_TYPE postcode; DOUBLE_TYPE h2; DOUBLE_TYPE h3; DOUBLE_TYPE h4; DOUBLE_TYPE h5; DOUBLE_TYPE h6; DOUBLE_TYPE h7; DOUBLE_TYPE h8; DOUBLE_TYPE h9; DOUBLE_TYPE h10; DOUBLE_TYPE h11;
    DOUBLE_TYPE s2; DOUBLE_TYPE s3; DOUBLE_TYPE s4; DOUBLE_TYPE s5; DOUBLE_TYPE s6;
    DOUBLE_TYPE i2; DOUBLE_TYPE i3;
    DOUBLE_TYPE r2; DOUBLE_TYPE r3;
    DOUBLE_TYPE d2; DOUBLE_TYPE d3; DOUBLE_TYPE d4; DOUBLE_TYPE d5;
    DOUBLE_TYPE t2; DOUBLE_TYPE t3; DOUBLE_TYPE t4;
    long __av; PAYLOAD_POSTCODE* nxt; PAYLOAD_POSTCODE* prv;
    explicit PAYLOAD_POSTCODE() : nxt(nullptr), prv(nullptr) { }
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
      DBT_SERIALIZATION_NVP(ar, i2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, i3);   
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r2);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, r3);   
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d2);      
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d3);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d4);   
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, d5);
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

  struct PAYLOAD_POSTCODE_mapkey012345678910_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_POSTCODE& e) {
      size_t h = 0;
      hash_combine(h, e.postcode);      
      // hash_combine(h, e.h2);
      // hash_combine(h, e.h3);
      // hash_combine(h, e.h4);
      // hash_combine(h, e.h5);
      // hash_combine(h, e.h6);
      // hash_combine(h, e.h7);
      // hash_combine(h, e.h8);
      // hash_combine(h, e.h9);
      // hash_combine(h, e.h10);
      // hash_combine(h, e.h11);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_POSTCODE& x, const PAYLOAD_POSTCODE& y) {
      return 
        x.postcode == y.postcode &&
        x.h2 == y.h2 && x.h3 == y.h3 && x.h4 == y.h4 && x.h5 == y.h5 && x.h6 == y.h6 && x.h7 == y.h7 && x.h8 == y.h8 && x.h9 == y.h9 && x.h10 == y.h10 && x.h11 == y.h11 &&
        x.s2 == y.s2 && x.s3 == y.s3 && x.s4 == y.s4 && x.s5 == y.s5 && x.s6 == y.s6 &&
        x.i2 == y.i2 && x.i3 == y.i3 &&
        x.r2 == y.r2 && x.r3 == y.r3 &&
        x.d2 == y.d2 && x.d3 == y.d3 && x.d4 == y.d4 && x.d5 == y.d5 &&
        x.t2 == y.t2 && x.t3 == y.t3 && x.t4 == y.t4;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_POSTCODE, long, 
    PrimaryHashIndex<PAYLOAD_POSTCODE, PAYLOAD_POSTCODE_mapkey012345678910_idxfn>
  > PAYLOAD_POSTCODE_map;
    
  
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
  
   typedef MultiHashMap<KEY_H2_HOUSE, PAYLOAD_H2_HOUSE_map, 
    PrimaryHashIndex<KEY_H2_HOUSE, KEY_H2_HOUSE_mapkey0_idxfn>
  > TMP1_map;
  
  typedef MultiHashMap<KEY_S2_SHOP, PAYLOAD_S2_SHOP_map,
    PrimaryHashIndex<KEY_S2_SHOP, KEY_S2_SHOP_mapkey0_idxfn>
  > TMP2_map;
  
  typedef MultiHashMap<KEY_I2_INSTITUTION, PAYLOAD_I2_INSTITUTION_map,
    PrimaryHashIndex<KEY_I2_INSTITUTION, KEY_I2_INSTITUTION_mapkey0_idxfn>
  > TMP3_map;
  
  typedef MultiHashMap<KEY_R2_RESTAURANT, PAYLOAD_R2_RESTAURANT_map,
    PrimaryHashIndex<KEY_R2_RESTAURANT, KEY_R2_RESTAURANT_mapkey0_idxfn>
  > TMP4_map;
   
  typedef MultiHashMap<KEY_D2_DEMOGRAPHICS, PAYLOAD_D2_DEMOGRAPHICS_map,
    PrimaryHashIndex<KEY_D2_DEMOGRAPHICS, KEY_D2_DEMOGRAPHICS_mapkey0_idxfn>
  > TMP5_map;
  
  typedef MultiHashMap<KEY_T2_TRANSPORT, PAYLOAD_T2_TRANSPORT_map,
    PrimaryHashIndex<KEY_T2_TRANSPORT, KEY_T2_TRANSPORT_mapkey0_idxfn>
  > TMP6_map;
  
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0) , AGG(16U) { gettimeofday(&t0, NULL); }
  
    /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";      
      // dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG), AGG, "\t");
      ar << AGG.count();
    }
    
  protected:
    /* Data structures used for storing / computing top level queries */
    PAYLOAD_POSTCODE_map AGG;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t() {
      
    }
  
  
    /* Trigger functions for table relations */
    void on_insert_HOUSE(const DOUBLE_TYPE postcode, const DOUBLE_TYPE h2, const DOUBLE_TYPE h3, const DOUBLE_TYPE h4, const DOUBLE_TYPE h5, const DOUBLE_TYPE h6, const DOUBLE_TYPE h7, const DOUBLE_TYPE h8, const DOUBLE_TYPE h9, const DOUBLE_TYPE h10, const DOUBLE_TYPE h11) {
      HOUSE_entry e(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, 1L);
      HOUSE.addOrDelOnZero(e, 1L);
    }
    void on_insert_HOUSE(HOUSE_entry &e) {
      e.__av = 1L;
      HOUSE.addOrDelOnZero(e, 1L);
    }
    void on_insert_SHOP(const DOUBLE_TYPE postcode, const DOUBLE_TYPE s2, const DOUBLE_TYPE s3, const DOUBLE_TYPE s4, const DOUBLE_TYPE s5, const DOUBLE_TYPE s6) {
      SHOP_entry e(postcode, s2, s3, s4, s5, s6, 1L);
      SHOP.addOrDelOnZero(e, 1L);
    }
    void on_insert_SHOP(SHOP_entry &e) {
      e.__av = 1L;
      SHOP.addOrDelOnZero(e, 1L);
    }
    void on_insert_INSTITUTION(const DOUBLE_TYPE postcode, const DOUBLE_TYPE i2, const DOUBLE_TYPE i3) {
      INSTITUTION_entry e(postcode, i2, i3, 1L);
      INSTITUTION.addOrDelOnZero(e, 1L);
    }
    void on_insert_INSTITUTION(INSTITUTION_entry &e) {
      e.__av = 1L;
      INSTITUTION.addOrDelOnZero(e, 1L);
    }
    void on_insert_RESTAURANT(const DOUBLE_TYPE postcode, const DOUBLE_TYPE r2, const DOUBLE_TYPE r3) {
      RESTAURANT_entry e(postcode, r2, r3, 1L);
      RESTAURANT.addOrDelOnZero(e, 1L);
    }
    void on_insert_RESTAURANT(RESTAURANT_entry &e) {
      e.__av = 1L;
      RESTAURANT.addOrDelOnZero(e, 1L);
    }
    void on_insert_DEMOGRAPHICS(const DOUBLE_TYPE postcode, const DOUBLE_TYPE d2, const DOUBLE_TYPE d3, const DOUBLE_TYPE d4, const DOUBLE_TYPE d5) {
      DEMOGRAPHICS_entry e(postcode, d2, d3, d4, d5, 1L);
      DEMOGRAPHICS.addOrDelOnZero(e, 1L);
    }
    void on_insert_DEMOGRAPHICS(DEMOGRAPHICS_entry &e) {
      e.__av = 1L;
      DEMOGRAPHICS.addOrDelOnZero(e, 1L);
    }
    void on_insert_TRANSPORT(const DOUBLE_TYPE postcode, const DOUBLE_TYPE t2, const DOUBLE_TYPE t3, const DOUBLE_TYPE t4) {
      TRANSPORT_entry e(postcode, t2, t3, t4, 1L);
      TRANSPORT.addOrDelOnZero(e, 1L);
    }
    void on_insert_TRANSPORT(TRANSPORT_entry &e) {
      e.__av = 1L;
      TRANSPORT.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_system_ready_event() {
      TMP1.clear();
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
          long v1 = e1->__av;

          // Create payload
          payload_h2_house.modify(h2, h3, h4, h5, h6, h7, h8, h9, h10, h11);
          payload_h2_house.__av = v1;
          payload_h2_house_map.clear();
          payload_h2_house_map.insert(payload_h2_house);

          // Aggregate by key
          key_h2_house.modify(postcode);
          TMP1.addOrDelOnZero(key_h2_house, payload_h2_house_map);
          
          e1 = e1->nxt;
        }
      }
      TMP2.clear();
      { //foreach
        SHOP_entry* e2 = SHOP.head;
        while (e2) {
          DOUBLE_TYPE postcode = e2->postcode;
          DOUBLE_TYPE s2 = e2->s2;
          DOUBLE_TYPE s3 = e2->s3;
          DOUBLE_TYPE s4 = e2->s4;
          DOUBLE_TYPE s5 = e2->s5;
          DOUBLE_TYPE s6 = e2->s6;
          long v2 = e2->__av;

          // Create payload
          payload_s2_shop.modify(s2, s3, s4, s5, s6);
          payload_s2_shop.__av = v2;
          payload_s2_shop_map.clear();
          payload_s2_shop_map.insert(payload_s2_shop);

          // Aggregate by key
          key_s2_shop.modify(postcode);
          TMP2.addOrDelOnZero(key_s2_shop, payload_s2_shop_map);

          e2 = e2->nxt;
        }
      }
      TMP3.clear();
      { //foreach
        INSTITUTION_entry* e3 = INSTITUTION.head;
        while (e3) {
          DOUBLE_TYPE postcode = e3->postcode;
          DOUBLE_TYPE i2 = e3->i2;
          DOUBLE_TYPE i3 = e3->i3;
          long v3 = e3->__av;

          // Create payload
          payload_i2_institution.modify(i2, i3);
          payload_i2_institution.__av = v3;
          payload_i2_institution_map.clear();
          payload_i2_institution_map.insert(payload_i2_institution);

          // Aggregate by key
          key_i2_instution.modify(postcode);
          TMP3.addOrDelOnZero(key_i2_instution, payload_i2_institution_map);

          e3 = e3->nxt;
        }
      }
      TMP4.clear();
      { //foreach
        RESTAURANT_entry* e4 = RESTAURANT.head;
        while (e4) {
          DOUBLE_TYPE postcode = e4->postcode;
          DOUBLE_TYPE r2 = e4->r2;
          DOUBLE_TYPE r3 = e4->r3;
          long v4 = e4->__av;

          // Create payload
          payload_r2_restaurant.modify(r2, r3);
          payload_r2_restaurant.__av = v4;
          payload_r2_restaurant_map.clear();
          payload_r2_restaurant_map.insert(payload_r2_restaurant);

          // Aggregate by key
          key_r2_restaurant.modify(postcode);
          TMP4.addOrDelOnZero(key_r2_restaurant, payload_r2_restaurant_map);

          e4 = e4->nxt;
        }
      }
      TMP5.clear();
      { //foreach
        DEMOGRAPHICS_entry* e5 = DEMOGRAPHICS.head;
        while (e5) {
          DOUBLE_TYPE postcode = e5->postcode;
          DOUBLE_TYPE d2 = e5->d2;
          DOUBLE_TYPE d3 = e5->d3;
          DOUBLE_TYPE d4 = e5->d4;
          DOUBLE_TYPE d5 = e5->d5;
          long v5 = e5->__av;

          // Create payload
          payload_d2_demographics.modify(d2, d3, d4, d5);
          payload_d2_demographics.__av = v5;
          payload_d2_demographics_map.clear();
          payload_d2_demographics_map.insert(payload_d2_demographics);

          // Aggregate by key
          key_d2_demographics.modify(postcode);
          TMP5.addOrDelOnZero(key_d2_demographics, payload_d2_demographics_map);

          e5 = e5->nxt;
        }
      }
      TMP6.clear();
      { //foreach
        TRANSPORT_entry* e6 = TRANSPORT.head;
        while (e6) {
          DOUBLE_TYPE postcode = e6->postcode;
          DOUBLE_TYPE t2 = e6->t2;
          DOUBLE_TYPE t3 = e6->t3;
          DOUBLE_TYPE t4 = e6->t4;
          long v6 = e6->__av;

          // Create payload
          payload_t2_transport.modify(t2, t3, t4);
          payload_t2_transport.__av = v6;
          payload_t2_transport_map.clear();
          payload_t2_transport_map.insert(payload_t2_transport);

          // Aggregate by key
          key_t2_transport.modify(postcode);
          TMP6.addOrDelOnZero(key_t2_transport, payload_t2_transport_map);

          e6 = e6->nxt;
        }
      }

      AGG.clear();
      { //foreach
        KEY_H2_HOUSE* e7 = TMP1.head;
        while (e7) {
          DOUBLE_TYPE postcode = e7->postcode;
          const PAYLOAD_H2_HOUSE_map& payload_h2_house_map = e7->__av;
          const PAYLOAD_S2_SHOP_map& payload_s2_shop_map = TMP2.getValueOrDefault(key_s2_shop.modify(postcode));
          const PAYLOAD_I2_INSTITUTION_map& payload_i2_institution_map = TMP3.getValueOrDefault(key_i2_instution.modify(postcode));
          const PAYLOAD_R2_RESTAURANT_map& payload_r2_restaurant_map = TMP4.getValueOrDefault(key_r2_restaurant.modify(postcode));
          const PAYLOAD_D2_DEMOGRAPHICS_map& payload_d2_demographics_map = TMP5.getValueOrDefault(key_d2_demographics.modify(postcode));
          const PAYLOAD_T2_TRANSPORT_map& payload_t2_transport_map = TMP6.getValueOrDefault(key_t2_transport.modify(postcode));
          
          payload_postcode.postcode = postcode;

          PAYLOAD_H2_HOUSE* p1 = payload_h2_house_map.head;
          while (p1) {
            payload_postcode.h2 = p1->h2;
            payload_postcode.h3 = p1->h3;
            payload_postcode.h4 = p1->h4;
            payload_postcode.h5 = p1->h5;
            payload_postcode.h6 = p1->h6;
            payload_postcode.h7 = p1->h7;
            payload_postcode.h8 = p1->h8;
            payload_postcode.h9 = p1->h9;
            payload_postcode.h10 = p1->h10;
            payload_postcode.h11 = p1->h11;
            payload_postcode.__av = p1->__av;

            PAYLOAD_S2_SHOP* p2 = payload_s2_shop_map.head;
            while (p2) {
              payload_postcode.s2 = p2->s2;
              payload_postcode.s3 = p2->s3;
              payload_postcode.s4 = p2->s4;
              payload_postcode.s5 = p2->s5;
              payload_postcode.s6 = p2->s6;
              payload_postcode.__av *= p2->__av;

              PAYLOAD_I2_INSTITUTION* p3 = payload_i2_institution_map.head;
              while (p3) {
                payload_postcode.i2 = p3->i2;
                payload_postcode.i3 = p3->i3;
                payload_postcode.__av *= p3->__av;

                PAYLOAD_R2_RESTAURANT* p4 = payload_r2_restaurant_map.head;
                while (p4) {
                  payload_postcode.r2 = p4->r2;
                  payload_postcode.r3 = p4->r3;
                  payload_postcode.__av *= p4->__av;

                  PAYLOAD_D2_DEMOGRAPHICS* p5 = payload_d2_demographics_map.head;
                  while (p5) {
                    payload_postcode.d2 = p5->d2;
                    payload_postcode.d3 = p5->d3;
                    payload_postcode.d4 = p5->d4;
                    payload_postcode.d5 = p5->d5;
                    payload_postcode.__av *= p5->__av;

                    PAYLOAD_T2_TRANSPORT* p6 = payload_t2_transport_map.head;
                    while (p6) {

                      payload_postcode.t2 = p6->t2;
                      payload_postcode.t3 = p6->t3;
                      payload_postcode.t4 = p6->t4;
                      payload_postcode.__av *= p6->__av;

                      AGG.addOrDelOnZero(payload_postcode, payload_postcode.__av);

                      p6 = p6->nxt;
                    }
                    p5 = p5->nxt;
                  }
                  p4 = p4->nxt;
                }
                p3 = p3->nxt;
              }
              p2 = p2->nxt;
            }
            p1 = p1->nxt;
          }
          e7 = e7->nxt;
        }
      }
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
  
    /* Data structures used for storing materialized views */
    HOUSE_map HOUSE;
    SHOP_map SHOP;
    INSTITUTION_map INSTITUTION;
    RESTAURANT_map RESTAURANT;
    DEMOGRAPHICS_map DEMOGRAPHICS;
    TRANSPORT_map TRANSPORT;
    TMP1_map TMP1;
    TMP2_map TMP2;
    TMP3_map TMP3;
    TMP4_map TMP4;
    TMP5_map TMP5;
    TMP6_map TMP6;
    
    KEY_H2_HOUSE key_h2_house;
    PAYLOAD_H2_HOUSE payload_h2_house;
    PAYLOAD_H2_HOUSE_map payload_h2_house_map = PAYLOAD_H2_HOUSE_map(2);

    KEY_S2_SHOP key_s2_shop;
    PAYLOAD_S2_SHOP payload_s2_shop;
    PAYLOAD_S2_SHOP_map payload_s2_shop_map = PAYLOAD_S2_SHOP_map(2);
  
    KEY_I2_INSTITUTION key_i2_instution;
    PAYLOAD_I2_INSTITUTION payload_i2_institution;
    PAYLOAD_I2_INSTITUTION_map payload_i2_institution_map = PAYLOAD_I2_INSTITUTION_map(2);

    KEY_R2_RESTAURANT key_r2_restaurant;
    PAYLOAD_R2_RESTAURANT payload_r2_restaurant;
    PAYLOAD_R2_RESTAURANT_map payload_r2_restaurant_map = PAYLOAD_R2_RESTAURANT_map(2);

    KEY_D2_DEMOGRAPHICS key_d2_demographics;
    PAYLOAD_D2_DEMOGRAPHICS payload_d2_demographics;
    PAYLOAD_D2_DEMOGRAPHICS_map payload_d2_demographics_map = PAYLOAD_D2_DEMOGRAPHICS_map(2);

    KEY_T2_TRANSPORT key_t2_transport;
    PAYLOAD_T2_TRANSPORT payload_t2_transport;
    PAYLOAD_T2_TRANSPORT_map payload_t2_transport_map = PAYLOAD_T2_TRANSPORT_map(2);

    PAYLOAD_POSTCODE payload_postcode;
  };
}
        