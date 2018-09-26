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
#define RELATION_PART_DYNAMIC
#define RELATION_SUPPLIER_DYNAMIC
#define RELATION_PARTSUPP_DYNAMIC
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

  struct LINEITEM_entry {
    long orderkey; long partkey; long suppkey; long linenumber; DOUBLE_TYPE quantity; DOUBLE_TYPE extendedprice; DOUBLE_TYPE discount; DOUBLE_TYPE tax; STRING_TYPE returnflag; STRING_TYPE linestatus; date shipdate; date commitdate; date receiptdate; STRING_TYPE shipinstruct; STRING_TYPE shipmode; STRING_TYPE comment; long __av; LINEITEM_entry* nxt; LINEITEM_entry* prv;
  
    explicit LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit LINEITEM_entry(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15, const long c16) { orderkey = c0; partkey = c1; suppkey = c2; linenumber = c3; quantity = c4; extendedprice = c5; discount = c6; tax = c7; returnflag = c8; linestatus = c9; shipdate = c10; commitdate = c11; receiptdate = c12; shipinstruct = c13; shipmode = c14; comment = c15; __av = c16;  }
    LINEITEM_entry(const LINEITEM_entry& other) : orderkey(other.orderkey), partkey(other.partkey), suppkey(other.suppkey), linenumber(other.linenumber), quantity(other.quantity), extendedprice(other.extendedprice), discount(other.discount), tax(other.tax), returnflag(other.returnflag), linestatus(other.linestatus), shipdate(other.shipdate), commitdate(other.commitdate), receiptdate(other.receiptdate), shipinstruct(other.shipinstruct), shipmode(other.shipmode), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    LINEITEM_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 16) return; */
        orderkey = std::stol(f[0]); partkey = std::stol(f[1]); suppkey = std::stol(f[2]); linenumber = std::stol(f[3]); quantity = std::stod(f[4]); extendedprice = std::stod(f[5]); discount = std::stod(f[6]); tax = std::stod(f[7]); returnflag = f[8]; linestatus = f[9]; shipdate = Udate(f[10]); commitdate = Udate(f[11]); receiptdate = Udate(f[12]); shipinstruct = f[13]; shipmode = f[14]; comment = f[15]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  struct ORDERS_entry {
    long orderkey; long custkey; STRING_TYPE orderstatus; DOUBLE_TYPE totalprice; date orderdate; STRING_TYPE orderpriority; STRING_TYPE clerk; long shippriority; STRING_TYPE comment; long __av; ORDERS_entry* nxt; ORDERS_entry* prv;
  
    explicit ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit ORDERS_entry(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8, const long c9) { orderkey = c0; custkey = c1; orderstatus = c2; totalprice = c3; orderdate = c4; orderpriority = c5; clerk = c6; shippriority = c7; comment = c8; __av = c9;  }
    ORDERS_entry(const ORDERS_entry& other) : orderkey(other.orderkey), custkey(other.custkey), orderstatus(other.orderstatus), totalprice(other.totalprice), orderdate(other.orderdate), orderpriority(other.orderpriority), clerk(other.clerk), shippriority(other.shippriority), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    ORDERS_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        orderkey = std::stol(f[0]); custkey = std::stol(f[1]); orderstatus = f[2]; totalprice = std::stod(f[3]); orderdate = Udate(f[4]); orderpriority = f[5]; clerk = f[6]; shippriority = std::stol(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  struct PART_entry {
    long partkey; STRING_TYPE name; STRING_TYPE mfgr; STRING_TYPE brand; STRING_TYPE type; long size; STRING_TYPE container; DOUBLE_TYPE retailprice; STRING_TYPE comment; long __av; PART_entry* nxt; PART_entry* prv;
  
    explicit PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit PART_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const long c9) { partkey = c0; name = c1; mfgr = c2; brand = c3; type = c4; size = c5; container = c6; retailprice = c7; comment = c8; __av = c9;  }
    PART_entry(const PART_entry& other) : partkey(other.partkey), name(other.name), mfgr(other.mfgr), brand(other.brand), type(other.type), size(other.size), container(other.container), retailprice(other.retailprice), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    PART_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        partkey = std::stol(f[0]); name = f[1]; mfgr = f[2]; brand = f[3]; type = f[4]; size = std::stol(f[5]); container = f[6]; retailprice = std::stod(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  struct SUPPLIER_entry {
    long suppkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE comment; long __av; SUPPLIER_entry* nxt; SUPPLIER_entry* prv;
  
    explicit SUPPLIER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit SUPPLIER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const long c7) { suppkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; comment = c6; __av = c7;  }
    SUPPLIER_entry(const SUPPLIER_entry& other) : suppkey(other.suppkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    SUPPLIER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 7) return; */
        suppkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); comment = f[6]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  struct PARTSUPP_entry {
    long partkey; long suppkey; long availqty; DOUBLE_TYPE supplycost; STRING_TYPE comment; long __av; PARTSUPP_entry* nxt; PARTSUPP_entry* prv;
  
    explicit PARTSUPP_entry() : nxt(nullptr), prv(nullptr) { }
    explicit PARTSUPP_entry(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3, const STRING_TYPE& c4, const long c5) { partkey = c0; suppkey = c1; availqty = c2; supplycost = c3; comment = c4; __av = c5;  }
    PARTSUPP_entry(const PARTSUPP_entry& other) : partkey(other.partkey), suppkey(other.suppkey), availqty(other.availqty), supplycost(other.supplycost), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    PARTSUPP_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 5) return; */
        partkey = std::stol(f[0]); suppkey = std::stol(f[1]); availqty = std::stol(f[2]); supplycost = std::stod(f[3]); comment = f[4]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  
  struct AGG_Lineitem_entry {
    long L_ORDERKEY; long L_PARTKEY; long L_SUPPKEY; PAYLOAD_entry __av; AGG_Lineitem_entry* nxt; AGG_Lineitem_entry* prv;
  
    explicit AGG_Lineitem_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Lineitem_entry(const long c0, const long c1, const long c2, const PAYLOAD_entry& c3) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2; __av = c3;  }
    AGG_Lineitem_entry(const AGG_Lineitem_entry& other) : L_ORDERKEY(other.L_ORDERKEY), L_PARTKEY(other.L_PARTKEY), L_SUPPKEY(other.L_SUPPKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE AGG_Lineitem_entry& modify(const long c0, const long c1, const long c2) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2;  return *this; }
    FORCE_INLINE AGG_Lineitem_entry& modify12(const long c1, const long c2) { L_PARTKEY = c1; L_SUPPKEY = c2;  return *this; }
    FORCE_INLINE AGG_Lineitem_entry& modify1(const long c1) { L_PARTKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Lineitem_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Lineitem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      hash_combine(h, e.L_PARTKEY);
      hash_combine(h, e.L_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Lineitem_entry& x, const AGG_Lineitem_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY && x.L_PARTKEY == y.L_PARTKEY && x.L_SUPPKEY == y.L_SUPPKEY;
    }
  };
  
  struct AGG_Lineitem_mapkey12_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Lineitem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_PARTKEY);
      hash_combine(h, e.L_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Lineitem_entry& x, const AGG_Lineitem_entry& y) {
      return x.L_PARTKEY == y.L_PARTKEY && x.L_SUPPKEY == y.L_SUPPKEY;
    }
  };
  
  struct AGG_Lineitem_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Lineitem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_PARTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Lineitem_entry& x, const AGG_Lineitem_entry& y) {
      return x.L_PARTKEY == y.L_PARTKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Lineitem_entry, PAYLOAD_entry, 
    PrimaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey012_idxfn>,
    SecondaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey12_idxfn>,
    SecondaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey1_idxfn>
  > AGG_Lineitem_map;
  
  
  struct AGG_Part_entry {
    long P_PARTKEY; DOUBLE_TYPE __av; AGG_Part_entry* nxt; AGG_Part_entry* prv;
  
    explicit AGG_Part_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Part_entry(const long c0, const DOUBLE_TYPE c1) { P_PARTKEY = c0; __av = c1;  }
    AGG_Part_entry(const AGG_Part_entry& other) : P_PARTKEY(other.P_PARTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Part_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 1) return; */
        P_PARTKEY = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Part_entry& modify(const long c0) { P_PARTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, P_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Part_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Part_entry& e) {
      size_t h = 0;
      hash_combine(h, e.P_PARTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Part_entry& x, const AGG_Part_entry& y) {
      return x.P_PARTKEY == y.P_PARTKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Part_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<AGG_Part_entry, AGG_Part_mapkey0_idxfn>
  > AGG_Part_map;
  
  
  struct AGG_Supplier_entry {
    long S_SUPPKEY; long N_NATIONKEY; long __av; AGG_Supplier_entry* nxt; AGG_Supplier_entry* prv;
  
    explicit AGG_Supplier_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Supplier_entry(const long c0, const long c1, const long c2) { S_SUPPKEY = c0; N_NATIONKEY = c1; __av = c2;  }
    AGG_Supplier_entry(const AGG_Supplier_entry& other) : S_SUPPKEY(other.S_SUPPKEY), N_NATIONKEY(other.N_NATIONKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Supplier_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        S_SUPPKEY = std::stol(f[0]); N_NATIONKEY = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Supplier_entry& modify(const long c0, const long c1) { S_SUPPKEY = c0; N_NATIONKEY = c1;  return *this; }
    FORCE_INLINE AGG_Supplier_entry& modify0(const long c0) { S_SUPPKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, S_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Supplier_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Supplier_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_SUPPKEY);
      hash_combine(h, e.N_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Supplier_entry& x, const AGG_Supplier_entry& y) {
      return x.S_SUPPKEY == y.S_SUPPKEY && x.N_NATIONKEY == y.N_NATIONKEY;
    }
  };
  
  struct AGG_Supplier_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Supplier_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Supplier_entry& x, const AGG_Supplier_entry& y) {
      return x.S_SUPPKEY == y.S_SUPPKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Supplier_entry, long, 
    PrimaryHashIndex<AGG_Supplier_entry, AGG_Supplier_mapkey01_idxfn>,
    SecondaryHashIndex<AGG_Supplier_entry, AGG_Supplier_mapkey0_idxfn>
  > AGG_Supplier_map;
  
  
  struct AGG_PartSupp_entry {
    long PS_PARTKEY; long PS_SUPPKEY; PAYLOAD_entry __av; AGG_PartSupp_entry* nxt; AGG_PartSupp_entry* prv;
  
    explicit AGG_PartSupp_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_PartSupp_entry(const long c0, const long c1, const PAYLOAD_entry& c2) { PS_PARTKEY = c0; PS_SUPPKEY = c1; __av = c2;  }
    AGG_PartSupp_entry(const AGG_PartSupp_entry& other) : PS_PARTKEY(other.PS_PARTKEY), PS_SUPPKEY(other.PS_SUPPKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE AGG_PartSupp_entry& modify(const long c0, const long c1) { PS_PARTKEY = c0; PS_SUPPKEY = c1;  return *this; }
    FORCE_INLINE AGG_PartSupp_entry& modify1(const long c1) { PS_SUPPKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, PS_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, PS_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_PartSupp_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_PartSupp_entry& e) {
      size_t h = 0;
      hash_combine(h, e.PS_PARTKEY);
      hash_combine(h, e.PS_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_PartSupp_entry& x, const AGG_PartSupp_entry& y) {
      return x.PS_PARTKEY == y.PS_PARTKEY && x.PS_SUPPKEY == y.PS_SUPPKEY;
    }
  };
  
  struct AGG_PartSupp_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const AGG_PartSupp_entry& e) {
      size_t h = 0;
      hash_combine(h, e.PS_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_PartSupp_entry& x, const AGG_PartSupp_entry& y) {
      return x.PS_SUPPKEY == y.PS_SUPPKEY;
    }
  };
  
  typedef MultiHashMap<AGG_PartSupp_entry, PAYLOAD_entry, 
    PrimaryHashIndex<AGG_PartSupp_entry, AGG_PartSupp_mapkey01_idxfn>,
    SecondaryHashIndex<AGG_PartSupp_entry, AGG_PartSupp_mapkey1_idxfn>
  > AGG_PartSupp_map;
  
  
  struct AGG_LineitemSupplierPart_entry {
    long L_ORDERKEY; long N_NATIONKEY; DOUBLE_TYPE __av; AGG_LineitemSupplierPart_entry* nxt; AGG_LineitemSupplierPart_entry* prv;
  
    explicit AGG_LineitemSupplierPart_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_LineitemSupplierPart_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { L_ORDERKEY = c0; N_NATIONKEY = c1; __av = c2;  }
    AGG_LineitemSupplierPart_entry(const AGG_LineitemSupplierPart_entry& other) : L_ORDERKEY(other.L_ORDERKEY), N_NATIONKEY(other.N_NATIONKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_LineitemSupplierPart_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) {
        /* if (f.size() < 2) return; */
        L_ORDERKEY = std::stol(f[0]); N_NATIONKEY = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_LineitemSupplierPart_entry& modify(const long c0, const long c1) { L_ORDERKEY = c0; N_NATIONKEY = c1;  return *this; }
    FORCE_INLINE AGG_LineitemSupplierPart_entry& modify0(const long c0) { L_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_LineitemSupplierPart_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_LineitemSupplierPart_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      hash_combine(h, e.N_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_LineitemSupplierPart_entry& x, const AGG_LineitemSupplierPart_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY && x.N_NATIONKEY == y.N_NATIONKEY;
    }
  };
  
  struct AGG_LineitemSupplierPart_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_LineitemSupplierPart_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_LineitemSupplierPart_entry& x, const AGG_LineitemSupplierPart_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<AGG_LineitemSupplierPart_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<AGG_LineitemSupplierPart_entry, AGG_LineitemSupplierPart_mapkey01_idxfn>,
    SecondaryHashIndex<AGG_LineitemSupplierPart_entry, AGG_LineitemSupplierPart_mapkey0_idxfn>
  > AGG_LineitemSupplierPart_map;
  
  
  struct AGG_Orders_entry {
    long O_ORDERKEY; long YEAR; long __av; AGG_Orders_entry* nxt; AGG_Orders_entry* prv;
  
    explicit AGG_Orders_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Orders_entry(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; YEAR = c1; __av = c2;  }
    AGG_Orders_entry(const AGG_Orders_entry& other) : O_ORDERKEY(other.O_ORDERKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Orders_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        O_ORDERKEY = std::stol(f[0]); YEAR = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Orders_entry& modify(const long c0, const long c1) { O_ORDERKEY = c0; YEAR = c1;  return *this; }
    FORCE_INLINE AGG_Orders_entry& modify0(const long c0) { O_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Orders_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Orders_entry& x, const AGG_Orders_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.YEAR == y.YEAR;
    }
  };
  
  struct AGG_Orders_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Orders_entry& x, const AGG_Orders_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Orders_entry, long, 
    PrimaryHashIndex<AGG_Orders_entry, AGG_Orders_mapkey01_idxfn>,
    SecondaryHashIndex<AGG_Orders_entry, AGG_Orders_mapkey0_idxfn>
  > AGG_Orders_map;
  
  
  struct DELTA_AGG_Lineitem_entry {
    long L_ORDERKEY; long L_PARTKEY; long L_SUPPKEY; PAYLOAD_entry __av; DELTA_AGG_Lineitem_entry* nxt; DELTA_AGG_Lineitem_entry* prv;
  
    explicit DELTA_AGG_Lineitem_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Lineitem_entry(const long c0, const long c1, const long c2, const PAYLOAD_entry& c3) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2; __av = c3;  }
    DELTA_AGG_Lineitem_entry(const DELTA_AGG_Lineitem_entry& other) : L_ORDERKEY(other.L_ORDERKEY), L_PARTKEY(other.L_PARTKEY), L_SUPPKEY(other.L_SUPPKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE DELTA_AGG_Lineitem_entry& modify(const long c0, const long c1, const long c2) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Lineitem_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Lineitem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      hash_combine(h, e.L_PARTKEY);
      hash_combine(h, e.L_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Lineitem_entry& x, const DELTA_AGG_Lineitem_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY && x.L_PARTKEY == y.L_PARTKEY && x.L_SUPPKEY == y.L_SUPPKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Lineitem_entry, PAYLOAD_entry, 
    PrimaryHashIndex<DELTA_AGG_Lineitem_entry, DELTA_AGG_Lineitem_mapkey012_idxfn>
  > DELTA_AGG_Lineitem_map;
  
  
  struct DELTA_AGG_Part_entry {
    long P_PARTKEY; DOUBLE_TYPE __av; DELTA_AGG_Part_entry* nxt; DELTA_AGG_Part_entry* prv;
  
    explicit DELTA_AGG_Part_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Part_entry(const long c0, const DOUBLE_TYPE c1) { P_PARTKEY = c0; __av = c1;  }
    DELTA_AGG_Part_entry(const DELTA_AGG_Part_entry& other) : P_PARTKEY(other.P_PARTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Part_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 1) return; */
        P_PARTKEY = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Part_entry& modify(const long c0) { P_PARTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, P_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Part_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Part_entry& e) {
      size_t h = 0;
      hash_combine(h, e.P_PARTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Part_entry& x, const DELTA_AGG_Part_entry& y) {
      return x.P_PARTKEY == y.P_PARTKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Part_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<DELTA_AGG_Part_entry, DELTA_AGG_Part_mapkey0_idxfn>
  > DELTA_AGG_Part_map;
  
  
  struct DELTA_AGG_Supplier_entry {
    long S_SUPPKEY; long N_NATIONKEY; long __av; DELTA_AGG_Supplier_entry* nxt; DELTA_AGG_Supplier_entry* prv;
  
    explicit DELTA_AGG_Supplier_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Supplier_entry(const long c0, const long c1, const long c2) { S_SUPPKEY = c0; N_NATIONKEY = c1; __av = c2;  }
    DELTA_AGG_Supplier_entry(const DELTA_AGG_Supplier_entry& other) : S_SUPPKEY(other.S_SUPPKEY), N_NATIONKEY(other.N_NATIONKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Supplier_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        S_SUPPKEY = std::stol(f[0]); N_NATIONKEY = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Supplier_entry& modify(const long c0, const long c1) { S_SUPPKEY = c0; N_NATIONKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, S_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Supplier_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Supplier_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_SUPPKEY);
      hash_combine(h, e.N_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Supplier_entry& x, const DELTA_AGG_Supplier_entry& y) {
      return x.S_SUPPKEY == y.S_SUPPKEY && x.N_NATIONKEY == y.N_NATIONKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Supplier_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Supplier_entry, DELTA_AGG_Supplier_mapkey01_idxfn>
  > DELTA_AGG_Supplier_map;
  
  
  struct DELTA_AGG_PartSupp_entry {
    long PS_PARTKEY; long PS_SUPPKEY; PAYLOAD_entry __av; DELTA_AGG_PartSupp_entry* nxt; DELTA_AGG_PartSupp_entry* prv;
  
    explicit DELTA_AGG_PartSupp_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_PartSupp_entry(const long c0, const long c1, const PAYLOAD_entry& c2) { PS_PARTKEY = c0; PS_SUPPKEY = c1; __av = c2;  }
    DELTA_AGG_PartSupp_entry(const DELTA_AGG_PartSupp_entry& other) : PS_PARTKEY(other.PS_PARTKEY), PS_SUPPKEY(other.PS_SUPPKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE DELTA_AGG_PartSupp_entry& modify(const long c0, const long c1) { PS_PARTKEY = c0; PS_SUPPKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, PS_PARTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, PS_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_PartSupp_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_PartSupp_entry& e) {
      size_t h = 0;
      hash_combine(h, e.PS_PARTKEY);
      hash_combine(h, e.PS_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_PartSupp_entry& x, const DELTA_AGG_PartSupp_entry& y) {
      return x.PS_PARTKEY == y.PS_PARTKEY && x.PS_SUPPKEY == y.PS_SUPPKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_PartSupp_entry, PAYLOAD_entry, 
    PrimaryHashIndex<DELTA_AGG_PartSupp_entry, DELTA_AGG_PartSupp_mapkey01_idxfn>
  > DELTA_AGG_PartSupp_map;
  
  
  struct DELTA_AGG_LineitemSupplierPart_entry {
    long L_ORDERKEY; long N_NATIONKEY; DOUBLE_TYPE __av; DELTA_AGG_LineitemSupplierPart_entry* nxt; DELTA_AGG_LineitemSupplierPart_entry* prv;
  
    explicit DELTA_AGG_LineitemSupplierPart_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_LineitemSupplierPart_entry(const long c0, const long c1, const DOUBLE_TYPE c2) { L_ORDERKEY = c0; N_NATIONKEY = c1; __av = c2;  }
    DELTA_AGG_LineitemSupplierPart_entry(const DELTA_AGG_LineitemSupplierPart_entry& other) : L_ORDERKEY(other.L_ORDERKEY), N_NATIONKEY(other.N_NATIONKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_LineitemSupplierPart_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) {
        /* if (f.size() < 2) return; */
        L_ORDERKEY = std::stol(f[0]); N_NATIONKEY = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_LineitemSupplierPart_entry& modify(const long c0, const long c1) { L_ORDERKEY = c0; N_NATIONKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NATIONKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_LineitemSupplierPart_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_LineitemSupplierPart_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      hash_combine(h, e.N_NATIONKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_LineitemSupplierPart_entry& x, const DELTA_AGG_LineitemSupplierPart_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY && x.N_NATIONKEY == y.N_NATIONKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_LineitemSupplierPart_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<DELTA_AGG_LineitemSupplierPart_entry, DELTA_AGG_LineitemSupplierPart_mapkey01_idxfn>
  > DELTA_AGG_LineitemSupplierPart_map;
  
  
  struct DELTA_AGG_Orders_entry {
    long O_ORDERKEY; long YEAR; long __av; DELTA_AGG_Orders_entry* nxt; DELTA_AGG_Orders_entry* prv;
  
    explicit DELTA_AGG_Orders_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Orders_entry(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; YEAR = c1; __av = c2;  }
    DELTA_AGG_Orders_entry(const DELTA_AGG_Orders_entry& other) : O_ORDERKEY(other.O_ORDERKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Orders_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        O_ORDERKEY = std::stol(f[0]); YEAR = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Orders_entry& modify(const long c0, const long c1) { O_ORDERKEY = c0; YEAR = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Orders_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Orders_entry& x, const DELTA_AGG_Orders_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.YEAR == y.YEAR;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Orders_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Orders_entry, DELTA_AGG_Orders_mapkey01_idxfn>
  > DELTA_AGG_Orders_map;
  
  
  struct QUERY9_entry {
    STRING_TYPE N_NAME; long YEAR; DOUBLE_TYPE __av; QUERY9_entry* nxt; QUERY9_entry* prv;
  
    explicit QUERY9_entry() : nxt(nullptr), prv(nullptr) { }
    explicit QUERY9_entry(const STRING_TYPE& c0, const long c1, const DOUBLE_TYPE c2) { N_NAME = c0; YEAR = c1; __av = c2;  }
    QUERY9_entry(const QUERY9_entry& other) : N_NAME(other.N_NAME), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    QUERY9_entry(const std::vector<std::string>& f, const DOUBLE_TYPE v) {
        /* if (f.size() < 2) return; */
        N_NAME = f[0]; YEAR = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  
  struct DELTA_LINEITEM_entry {
    long orderkey; long partkey; long suppkey; long linenumber; DOUBLE_TYPE quantity; DOUBLE_TYPE extendedprice; DOUBLE_TYPE discount; DOUBLE_TYPE tax; STRING_TYPE returnflag; STRING_TYPE linestatus; date shipdate; date commitdate; date receiptdate; STRING_TYPE shipinstruct; STRING_TYPE shipmode; STRING_TYPE comment; long __av; DELTA_LINEITEM_entry* nxt; DELTA_LINEITEM_entry* prv;
  
    explicit DELTA_LINEITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_LINEITEM_entry(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15, const long c16) { orderkey = c0; partkey = c1; suppkey = c2; linenumber = c3; quantity = c4; extendedprice = c5; discount = c6; tax = c7; returnflag = c8; linestatus = c9; shipdate = c10; commitdate = c11; receiptdate = c12; shipinstruct = c13; shipmode = c14; comment = c15; __av = c16;  }
    DELTA_LINEITEM_entry(const DELTA_LINEITEM_entry& other) : orderkey(other.orderkey), partkey(other.partkey), suppkey(other.suppkey), linenumber(other.linenumber), quantity(other.quantity), extendedprice(other.extendedprice), discount(other.discount), tax(other.tax), returnflag(other.returnflag), linestatus(other.linestatus), shipdate(other.shipdate), commitdate(other.commitdate), receiptdate(other.receiptdate), shipinstruct(other.shipinstruct), shipmode(other.shipmode), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_LINEITEM_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 16) return; */
        orderkey = std::stol(f[0]); partkey = std::stol(f[1]); suppkey = std::stol(f[2]); linenumber = std::stol(f[3]); quantity = std::stod(f[4]); extendedprice = std::stod(f[5]); discount = std::stod(f[6]); tax = std::stod(f[7]); returnflag = f[8]; linestatus = f[9]; shipdate = Udate(f[10]); commitdate = Udate(f[11]); receiptdate = Udate(f[12]); shipinstruct = f[13]; shipmode = f[14]; comment = f[15]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_LINEITEM_entry& modify(const long c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const STRING_TYPE& c9, const date c10, const date c11, const date c12, const STRING_TYPE& c13, const STRING_TYPE& c14, const STRING_TYPE& c15) { orderkey = c0; partkey = c1; suppkey = c2; linenumber = c3; quantity = c4; extendedprice = c5; discount = c6; tax = c7; returnflag = c8; linestatus = c9; shipdate = c10; commitdate = c11; receiptdate = c12; shipinstruct = c13; shipmode = c14; comment = c15;  return *this; }
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
  
  struct DELTA_LINEITEM_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_LINEITEM_entry& e) {
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
    
    FORCE_INLINE static bool equals(const DELTA_LINEITEM_entry& x, const DELTA_LINEITEM_entry& y) {
      return x.orderkey == y.orderkey && x.partkey == y.partkey && x.suppkey == y.suppkey && x.linenumber == y.linenumber && x.quantity == y.quantity && x.extendedprice == y.extendedprice && x.discount == y.discount && x.tax == y.tax && x.returnflag == y.returnflag && x.linestatus == y.linestatus && x.shipdate == y.shipdate && x.commitdate == y.commitdate && x.receiptdate == y.receiptdate && x.shipinstruct == y.shipinstruct && x.shipmode == y.shipmode && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_LINEITEM_entry, long, 
    PrimaryHashIndex<DELTA_LINEITEM_entry, DELTA_LINEITEM_mapkey0123456789101112131415_idxfn>
  > DELTA_LINEITEM_map;
  
  
  struct DELTA_ORDERS_entry {
    long orderkey; long custkey; STRING_TYPE orderstatus; DOUBLE_TYPE totalprice; date orderdate; STRING_TYPE orderpriority; STRING_TYPE clerk; long shippriority; STRING_TYPE comment; long __av; DELTA_ORDERS_entry* nxt; DELTA_ORDERS_entry* prv;
  
    explicit DELTA_ORDERS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_ORDERS_entry(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8, const long c9) { orderkey = c0; custkey = c1; orderstatus = c2; totalprice = c3; orderdate = c4; orderpriority = c5; clerk = c6; shippriority = c7; comment = c8; __av = c9;  }
    DELTA_ORDERS_entry(const DELTA_ORDERS_entry& other) : orderkey(other.orderkey), custkey(other.custkey), orderstatus(other.orderstatus), totalprice(other.totalprice), orderdate(other.orderdate), orderpriority(other.orderpriority), clerk(other.clerk), shippriority(other.shippriority), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_ORDERS_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        orderkey = std::stol(f[0]); custkey = std::stol(f[1]); orderstatus = f[2]; totalprice = std::stod(f[3]); orderdate = Udate(f[4]); orderpriority = f[5]; clerk = f[6]; shippriority = std::stol(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_ORDERS_entry& modify(const long c0, const long c1, const STRING_TYPE& c2, const DOUBLE_TYPE c3, const date c4, const STRING_TYPE& c5, const STRING_TYPE& c6, const long c7, const STRING_TYPE& c8) { orderkey = c0; custkey = c1; orderstatus = c2; totalprice = c3; orderdate = c4; orderpriority = c5; clerk = c6; shippriority = c7; comment = c8;  return *this; }
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
  
  struct DELTA_ORDERS_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_ORDERS_entry& e) {
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
    
    FORCE_INLINE static bool equals(const DELTA_ORDERS_entry& x, const DELTA_ORDERS_entry& y) {
      return x.orderkey == y.orderkey && x.custkey == y.custkey && x.orderstatus == y.orderstatus && x.totalprice == y.totalprice && x.orderdate == y.orderdate && x.orderpriority == y.orderpriority && x.clerk == y.clerk && x.shippriority == y.shippriority && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_ORDERS_entry, long, 
    PrimaryHashIndex<DELTA_ORDERS_entry, DELTA_ORDERS_mapkey012345678_idxfn>
  > DELTA_ORDERS_map;
  
  
  struct DELTA_PART_entry {
    long partkey; STRING_TYPE name; STRING_TYPE mfgr; STRING_TYPE brand; STRING_TYPE type; long size; STRING_TYPE container; DOUBLE_TYPE retailprice; STRING_TYPE comment; long __av; DELTA_PART_entry* nxt; DELTA_PART_entry* prv;
  
    explicit DELTA_PART_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_PART_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8, const long c9) { partkey = c0; name = c1; mfgr = c2; brand = c3; type = c4; size = c5; container = c6; retailprice = c7; comment = c8; __av = c9;  }
    DELTA_PART_entry(const DELTA_PART_entry& other) : partkey(other.partkey), name(other.name), mfgr(other.mfgr), brand(other.brand), type(other.type), size(other.size), container(other.container), retailprice(other.retailprice), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_PART_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 9) return; */
        partkey = std::stol(f[0]); name = f[1]; mfgr = f[2]; brand = f[3]; type = f[4]; size = std::stol(f[5]); container = f[6]; retailprice = std::stod(f[7]); comment = f[8]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_PART_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const STRING_TYPE& c3, const STRING_TYPE& c4, const long c5, const STRING_TYPE& c6, const DOUBLE_TYPE c7, const STRING_TYPE& c8) { partkey = c0; name = c1; mfgr = c2; brand = c3; type = c4; size = c5; container = c6; retailprice = c7; comment = c8;  return *this; }
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
  
  struct DELTA_PART_mapkey012345678_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_PART_entry& e) {
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
    
    FORCE_INLINE static bool equals(const DELTA_PART_entry& x, const DELTA_PART_entry& y) {
      return x.partkey == y.partkey && x.name == y.name && x.mfgr == y.mfgr && x.brand == y.brand && x.type == y.type && x.size == y.size && x.container == y.container && x.retailprice == y.retailprice && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_PART_entry, long, 
    PrimaryHashIndex<DELTA_PART_entry, DELTA_PART_mapkey012345678_idxfn>
  > DELTA_PART_map;
  
  
  struct DELTA_SUPPLIER_entry {
    long suppkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE comment; long __av; DELTA_SUPPLIER_entry* nxt; DELTA_SUPPLIER_entry* prv;
  
    explicit DELTA_SUPPLIER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_SUPPLIER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const long c7) { suppkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; comment = c6; __av = c7;  }
    DELTA_SUPPLIER_entry(const DELTA_SUPPLIER_entry& other) : suppkey(other.suppkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_SUPPLIER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 7) return; */
        suppkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); comment = f[6]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_SUPPLIER_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6) { suppkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; comment = c6;  return *this; }
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
  
  struct DELTA_SUPPLIER_mapkey0123456_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_SUPPLIER_entry& e) {
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
    
    FORCE_INLINE static bool equals(const DELTA_SUPPLIER_entry& x, const DELTA_SUPPLIER_entry& y) {
      return x.suppkey == y.suppkey && x.name == y.name && x.address == y.address && x.nationkey == y.nationkey && x.phone == y.phone && x.acctbal == y.acctbal && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_SUPPLIER_entry, long, 
    PrimaryHashIndex<DELTA_SUPPLIER_entry, DELTA_SUPPLIER_mapkey0123456_idxfn>
  > DELTA_SUPPLIER_map;
  
  
  struct DELTA_PARTSUPP_entry {
    long partkey; long suppkey; long availqty; DOUBLE_TYPE supplycost; STRING_TYPE comment; long __av; DELTA_PARTSUPP_entry* nxt; DELTA_PARTSUPP_entry* prv;
  
    explicit DELTA_PARTSUPP_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_PARTSUPP_entry(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3, const STRING_TYPE& c4, const long c5) { partkey = c0; suppkey = c1; availqty = c2; supplycost = c3; comment = c4; __av = c5;  }
    DELTA_PARTSUPP_entry(const DELTA_PARTSUPP_entry& other) : partkey(other.partkey), suppkey(other.suppkey), availqty(other.availqty), supplycost(other.supplycost), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_PARTSUPP_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 5) return; */
        partkey = std::stol(f[0]); suppkey = std::stol(f[1]); availqty = std::stol(f[2]); supplycost = std::stod(f[3]); comment = f[4]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_PARTSUPP_entry& modify(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3, const STRING_TYPE& c4) { partkey = c0; suppkey = c1; availqty = c2; supplycost = c3; comment = c4;  return *this; }
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
  
  struct DELTA_PARTSUPP_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_PARTSUPP_entry& e) {
      size_t h = 0;
      hash_combine(h, e.partkey);
      hash_combine(h, e.suppkey);
      hash_combine(h, e.availqty);
      hash_combine(h, e.supplycost);
      hash_combine(h, e.comment);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_PARTSUPP_entry& x, const DELTA_PARTSUPP_entry& y) {
      return x.partkey == y.partkey && x.suppkey == y.suppkey && x.availqty == y.availqty && x.supplycost == y.supplycost && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_PARTSUPP_entry, long, 
    PrimaryHashIndex<DELTA_PARTSUPP_entry, DELTA_PARTSUPP_mapkey01234_idxfn>
  > DELTA_PARTSUPP_map;

  

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
    void on_insert_NATION(const long nationkey, const STRING_TYPE name, const long regionkey, const STRING_TYPE comment) {
      NATION_entry e(nationkey, name, regionkey, comment, 1L);
      NATION.addOrDelOnZero(e, 1L);
    }
    void on_insert_NATION(NATION_entry &e) {
      e.__av = 1L;
      NATION.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_batch_update_LINEITEM(const std::vector<DELTA_LINEITEM_entry>::iterator &begin, const std::vector<DELTA_LINEITEM_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Lineitem.clear();
      { //foreach
        for (std::vector<DELTA_LINEITEM_entry>::iterator e1 = begin; e1 != end; e1++) {
          long l_orderkey = e1->orderkey;
          long l_partkey = e1->partkey;
          long l_suppkey = e1->suppkey;
          long l_linenumber = e1->linenumber;
          DOUBLE_TYPE l_quantity = e1->quantity;
          DOUBLE_TYPE l_extendedprice = e1->extendedprice;
          DOUBLE_TYPE l_discount = e1->discount;
          DOUBLE_TYPE l_tax = e1->tax;
          STRING_TYPE l_returnflag = e1->returnflag;
          STRING_TYPE l_linestatus = e1->linestatus;
          date l_shipdate = e1->shipdate;
          date l_commitdate = e1->commitdate;
          date l_receiptdate = e1->receiptdate;
          STRING_TYPE l_shipinstruct = e1->shipinstruct;
          STRING_TYPE l_shipmode = e1->shipmode;
          STRING_TYPE l_comment = e1->comment;
          long v1 = e1->__av;

          entry_PAYLOAD.value1 = v1 * l_extendedprice * (1L + (-1L * l_discount));
          entry_PAYLOAD.value2 = v1 * l_quantity;
          DELTA_AGG_Lineitem.addOrDelOnZero(se1.modify(l_orderkey, l_partkey, l_suppkey), entry_PAYLOAD);
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Lineitem_entry* e2 = DELTA_AGG_Lineitem.head;
        while (e2) {
          long l_orderkey = e2->L_ORDERKEY;
          long l_partkey = e2->L_PARTKEY;
          long l_suppkey = e2->L_SUPPKEY;
          const PAYLOAD_entry& v2 = e2->__av;
          { //slice
            const SecondaryIdxNode<AGG_Supplier_entry>* n1_head = static_cast<const SecondaryIdxNode<AGG_Supplier_entry>*>(AGG_Supplier.slice(se5.modify0(l_suppkey), 0));
            const SecondaryIdxNode<AGG_Supplier_entry>* n1 = n1_head;
            AGG_Supplier_entry* e3;
            while (n1) {
              e3 = n1->obj;
              long n_nationkey = e3->N_NATIONKEY;
              long v3 = e3->__av;

              long v4 = AGG_Part.getValueOrDefault(se3.modify(l_partkey));
              const PAYLOAD_entry& v5 = AGG_PartSupp.getValueOrDefault(se4.modify(l_partkey, l_suppkey));

              DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se2.modify(l_orderkey, n_nationkey), (v2.value1 * v5.value1 - v2.value2 * v5.value2)  * v3 * v4);
              n1 = (n1 != n1_head ? n1->nxt : n1->child);
            }
          }
          e2 = e2->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e4 = DELTA_AGG_LineitemSupplierPart.head;
        while (e4) {
          long l_orderkey = e4->L_ORDERKEY;
          long n_nationkey = e4->N_NATIONKEY;
          DOUBLE_TYPE v4 = e4->__av;
          { //slice
            const SecondaryIdxNode<AGG_Orders_entry>* n3_head = static_cast<const SecondaryIdxNode<AGG_Orders_entry>*>(AGG_Orders.slice(se8.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_Orders_entry>* n3 = n3_head;
            AGG_Orders_entry* e5;
            while (n3) {
              e5 = n3->obj;
              long year = e5->YEAR;
              long v5 = e5->__av;
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n2_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se7.modify0(n_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n2 = n2_head;
                V_NATION_entry* e6;
                while (n2) {
                  e6 = n2->obj;
                  STRING_TYPE n_name = e6->N_NAME;
                  long v6 = e6->__av;
                  QUERY9.addOrDelOnZero(se6.modify(n_name, year), (v4 * (v5 * v6)));
                  n2 = (n2 != n2_head ? n2->nxt : n2->child);
                }
              }
              n3 = (n3 != n3_head ? n3->nxt : n3->child);
            }
          }
          e4 = e4->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e7 = DELTA_AGG_LineitemSupplierPart.head;
        while (e7) {
          long l_orderkey = e7->L_ORDERKEY;
          long n_nationkey = e7->N_NATIONKEY;
          DOUBLE_TYPE v7 = e7->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se9.modify(l_orderkey, n_nationkey), v7);
          e7 = e7->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Lineitem_entry* e8 = DELTA_AGG_Lineitem.head;
        while (e8) {
          long l_orderkey = e8->L_ORDERKEY;
          long l_partkey = e8->L_PARTKEY;
          long l_suppkey = e8->L_SUPPKEY;
          const PAYLOAD_entry& v8 = e8->__av;
          AGG_Lineitem.addOrDelOnZero(se10.modify(l_orderkey, l_partkey, l_suppkey), v8);
          e8 = e8->nxt;
        }
      }
    }
    
    
    void on_batch_update_ORDERS(const std::vector<DELTA_ORDERS_entry>::iterator &begin, const std::vector<DELTA_ORDERS_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Orders.clear();
      { //foreach
        for (std::vector<DELTA_ORDERS_entry>::iterator e9 = begin; e9 != end; e9++) {
          long o_orderkey = e9->orderkey;
          long o_custkey = e9->custkey;
          STRING_TYPE o_orderstatus = e9->orderstatus;
          DOUBLE_TYPE o_totalprice = e9->totalprice;
          date o_orderdate = e9->orderdate;
          STRING_TYPE o_orderpriority = e9->orderpriority;
          STRING_TYPE o_clerk = e9->clerk;
          long o_shippriority = e9->shippriority;
          STRING_TYPE o_comment = e9->comment;
          long v9 = e9->__av;
          long l1 = Udate_year(o_orderdate);
          DELTA_AGG_Orders.addOrDelOnZero(se11.modify(o_orderkey, l1), v9);
        }
      }
      
      { //foreach
        DELTA_AGG_Orders_entry* e10 = DELTA_AGG_Orders.head;
        while (e10) {
          long o_orderkey = e10->O_ORDERKEY;
          long year = e10->YEAR;
          long v10 = e10->__av;
          { //slice
            const SecondaryIdxNode<AGG_LineitemSupplierPart_entry>* n5_head = static_cast<const SecondaryIdxNode<AGG_LineitemSupplierPart_entry>*>(AGG_LineitemSupplierPart.slice(se14.modify0(o_orderkey), 0));
            const SecondaryIdxNode<AGG_LineitemSupplierPart_entry>* n5 = n5_head;
            AGG_LineitemSupplierPart_entry* e11;
            while (n5) {
              e11 = n5->obj;
              long n_nationkey = e11->N_NATIONKEY;
              DOUBLE_TYPE v11 = e11->__av;
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n4_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se13.modify0(n_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n4 = n4_head;
                V_NATION_entry* e12;
                while (n4) {
                  e12 = n4->obj;
                  STRING_TYPE n_name = e12->N_NAME;
                  long v12 = e12->__av;
                  QUERY9.addOrDelOnZero(se12.modify(n_name, year), (v10 * (v11 * v12)));
                  n4 = (n4 != n4_head ? n4->nxt : n4->child);
                }
              }
              n5 = (n5 != n5_head ? n5->nxt : n5->child);
            }
          }
          e10 = e10->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Orders_entry* e13 = DELTA_AGG_Orders.head;
        while (e13) {
          long o_orderkey = e13->O_ORDERKEY;
          long year = e13->YEAR;
          long v13 = e13->__av;
          AGG_Orders.addOrDelOnZero(se15.modify(o_orderkey, year), v13);
          e13 = e13->nxt;
        }
      }
    }
    
    
    void on_batch_update_PARTSUPP(const std::vector<DELTA_PARTSUPP_entry>::iterator &begin, const std::vector<DELTA_PARTSUPP_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_PartSupp.clear();
      { //foreach
        for (std::vector<DELTA_PARTSUPP_entry>::iterator e14 = begin; e14 != end; e14++) {
          long ps_partkey = e14->partkey;
          long ps_suppkey = e14->suppkey;
          long ps_availqty = e14->availqty;
          DOUBLE_TYPE ps_supplycost = e14->supplycost;
          STRING_TYPE ps_comment = e14->comment;
          long v14 = e14->__av;

          entry_PAYLOAD.value1 = v14;
          entry_PAYLOAD.value2 = v14 * ps_supplycost;
          DELTA_AGG_PartSupp.addOrDelOnZero(se16.modify(ps_partkey, ps_suppkey), entry_PAYLOAD);
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_PartSupp_entry* e15 = DELTA_AGG_PartSupp.head;
        while (e15) {
          long l_partkey = e15->PS_PARTKEY;
          long l_suppkey = e15->PS_SUPPKEY;
          const PAYLOAD_entry& v15 = e15->__av;
          { //slice
            const SecondaryIdxNode<AGG_Lineitem_entry>* n7_head = static_cast<const SecondaryIdxNode<AGG_Lineitem_entry>*>(AGG_Lineitem.slice(se20.modify12(l_partkey, l_suppkey), 0));
            const SecondaryIdxNode<AGG_Lineitem_entry>* n7 = n7_head;
            AGG_Lineitem_entry* e16;
            while (n7) {
              e16 = n7->obj;
              long l_orderkey = e16->L_ORDERKEY;
              const PAYLOAD_entry& v16 = e16->__av;
              { //slice
                const SecondaryIdxNode<AGG_Supplier_entry>* n6_head = static_cast<const SecondaryIdxNode<AGG_Supplier_entry>*>(AGG_Supplier.slice(se19.modify0(l_suppkey), 0));
                const SecondaryIdxNode<AGG_Supplier_entry>* n6 = n6_head;
                AGG_Supplier_entry* e17;
                while (n6) {
                  e17 = n6->obj;
                  long n_nationkey = e17->N_NATIONKEY;
                  long v17 = e17->__av;

                  long v18 = AGG_Part.getValueOrDefault(se18.modify(l_partkey));

                  DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se17.modify(l_orderkey, n_nationkey), (v15.value1 * v16.value1 - v15.value2 * v16.value2) * v17 * v18);

                  n6 = (n6 != n6_head ? n6->nxt : n6->child);
                }
              }
              n7 = (n7 != n7_head ? n7->nxt : n7->child);
            }
          }
          e15 = e15->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e18 = DELTA_AGG_LineitemSupplierPart.head;
        while (e18) {
          long l_orderkey = e18->L_ORDERKEY;
          long n_nationkey = e18->N_NATIONKEY;
          DOUBLE_TYPE v18 = e18->__av;
          { //slice
            const SecondaryIdxNode<AGG_Orders_entry>* n9_head = static_cast<const SecondaryIdxNode<AGG_Orders_entry>*>(AGG_Orders.slice(se23.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_Orders_entry>* n9 = n9_head;
            AGG_Orders_entry* e19;
            while (n9) {
              e19 = n9->obj;
              long year = e19->YEAR;
              long v19 = e19->__av;
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n8_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se22.modify0(n_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n8 = n8_head;
                V_NATION_entry* e20;
                while (n8) {
                  e20 = n8->obj;
                  STRING_TYPE n_name = e20->N_NAME;
                  long v20 = e20->__av;
                  QUERY9.addOrDelOnZero(se21.modify(n_name, year), (v18 * (v19 * v20)));
                  n8 = (n8 != n8_head ? n8->nxt : n8->child);
                }
              }
              n9 = (n9 != n9_head ? n9->nxt : n9->child);
            }
          }
          e18 = e18->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e21 = DELTA_AGG_LineitemSupplierPart.head;
        while (e21) {
          long l_orderkey = e21->L_ORDERKEY;
          long n_nationkey = e21->N_NATIONKEY;
          DOUBLE_TYPE v21 = e21->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se24.modify(l_orderkey, n_nationkey), v21);
          e21 = e21->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_PartSupp_entry* e22 = DELTA_AGG_PartSupp.head;
        while (e22) {
          long ps_partkey = e22->PS_PARTKEY;
          long ps_suppkey = e22->PS_SUPPKEY;
          const PAYLOAD_entry& v22 = e22->__av;
          AGG_PartSupp.addOrDelOnZero(se25.modify(ps_partkey, ps_suppkey), v22);
          e22 = e22->nxt;
        }
      }
    }
    
    
    void on_batch_update_SUPPLIER(const std::vector<DELTA_SUPPLIER_entry>::iterator &begin, const std::vector<DELTA_SUPPLIER_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Supplier.clear();
      { //foreach
        for (std::vector<DELTA_SUPPLIER_entry>::iterator e23 = begin; e23 != end; e23++) {
          long s_suppkey = e23->suppkey;
          STRING_TYPE s_name = e23->name;
          STRING_TYPE s_address = e23->address;
          long s_nationkey = e23->nationkey;
          STRING_TYPE s_phone = e23->phone;
          DOUBLE_TYPE s_acctbal = e23->acctbal;
          STRING_TYPE s_comment = e23->comment;
          long v23 = e23->__av;
          DELTA_AGG_Supplier.addOrDelOnZero(se26.modify(s_suppkey, s_nationkey), v23);
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Supplier_entry* e24 = DELTA_AGG_Supplier.head;
        while (e24) {
          long l_suppkey = e24->S_SUPPKEY;
          long n_nationkey = e24->N_NATIONKEY;
          long v24 = e24->__av;
          { //slice
            const SecondaryIdxNode<AGG_PartSupp_entry>* n11_head = static_cast<const SecondaryIdxNode<AGG_PartSupp_entry>*>(AGG_PartSupp.slice(se30.modify1(l_suppkey), 0));
            const SecondaryIdxNode<AGG_PartSupp_entry>* n11 = n11_head;
            AGG_PartSupp_entry* e25;
            while (n11) {
              e25 = n11->obj;
              long l_partkey = e25->PS_PARTKEY;
              const PAYLOAD_entry& v25 = e25->__av;
              { //slice
                const SecondaryIdxNode<AGG_Lineitem_entry>* n10_head = static_cast<const SecondaryIdxNode<AGG_Lineitem_entry>*>(AGG_Lineitem.slice(se29.modify12(l_partkey, l_suppkey), 0));
                const SecondaryIdxNode<AGG_Lineitem_entry>* n10 = n10_head;
                AGG_Lineitem_entry* e26;
                while (n10) {
                  e26 = n10->obj;
                  long l_orderkey = e26->L_ORDERKEY;
                  const PAYLOAD_entry& v26 = e26->__av;

                  long v27 = AGG_Part.getValueOrDefault(se28.modify(l_partkey));

                  DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se27.modify(l_orderkey, n_nationkey), (v25.value1 * v26.value1 - v25.value2 * v26.value2) * v24 * v27);

                  n10 = (n10 != n10_head ? n10->nxt : n10->child);
                }
              }
              n11 = (n11 != n11_head ? n11->nxt : n11->child);
            }
          }
          e24 = e24->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e27 = DELTA_AGG_LineitemSupplierPart.head;
        while (e27) {
          long l_orderkey = e27->L_ORDERKEY;
          long n_nationkey = e27->N_NATIONKEY;
          DOUBLE_TYPE v27 = e27->__av;
          { //slice
            const SecondaryIdxNode<AGG_Orders_entry>* n13_head = static_cast<const SecondaryIdxNode<AGG_Orders_entry>*>(AGG_Orders.slice(se33.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_Orders_entry>* n13 = n13_head;
            AGG_Orders_entry* e28;
            while (n13) {
              e28 = n13->obj;
              long year = e28->YEAR;
              long v28 = e28->__av;
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n12_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se32.modify0(n_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n12 = n12_head;
                V_NATION_entry* e29;
                while (n12) {
                  e29 = n12->obj;
                  STRING_TYPE n_name = e29->N_NAME;
                  long v29 = e29->__av;
                  QUERY9.addOrDelOnZero(se31.modify(n_name, year), (v27 * (v28 * v29)));
                  n12 = (n12 != n12_head ? n12->nxt : n12->child);
                }
              }
              n13 = (n13 != n13_head ? n13->nxt : n13->child);
            }
          }
          e27 = e27->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e30 = DELTA_AGG_LineitemSupplierPart.head;
        while (e30) {
          long l_orderkey = e30->L_ORDERKEY;
          long n_nationkey = e30->N_NATIONKEY;
          DOUBLE_TYPE v30 = e30->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se34.modify(l_orderkey, n_nationkey), v30);
          e30 = e30->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Supplier_entry* e31 = DELTA_AGG_Supplier.head;
        while (e31) {
          long s_suppkey = e31->S_SUPPKEY;
          long s_nationkey = e31->N_NATIONKEY;
          long v31 = e31->__av;
          AGG_Supplier.addOrDelOnZero(se35.modify(s_suppkey, s_nationkey), v31);
          e31 = e31->nxt;
        }
      }
    }
    
    
    void on_batch_update_PART(const std::vector<DELTA_PART_entry>::iterator &begin, const std::vector<DELTA_PART_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Part.clear();
      { //foreach
        for (std::vector<DELTA_PART_entry>::iterator e32 = begin; e32 != end; e32++) {
          long p_partkey = e32->partkey;
          STRING_TYPE p_name = e32->name;
          STRING_TYPE p_mfgr = e32->mfgr;
          STRING_TYPE p_brand = e32->brand;
          STRING_TYPE p_type = e32->type;
          long p_size = e32->size;
          STRING_TYPE p_container = e32->container;
          DOUBLE_TYPE p_retailprice = e32->retailprice;
          STRING_TYPE p_comment = e32->comment;
          long v32 = e32->__av;
          (/*if */(0L != Upreg_match(preg1,p_name)) ? DELTA_AGG_Part.addOrDelOnZero(se36.modify(p_partkey), v32) : (void)0);
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Part_entry* e33 = DELTA_AGG_Part.head;
        while (e33) {
          long l_partkey = e33->P_PARTKEY;
          long v33 = e33->__av;
          { //slice
            const SecondaryIdxNode<AGG_Lineitem_entry>* n15_head = static_cast<const SecondaryIdxNode<AGG_Lineitem_entry>*>(AGG_Lineitem.slice(se40.modify1(l_partkey), 1));
            const SecondaryIdxNode<AGG_Lineitem_entry>* n15 = n15_head;
            AGG_Lineitem_entry* e34;
            while (n15) {
              e34 = n15->obj;
              long l_orderkey = e34->L_ORDERKEY;
              long l_suppkey = e34->L_SUPPKEY;
              const PAYLOAD_entry& v34 = e34->__av;
              { //slice
                const SecondaryIdxNode<AGG_Supplier_entry>* n14_head = static_cast<const SecondaryIdxNode<AGG_Supplier_entry>*>(AGG_Supplier.slice(se39.modify0(l_suppkey), 0));
                const SecondaryIdxNode<AGG_Supplier_entry>* n14 = n14_head;
                AGG_Supplier_entry* e35;
                while (n14) {
                  e35 = n14->obj;
                  long n_nationkey = e35->N_NATIONKEY;
                  long v35 = e35->__av;

                  const PAYLOAD_entry& v36 = AGG_PartSupp.getValueOrDefault(se38.modify(l_partkey, l_suppkey));

                  DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se37.modify(l_orderkey, n_nationkey), (v34.value1 * v36.value1 - v34.value2 * v36.value2) * v33 * v35);
                  n14 = (n14 != n14_head ? n14->nxt : n14->child);
                }
              }
              n15 = (n15 != n15_head ? n15->nxt : n15->child);
            }
          }
          e33 = e33->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e36 = DELTA_AGG_LineitemSupplierPart.head;
        while (e36) {
          long l_orderkey = e36->L_ORDERKEY;
          long n_nationkey = e36->N_NATIONKEY;
          DOUBLE_TYPE v36 = e36->__av;
          { //slice
            const SecondaryIdxNode<AGG_Orders_entry>* n17_head = static_cast<const SecondaryIdxNode<AGG_Orders_entry>*>(AGG_Orders.slice(se43.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_Orders_entry>* n17 = n17_head;
            AGG_Orders_entry* e37;
            while (n17) {
              e37 = n17->obj;
              long year = e37->YEAR;
              long v37 = e37->__av;
              { //slice
                const SecondaryIdxNode<V_NATION_entry>* n16_head = static_cast<const SecondaryIdxNode<V_NATION_entry>*>(V_NATION.slice(se42.modify0(n_nationkey), 0));
                const SecondaryIdxNode<V_NATION_entry>* n16 = n16_head;
                V_NATION_entry* e38;
                while (n16) {
                  e38 = n16->obj;
                  STRING_TYPE n_name = e38->N_NAME;
                  long v38 = e38->__av;
                  QUERY9.addOrDelOnZero(se41.modify(n_name, year), (v36 * (v37 * v38)));
                  n16 = (n16 != n16_head ? n16->nxt : n16->child);
                }
              }
              n17 = (n17 != n17_head ? n17->nxt : n17->child);
            }
          }
          e36 = e36->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e39 = DELTA_AGG_LineitemSupplierPart.head;
        while (e39) {
          long l_orderkey = e39->L_ORDERKEY;
          long n_nationkey = e39->N_NATIONKEY;
          DOUBLE_TYPE v39 = e39->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se44.modify(l_orderkey, n_nationkey), v39);
          e39 = e39->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Part_entry* e40 = DELTA_AGG_Part.head;
        while (e40) {
          long p_partkey = e40->P_PARTKEY;
          long v40 = e40->__av;
          AGG_Part.addOrDelOnZero(se45.modify(p_partkey), v40);
          e40 = e40->nxt;
        }
      }
    }
    
    
    void on_system_ready_event() {
      V_NATION.clear();
      { //foreach
        NATION_entry* e41 = NATION.head;
        while (e41) {
          long n_nationkey = e41->nationkey;
          STRING_TYPE n_name = e41->name;
          long n_regionkey = e41->regionkey;
          STRING_TYPE n_comment = e41->comment;
          long v41 = e41->__av;
          V_NATION.addOrDelOnZero(se46.modify(n_nationkey, n_name), v41);
          e41 = e41->nxt;
        }
      }
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      DELTA_AGG_Lineitem_entry se1;
      DELTA_AGG_LineitemSupplierPart_entry se2;
      AGG_Part_entry se3;
      AGG_PartSupp_entry se4;
      AGG_Supplier_entry se5;
      QUERY9_entry se6;
      V_NATION_entry se7;
      AGG_Orders_entry se8;
      AGG_LineitemSupplierPart_entry se9;
      AGG_Lineitem_entry se10;
      DELTA_AGG_Orders_entry se11;
      QUERY9_entry se12;
      V_NATION_entry se13;
      AGG_LineitemSupplierPart_entry se14;
      AGG_Orders_entry se15;
      DELTA_AGG_PartSupp_entry se16;
      DELTA_AGG_LineitemSupplierPart_entry se17;
      AGG_Part_entry se18;
      AGG_Supplier_entry se19;
      AGG_Lineitem_entry se20;
      QUERY9_entry se21;
      V_NATION_entry se22;
      AGG_Orders_entry se23;
      AGG_LineitemSupplierPart_entry se24;
      AGG_PartSupp_entry se25;
      DELTA_AGG_Supplier_entry se26;
      DELTA_AGG_LineitemSupplierPart_entry se27;
      AGG_Part_entry se28;
      AGG_Lineitem_entry se29;
      AGG_PartSupp_entry se30;
      QUERY9_entry se31;
      V_NATION_entry se32;
      AGG_Orders_entry se33;
      AGG_LineitemSupplierPart_entry se34;
      AGG_Supplier_entry se35;
      DELTA_AGG_Part_entry se36;
      DELTA_AGG_LineitemSupplierPart_entry se37;
      AGG_PartSupp_entry se38;
      AGG_Supplier_entry se39;
      AGG_Lineitem_entry se40;
      QUERY9_entry se41;
      V_NATION_entry se42;
      AGG_Orders_entry se43;
      AGG_LineitemSupplierPart_entry se44;
      AGG_Part_entry se45;
      V_NATION_entry se46;

      PAYLOAD_entry entry_PAYLOAD;
    
      /* Regular expression objects */
      regex_t preg1;
    
      /* Data structures used for storing materialized views */
      V_NATION_map V_NATION;
      AGG_Lineitem_map AGG_Lineitem;
      AGG_Part_map AGG_Part;
      AGG_Supplier_map AGG_Supplier;
      AGG_PartSupp_map AGG_PartSupp;
      AGG_LineitemSupplierPart_map AGG_LineitemSupplierPart;
      AGG_Orders_map AGG_Orders;
      DELTA_AGG_Lineitem_map DELTA_AGG_Lineitem;
      DELTA_AGG_Part_map DELTA_AGG_Part;
      DELTA_AGG_Supplier_map DELTA_AGG_Supplier;
      DELTA_AGG_PartSupp_map DELTA_AGG_PartSupp;
      DELTA_AGG_LineitemSupplierPart_map DELTA_AGG_LineitemSupplierPart;
      DELTA_AGG_Orders_map DELTA_AGG_Orders;
      NATION_map NATION;
      DELTA_LINEITEM_map DELTA_LINEITEM;
      DELTA_ORDERS_map DELTA_ORDERS;
      DELTA_PART_map DELTA_PART;
      DELTA_SUPPLIER_map DELTA_SUPPLIER;
      DELTA_PARTSUPP_map DELTA_PARTSUPP;
    
  };

  

}