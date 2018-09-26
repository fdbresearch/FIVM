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
#define RELATION_SUPPLIER_STATIC
#define RELATION_PARTSUPP_STATIC
#define RELATION_NATION_STATIC

namespace dbtoaster {

  /* Definitions of maps used for storing materialized views. */
  struct PAYLOAD_entry {
    static PAYLOAD_entry zero;

    DOUBLE_TYPE value1;
    DOUBLE_TYPE value2;

    explicit PAYLOAD_entry() { 
      clear();
    }

    FORCE_INLINE PAYLOAD_entry& operator +=(const PAYLOAD_entry &obj) {
      value1 += obj.value1;
      value2 += obj.value2;
      return *this;
    }

    FORCE_INLINE bool operator ==(const PAYLOAD_entry &obj) const {
      return (value1 == obj.value1 && value2 == obj.value2);
    }

    FORCE_INLINE void clear() { 
      value1 = 0.0;
      value2 = 0.0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, value1);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, value2);
    }
  };

  PAYLOAD_entry PAYLOAD_entry::zero = PAYLOAD_entry();

  struct V_NATION_entry {
    long N_NATIONKEY; STRING_TYPE N_NAME; long __av; V_NATION_entry* nxt; V_NATION_entry* prv;
  
    explicit V_NATION_entry() : nxt(nullptr), prv(nullptr) { }
    explicit V_NATION_entry(const long c0, const STRING_TYPE& c1, const long c2) { N_NATIONKEY = c0; N_NAME = c1; __av = c2;  }
    V_NATION_entry(const V_NATION_entry& other) : N_NATIONKEY(other.N_NATIONKEY), N_NAME(other.N_NAME), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    V_NATION_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        N_NATIONKEY = std::stol(f[0]); N_NAME = f[1]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE V_NATION_entry& modify(const long c0, const STRING_TYPE& c1) { N_NATIONKEY = c0; N_NAME = c1;  return *this; }
    FORCE_INLINE V_NATION_entry& modify0(const long c0) { N_NATIONKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NAME);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct V_NATION_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const V_NATION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.N_NATIONKEY);
      hash_combine(h, e.N_NAME);
      return h;
    }
    
    FORCE_INLINE static bool equals(const V_NATION_entry& x, const V_NATION_entry& y) {
      return x.N_NATIONKEY == y.N_NATIONKEY && x.N_NAME == y.N_NAME;
    }
  };
  
  struct V_NATION_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const V_NATION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.N_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const V_NATION_entry& x, const V_NATION_entry& y) {
      return x.N_NATIONKEY == y.N_NATIONKEY;
    }
  };
  
  typedef MultiHashMap<V_NATION_entry, long, 
    PrimaryHashIndex<V_NATION_entry, V_NATION_mapkey01_idxfn>,
    SecondaryHashIndex<V_NATION_entry, V_NATION_mapkey0_idxfn>
  > V_NATION_map;
  
  
  struct QUERY9_entry {
    STRING_TYPE N_NAME; long YEAR; DOUBLE_TYPE __av; QUERY9_entry* nxt; QUERY9_entry* prv;
  
    explicit QUERY9_entry() : nxt(nullptr), prv(nullptr) { }
    explicit QUERY9_entry(const STRING_TYPE& c0, const long c1, const DOUBLE_TYPE c2) { N_NAME = c0; YEAR = c1; __av = c2;  }
    QUERY9_entry(const QUERY9_entry& other) : N_NAME(other.N_NAME), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }

    
    FORCE_INLINE QUERY9_entry& modify(const STRING_TYPE& c0, const long c1) { N_NAME = c0; YEAR = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NAME);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct QUERY9_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const QUERY9_entry& e) {
      size_t h = 0;
      hash_combine(h, e.N_NAME);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const QUERY9_entry& x, const QUERY9_entry& y) {
      return x.N_NAME == y.N_NAME && x.YEAR == y.YEAR;
    }
  };
  
  typedef MultiHashMap<QUERY9_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<QUERY9_entry, QUERY9_mapkey01_idxfn>
  > QUERY9_map;
  
  
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
  
  typedef MultiHashMap<PART_entry, long, 
    PrimaryHashIndex<PART_entry, PART_mapkey012345678_idxfn>
  > PART_map;
  
  
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
  
  
  struct PARTSUPP_entry {
    long partkey; long suppkey; long availqty; DOUBLE_TYPE supplycost; STRING_TYPE comment; long __av; PARTSUPP_entry* nxt; PARTSUPP_entry* prv;
  
    explicit PARTSUPP_entry() : nxt(nullptr), prv(nullptr) { }
    explicit PARTSUPP_entry(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3, const STRING_TYPE& c4, const long c5) { partkey = c0; suppkey = c1; availqty = c2; supplycost = c3; comment = c4; __av = c5;  }
    PARTSUPP_entry(const PARTSUPP_entry& other) : partkey(other.partkey), suppkey(other.suppkey), availqty(other.availqty), supplycost(other.supplycost), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    PARTSUPP_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 5) return; */
        partkey = std::stol(f[0]); suppkey = std::stol(f[1]); availqty = std::stol(f[2]); supplycost = std::stod(f[3]); comment = f[4]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE PARTSUPP_entry& modify(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3, const STRING_TYPE& c4) { partkey = c0; suppkey = c1; availqty = c2; supplycost = c3; comment = c4;  return *this; }
    FORCE_INLINE PARTSUPP_entry& modify01(const long c0, const long c1) { partkey = c0; suppkey = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, partkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, suppkey);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, availqty);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supplycost);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, comment);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PARTSUPP_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const PARTSUPP_entry& e) {
      size_t h = 0;
      hash_combine(h, e.partkey);
      hash_combine(h, e.suppkey);
      hash_combine(h, e.availqty);
      hash_combine(h, e.supplycost);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const PARTSUPP_entry& x, const PARTSUPP_entry& y) {
      return x.partkey == y.partkey && x.suppkey == y.suppkey && x.availqty == y.availqty && x.supplycost == y.supplycost && x.comment == y.comment;
    }
  };
  
  struct PARTSUPP_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const PARTSUPP_entry& e) {
      size_t h = 0;
      hash_combine(h, e.partkey);
      hash_combine(h, e.suppkey);
      return h;
    }
    
    FORCE_INLINE static bool equals(const PARTSUPP_entry& x, const PARTSUPP_entry& y) {
      return x.partkey == y.partkey && x.suppkey == y.suppkey;
    }
  };
  
  typedef MultiHashMap<PARTSUPP_entry, long, 
    PrimaryHashIndex<PARTSUPP_entry, PARTSUPP_mapkey01234_idxfn>,
    SecondaryHashIndex<PARTSUPP_entry, PARTSUPP_mapkey01_idxfn>
  > PARTSUPP_map;
  
  
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

  /* Temporary entry type definitions */
  struct tuple3_LL_D {
    long _1; long _2; PAYLOAD_entry __av; tuple3_LL_D* nxt; tuple3_LL_D* prv;
    explicit tuple3_LL_D() : nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE tuple3_LL_D& modify(const long c1, const long c2) { _1 = c1; _2 = c2; return *this; }
    static bool equals(const tuple3_LL_D &x, const tuple3_LL_D &y) {
      return ((x._1 == y._1) && (x._2 == y._2));
    }
    static long hash(const tuple3_LL_D &e) {
      size_t h = 0;
      hash_combine(h, e._1);
      hash_combine(h, e._2);
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
      const QUERY9_map& _QUERY9 = get_QUERY9();
      dbtoaster::serialize_nvp_tabbed(ar, STRING(QUERY9), _QUERY9, "\t");
    }
  
    /* Functions returning / computing the results of top level queries */
    const QUERY9_map& get_QUERY9() const {
      return QUERY9;
    }
  
  protected:
    /* Data structures used for storing / computing top-level queries */
    QUERY9_map QUERY9;
    
  };

  /* Contains materialized views and processing (IVM) logic */
  struct data_t : tlq_t {
  
    data_t(): tlq_t() {
      
      /* Regular expression initialization */
      if (regcomp(&preg1, "^.*green.*$", REG_EXTENDED | REG_NOSUB)) {
        cerr << "Error compiling regular expression: /^.*green.*$/" << endl;
        exit(-1);
      }
    }
  
    ~data_t() {
      regfree(&preg1);
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
    
    void on_insert_SUPPLIER(const long suppkey, const STRING_TYPE name, const STRING_TYPE address, const long nationkey, const STRING_TYPE phone, const DOUBLE_TYPE acctbal, const STRING_TYPE comment) {
      SUPPLIER_entry e(suppkey, name, address, nationkey, phone, acctbal, comment, 1L);
      SUPPLIER.addOrDelOnZero(e, 1L);
    }
    void on_insert_SUPPLIER(SUPPLIER_entry &e) {
      e.__av = 1L;
      SUPPLIER.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_PARTSUPP(const long partkey, const long suppkey, const long availqty, const DOUBLE_TYPE supplycost, const STRING_TYPE comment) {
      PARTSUPP_entry e(partkey, suppkey, availqty, supplycost, comment, 1L);
      PARTSUPP.addOrDelOnZero(e, 1L);
    }
    void on_insert_PARTSUPP(PARTSUPP_entry &e) {
      e.__av = 1L;
      PARTSUPP.addOrDelOnZero(e, 1L);
    }
    
    void on_insert_NATION(const long nationkey, const STRING_TYPE name, const long regionkey, const STRING_TYPE comment) {
      NATION_entry e(nationkey, name, regionkey, comment, 1L);
      NATION.addOrDelOnZero(e, 1L);
    }
    void on_insert_NATION(NATION_entry &e) {
      e.__av = 1L;
      NATION.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_system_ready_event() {
      V_NATION.clear();
      { //foreach
        NATION_entry* e1 = NATION.head;
        while (e1) {
          long n_nationkey = e1->nationkey;
          STRING_TYPE n_name = e1->name;
          long n_regionkey = e1->regionkey;
          STRING_TYPE n_comment = e1->comment;
          long v1 = e1->__av;
          V_NATION.addOrDelOnZero(se1.modify(n_nationkey, n_name), v1);
          e1 = e1->nxt;
        }
      }
      
      QUERY9.clear();
      agg1.clear();
      agg2.clear();
      { //foreach
        PART_entry* e2 = PART.head;
        while (e2) {
          long p_partkey = e2->partkey;
          STRING_TYPE p_name = e2->name;
          STRING_TYPE p_mfgr = e2->mfgr;
          STRING_TYPE p_brand = e2->brand;
          STRING_TYPE p_type = e2->type;
          long p_size = e2->size;
          STRING_TYPE p_container = e2->container;
          DOUBLE_TYPE p_retailprice = e2->retailprice;
          STRING_TYPE p_comment = e2->comment;
          long v2 = e2->__av;
          (/*if */(0L != Upreg_match(preg1,p_name)) ? agg2.add(st1.modify(p_partkey), v2) : (void)0);
          e2 = e2->nxt;
        }
      }
      { // temp foreach
        tuple2_L_L* e3 = agg2.head;
        while(e3) {
          long p_partkey = e3->_1; 
          long v3 = e3->__av; 
      
          agg3.clear();
          { //slice
            const SecondaryIdxNode<LINEITEM_entry>* n1_head = static_cast<const SecondaryIdxNode<LINEITEM_entry>*>(LINEITEM.slice(se3.modify1(p_partkey), 0));
            const SecondaryIdxNode<LINEITEM_entry>* n1 = n1_head;
            LINEITEM_entry* e4;
            while (n1) {
              e4 = n1->obj;
              long o_orderkey = e4->orderkey;
              long l_suppkey = e4->suppkey;
              long l_linenumber = e4->linenumber;
              DOUBLE_TYPE l_quantity = e4->quantity;
              DOUBLE_TYPE l_extendedprice = e4->extendedprice;
              DOUBLE_TYPE l_discount = e4->discount;
              DOUBLE_TYPE l_tax = e4->tax;
              STRING_TYPE l_returnflag = e4->returnflag;
              STRING_TYPE l_linestatus = e4->linestatus;
              date l_shipdate = e4->shipdate;
              date l_commitdate = e4->commitdate;
              date l_receiptdate = e4->receiptdate;
              STRING_TYPE l_shipinstruct = e4->shipinstruct;
              STRING_TYPE l_shipmode = e4->shipmode;
              STRING_TYPE l_comment = e4->comment;
              long v4 = e4->__av;

              entry_PAYLOAD.value1 = v4 * l_extendedprice * (1L + (-1L * l_discount));
              entry_PAYLOAD.value2 = v4 * l_quantity;
              agg3.add(st2.modify(o_orderkey, l_suppkey), entry_PAYLOAD);

              n1 = (n1 != n1_head ? n1->nxt : n1->child);
            }
          }
          { // temp foreach
            tuple3_LL_D* e5 = agg3.head;
            while(e5) {
              long o_orderkey = e5->_1;
              long l_suppkey = e5->_2; 
              const PAYLOAD_entry& v5 = e5->__av; 
          
              PAYLOAD_entry agg4;
              { //slice
                const SecondaryIdxNode<PARTSUPP_entry>* n2_head = static_cast<const SecondaryIdxNode<PARTSUPP_entry>*>(PARTSUPP.slice(se4.modify01(p_partkey, l_suppkey), 0));
                const SecondaryIdxNode<PARTSUPP_entry>* n2 = n2_head;
                PARTSUPP_entry* e6;
                while (n2) {
                  e6 = n2->obj;
                  long ps_availqty = e6->availqty;
                  DOUBLE_TYPE ps_supplycost = e6->supplycost;
                  STRING_TYPE ps_comment = e6->comment;
                  long v6 = e6->__av;
                  agg4.value1 += v6;
                  agg4.value2 += v6 * ps_supplycost;
                  n2 = (n2 != n2_head ? n2->nxt : n2->child);
                }
              }
              agg5.clear();
              { //slice
                const SecondaryIdxNode<SUPPLIER_entry>* n3_head = static_cast<const SecondaryIdxNode<SUPPLIER_entry>*>(SUPPLIER.slice(se5.modify0(l_suppkey), 0));
                const SecondaryIdxNode<SUPPLIER_entry>* n3 = n3_head;
                SUPPLIER_entry* e7;
                while (n3) {
                  e7 = n3->obj;
                  STRING_TYPE s_name = e7->name;
                  STRING_TYPE s_address = e7->address;
                  long s_nationkey = e7->nationkey;
                  STRING_TYPE s_phone = e7->phone;
                  DOUBLE_TYPE s_acctbal = e7->acctbal;
                  STRING_TYPE s_comment = e7->comment;
                  long v7 = e7->__av;
                  agg5.add(st3.modify(s_nationkey), v7);
                  n3 = (n3 != n3_head ? n3->nxt : n3->child);
                }
              }
              { // temp foreach
                tuple2_L_L* e8 = agg5.head;
                while(e8) {
                  long s_nationkey = e8->_1; 
                  long v8 = e8->__av; 
              
                  entry_PAYLOAD.value1 = v3 * v5.value1 * agg4.value1 * v8;
                  entry_PAYLOAD.value2 = v3 * v5.value2 * agg4.value2 * v8;

                  agg1.add(st4.modify(o_orderkey, s_nationkey), entry_PAYLOAD);
              
                  e8 = e8->nxt;
                }
              }
          
              e5 = e5->nxt;
            }
          }
      
          e3 = e3->nxt;
        }
      }
      { // temp foreach
        tuple3_LL_D* e9 = agg1.head;
        while(e9) {
          long o_orderkey = e9->_1;
          long s_nationkey = e9->_2; 
          const PAYLOAD_entry& v9 = e9->__av; 
      
          agg6.clear();
          { //slice
            const SecondaryIdxNode<ORDERS_entry>* n4_head = static_cast<const SecondaryIdxNode<ORDERS_entry>*>(ORDERS.slice(se6.modify0(o_orderkey), 0));
            const SecondaryIdxNode<ORDERS_entry>* n4 = n4_head;
            ORDERS_entry* e10;
            while (n4) {
              e10 = n4->obj;
              long c_custkey = e10->custkey;
              STRING_TYPE o_orderstatus = e10->orderstatus;
              DOUBLE_TYPE o_totalprice = e10->totalprice;
              date o_orderdate = e10->orderdate;
              STRING_TYPE o_orderpriority = e10->orderpriority;
              STRING_TYPE o_clerk = e10->clerk;
              long o_shippriority = e10->shippriority;
              STRING_TYPE o_comment = e10->comment;
              long v10 = e10->__av;
              long l1 = Udate_year(o_orderdate);
              agg6.add(st5.modify(l1), v10);
              n4 = (n4 != n4_head ? n4->nxt : n4->child);
            }
          }
          { // temp foreach
            tuple2_L_L* e11 = agg6.head;
            while(e11) {
              long year = e11->_1; 
              long v11 = e11->__av; 
          
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n5_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se7.modify0(s_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n5 = n5_head;
                V_NATION_entry* e12;
                while (n5) {
                  e12 = n5->obj;
                  STRING_TYPE n_name = e12->N_NAME;
                  long v12 = e12->__av;

                  QUERY9.addOrDelOnZero(se2.modify(n_name, year), (v9.value1 - v9.value2) * v11 * v12);
                  n5 = (n5 != n5_head ? n5->nxt : n5->child);
                }
              }
          
              e11 = e11->nxt;
            }
          }
      
          e9 = e9->nxt;
        }
      }
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      V_NATION_entry se1;
      QUERY9_entry se2;
      tuple2_L_L st1;
      tuple3_LL_D st2;
      LINEITEM_entry se3;
      PARTSUPP_entry se4;
      tuple2_L_L st3;
      SUPPLIER_entry se5;
      tuple3_LL_D st4;
      tuple2_L_L st5;
      ORDERS_entry se6;
      V_NATION_entry se7;
    
      /* Regular expression objects */
      regex_t preg1;
    
      /* Data structures used for storing materialized views */
      V_NATION_map V_NATION;
      LINEITEM_map LINEITEM;
      ORDERS_map ORDERS;
      PART_map PART;
      SUPPLIER_map SUPPLIER;
      PARTSUPP_map PARTSUPP;
      NATION_map NATION;
    
      PAYLOAD_entry entry_PAYLOAD;

      /* Data structures used as temporary materialized views */
      MultiHashMap<tuple2_L_L, long, PrimaryHashIndex<tuple2_L_L> > agg6;
      MultiHashMap<tuple2_L_L, long, PrimaryHashIndex<tuple2_L_L> > agg2;
      MultiHashMap<tuple2_L_L, long, PrimaryHashIndex<tuple2_L_L> > agg5;
      MultiHashMap<tuple3_LL_D, PAYLOAD_entry, PrimaryHashIndex<tuple3_LL_D> > agg1;
      MultiHashMap<tuple3_LL_D, PAYLOAD_entry, PrimaryHashIndex<tuple3_LL_D> > agg3;
    
  };

  

}