#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_LINEITEM_DYNAMIC
#define RELATION_PART_DYNAMIC

namespace dbtoaster {

  /* Definitions of maps used for storing materialized views. */
  struct PAYLOAD_entry {
    static PAYLOAD_entry zero;

    DOUBLE_TYPE promo_revenue;
    DOUBLE_TYPE total_revenue;

    explicit PAYLOAD_entry() { 
      clear();
    }

    FORCE_INLINE PAYLOAD_entry& operator +=(const PAYLOAD_entry &obj) {
      promo_revenue += obj.promo_revenue;
      total_revenue += obj.total_revenue;
      return *this;
    }

    FORCE_INLINE bool operator ==(const PAYLOAD_entry &obj) const {
      return (promo_revenue == obj.promo_revenue && total_revenue == obj.total_revenue);
    }

    FORCE_INLINE void clear() { 
      promo_revenue = 0;
      total_revenue = 0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, promo_revenue);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, total_revenue);
    }
  };

  PAYLOAD_entry PAYLOAD_entry::zero = PAYLOAD_entry();

  struct LINEITEM_entry {
    long lineitem_orderkey; long lineitem_partkey; long lineitem_suppkey; long lineitem_linenumber; DOUBLE_TYPE lineitem_quantity; DOUBLE_TYPE lineitem_extendedprice; DOUBLE_TYPE lineitem_discount; DOUBLE_TYPE lineitem_tax; STRING_TYPE lineitem_returnflag; STRING_TYPE lineitem_linestatus; date lineitem_shipdate; date lineitem_commitdate; date lineitem_receiptdate; STRING_TYPE lineitem_shipinstruct; STRING_TYPE lineitem_shipmode; STRING_TYPE lineitem_comment; long __av; LINEITEM_entry* nxt; LINEITEM_entry* prv;
  
    explicit LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit LINEITEM_entry(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15, const long c16) { lineitem_orderkey = c0; lineitem_partkey = c1; lineitem_suppkey = c2; lineitem_linenumber = c3; lineitem_quantity = c4; lineitem_extendedprice = c5; lineitem_discount = c6; lineitem_tax = c7; lineitem_returnflag = c8; lineitem_linestatus = c9; lineitem_shipdate = c10; lineitem_commitdate = c11; lineitem_receiptdate = c12; lineitem_shipinstruct = c13; lineitem_shipmode = c14; lineitem_comment = c15; __av = c16;  }
    LINEITEM_entry(const LINEITEM_entry& other) : lineitem_orderkey(other.lineitem_orderkey), lineitem_partkey(other.lineitem_partkey), lineitem_suppkey(other.lineitem_suppkey), lineitem_linenumber(other.lineitem_linenumber), lineitem_quantity(other.lineitem_quantity), lineitem_extendedprice(other.lineitem_extendedprice), lineitem_discount(other.lineitem_discount), lineitem_tax(other.lineitem_tax), lineitem_returnflag(other.lineitem_returnflag), lineitem_linestatus(other.lineitem_linestatus), lineitem_shipdate(other.lineitem_shipdate), lineitem_commitdate(other.lineitem_commitdate), lineitem_receiptdate(other.lineitem_receiptdate), lineitem_shipinstruct(other.lineitem_shipinstruct), lineitem_shipmode(other.lineitem_shipmode), lineitem_comment(other.lineitem_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    LINEITEM_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 16) return; */
        lineitem_orderkey = std::stol(f[0]); lineitem_partkey = std::stol(f[1]); lineitem_suppkey = std::stol(f[2]); lineitem_linenumber = std::stol(f[3]); lineitem_quantity = std::stod(f[4]); lineitem_extendedprice = std::stod(f[5]); lineitem_discount = std::stod(f[6]); lineitem_tax = std::stod(f[7]); lineitem_returnflag = f[8]; lineitem_linestatus = f[9]; lineitem_shipdate = Udate(f[10]); lineitem_commitdate = Udate(f[11]); lineitem_receiptdate = Udate(f[12]); lineitem_shipinstruct = f[13]; lineitem_shipmode = f[14]; lineitem_comment = f[15]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_suppkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_linenumber);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_quantity);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_extendedprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_discount);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_tax);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_returnflag);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_linestatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_commitdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_receiptdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipinstruct);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipmode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PART_entry {
    long part_partkey; STRING_TYPE part_name; STRING_TYPE part_mfgr; STRING_TYPE part_brand; STRING_TYPE part_type; long part_size; STRING_TYPE part_container; DOUBLE_TYPE part_retailprice; STRING_TYPE part_comment; long __av; PART_entry* nxt; PART_entry* prv;
  
    explicit PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit PART_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const long c9) { part_partkey = c0; part_name = c1; part_mfgr = c2; part_brand = c3; part_type = c4; part_size = c5; part_container = c6; part_retailprice = c7; part_comment = c8; __av = c9;  }
    PART_entry(const PART_entry& other) : part_partkey(other.part_partkey), part_name(other.part_name), part_mfgr(other.part_mfgr), part_brand(other.part_brand), part_type(other.part_type), part_size(other.part_size), part_container(other.part_container), part_retailprice(other.part_retailprice), part_comment(other.part_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    PART_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        part_partkey = std::stol(f[0]); part_name = f[1]; part_mfgr = f[2]; part_brand = f[3]; part_type = f[4]; part_size = std::stol(f[5]); part_container = f[6]; part_retailprice = std::stod(f[7]); part_comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_mfgr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_brand);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_type);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_size);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_container);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_retailprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct VIEW_LINEITEM_entry {
    long L_PARTKEY; PAYLOAD_entry __av; VIEW_LINEITEM_entry* nxt; VIEW_LINEITEM_entry* prv;
  
    explicit VIEW_LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit VIEW_LINEITEM_entry(const long c0, const PAYLOAD_entry& c1) { L_PARTKEY = c0; __av = c1;  }
    VIEW_LINEITEM_entry(const VIEW_LINEITEM_entry& other) : L_PARTKEY(other.L_PARTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE VIEW_LINEITEM_entry& modify(const long c0) { L_PARTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct VIEW_LINEITEM_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const VIEW_LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_PARTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const VIEW_LINEITEM_entry& x, const VIEW_LINEITEM_entry& y) {
      return x.L_PARTKEY == y.L_PARTKEY;
    }
  };
  
  typedef MultiHashMap<VIEW_LINEITEM_entry, PAYLOAD_entry, 
    PrimaryHashIndex<VIEW_LINEITEM_entry, VIEW_LINEITEM_mapkey0_idxfn>
  > VIEW_LINEITEM_map;
   
  struct VIEW_PART_entry {
    long L_PARTKEY; PAYLOAD_entry __av; VIEW_PART_entry* nxt; VIEW_PART_entry* prv;
  
    explicit VIEW_PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit VIEW_PART_entry(const long c0, const PAYLOAD_entry& c1) { L_PARTKEY = c0; __av = c1;  }
    VIEW_PART_entry(const VIEW_PART_entry& other) : L_PARTKEY(other.L_PARTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE VIEW_PART_entry& modify(const long c0) { L_PARTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct VIEW_PART_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const VIEW_PART_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_PARTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const VIEW_PART_entry& x, const VIEW_PART_entry& y) {
      return x.L_PARTKEY == y.L_PARTKEY;
    }
  };
  
  typedef MultiHashMap<VIEW_PART_entry, PAYLOAD_entry, 
    PrimaryHashIndex<VIEW_PART_entry, VIEW_PART_mapkey0_idxfn>
  > VIEW_PART_map;
  

  struct DELTA_LINEITEM_entry {
    long lineitem_orderkey; long lineitem_partkey; long lineitem_suppkey; long lineitem_linenumber; DOUBLE_TYPE lineitem_quantity; DOUBLE_TYPE lineitem_extendedprice; DOUBLE_TYPE lineitem_discount; DOUBLE_TYPE lineitem_tax; STRING_TYPE lineitem_returnflag; STRING_TYPE lineitem_linestatus; date lineitem_shipdate; date lineitem_commitdate; date lineitem_receiptdate; STRING_TYPE lineitem_shipinstruct; STRING_TYPE lineitem_shipmode; STRING_TYPE lineitem_comment; long __av; DELTA_LINEITEM_entry* nxt; DELTA_LINEITEM_entry* prv;
  
    explicit DELTA_LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_LINEITEM_entry(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15, const long c16) { lineitem_orderkey = c0; lineitem_partkey = c1; lineitem_suppkey = c2; lineitem_linenumber = c3; lineitem_quantity = c4; lineitem_extendedprice = c5; lineitem_discount = c6; lineitem_tax = c7; lineitem_returnflag = c8; lineitem_linestatus = c9; lineitem_shipdate = c10; lineitem_commitdate = c11; lineitem_receiptdate = c12; lineitem_shipinstruct = c13; lineitem_shipmode = c14; lineitem_comment = c15; __av = c16;  }
    DELTA_LINEITEM_entry(const DELTA_LINEITEM_entry& other) : lineitem_orderkey(other.lineitem_orderkey), lineitem_partkey(other.lineitem_partkey), lineitem_suppkey(other.lineitem_suppkey), lineitem_linenumber(other.lineitem_linenumber), lineitem_quantity(other.lineitem_quantity), lineitem_extendedprice(other.lineitem_extendedprice), lineitem_discount(other.lineitem_discount), lineitem_tax(other.lineitem_tax), lineitem_returnflag(other.lineitem_returnflag), lineitem_linestatus(other.lineitem_linestatus), lineitem_shipdate(other.lineitem_shipdate), lineitem_commitdate(other.lineitem_commitdate), lineitem_receiptdate(other.lineitem_receiptdate), lineitem_shipinstruct(other.lineitem_shipinstruct), lineitem_shipmode(other.lineitem_shipmode), lineitem_comment(other.lineitem_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_LINEITEM_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 16) return; */
        lineitem_orderkey = std::stol(f[0]); lineitem_partkey = std::stol(f[1]); lineitem_suppkey = std::stol(f[2]); lineitem_linenumber = std::stol(f[3]); lineitem_quantity = std::stod(f[4]); lineitem_extendedprice = std::stod(f[5]); lineitem_discount = std::stod(f[6]); lineitem_tax = std::stod(f[7]); lineitem_returnflag = f[8]; lineitem_linestatus = f[9]; lineitem_shipdate = Udate(f[10]); lineitem_commitdate = Udate(f[11]); lineitem_receiptdate = Udate(f[12]); lineitem_shipinstruct = f[13]; lineitem_shipmode = f[14]; lineitem_comment = f[15]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_LINEITEM_entry& modify(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15) { lineitem_orderkey = c0; lineitem_partkey = c1; lineitem_suppkey = c2; lineitem_linenumber = c3; lineitem_quantity = c4; lineitem_extendedprice = c5; lineitem_discount = c6; lineitem_tax = c7; lineitem_returnflag = c8; lineitem_linestatus = c9; lineitem_shipdate = c10; lineitem_commitdate = c11; lineitem_receiptdate = c12; lineitem_shipinstruct = c13; lineitem_shipmode = c14; lineitem_comment = c15;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_suppkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_linenumber);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_quantity);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_extendedprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_discount);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_tax);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_returnflag);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_linestatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_commitdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_receiptdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipinstruct);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_shipmode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, lineitem_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_LINEITEM_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.lineitem_orderkey);
      hash_combine(h, e.lineitem_partkey);
      hash_combine(h, e.lineitem_suppkey);
      hash_combine(h, e.lineitem_linenumber);
      hash_combine(h, e.lineitem_quantity);
      hash_combine(h, e.lineitem_extendedprice);
      hash_combine(h, e.lineitem_discount);
      hash_combine(h, e.lineitem_tax);
      hash_combine(h, e.lineitem_returnflag);
      hash_combine(h, e.lineitem_linestatus);
      hash_combine(h, e.lineitem_shipdate);
      hash_combine(h, e.lineitem_commitdate);
      hash_combine(h, e.lineitem_receiptdate);
      hash_combine(h, e.lineitem_shipinstruct);
      hash_combine(h, e.lineitem_shipmode);
      hash_combine(h, e.lineitem_comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_LINEITEM_entry& x, const DELTA_LINEITEM_entry& y) {
      return x.lineitem_orderkey == y.lineitem_orderkey && x.lineitem_partkey == y.lineitem_partkey && x.lineitem_suppkey == y.lineitem_suppkey && x.lineitem_linenumber == y.lineitem_linenumber && x.lineitem_quantity == y.lineitem_quantity && x.lineitem_extendedprice == y.lineitem_extendedprice && x.lineitem_discount == y.lineitem_discount && x.lineitem_tax == y.lineitem_tax && x.lineitem_returnflag == y.lineitem_returnflag && x.lineitem_linestatus == y.lineitem_linestatus && x.lineitem_shipdate == y.lineitem_shipdate && x.lineitem_commitdate == y.lineitem_commitdate && x.lineitem_receiptdate == y.lineitem_receiptdate && x.lineitem_shipinstruct == y.lineitem_shipinstruct && x.lineitem_shipmode == y.lineitem_shipmode && x.lineitem_comment == y.lineitem_comment;
    }
  };
  
  typedef MultiHashMap<DELTA_LINEITEM_entry, long, 
    PrimaryHashIndex<DELTA_LINEITEM_entry, DELTA_LINEITEM_mapkey0123456789101112131415_idxfn>
  > DELTA_LINEITEM_map;
  
  
  struct DELTA_PART_entry {
    long part_partkey; STRING_TYPE part_name; STRING_TYPE part_mfgr; STRING_TYPE part_brand; STRING_TYPE part_type; long part_size; STRING_TYPE part_container; DOUBLE_TYPE part_retailprice; STRING_TYPE part_comment; long __av; DELTA_PART_entry* nxt; DELTA_PART_entry* prv;
  
    explicit DELTA_PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_PART_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const long c9) { part_partkey = c0; part_name = c1; part_mfgr = c2; part_brand = c3; part_type = c4; part_size = c5; part_container = c6; part_retailprice = c7; part_comment = c8; __av = c9;  }
    DELTA_PART_entry(const DELTA_PART_entry& other) : part_partkey(other.part_partkey), part_name(other.part_name), part_mfgr(other.part_mfgr), part_brand(other.part_brand), part_type(other.part_type), part_size(other.part_size), part_container(other.part_container), part_retailprice(other.part_retailprice), part_comment(other.part_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_PART_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        part_partkey = std::stol(f[0]); part_name = f[1]; part_mfgr = f[2]; part_brand = f[3]; part_type = f[4]; part_size = std::stol(f[5]); part_container = f[6]; part_retailprice = std::stod(f[7]); part_comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_PART_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8) { part_partkey = c0; part_name = c1; part_mfgr = c2; part_brand = c3; part_type = c4; part_size = c5; part_container = c6; part_retailprice = c7; part_comment = c8;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_mfgr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_brand);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_type);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_size);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_container);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_retailprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, part_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_PART_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_PART_entry& e) {
      size_t h = 0;
      hash_combine(h, e.part_partkey);
      hash_combine(h, e.part_name);
      hash_combine(h, e.part_mfgr);
      hash_combine(h, e.part_brand);
      hash_combine(h, e.part_type);
      hash_combine(h, e.part_size);
      hash_combine(h, e.part_container);
      hash_combine(h, e.part_retailprice);
      hash_combine(h, e.part_comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_PART_entry& x, const DELTA_PART_entry& y) {
      return x.part_partkey == y.part_partkey && x.part_name == y.part_name && x.part_mfgr == y.part_mfgr && x.part_brand == y.part_brand && x.part_type == y.part_type && x.part_size == y.part_size && x.part_container == y.part_container && x.part_retailprice == y.part_retailprice && x.part_comment == y.part_comment;
    }
  };
  
  typedef MultiHashMap<DELTA_PART_entry, long, 
    PrimaryHashIndex<DELTA_PART_entry, DELTA_PART_mapkey012345678_idxfn>
  > DELTA_PART_map;

  

  /* Defines top-level materialized views */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0), PROMO_REVENUE_PERCENT(0.0) { 
      gettimeofday(&t0, NULL); 
    }
  
    /* Serialization code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG), AGG, "\t");
      ar << "\n";
      dbtoaster::serialize_nvp_tabbed(ar, STRING(PROMO_REVENUE_PERCENT), PROMO_REVENUE_PERCENT, "\t");
      ar << "\n";
    }
 
  protected:
    /* Data structures used for storing / computing top-level queries */
    PAYLOAD_entry AGG;
    DOUBLE_TYPE PROMO_REVENUE_PERCENT;
    
  };

  /* Contains materialized views and processing (IVM) logic */
  struct data_t : tlq_t {
  
    data_t(): tlq_t() {
      c2 = Udate(STRING_TYPE("1995-9-1"));
      c1 = Udate(STRING_TYPE("1995-10-1"));
      /* Regular expression initialization */
      if (regcomp(&preg1, "^PROMO.*$", REG_EXTENDED | REG_NOSUB)) {
        cerr << "Error compiling regular expression: /^PROMO.*$/" << endl;
        exit(-1);
      }
    }
  
    ~data_t() {
      regfree(&preg1);
    }
  
    
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_batch_update_LINEITEM(const std::vector<DELTA_LINEITEM_entry>::iterator &begin, const std::vector<DELTA_LINEITEM_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;

      { //foreach
        for (std::vector<DELTA_LINEITEM_entry>::iterator e1 = begin; e1 != end; e1++) {
          long l_orderkey = e1->lineitem_orderkey;
          long l_partkey = e1->lineitem_partkey;
          long l_suppkey = e1->lineitem_suppkey;
          long l_linenumber = e1->lineitem_linenumber;
          DOUBLE_TYPE l_quantity = e1->lineitem_quantity;
          DOUBLE_TYPE l_extendedprice = e1->lineitem_extendedprice;
          DOUBLE_TYPE l_discount = e1->lineitem_discount;
          DOUBLE_TYPE l_tax = e1->lineitem_tax;
          STRING_TYPE l_returnflag = e1->lineitem_returnflag;
          STRING_TYPE l_linestatus = e1->lineitem_linestatus;
          date l_shipdate = e1->lineitem_shipdate;
          date l_commitdate = e1->lineitem_commitdate;
          date l_receiptdate = e1->lineitem_receiptdate;
          STRING_TYPE l_shipinstruct = e1->lineitem_shipinstruct;
          STRING_TYPE l_shipmode = e1->lineitem_shipmode;
          STRING_TYPE l_comment = e1->lineitem_comment;
          long v1 = e1->__av;
          if (c1 > l_shipdate && l_shipdate >= c2) {

            DOUBLE_TYPE tmp = (v1 * (l_extendedprice * (1L + (-1L * l_discount))));            

            const PAYLOAD_entry& v2 = VIEW_PART.getValueOrDefault(entry_VIEW_PART.modify(l_partkey));
            AGG.total_revenue += tmp * v2.total_revenue;
            AGG.promo_revenue += tmp * v2.promo_revenue;

            entry_PAYLOAD.total_revenue = tmp;
            entry_PAYLOAD.promo_revenue = tmp;
            VIEW_LINEITEM.addOrDelOnZero(entry_VIEW_LINEITEM.modify(l_partkey), entry_PAYLOAD);
          }
        }
      }

      PROMO_REVENUE_PERCENT = 100.0 * (AGG.promo_revenue / Ulistmax(1.0, AGG.total_revenue));
    }
    
    
    void on_batch_update_PART(const std::vector<DELTA_PART_entry>::iterator &begin, const std::vector<DELTA_PART_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;

      { //foreach
        for (std::vector<DELTA_PART_entry>::iterator e5 = begin; e5 != end; e5++) {
          long l_partkey = e5->part_partkey;
          STRING_TYPE p_name = e5->part_name;
          STRING_TYPE p_mfgr = e5->part_mfgr;
          STRING_TYPE p_brand = e5->part_brand;
          STRING_TYPE p_type = e5->part_type;
          long p_size = e5->part_size;
          STRING_TYPE p_container = e5->part_container;
          DOUBLE_TYPE p_retailprice = e5->part_retailprice;
          STRING_TYPE p_comment = e5->part_comment;
          long v5 = e5->__av;

          long tmp = (0L != Upreg_match(preg1, p_type)) ? v5 : 0;

          const PAYLOAD_entry& v6 = VIEW_LINEITEM.getValueOrDefault(entry_VIEW_LINEITEM.modify(l_partkey));
          AGG.total_revenue += v5 * v6.total_revenue;
          AGG.promo_revenue += tmp * v6.promo_revenue;

          entry_PAYLOAD.total_revenue = v5;
          entry_PAYLOAD.promo_revenue = tmp;
          VIEW_PART.addOrDelOnZero(entry_VIEW_PART.modify(l_partkey), entry_PAYLOAD);
        }
      }

      PROMO_REVENUE_PERCENT = 100.0 * (AGG.promo_revenue / Ulistmax(1.0, AGG.total_revenue));      
    }
    
    
    void on_system_ready_event() {
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      PAYLOAD_entry entry_PAYLOAD;
      VIEW_PART_entry entry_VIEW_PART;
      VIEW_LINEITEM_entry entry_VIEW_LINEITEM;

      /* Regular expression objects */
      regex_t preg1;
    
      /* Data structures used for storing materialized views */
      VIEW_PART_map VIEW_PART;
      VIEW_LINEITEM_map VIEW_LINEITEM;

      /* Constant defitions */
      /* const static */ long c2;
      /* const static */ long c1;
  };

  

}