#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_LINEITEM_DYNAMIC
#define RELATION_ORDERS_DYNAMIC

namespace dbtoaster {

  /* Definitions of maps used for storing materialized views. */
  struct PAYLOAD_entry {
    static PAYLOAD_entry zero;

    long high_line_count;
    long low_line_count;

    explicit PAYLOAD_entry() { 
      clear();
    }

    FORCE_INLINE PAYLOAD_entry& operator +=(const PAYLOAD_entry &obj) {
      high_line_count += obj.high_line_count;
      low_line_count += obj.low_line_count;
      return *this;
    }

    FORCE_INLINE bool operator ==(const PAYLOAD_entry &obj) const {
      return (high_line_count == obj.high_line_count && low_line_count == obj.low_line_count);
    }

    FORCE_INLINE void clear() { 
      high_line_count = 0;
      low_line_count = 0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, high_line_count);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, low_line_count);
    }
  };

  PAYLOAD_entry PAYLOAD_entry::zero = PAYLOAD_entry();

  struct AGG_entry {
    STRING_TYPE L_SHIPMODE; PAYLOAD_entry __av; AGG_entry* nxt; AGG_entry* prv;
  
    explicit AGG_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_entry(const STRING_TYPE& c0, const PAYLOAD_entry& c1) { L_SHIPMODE = c0; __av = c1;  }
    AGG_entry(const AGG_entry& other) : L_SHIPMODE(other.L_SHIPMODE), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE AGG_entry& modify(const STRING_TYPE& c0) { L_SHIPMODE = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_SHIPMODE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_SHIPMODE);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_entry& x, const AGG_entry& y) {
      return x.L_SHIPMODE == y.L_SHIPMODE;
    }
  };
  
  typedef MultiHashMap<AGG_entry, PAYLOAD_entry, 
    PrimaryHashIndex<AGG_entry, AGG_mapkey0_idxfn>
  > AGG_map;

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
  
  struct ORDERS_entry {
    long orders_orderkey; long orders_custkey; STRING_TYPE orders_orderstatus; DOUBLE_TYPE orders_totalprice; date orders_orderdate; STRING_TYPE orders_orderpriority; STRING_TYPE orders_clerk; long orders_shippriority; STRING_TYPE orders_comment; long __av; ORDERS_entry* nxt; ORDERS_entry* prv;
  
    explicit ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit ORDERS_entry(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8, const long c9) { orders_orderkey = c0; orders_custkey = c1; orders_orderstatus = c2; orders_totalprice = c3; orders_orderdate = c4; orders_orderpriority = c5; orders_clerk = c6; orders_shippriority = c7; orders_comment = c8; __av = c9;  }
    ORDERS_entry(const ORDERS_entry& other) : orders_orderkey(other.orders_orderkey), orders_custkey(other.orders_custkey), orders_orderstatus(other.orders_orderstatus), orders_totalprice(other.orders_totalprice), orders_orderdate(other.orders_orderdate), orders_orderpriority(other.orders_orderpriority), orders_clerk(other.orders_clerk), orders_shippriority(other.orders_shippriority), orders_comment(other.orders_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    ORDERS_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        orders_orderkey = std::stol(f[0]); orders_custkey = std::stol(f[1]); orders_orderstatus = f[2]; orders_totalprice = std::stod(f[3]); orders_orderdate = Udate(f[4]); orders_orderpriority = f[5]; orders_clerk = f[6]; orders_shippriority = std::stol(f[7]); orders_comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_custkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderstatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_totalprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderpriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_clerk);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_shippriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  
  struct VIEW_LINEITEM_entry {
    long L_ORDERKEY; STRING_TYPE L_SHIPMODE; PAYLOAD_entry __av; VIEW_LINEITEM_entry* nxt; VIEW_LINEITEM_entry* prv;
  
    explicit VIEW_LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit VIEW_LINEITEM_entry(const long c0, const STRING_TYPE& c1, const PAYLOAD_entry& c2) { L_ORDERKEY = c0; L_SHIPMODE = c1; __av = c2;  }
    VIEW_LINEITEM_entry(const VIEW_LINEITEM_entry& other) : L_ORDERKEY(other.L_ORDERKEY), L_SHIPMODE(other.L_SHIPMODE), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE VIEW_LINEITEM_entry& modify(const long c0, const STRING_TYPE& c1) { L_ORDERKEY = c0; L_SHIPMODE = c1;  return *this; }
    FORCE_INLINE VIEW_LINEITEM_entry& modify0(const long c0) { L_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_SHIPMODE);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct VIEW_LINEITEM_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const VIEW_LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      hash_combine(h, e.L_SHIPMODE);
      return h;
    }
    
    FORCE_INLINE static bool equals(const VIEW_LINEITEM_entry& x, const VIEW_LINEITEM_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY && x.L_SHIPMODE == y.L_SHIPMODE;
    }
  };
  
  struct VIEW_LINEITEM_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const VIEW_LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const VIEW_LINEITEM_entry& x, const VIEW_LINEITEM_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<VIEW_LINEITEM_entry, PAYLOAD_entry, 
    PrimaryHashIndex<VIEW_LINEITEM_entry, VIEW_LINEITEM_mapkey01_idxfn>,
    SecondaryHashIndex<VIEW_LINEITEM_entry, VIEW_LINEITEM_mapkey0_idxfn>
  > VIEW_LINEITEM_map;


  struct VIEW_ORDERS_entry {
    long L_ORDERKEY; PAYLOAD_entry __av; VIEW_ORDERS_entry* nxt; VIEW_ORDERS_entry* prv;
  
    explicit VIEW_ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit VIEW_ORDERS_entry(const long c0, const PAYLOAD_entry& c1) { L_ORDERKEY = c0; __av = c1;  }
    VIEW_ORDERS_entry(const VIEW_ORDERS_entry& other) : L_ORDERKEY(other.L_ORDERKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE VIEW_ORDERS_entry& modify(const long c0) { L_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct VIEW_ORDERS_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const VIEW_ORDERS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const VIEW_ORDERS_entry& x, const VIEW_ORDERS_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<VIEW_ORDERS_entry, PAYLOAD_entry, 
    PrimaryHashIndex<VIEW_ORDERS_entry, VIEW_ORDERS_mapkey0_idxfn>
  > VIEW_ORDERS_map;
  
  
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
  
  
  struct DELTA_ORDERS_entry {
    long orders_orderkey; long orders_custkey; STRING_TYPE orders_orderstatus; DOUBLE_TYPE orders_totalprice; date orders_orderdate; STRING_TYPE orders_orderpriority; STRING_TYPE orders_clerk; long orders_shippriority; STRING_TYPE orders_comment; long __av; DELTA_ORDERS_entry* nxt; DELTA_ORDERS_entry* prv;
  
    explicit DELTA_ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_ORDERS_entry(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8, const long c9) { orders_orderkey = c0; orders_custkey = c1; orders_orderstatus = c2; orders_totalprice = c3; orders_orderdate = c4; orders_orderpriority = c5; orders_clerk = c6; orders_shippriority = c7; orders_comment = c8; __av = c9;  }
    DELTA_ORDERS_entry(const DELTA_ORDERS_entry& other) : orders_orderkey(other.orders_orderkey), orders_custkey(other.orders_custkey), orders_orderstatus(other.orders_orderstatus), orders_totalprice(other.orders_totalprice), orders_orderdate(other.orders_orderdate), orders_orderpriority(other.orders_orderpriority), orders_clerk(other.orders_clerk), orders_shippriority(other.orders_shippriority), orders_comment(other.orders_comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_ORDERS_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        orders_orderkey = std::stol(f[0]); orders_custkey = std::stol(f[1]); orders_orderstatus = f[2]; orders_totalprice = std::stod(f[3]); orders_orderdate = Udate(f[4]); orders_orderpriority = f[5]; orders_clerk = f[6]; orders_shippriority = std::stol(f[7]); orders_comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_ORDERS_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8) { orders_orderkey = c0; orders_custkey = c1; orders_orderstatus = c2; orders_totalprice = c3; orders_orderdate = c4; orders_orderpriority = c5; orders_clerk = c6; orders_shippriority = c7; orders_comment = c8;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_custkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderstatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_totalprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_orderpriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_clerk);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_shippriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orders_comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_ORDERS_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_ORDERS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.orders_orderkey);
      hash_combine(h, e.orders_custkey);
      hash_combine(h, e.orders_orderstatus);
      hash_combine(h, e.orders_totalprice);
      hash_combine(h, e.orders_orderdate);
      hash_combine(h, e.orders_orderpriority);
      hash_combine(h, e.orders_clerk);
      hash_combine(h, e.orders_shippriority);
      hash_combine(h, e.orders_comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_ORDERS_entry& x, const DELTA_ORDERS_entry& y) {
      return x.orders_orderkey == y.orders_orderkey && x.orders_custkey == y.orders_custkey && x.orders_orderstatus == y.orders_orderstatus && x.orders_totalprice == y.orders_totalprice && x.orders_orderdate == y.orders_orderdate && x.orders_orderpriority == y.orders_orderpriority && x.orders_clerk == y.orders_clerk && x.orders_shippriority == y.orders_shippriority && x.orders_comment == y.orders_comment;
    }
  };
  
  typedef MultiHashMap<DELTA_ORDERS_entry, long, 
    PrimaryHashIndex<DELTA_ORDERS_entry, DELTA_ORDERS_mapkey012345678_idxfn>
  > DELTA_ORDERS_map;

  

  /* Defines top-level materialized views */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0)  { 
      gettimeofday(&t0, NULL); 
    }
  
    /* Serialization code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      const AGG_map& _AGG = get_AGG();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG), _AGG, "\t");
    }
  
    /* Functions returning / computing the results of top level queries */
    const AGG_map& get_AGG() const {
      return AGG;
    }
  
  protected:
    /* Data structures used for storing / computing top-level queries */
    AGG_map AGG;
    
  };

  /* Contains materialized views and processing (IVM) logic */
  struct data_t : tlq_t {
  
    data_t(): tlq_t() {
      c6 = STRING_TYPE("2-HIGH");
      c5 = STRING_TYPE("1-URGENT");
      c4 = STRING_TYPE("SHIP");
      c2 = Udate(STRING_TYPE("1994-1-1"));
      c1 = Udate(STRING_TYPE("1995-1-1"));
      c3 = STRING_TYPE("MAIL");
      
    }
    
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
          if (c1 > l_receiptdate && l_receiptdate >= c2 &&
              l_commitdate > l_shipdate && l_receiptdate > l_commitdate &&
              (((l_shipmode == c3) || (l_shipmode == c4)))) {

              const PAYLOAD_entry& v2 = VIEW_ORDERS.getValueOrDefault(entry_VIEW_ORDERS.modify(l_orderkey));
              entry_PAYLOAD.high_line_count = v1 * v2.high_line_count;
              entry_PAYLOAD.low_line_count = v1 * v2.low_line_count;
              AGG.addOrDelOnZero(entry_AGG.modify(l_shipmode), entry_PAYLOAD);

              entry_PAYLOAD.high_line_count = v1;
              entry_PAYLOAD.low_line_count = v1;
              VIEW_LINEITEM.addOrDelOnZero(entry_VIEW_LINEITEM.modify(l_orderkey, l_shipmode), entry_PAYLOAD);
          }
        }
      }
    }
    
    
    void on_batch_update_ORDERS(const std::vector<DELTA_ORDERS_entry>::iterator &begin, const std::vector<DELTA_ORDERS_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;

      { //foreach
        for (std::vector<DELTA_ORDERS_entry>::iterator e5 = begin; e5 != end; e5++) {
          long l_orderkey = e5->orders_orderkey;
          long o_custkey = e5->orders_custkey;
          STRING_TYPE o_orderstatus = e5->orders_orderstatus;
          DOUBLE_TYPE o_totalprice = e5->orders_totalprice;
          date o_orderdate = e5->orders_orderdate;
          STRING_TYPE o_orderpriority = e5->orders_orderpriority;
          STRING_TYPE o_clerk = e5->orders_clerk;
          long o_shippriority = e5->orders_shippriority;
          STRING_TYPE o_comment = e5->orders_comment;
          long v5 = e5->__av;

          long p_high_line_count = ((((o_orderpriority == c5) || (o_orderpriority == c6))) ? 1 : 0);
          long p_low_line_count = ((((o_orderpriority == c5) || (o_orderpriority == c6))) ? 0 : 1);

          { //slice
            const SecondaryIdxNode<VIEW_LINEITEM_entry>* n1_head = static_cast<const SecondaryIdxNode<VIEW_LINEITEM_entry>*>(VIEW_LINEITEM.slice(entry_VIEW_LINEITEM.modify0(l_orderkey), 0));
            const SecondaryIdxNode<VIEW_LINEITEM_entry>* n1 = n1_head;
            VIEW_LINEITEM_entry* e8;
            while (n1) {
              e8 = n1->obj;
              STRING_TYPE l_shipmode = e8->L_SHIPMODE;
              const PAYLOAD_entry& v8 = e8->__av;

              entry_PAYLOAD.high_line_count = v5 * v8.high_line_count * p_high_line_count;
              entry_PAYLOAD.low_line_count = v5 * v8.low_line_count * p_low_line_count;
              AGG.addOrDelOnZero(entry_AGG.modify(l_shipmode), entry_PAYLOAD);

              n1 = (n1 != n1_head ? n1->nxt : n1->child);
            }
          }

          entry_PAYLOAD.high_line_count = v5 * p_high_line_count;
          entry_PAYLOAD.low_line_count = v5 * p_low_line_count;
          VIEW_ORDERS.addOrDelOnZero(entry_VIEW_ORDERS.modify(l_orderkey), entry_PAYLOAD);
        }
      }
    }
    
    
    void on_system_ready_event() {
      
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      AGG_entry entry_AGG;
      VIEW_ORDERS_entry entry_VIEW_ORDERS;
      VIEW_LINEITEM_entry entry_VIEW_LINEITEM;
      PAYLOAD_entry entry_PAYLOAD;    

      /* Data structures used for storing materialized views */    
      VIEW_LINEITEM_map VIEW_LINEITEM;
      VIEW_ORDERS_map VIEW_ORDERS;
    
      /* Constant defitions */
      /* const static */ STRING_TYPE c6;
      /* const static */ STRING_TYPE c5;
      /* const static */ STRING_TYPE c4;
      /* const static */ long c2;
      /* const static */ long c1;
      /* const static */ STRING_TYPE c3;
  };

  

}