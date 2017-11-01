#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_LINEITEM_STATIC
#define RELATION_ORDERS_STATIC
#define RELATION_PART_STATIC
#define RELATION_CUSTOMER_STATIC
#define RELATION_SUPPLIER_STATIC
#define RELATION_NATION_STATIC
#define RELATION_REGION_STATIC

namespace dbtoaster {

  /* Definitions of maps used for storing materialized views. */
  struct PAYLOAD_entry {
    static PAYLOAD_entry zero;

    DOUBLE_TYPE partial_mkt_share;
    DOUBLE_TYPE total_mkt_share;
    DOUBLE_TYPE ratio;

    explicit PAYLOAD_entry() { 
      clear();
    }

    FORCE_INLINE PAYLOAD_entry& operator +=(const PAYLOAD_entry &obj) {
      partial_mkt_share += obj.partial_mkt_share;
      total_mkt_share += obj.total_mkt_share;
      ratio = (total_mkt_share != 0.0 ? partial_mkt_share / total_mkt_share : 0.0);
      return *this;
    }

    FORCE_INLINE bool operator ==(const PAYLOAD_entry &obj) const {
      return (partial_mkt_share == obj.partial_mkt_share && total_mkt_share == obj.total_mkt_share);
    }

    FORCE_INLINE void clear() { 
      partial_mkt_share = 0;
      total_mkt_share = 0;
      ratio = 0.0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, partial_mkt_share);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, total_mkt_share);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, ratio);
    }
  };

  PAYLOAD_entry PAYLOAD_entry::zero = PAYLOAD_entry();

  struct V_NATION1_entry {
    long C_NATIONKEY; long __av; V_NATION1_entry* nxt; V_NATION1_entry* prv;
  
    explicit V_NATION1_entry() : nxt(nullptr), prv(nullptr) { }
    explicit V_NATION1_entry(const long c0, const long c1) { C_NATIONKEY = c0; __av = c1;  }
    V_NATION1_entry(const V_NATION1_entry& other) : C_NATIONKEY(other.C_NATIONKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    V_NATION1_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 1) return; */
        C_NATIONKEY = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE V_NATION1_entry& modify(const long c0) { C_NATIONKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct V_NATION1_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const V_NATION1_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const V_NATION1_entry& x, const V_NATION1_entry& y) {
      return x.C_NATIONKEY == y.C_NATIONKEY;
    }
  };
  
  typedef MultiHashMap<V_NATION1_entry, long, 
    PrimaryHashIndex<V_NATION1_entry, V_NATION1_mapkey0_idxfn>
  > V_NATION1_map;
  
  
  struct V_NATION2_entry {
    long S_NATIONKEY; STRING_TYPE N_NAME; long __av; V_NATION2_entry* nxt; V_NATION2_entry* prv;
  
    explicit V_NATION2_entry() : nxt(nullptr), prv(nullptr) { }
    explicit V_NATION2_entry(const long c0, const STRING_TYPE& c1, const long c2) { S_NATIONKEY = c0; N_NAME = c1; __av = c2;  }
    V_NATION2_entry(const V_NATION2_entry& other) : S_NATIONKEY(other.S_NATIONKEY), N_NAME(other.N_NAME), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    V_NATION2_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        S_NATIONKEY = std::stol(f[0]); N_NAME = f[1]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE V_NATION2_entry& modify(const long c0, const STRING_TYPE& c1) { S_NATIONKEY = c0; N_NAME = c1;  return *this; }
    FORCE_INLINE V_NATION2_entry& modify0(const long c0) { S_NATIONKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, S_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NAME);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct V_NATION2_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const V_NATION2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_NATIONKEY);
      hash_combine(h, e.N_NAME);
      return h;
    }
    
    FORCE_INLINE static bool equals(const V_NATION2_entry& x, const V_NATION2_entry& y) {
      return x.S_NATIONKEY == y.S_NATIONKEY && x.N_NAME == y.N_NAME;
    }
  };
  
  struct V_NATION2_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const V_NATION2_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const V_NATION2_entry& x, const V_NATION2_entry& y) {
      return x.S_NATIONKEY == y.S_NATIONKEY;
    }
  };
  
  typedef MultiHashMap<V_NATION2_entry, long, 
    PrimaryHashIndex<V_NATION2_entry, V_NATION2_mapkey01_idxfn>,
    SecondaryHashIndex<V_NATION2_entry, V_NATION2_mapkey0_idxfn>
  > V_NATION2_map;
  
  
  struct QUERY8_entry {
    long YEAR; PAYLOAD_entry __av; QUERY8_entry* nxt; QUERY8_entry* prv;
  
    explicit QUERY8_entry() : nxt(nullptr), prv(nullptr) { }
    explicit QUERY8_entry(const long c0, const PAYLOAD_entry& c1) { YEAR = c0; __av = c1;  }
    QUERY8_entry(const QUERY8_entry& other) : YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE QUERY8_entry& modify(const long c0) { YEAR = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct QUERY8_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const QUERY8_entry& e) {
      size_t h = 0;
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const QUERY8_entry& x, const QUERY8_entry& y) {
      return x.YEAR == y.YEAR;
    }
  };
  
  typedef MultiHashMap<QUERY8_entry, PAYLOAD_entry, 
    PrimaryHashIndex<QUERY8_entry, QUERY8_mapkey0_idxfn>
  > QUERY8_map;
  
  
  struct LINEITEM_entry {
    long orderkey; long partkey; long suppkey; long linenumber; DOUBLE_TYPE quantity; DOUBLE_TYPE extendedprice; DOUBLE_TYPE discount; DOUBLE_TYPE tax; STRING_TYPE returnflag; STRING_TYPE linestatus; date shipdate; date commitdate; date receiptdate; STRING_TYPE shipinstruct; STRING_TYPE shipmode; STRING_TYPE comment; long __av; LINEITEM_entry* nxt; LINEITEM_entry* prv;
  
    explicit LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit LINEITEM_entry(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15, const long c16) { orderkey = c0; partkey = c1; suppkey = c2; linenumber = c3; quantity = c4; extendedprice = c5; discount = c6; tax = c7; returnflag = c8; linestatus = c9; shipdate = c10; commitdate = c11; receiptdate = c12; shipinstruct = c13; shipmode = c14; comment = c15; __av = c16;  }
    LINEITEM_entry(const LINEITEM_entry& other) : orderkey(other.orderkey), partkey(other.partkey), suppkey(other.suppkey), linenumber(other.linenumber), quantity(other.quantity), extendedprice(other.extendedprice), discount(other.discount), tax(other.tax), returnflag(other.returnflag), linestatus(other.linestatus), shipdate(other.shipdate), commitdate(other.commitdate), receiptdate(other.receiptdate), shipinstruct(other.shipinstruct), shipmode(other.shipmode), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    LINEITEM_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 16) return; */
        orderkey = std::stol(f[0]); partkey = std::stol(f[1]); suppkey = std::stol(f[2]); linenumber = std::stol(f[3]); quantity = std::stod(f[4]); extendedprice = std::stod(f[5]); discount = std::stod(f[6]); tax = std::stod(f[7]); returnflag = f[8]; linestatus = f[9]; shipdate = Udate(f[10]); commitdate = Udate(f[11]); receiptdate = Udate(f[12]); shipinstruct = f[13]; shipmode = f[14]; comment = f[15]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE LINEITEM_entry& modify(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15) { orderkey = c0; partkey = c1; suppkey = c2; linenumber = c3; quantity = c4; extendedprice = c5; discount = c6; tax = c7; returnflag = c8; linestatus = c9; shipdate = c10; commitdate = c11; receiptdate = c12; shipinstruct = c13; shipmode = c14; comment = c15;  return *this; }
    FORCE_INLINE LINEITEM_entry& modify1(const long c1) { partkey = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, suppkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, linenumber);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, quantity);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, extendedprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, discount);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, tax);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, returnflag);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, linestatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, shipdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, commitdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, receiptdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, shipinstruct);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, shipmode);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct LINEITEM_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.orderkey);
      hash_combine(h, e.partkey);
      hash_combine(h, e.suppkey);
      hash_combine(h, e.linenumber);
      hash_combine(h, e.quantity);
      hash_combine(h, e.extendedprice);
      hash_combine(h, e.discount);
      hash_combine(h, e.tax);
      hash_combine(h, e.returnflag);
      hash_combine(h, e.linestatus);
      hash_combine(h, e.shipdate);
      hash_combine(h, e.commitdate);
      hash_combine(h, e.receiptdate);
      hash_combine(h, e.shipinstruct);
      hash_combine(h, e.shipmode);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const LINEITEM_entry& x, const LINEITEM_entry& y) {
      return x.orderkey == y.orderkey && x.partkey == y.partkey && x.suppkey == y.suppkey && x.linenumber == y.linenumber && x.quantity == y.quantity && x.extendedprice == y.extendedprice && x.discount == y.discount && x.tax == y.tax && x.returnflag == y.returnflag && x.linestatus == y.linestatus && x.shipdate == y.shipdate && x.commitdate == y.commitdate && x.receiptdate == y.receiptdate && x.shipinstruct == y.shipinstruct && x.shipmode == y.shipmode && x.comment == y.comment;
    }
  };
  
  struct LINEITEM_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const LINEITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.partkey);
      return h;
    }
    
    FORCE_INLINE static bool equals(const LINEITEM_entry& x, const LINEITEM_entry& y) {
      return x.partkey == y.partkey;
    }
  };
  
  typedef MultiHashMap<LINEITEM_entry, long, 
    PrimaryHashIndex<LINEITEM_entry, LINEITEM_mapkey0123456789101112131415_idxfn>,
    SecondaryHashIndex<LINEITEM_entry, LINEITEM_mapkey1_idxfn>
  > LINEITEM_map;
  
  
  struct ORDERS_entry {
    long orderkey; long custkey; STRING_TYPE orderstatus; DOUBLE_TYPE totalprice; date orderdate; STRING_TYPE orderpriority; STRING_TYPE clerk; long shippriority; STRING_TYPE comment; long __av; ORDERS_entry* nxt; ORDERS_entry* prv;
  
    explicit ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit ORDERS_entry(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8, const long c9) { orderkey = c0; custkey = c1; orderstatus = c2; totalprice = c3; orderdate = c4; orderpriority = c5; clerk = c6; shippriority = c7; comment = c8; __av = c9;  }
    ORDERS_entry(const ORDERS_entry& other) : orderkey(other.orderkey), custkey(other.custkey), orderstatus(other.orderstatus), totalprice(other.totalprice), orderdate(other.orderdate), orderpriority(other.orderpriority), clerk(other.clerk), shippriority(other.shippriority), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    ORDERS_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        orderkey = std::stol(f[0]); custkey = std::stol(f[1]); orderstatus = f[2]; totalprice = std::stod(f[3]); orderdate = Udate(f[4]); orderpriority = f[5]; clerk = f[6]; shippriority = std::stol(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE ORDERS_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8) { orderkey = c0; custkey = c1; orderstatus = c2; totalprice = c3; orderdate = c4; orderpriority = c5; clerk = c6; shippriority = c7; comment = c8;  return *this; }
    FORCE_INLINE ORDERS_entry& modify0(const long c0) { orderkey = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orderkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, custkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orderstatus);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, totalprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orderdate);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, orderpriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, clerk);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, shippriority);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct ORDERS_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const ORDERS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.orderkey);
      hash_combine(h, e.custkey);
      hash_combine(h, e.orderstatus);
      hash_combine(h, e.totalprice);
      hash_combine(h, e.orderdate);
      hash_combine(h, e.orderpriority);
      hash_combine(h, e.clerk);
      hash_combine(h, e.shippriority);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const ORDERS_entry& x, const ORDERS_entry& y) {
      return x.orderkey == y.orderkey && x.custkey == y.custkey && x.orderstatus == y.orderstatus && x.totalprice == y.totalprice && x.orderdate == y.orderdate && x.orderpriority == y.orderpriority && x.clerk == y.clerk && x.shippriority == y.shippriority && x.comment == y.comment;
    }
  };
  
  struct ORDERS_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const ORDERS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.orderkey);
      return h;
    }
    
    FORCE_INLINE static bool equals(const ORDERS_entry& x, const ORDERS_entry& y) {
      return x.orderkey == y.orderkey;
    }
  };
  
  typedef MultiHashMap<ORDERS_entry, long, 
    PrimaryHashIndex<ORDERS_entry, ORDERS_mapkey012345678_idxfn>,
    SecondaryHashIndex<ORDERS_entry, ORDERS_mapkey0_idxfn>
  > ORDERS_map;
  
  
  struct PART_entry {
    long partkey; STRING_TYPE name; STRING_TYPE mfgr; STRING_TYPE brand; STRING_TYPE type; long size; STRING_TYPE container; DOUBLE_TYPE retailprice; STRING_TYPE comment; long __av; PART_entry* nxt; PART_entry* prv;
  
    explicit PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit PART_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const long c9) { partkey = c0; name = c1; mfgr = c2; brand = c3; type = c4; size = c5; container = c6; retailprice = c7; comment = c8; __av = c9;  }
    PART_entry(const PART_entry& other) : partkey(other.partkey), name(other.name), mfgr(other.mfgr), brand(other.brand), type(other.type), size(other.size), container(other.container), retailprice(other.retailprice), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    PART_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        partkey = std::stol(f[0]); name = f[1]; mfgr = f[2]; brand = f[3]; type = f[4]; size = std::stol(f[5]); container = f[6]; retailprice = std::stod(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE PART_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8) { partkey = c0; name = c1; mfgr = c2; brand = c3; type = c4; size = c5; container = c6; retailprice = c7; comment = c8;  return *this; }
    FORCE_INLINE PART_entry& modify4(const STRING_TYPE& c4) { type = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mfgr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, brand);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, type);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, size);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, container);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, retailprice);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PART_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const PART_entry& e) {
      size_t h = 0;
      hash_combine(h, e.partkey);
      hash_combine(h, e.name);
      hash_combine(h, e.mfgr);
      hash_combine(h, e.brand);
      hash_combine(h, e.type);
      hash_combine(h, e.size);
      hash_combine(h, e.container);
      hash_combine(h, e.retailprice);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const PART_entry& x, const PART_entry& y) {
      return x.partkey == y.partkey && x.name == y.name && x.mfgr == y.mfgr && x.brand == y.brand && x.type == y.type && x.size == y.size && x.container == y.container && x.retailprice == y.retailprice && x.comment == y.comment;
    }
  };
  
  struct PART_mapkey4_idxfn {
    FORCE_INLINE static size_t hash(const PART_entry& e) {
      size_t h = 0;
      hash_combine(h, e.type);
      return h;
    }
    
    FORCE_INLINE static bool equals(const PART_entry& x, const PART_entry& y) {
      return x.type == y.type;
    }
  };
  
  typedef MultiHashMap<PART_entry, long, 
    PrimaryHashIndex<PART_entry, PART_mapkey012345678_idxfn>,
    SecondaryHashIndex<PART_entry, PART_mapkey4_idxfn>
  > PART_map;
  
  
  struct CUSTOMER_entry {
    long custkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE mktsegment; STRING_TYPE comment; long __av; CUSTOMER_entry* nxt; CUSTOMER_entry* prv;
  
    explicit CUSTOMER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit CUSTOMER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const STRING_TYPE& c7, const long c8) { custkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; mktsegment = c6; comment = c7; __av = c8;  }
    CUSTOMER_entry(const CUSTOMER_entry& other) : custkey(other.custkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), mktsegment(other.mktsegment), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    CUSTOMER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 8) return; */
        custkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); mktsegment = f[6]; comment = f[7]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE CUSTOMER_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const STRING_TYPE& c7) { custkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; mktsegment = c6; comment = c7;  return *this; }
    FORCE_INLINE CUSTOMER_entry& modify03(const long c0, const long c3) { custkey = c0; nationkey = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, custkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, address);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, nationkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, phone);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, acctbal);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mktsegment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct CUSTOMER_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const CUSTOMER_entry& e) {
      size_t h = 0;
      hash_combine(h, e.custkey);
      hash_combine(h, e.name);
      hash_combine(h, e.address);
      hash_combine(h, e.nationkey);
      hash_combine(h, e.phone);
      hash_combine(h, e.acctbal);
      hash_combine(h, e.mktsegment);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const CUSTOMER_entry& x, const CUSTOMER_entry& y) {
      return x.custkey == y.custkey && x.name == y.name && x.address == y.address && x.nationkey == y.nationkey && x.phone == y.phone && x.acctbal == y.acctbal && x.mktsegment == y.mktsegment && x.comment == y.comment;
    }
  };
  
  struct CUSTOMER_mapkey03_idxfn {
    FORCE_INLINE static size_t hash(const CUSTOMER_entry& e) {
      size_t h = 0;
      hash_combine(h, e.custkey);
      hash_combine(h, e.nationkey);
      return h;
    }
    
    FORCE_INLINE static bool equals(const CUSTOMER_entry& x, const CUSTOMER_entry& y) {
      return x.custkey == y.custkey && x.nationkey == y.nationkey;
    }
  };
  
  typedef MultiHashMap<CUSTOMER_entry, long, 
    PrimaryHashIndex<CUSTOMER_entry, CUSTOMER_mapkey01234567_idxfn>,
    SecondaryHashIndex<CUSTOMER_entry, CUSTOMER_mapkey03_idxfn>
  > CUSTOMER_map;
  
  
  struct SUPPLIER_entry {
    long suppkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE comment; long __av; SUPPLIER_entry* nxt; SUPPLIER_entry* prv;
  
    explicit SUPPLIER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit SUPPLIER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const long c7) { suppkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; comment = c6; __av = c7;  }
    SUPPLIER_entry(const SUPPLIER_entry& other) : suppkey(other.suppkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    SUPPLIER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 7) return; */
        suppkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); comment = f[6]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE SUPPLIER_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6) { suppkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; comment = c6;  return *this; }
    FORCE_INLINE SUPPLIER_entry& modify0(const long c0) { suppkey = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, suppkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, address);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, nationkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, phone);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, acctbal);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct SUPPLIER_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const SUPPLIER_entry& e) {
      size_t h = 0;
      hash_combine(h, e.suppkey);
      hash_combine(h, e.name);
      hash_combine(h, e.address);
      hash_combine(h, e.nationkey);
      hash_combine(h, e.phone);
      hash_combine(h, e.acctbal);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const SUPPLIER_entry& x, const SUPPLIER_entry& y) {
      return x.suppkey == y.suppkey && x.name == y.name && x.address == y.address && x.nationkey == y.nationkey && x.phone == y.phone && x.acctbal == y.acctbal && x.comment == y.comment;
    }
  };
  
  struct SUPPLIER_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const SUPPLIER_entry& e) {
      size_t h = 0;
      hash_combine(h, e.suppkey);
      return h;
    }
    
    FORCE_INLINE static bool equals(const SUPPLIER_entry& x, const SUPPLIER_entry& y) {
      return x.suppkey == y.suppkey;
    }
  };
  
  typedef MultiHashMap<SUPPLIER_entry, long, 
    PrimaryHashIndex<SUPPLIER_entry, SUPPLIER_mapkey0123456_idxfn>,
    SecondaryHashIndex<SUPPLIER_entry, SUPPLIER_mapkey0_idxfn>
  > SUPPLIER_map;
  
  
  struct NATION_entry {
    long nationkey; STRING_TYPE name; long regionkey; STRING_TYPE comment; long __av; NATION_entry* nxt; NATION_entry* prv;
  
    explicit NATION_entry() : nxt(nullptr), prv(nullptr) { }
    explicit NATION_entry(const long c0, const STRING_TYPE& c1, const long c2, const STRING_TYPE& c3, const long c4) { nationkey = c0; name = c1; regionkey = c2; comment = c3; __av = c4;  }
    NATION_entry(const NATION_entry& other) : nationkey(other.nationkey), name(other.name), regionkey(other.regionkey), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    NATION_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 4) return; */
        nationkey = std::stol(f[0]); name = f[1]; regionkey = std::stol(f[2]); comment = f[3]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE NATION_entry& modify(const long c0, const STRING_TYPE& c1, const long c2, const STRING_TYPE& c3) { nationkey = c0; name = c1; regionkey = c2; comment = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, nationkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, regionkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct NATION_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const NATION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.nationkey);
      hash_combine(h, e.name);
      hash_combine(h, e.regionkey);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const NATION_entry& x, const NATION_entry& y) {
      return x.nationkey == y.nationkey && x.name == y.name && x.regionkey == y.regionkey && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<NATION_entry, long, 
    PrimaryHashIndex<NATION_entry, NATION_mapkey0123_idxfn>
  > NATION_map;
  
  
  struct REGION_entry {
    long regionkey; STRING_TYPE name; STRING_TYPE comment; long __av; REGION_entry* nxt; REGION_entry* prv;
  
    explicit REGION_entry() : nxt(nullptr), prv(nullptr) { }
    explicit REGION_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3) { regionkey = c0; name = c1; comment = c2; __av = c3;  }
    REGION_entry(const REGION_entry& other) : regionkey(other.regionkey), name(other.name), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    REGION_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 3) return; */
        regionkey = std::stol(f[0]); name = f[1]; comment = f[2]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE REGION_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2) { regionkey = c0; name = c1; comment = c2;  return *this; }
    FORCE_INLINE REGION_entry& modify01(const long c0, const STRING_TYPE& c1) { regionkey = c0; name = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, regionkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, name);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct REGION_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const REGION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.regionkey);
      hash_combine(h, e.name);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const REGION_entry& x, const REGION_entry& y) {
      return x.regionkey == y.regionkey && x.name == y.name && x.comment == y.comment;
    }
  };
  
  struct REGION_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const REGION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.regionkey);
      hash_combine(h, e.name);
      return h;
    }
    
    FORCE_INLINE static bool equals(const REGION_entry& x, const REGION_entry& y) {
      return x.regionkey == y.regionkey && x.name == y.name;
    }
  };
  
  typedef MultiHashMap<REGION_entry, long, 
    PrimaryHashIndex<REGION_entry, REGION_mapkey012_idxfn>,
    SecondaryHashIndex<REGION_entry, REGION_mapkey01_idxfn>
  > REGION_map;

  /* Temporary entry type definitions */
  struct tuple2_L_D {
    long _1; PAYLOAD_entry __av; tuple2_L_D* nxt; tuple2_L_D* prv;
    explicit tuple2_L_D() : nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE tuple2_L_D& modify(const long c1) { _1 = c1; return *this; }
    static bool equals(const tuple2_L_D &x, const tuple2_L_D &y) {
      return ((x._1 == y._1));
    }
    static long hash(const tuple2_L_D &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      return h;
    }
  };
  
  struct tuple2_S_L {
    STRING_TYPE _1; long __av; tuple2_S_L* nxt; tuple2_S_L* prv;
    explicit tuple2_S_L() : nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE tuple2_S_L& modify(const STRING_TYPE& c1) { _1 = c1; return *this; }
    static bool equals(const tuple2_S_L &x, const tuple2_S_L &y) {
      return ((x._1 == y._1));
    }
    static long hash(const tuple2_S_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      return h;
    }
  };
  
  struct tuple2_L_L {
    long _1; long __av; tuple2_L_L* nxt; tuple2_L_L* prv;
    explicit tuple2_L_L() : nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE tuple2_L_L& modify(const long c1) { _1 = c1; return *this; }
    static bool equals(const tuple2_L_L &x, const tuple2_L_L &y) {
      return ((x._1 == y._1));
    }
    static long hash(const tuple2_L_L &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      return h;
    }
  };

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
      const QUERY8_map& _QUERY8 = get_QUERY8();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(QUERY8), _QUERY8, "\t");
    }
  
    /* Functions returning / computing the results of top level queries */
    const QUERY8_map& get_QUERY8() const {
      return QUERY8;
    }
  
  protected:
    /* Data structures used for storing / computing top-level queries */
    QUERY8_map QUERY8;
    
  };

  /* Contains materialized views and processing (IVM) logic */
  struct data_t : tlq_t {
  
    data_t(): tlq_t() {
      c3 = Udate(STRING_TYPE("1996-12-31"));
      c2 = STRING_TYPE("ECONOMY ANODIZED STEEL");
      c1 = STRING_TYPE("AMERICA");
      c4 = Udate(STRING_TYPE("1995-1-1"));
      c5 = STRING_TYPE("BRAZIL");
    }
    
  
    /* Trigger functions for table relations */
    void on_insert_LINEITEM(const long orderkey, const long partkey, const long suppkey, const long linenumber, const DOUBLE_TYPE quantity, const DOUBLE_TYPE extendedprice, const DOUBLE_TYPE discount, const DOUBLE_TYPE tax, const STRING_TYPE returnflag, const STRING_TYPE linestatus, const date shipdate, const date commitdate, const date receiptdate, const STRING_TYPE shipinstruct, const STRING_TYPE shipmode, const STRING_TYPE comment) {
      LINEITEM_entry e(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, comment, 1L);
      LINEITEM.addOrDelOnZero(e, 1L);
    }
    void on_insert_LINEITEM(LINEITEM_entry &e) {
      e.__av = 1L;
      LINEITEM.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_ORDERS(const long orderkey, const long custkey, const STRING_TYPE orderstatus, const DOUBLE_TYPE totalprice, const date orderdate, const STRING_TYPE orderpriority, const STRING_TYPE clerk, const long shippriority, const STRING_TYPE comment) {
      ORDERS_entry e(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, comment, 1L);
      ORDERS.addOrDelOnZero(e, 1L);
    }
    void on_insert_ORDERS(ORDERS_entry &e) {
      e.__av = 1L;
      ORDERS.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_PART(const long partkey, const STRING_TYPE name, const STRING_TYPE mfgr, const STRING_TYPE brand, const STRING_TYPE type, const long size, const STRING_TYPE container, const DOUBLE_TYPE retailprice, const STRING_TYPE comment) {
      PART_entry e(partkey, name, mfgr, brand, type, size, container, retailprice, comment, 1L);
      PART.addOrDelOnZero(e, 1L);
    }
    void on_insert_PART(PART_entry &e) {
      e.__av = 1L;
      PART.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_CUSTOMER(const long custkey, const STRING_TYPE name, const STRING_TYPE address, const long nationkey, const STRING_TYPE phone, const DOUBLE_TYPE acctbal, const STRING_TYPE mktsegment, const STRING_TYPE comment) {
      CUSTOMER_entry e(custkey, name, address, nationkey, phone, acctbal, mktsegment, comment, 1L);
      CUSTOMER.addOrDelOnZero(e, 1L);
    }
    void on_insert_CUSTOMER(CUSTOMER_entry &e) {
      e.__av = 1L;
      CUSTOMER.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_SUPPLIER(const long suppkey, const STRING_TYPE name, const STRING_TYPE address, const long nationkey, const STRING_TYPE phone, const DOUBLE_TYPE acctbal, const STRING_TYPE comment) {
      SUPPLIER_entry e(suppkey, name, address, nationkey, phone, acctbal, comment, 1L);
      SUPPLIER.addOrDelOnZero(e, 1L);
    }
    void on_insert_SUPPLIER(SUPPLIER_entry &e) {
      e.__av = 1L;
      SUPPLIER.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_NATION(const long nationkey, const STRING_TYPE name, const long regionkey, const STRING_TYPE comment) {
      NATION_entry e(nationkey, name, regionkey, comment, 1L);
      NATION.addOrDelOnZero(e, 1L);
    }
    void on_insert_NATION(NATION_entry &e) {
      e.__av = 1L;
      NATION.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_REGION(const long regionkey, const STRING_TYPE name, const STRING_TYPE comment) {
      REGION_entry e(regionkey, name, comment, 1L);
      REGION.addOrDelOnZero(e, 1L);
    }
    void on_insert_REGION(REGION_entry &e) {
      e.__av = 1L;
      REGION.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_system_ready_event() {
      V_NATION1.clear();
      { //foreach
        NATION_entry* e1 = NATION.head;
        while (e1) {
          long c_nationkey = e1->nationkey;
          STRING_TYPE n_name = e1->name;
          long n_regionkey = e1->regionkey;
          STRING_TYPE n_comment = e1->comment;
          long v1 = e1->__av;
          STRING_TYPE l1 = c1;
          { //slice
            const SecondaryIdxNode<REGION_entry>* n1_head = static_cast<const SecondaryIdxNode<REGION_entry>*>(REGION.slice(se2.modify01(n_regionkey, l1), 0));
            const SecondaryIdxNode<REGION_entry>* n1 = n1_head;
            REGION_entry* e2;
            while (n1) {
              e2 = n1->obj;
              STRING_TYPE r_comment = e2->comment;
              long v2 = e2->__av;
              V_NATION1.addOrDelOnZero(se1.modify(c_nationkey), (v1 * v2));
              n1 = (n1 != n1_head ? n1->nxt : n1->child);
            }
          }
          e1 = e1->nxt;
        }
      }
      
      V_NATION2.clear();
      { //foreach
        NATION_entry* e3 = NATION.head;
        while (e3) {
          long s_nationkey = e3->nationkey;
          STRING_TYPE n_name = e3->name;
          long n_regionkey = e3->regionkey;
          STRING_TYPE n_comment = e3->comment;
          long v3 = e3->__av;
          V_NATION2.addOrDelOnZero(se3.modify(s_nationkey, n_name), v3);
          e3 = e3->nxt;
        }
      }
      
      QUERY8.clear();
      agg1.clear();
      agg2.clear();
      STRING_TYPE l2 = c2;
      { //slice
        const SecondaryIdxNode<PART_entry>* n2_head = static_cast<const SecondaryIdxNode<PART_entry>*>(PART.slice(se5.modify4(l2), 0));
        const SecondaryIdxNode<PART_entry>* n2 = n2_head;
        PART_entry* e4;
        while (n2) {
          e4 = n2->obj;
          long l_partkey = e4->partkey;
          STRING_TYPE p_name = e4->name;
          STRING_TYPE p_mfgr = e4->mfgr;
          STRING_TYPE p_brand = e4->brand;
          long p_size = e4->size;
          STRING_TYPE p_container = e4->container;
          DOUBLE_TYPE p_retailprice = e4->retailprice;
          STRING_TYPE p_comment = e4->comment;
          long v4 = e4->__av;
          agg2.add(st1.modify(l_partkey), v4);
          n2 = (n2 != n2_head ? n2->nxt : n2->child);
        }
      }
      { // temp foreach
        tuple2_L_L* e5 = agg2.head;
        while(e5) {
          long l_partkey = e5->_1; 
          long v5 = e5->__av; 
      
          { //slice
            const SecondaryIdxNode<LINEITEM_entry>* n5_head = static_cast<const SecondaryIdxNode<LINEITEM_entry>*>(LINEITEM.slice(se8.modify1(l_partkey), 0));
            const SecondaryIdxNode<LINEITEM_entry>* n5 = n5_head;
            LINEITEM_entry* e6;
            while (n5) {
              e6 = n5->obj;
              long o_orderkey = e6->orderkey;
              long s_suppkey = e6->suppkey;
              long l_linenumber = e6->linenumber;
              DOUBLE_TYPE l_quantity = e6->quantity;
              DOUBLE_TYPE l_extendedprice = e6->extendedprice;
              DOUBLE_TYPE l_discount = e6->discount;
              DOUBLE_TYPE l_tax = e6->tax;
              STRING_TYPE l_returnflag = e6->returnflag;
              STRING_TYPE l_linestatus = e6->linestatus;
              date l_shipdate = e6->shipdate;
              date l_commitdate = e6->commitdate;
              date l_receiptdate = e6->receiptdate;
              STRING_TYPE l_shipinstruct = e6->shipinstruct;
              STRING_TYPE l_shipmode = e6->shipmode;
              STRING_TYPE l_comment = e6->comment;
              long v6 = e6->__av;
              agg3.clear();
              { //slice
                const SecondaryIdxNode<SUPPLIER_entry>* n4_head = static_cast<const SecondaryIdxNode<SUPPLIER_entry>*>(SUPPLIER.slice(se7.modify0(s_suppkey), 0));
                const SecondaryIdxNode<SUPPLIER_entry>* n4 = n4_head;
                SUPPLIER_entry* e7;
                while (n4) {
                  e7 = n4->obj;
                  STRING_TYPE s_name = e7->name;
                  STRING_TYPE s_address = e7->address;
                  long s_nationkey = e7->nationkey;
                  STRING_TYPE s_phone = e7->phone;
                  DOUBLE_TYPE s_acctbal = e7->acctbal;
                  STRING_TYPE s_comment = e7->comment;
                  long v7 = e7->__av;
                  { //slice
                    const SecondaryIdxNode<V_NATION2_entry>* n3_head = static_cast<const SecondaryIdxNode<V_NATION2_entry>*>(V_NATION2.slice(se6.modify0(s_nationkey), 0));
                    const SecondaryIdxNode<V_NATION2_entry>* n3 = n3_head;
                    V_NATION2_entry* e8;
                    while (n3) {
                      e8 = n3->obj;
                      STRING_TYPE n_name = e8->N_NAME;
                      long v8 = e8->__av;
                      agg3.add(st2.modify(n_name), (v7 * v8));
                      n3 = (n3 != n3_head ? n3->nxt : n3->child);
                    }
                  }
                  n4 = (n4 != n4_head ? n4->nxt : n4->child);
                }
              }
              { // temp foreach
                tuple2_S_L* e9 = agg3.head;
                while(e9) {
                  STRING_TYPE n_name = e9->_1; 
                  long v9 = e9->__av; 
                
                  DOUBLE_TYPE tmp = v5 * v6 * v9 * l_extendedprice * (1L + (-1L * l_discount));
                  entry_PAYLOAD.partial_mkt_share = (c5 == n_name) ? tmp : 0;
                  entry_PAYLOAD.total_mkt_share = tmp;
                  agg1.add(st3.modify(o_orderkey), entry_PAYLOAD);
              
                  e9 = e9->nxt;
                }
              }
              n5 = (n5 != n5_head ? n5->nxt : n5->child);
            }
          }
      
          e5 = e5->nxt;
        }
      }
      { // temp foreach
        tuple2_L_D* e10 = agg1.head;
        while(e10) {
          long o_orderkey = e10->_1; 
          const PAYLOAD_entry& v10 = e10->__av; 
      
          { //slice
            const SecondaryIdxNode<ORDERS_entry>* n7_head = static_cast<const SecondaryIdxNode<ORDERS_entry>*>(ORDERS.slice(se10.modify0(o_orderkey), 0));
            const SecondaryIdxNode<ORDERS_entry>* n7 = n7_head;
            ORDERS_entry* e11;
            while (n7) {
              e11 = n7->obj;
              long c_custkey = e11->custkey;
              STRING_TYPE o_orderstatus = e11->orderstatus;
              DOUBLE_TYPE o_totalprice = e11->totalprice;
              date o_orderdate = e11->orderdate;
              STRING_TYPE o_orderpriority = e11->orderpriority;
              STRING_TYPE o_clerk = e11->clerk;
              long o_shippriority = e11->shippriority;
              STRING_TYPE o_comment = e11->comment;
              long v11 = e11->__av;
              if (c3 >= o_orderdate) {
                if (o_orderdate >= c4) {
                  long l3 = Udate_year(o_orderdate);
                  long agg4 = 0L;
                  { //foreach
                    V_NATION1_entry* e12 = V_NATION1.head;
                    while (e12) {
                      long c_nationkey = e12->C_NATIONKEY;
                      long v12 = e12->__av;
                      { //slice
                        const SecondaryIdxNode<CUSTOMER_entry>* n6_head = static_cast<const SecondaryIdxNode<CUSTOMER_entry>*>(CUSTOMER.slice(se9.modify03(c_custkey, c_nationkey), 0));
                        const SecondaryIdxNode<CUSTOMER_entry>* n6 = n6_head;
                        CUSTOMER_entry* e13;
                        while (n6) {
                          e13 = n6->obj;
                          STRING_TYPE c_name = e13->name;
                          STRING_TYPE c_address = e13->address;
                          STRING_TYPE c_phone = e13->phone;
                          DOUBLE_TYPE c_acctbal = e13->acctbal;
                          STRING_TYPE c_mktsegment = e13->mktsegment;
                          STRING_TYPE c_comment = e13->comment;
                          long v13 = e13->__av;
                          agg4 += (v12 * v13);
                          n6 = (n6 != n6_head ? n6->nxt : n6->child);
                        }
                      }
                      e12 = e12->nxt;
                    }
                  }

                  long tmp = v11 * agg4;
                  entry_PAYLOAD.partial_mkt_share = v10.partial_mkt_share * tmp;
                  entry_PAYLOAD.total_mkt_share = v10.total_mkt_share * tmp;
                  QUERY8.addOrDelOnZero(se4.modify(l3), entry_PAYLOAD);
                }
              }
              n7 = (n7 != n7_head ? n7->nxt : n7->child);
            }
          }
      
          e10 = e10->nxt;
        }
      }
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      V_NATION1_entry se1;
      REGION_entry se2;
      V_NATION2_entry se3;
      QUERY8_entry se4;
      tuple2_L_L st1;
      PART_entry se5;
      tuple2_S_L st2;
      V_NATION2_entry se6;
      SUPPLIER_entry se7;
      tuple2_L_D st3;
      LINEITEM_entry se8;
      CUSTOMER_entry se9;
      ORDERS_entry se10;
    
      
    
      /* Data structures used for storing materialized views */
      V_NATION1_map V_NATION1;
      V_NATION2_map V_NATION2;
      LINEITEM_map LINEITEM;
      ORDERS_map ORDERS;
      PART_map PART;
      CUSTOMER_map CUSTOMER;
      SUPPLIER_map SUPPLIER;
      NATION_map NATION;
      REGION_map REGION;

      PAYLOAD_entry entry_PAYLOAD;
    
      /* Data structures used as temporary materialized views */
      MultiHashMap<tuple2_L_L, long, PrimaryHashIndex<tuple2_L_L> > agg2;
      MultiHashMap<tuple2_L_D, PAYLOAD_entry, PrimaryHashIndex<tuple2_L_D> > agg1;
      MultiHashMap<tuple2_S_L, long, PrimaryHashIndex<tuple2_S_L> > agg3;
    
      /* Constant defitions */
      /* const static */ long c3;
      /* const static */ STRING_TYPE c2;
      /* const static */ STRING_TYPE c1;
      /* const static */ long c4;
      /* const static */ STRING_TYPE c5;
  };

  

}