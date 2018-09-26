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
#define RELATION_CUSTOMER_DYNAMIC
#define RELATION_SUPPLIER_DYNAMIC
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
  
  struct CUSTOMER_entry {
    long custkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE mktsegment; STRING_TYPE comment; long __av; CUSTOMER_entry* nxt; CUSTOMER_entry* prv;
  
    explicit CUSTOMER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit CUSTOMER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const STRING_TYPE& c7, const long c8) { custkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; mktsegment = c6; comment = c7; __av = c8;  }
    CUSTOMER_entry(const CUSTOMER_entry& other) : custkey(other.custkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), mktsegment(other.mktsegment), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    CUSTOMER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 8) return; */
        custkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); mktsegment = f[6]; comment = f[7]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
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
  
  
  struct AGG_Part_entry {
    long P_PARTKEY; long __av; AGG_Part_entry* nxt; AGG_Part_entry* prv;
  
    explicit AGG_Part_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Part_entry(const long c0, const long c1) { P_PARTKEY = c0; __av = c1;  }
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
  
  typedef MultiHashMap<AGG_Part_entry, long, 
    PrimaryHashIndex<AGG_Part_entry, AGG_Part_mapkey0_idxfn>
  > AGG_Part_map;
  
  
  struct AGG_Lineitem_entry {
    long L_ORDERKEY; long L_PARTKEY; long L_SUPPKEY; DOUBLE_TYPE __av; AGG_Lineitem_entry* nxt; AGG_Lineitem_entry* prv;
  
    explicit AGG_Lineitem_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Lineitem_entry(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2; __av = c3;  }
    AGG_Lineitem_entry(const AGG_Lineitem_entry& other) : L_ORDERKEY(other.L_ORDERKEY), L_PARTKEY(other.L_PARTKEY), L_SUPPKEY(other.L_SUPPKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE AGG_Lineitem_entry& modify(const long c0, const long c1, const long c2) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2;  return *this; }
    FORCE_INLINE AGG_Lineitem_entry& modify2(const long c2) { L_SUPPKEY = c2;  return *this; }
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
  
  struct AGG_Lineitem_mapkey2_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Lineitem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_SUPPKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Lineitem_entry& x, const AGG_Lineitem_entry& y) {
      return x.L_SUPPKEY == y.L_SUPPKEY;
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
  
  typedef MultiHashMap<AGG_Lineitem_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey012_idxfn>,
    SecondaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey2_idxfn>,
    SecondaryHashIndex<AGG_Lineitem_entry, AGG_Lineitem_mapkey1_idxfn>
  > AGG_Lineitem_map;
  
  
  struct AGG_Supplier_entry {
    long S_SUPPKEY; STRING_TYPE N_NAME; long __av; AGG_Supplier_entry* nxt; AGG_Supplier_entry* prv;
  
    explicit AGG_Supplier_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Supplier_entry(const long c0, const STRING_TYPE& c1, const long c2) { S_SUPPKEY = c0; N_NAME = c1; __av = c2;  }
    AGG_Supplier_entry(const AGG_Supplier_entry& other) : S_SUPPKEY(other.S_SUPPKEY), N_NAME(other.N_NAME), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Supplier_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        S_SUPPKEY = std::stol(f[0]); N_NAME = f[1]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Supplier_entry& modify(const long c0, const STRING_TYPE& c1) { S_SUPPKEY = c0; N_NAME = c1;  return *this; }
    FORCE_INLINE AGG_Supplier_entry& modify0(const long c0) { S_SUPPKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, S_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NAME);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Supplier_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Supplier_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_SUPPKEY);
      hash_combine(h, e.N_NAME);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Supplier_entry& x, const AGG_Supplier_entry& y) {
      return x.S_SUPPKEY == y.S_SUPPKEY && x.N_NAME == y.N_NAME;
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
  
  
  struct AGG_LineitemSupplierPart_entry {
    long L_ORDERKEY; PAYLOAD_entry __av; AGG_LineitemSupplierPart_entry* nxt; AGG_LineitemSupplierPart_entry* prv;
  
    explicit AGG_LineitemSupplierPart_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_LineitemSupplierPart_entry(const long c0, const PAYLOAD_entry& c1) { L_ORDERKEY = c0; __av = c1;  }
    AGG_LineitemSupplierPart_entry(const AGG_LineitemSupplierPart_entry& other) : L_ORDERKEY(other.L_ORDERKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE AGG_LineitemSupplierPart_entry& modify(const long c0) { L_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
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
  
  typedef MultiHashMap<AGG_LineitemSupplierPart_entry, PAYLOAD_entry, 
    PrimaryHashIndex<AGG_LineitemSupplierPart_entry, AGG_LineitemSupplierPart_mapkey0_idxfn>
  > AGG_LineitemSupplierPart_map;
  
  
  struct AGG_Orders_entry {
    long O_ORDERKEY; long O_CUSTKEY; long YEAR; long __av; AGG_Orders_entry* nxt; AGG_Orders_entry* prv;
  
    explicit AGG_Orders_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Orders_entry(const long c0, const long c1, const long c2, const long c3) { O_ORDERKEY = c0; O_CUSTKEY = c1; YEAR = c2; __av = c3;  }
    AGG_Orders_entry(const AGG_Orders_entry& other) : O_ORDERKEY(other.O_ORDERKEY), O_CUSTKEY(other.O_CUSTKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Orders_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 3) return; */
        O_ORDERKEY = std::stol(f[0]); O_CUSTKEY = std::stol(f[1]); YEAR = std::stol(f[2]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Orders_entry& modify(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; O_CUSTKEY = c1; YEAR = c2;  return *this; }
    FORCE_INLINE AGG_Orders_entry& modify1(const long c1) { O_CUSTKEY = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_CUSTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Orders_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.O_CUSTKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Orders_entry& x, const AGG_Orders_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.O_CUSTKEY == y.O_CUSTKEY && x.YEAR == y.YEAR;
    }
  };
  
  struct AGG_Orders_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_CUSTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Orders_entry& x, const AGG_Orders_entry& y) {
      return x.O_CUSTKEY == y.O_CUSTKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Orders_entry, long, 
    PrimaryHashIndex<AGG_Orders_entry, AGG_Orders_mapkey012_idxfn>,
    SecondaryHashIndex<AGG_Orders_entry, AGG_Orders_mapkey1_idxfn>
  > AGG_Orders_map;
  
  
  struct AGG_Customer_entry {
    long C_CUSTKEY; long __av; AGG_Customer_entry* nxt; AGG_Customer_entry* prv;
  
    explicit AGG_Customer_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_Customer_entry(const long c0, const long c1) { C_CUSTKEY = c0; __av = c1;  }
    AGG_Customer_entry(const AGG_Customer_entry& other) : C_CUSTKEY(other.C_CUSTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_Customer_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 1) return; */
        C_CUSTKEY = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_Customer_entry& modify(const long c0) { C_CUSTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_CUSTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct AGG_Customer_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Customer_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C_CUSTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_Customer_entry& x, const AGG_Customer_entry& y) {
      return x.C_CUSTKEY == y.C_CUSTKEY;
    }
  };
  
  typedef MultiHashMap<AGG_Customer_entry, long, 
    PrimaryHashIndex<AGG_Customer_entry, AGG_Customer_mapkey0_idxfn>
  > AGG_Customer_map;
  
  
  struct AGG_OrdersCustomer_entry {
    long O_ORDERKEY; long YEAR; long __av; AGG_OrdersCustomer_entry* nxt; AGG_OrdersCustomer_entry* prv;
  
    explicit AGG_OrdersCustomer_entry() : nxt(nullptr), prv(nullptr) { }
    explicit AGG_OrdersCustomer_entry(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; YEAR = c1; __av = c2;  }
    AGG_OrdersCustomer_entry(const AGG_OrdersCustomer_entry& other) : O_ORDERKEY(other.O_ORDERKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    AGG_OrdersCustomer_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        O_ORDERKEY = std::stol(f[0]); YEAR = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE AGG_OrdersCustomer_entry& modify(const long c0, const long c1) { O_ORDERKEY = c0; YEAR = c1;  return *this; }
    FORCE_INLINE AGG_OrdersCustomer_entry& modify0(const long c0) { O_ORDERKEY = c0;  return *this; }
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
  
  struct AGG_OrdersCustomer_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const AGG_OrdersCustomer_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_OrdersCustomer_entry& x, const AGG_OrdersCustomer_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.YEAR == y.YEAR;
    }
  };
  
  struct AGG_OrdersCustomer_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const AGG_OrdersCustomer_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const AGG_OrdersCustomer_entry& x, const AGG_OrdersCustomer_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<AGG_OrdersCustomer_entry, long, 
    PrimaryHashIndex<AGG_OrdersCustomer_entry, AGG_OrdersCustomer_mapkey01_idxfn>,
    SecondaryHashIndex<AGG_OrdersCustomer_entry, AGG_OrdersCustomer_mapkey0_idxfn>
  > AGG_OrdersCustomer_map;
  
  
  struct DELTA_AGG_Part_entry {
    long P_PARTKEY; long __av; DELTA_AGG_Part_entry* nxt; DELTA_AGG_Part_entry* prv;
  
    explicit DELTA_AGG_Part_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Part_entry(const long c0, const long c1) { P_PARTKEY = c0; __av = c1;  }
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
  
  typedef MultiHashMap<DELTA_AGG_Part_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Part_entry, DELTA_AGG_Part_mapkey0_idxfn>
  > DELTA_AGG_Part_map;
  
  
  struct DELTA_AGG_Lineitem_entry {
    long L_ORDERKEY; long L_PARTKEY; long L_SUPPKEY; DOUBLE_TYPE __av; DELTA_AGG_Lineitem_entry* nxt; DELTA_AGG_Lineitem_entry* prv;
  
    explicit DELTA_AGG_Lineitem_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Lineitem_entry(const long c0, const long c1, const long c2, const DOUBLE_TYPE c3) { L_ORDERKEY = c0; L_PARTKEY = c1; L_SUPPKEY = c2; __av = c3;  }
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
  
  typedef MultiHashMap<DELTA_AGG_Lineitem_entry, DOUBLE_TYPE, 
    PrimaryHashIndex<DELTA_AGG_Lineitem_entry, DELTA_AGG_Lineitem_mapkey012_idxfn>
  > DELTA_AGG_Lineitem_map;
  
  
  struct DELTA_AGG_Supplier_entry {
    long S_SUPPKEY; STRING_TYPE N_NAME; long __av; DELTA_AGG_Supplier_entry* nxt; DELTA_AGG_Supplier_entry* prv;
  
    explicit DELTA_AGG_Supplier_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Supplier_entry(const long c0, const STRING_TYPE& c1, const long c2) { S_SUPPKEY = c0; N_NAME = c1; __av = c2;  }
    DELTA_AGG_Supplier_entry(const DELTA_AGG_Supplier_entry& other) : S_SUPPKEY(other.S_SUPPKEY), N_NAME(other.N_NAME), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Supplier_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        S_SUPPKEY = std::stol(f[0]); N_NAME = f[1]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Supplier_entry& modify(const long c0, const STRING_TYPE& c1) { S_SUPPKEY = c0; N_NAME = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, S_SUPPKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, N_NAME);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Supplier_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Supplier_entry& e) {
      size_t h = 0;
      hash_combine(h, e.S_SUPPKEY);
      hash_combine(h, e.N_NAME);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Supplier_entry& x, const DELTA_AGG_Supplier_entry& y) {
      return x.S_SUPPKEY == y.S_SUPPKEY && x.N_NAME == y.N_NAME;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Supplier_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Supplier_entry, DELTA_AGG_Supplier_mapkey01_idxfn>
  > DELTA_AGG_Supplier_map;
  
  
  struct DELTA_AGG_LineitemSupplierPart_entry {
    long L_ORDERKEY; PAYLOAD_entry __av; DELTA_AGG_LineitemSupplierPart_entry* nxt; DELTA_AGG_LineitemSupplierPart_entry* prv;
  
    explicit DELTA_AGG_LineitemSupplierPart_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_LineitemSupplierPart_entry(const long c0, const PAYLOAD_entry& c1) { L_ORDERKEY = c0; __av = c1;  }
    DELTA_AGG_LineitemSupplierPart_entry(const DELTA_AGG_LineitemSupplierPart_entry& other) : L_ORDERKEY(other.L_ORDERKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE DELTA_AGG_LineitemSupplierPart_entry& modify(const long c0) { L_ORDERKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, L_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_LineitemSupplierPart_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_LineitemSupplierPart_entry& e) {
      size_t h = 0;
      hash_combine(h, e.L_ORDERKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_LineitemSupplierPart_entry& x, const DELTA_AGG_LineitemSupplierPart_entry& y) {
      return x.L_ORDERKEY == y.L_ORDERKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_LineitemSupplierPart_entry, PAYLOAD_entry, 
    PrimaryHashIndex<DELTA_AGG_LineitemSupplierPart_entry, DELTA_AGG_LineitemSupplierPart_mapkey0_idxfn>
  > DELTA_AGG_LineitemSupplierPart_map;
  
  
  struct DELTA_AGG_Orders_entry {
    long O_ORDERKEY; long O_CUSTKEY; long YEAR; long __av; DELTA_AGG_Orders_entry* nxt; DELTA_AGG_Orders_entry* prv;
  
    explicit DELTA_AGG_Orders_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Orders_entry(const long c0, const long c1, const long c2, const long c3) { O_ORDERKEY = c0; O_CUSTKEY = c1; YEAR = c2; __av = c3;  }
    DELTA_AGG_Orders_entry(const DELTA_AGG_Orders_entry& other) : O_ORDERKEY(other.O_ORDERKEY), O_CUSTKEY(other.O_CUSTKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Orders_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 3) return; */
        O_ORDERKEY = std::stol(f[0]); O_CUSTKEY = std::stol(f[1]); YEAR = std::stol(f[2]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Orders_entry& modify(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; O_CUSTKEY = c1; YEAR = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_ORDERKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, O_CUSTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, YEAR);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Orders_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Orders_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.O_CUSTKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Orders_entry& x, const DELTA_AGG_Orders_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.O_CUSTKEY == y.O_CUSTKEY && x.YEAR == y.YEAR;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Orders_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Orders_entry, DELTA_AGG_Orders_mapkey012_idxfn>
  > DELTA_AGG_Orders_map;
  
  
  struct DELTA_AGG_Customer_entry {
    long C_CUSTKEY; long __av; DELTA_AGG_Customer_entry* nxt; DELTA_AGG_Customer_entry* prv;
  
    explicit DELTA_AGG_Customer_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_Customer_entry(const long c0, const long c1) { C_CUSTKEY = c0; __av = c1;  }
    DELTA_AGG_Customer_entry(const DELTA_AGG_Customer_entry& other) : C_CUSTKEY(other.C_CUSTKEY), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_Customer_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 1) return; */
        C_CUSTKEY = std::stol(f[0]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_Customer_entry& modify(const long c0) { C_CUSTKEY = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, C_CUSTKEY);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct DELTA_AGG_Customer_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Customer_entry& e) {
      size_t h = 0;
      hash_combine(h, e.C_CUSTKEY);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_Customer_entry& x, const DELTA_AGG_Customer_entry& y) {
      return x.C_CUSTKEY == y.C_CUSTKEY;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_Customer_entry, long, 
    PrimaryHashIndex<DELTA_AGG_Customer_entry, DELTA_AGG_Customer_mapkey0_idxfn>
  > DELTA_AGG_Customer_map;
  
  
  struct DELTA_AGG_OrdersCustomer_entry {
    long O_ORDERKEY; long YEAR; long __av; DELTA_AGG_OrdersCustomer_entry* nxt; DELTA_AGG_OrdersCustomer_entry* prv;
  
    explicit DELTA_AGG_OrdersCustomer_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_AGG_OrdersCustomer_entry(const long c0, const long c1, const long c2) { O_ORDERKEY = c0; YEAR = c1; __av = c2;  }
    DELTA_AGG_OrdersCustomer_entry(const DELTA_AGG_OrdersCustomer_entry& other) : O_ORDERKEY(other.O_ORDERKEY), YEAR(other.YEAR), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_AGG_OrdersCustomer_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 2) return; */
        O_ORDERKEY = std::stol(f[0]); YEAR = std::stol(f[1]); __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_AGG_OrdersCustomer_entry& modify(const long c0, const long c1) { O_ORDERKEY = c0; YEAR = c1;  return *this; }
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
  
  struct DELTA_AGG_OrdersCustomer_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_OrdersCustomer_entry& e) {
      size_t h = 0;
      hash_combine(h, e.O_ORDERKEY);
      hash_combine(h, e.YEAR);
      return h;
    }
    
    FORCE_INLINE static bool equals(const DELTA_AGG_OrdersCustomer_entry& x, const DELTA_AGG_OrdersCustomer_entry& y) {
      return x.O_ORDERKEY == y.O_ORDERKEY && x.YEAR == y.YEAR;
    }
  };
  
  typedef MultiHashMap<DELTA_AGG_OrdersCustomer_entry, long, 
    PrimaryHashIndex<DELTA_AGG_OrdersCustomer_entry, DELTA_AGG_OrdersCustomer_mapkey01_idxfn>
  > DELTA_AGG_OrdersCustomer_map;
  
  
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
  
  
  struct DELTA_CUSTOMER_entry {
    long custkey; STRING_TYPE name; STRING_TYPE address; long nationkey; STRING_TYPE phone; DOUBLE_TYPE acctbal; STRING_TYPE mktsegment; STRING_TYPE comment; long __av; DELTA_CUSTOMER_entry* nxt; DELTA_CUSTOMER_entry* prv;
  
    explicit DELTA_CUSTOMER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit DELTA_CUSTOMER_entry(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const STRING_TYPE& c7, const long c8) { custkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; mktsegment = c6; comment = c7; __av = c8;  }
    DELTA_CUSTOMER_entry(const DELTA_CUSTOMER_entry& other) : custkey(other.custkey), name(other.name), address(other.address), nationkey(other.nationkey), phone(other.phone), acctbal(other.acctbal), mktsegment(other.mktsegment), comment(other.comment), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    DELTA_CUSTOMER_entry(const std::vector<std::string>& f, const long v) {
        /* if (f.size() < 8) return; */
        custkey = std::stol(f[0]); name = f[1]; address = f[2]; nationkey = std::stol(f[3]); phone = f[4]; acctbal = std::stod(f[5]); mktsegment = f[6]; comment = f[7]; __av = v; nxt = nullptr; prv = nullptr;
    }
    
    FORCE_INLINE DELTA_CUSTOMER_entry& modify(const long c0, const STRING_TYPE& c1, const STRING_TYPE& c2, const long c3, const STRING_TYPE& c4, const DOUBLE_TYPE c5, const STRING_TYPE& c6, const STRING_TYPE& c7) { custkey = c0; name = c1; address = c2; nationkey = c3; phone = c4; acctbal = c5; mktsegment = c6; comment = c7;  return *this; }
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
  
  struct DELTA_CUSTOMER_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_CUSTOMER_entry& e) {
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
    
    FORCE_INLINE static bool equals(const DELTA_CUSTOMER_entry& x, const DELTA_CUSTOMER_entry& y) {
      return x.custkey == y.custkey && x.name == y.name && x.address == y.address && x.nationkey == y.nationkey && x.phone == y.phone && x.acctbal == y.acctbal && x.mktsegment == y.mktsegment && x.comment == y.comment;
    }
  };
  
  typedef MultiHashMap<DELTA_CUSTOMER_entry, long, 
    PrimaryHashIndex<DELTA_CUSTOMER_entry, DELTA_CUSTOMER_mapkey01234567_idxfn>
  > DELTA_CUSTOMER_map;
  
  
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
      c1 = Udate(STRING_TYPE("1996-12-31"));
      c3 = STRING_TYPE("ECONOMY ANODIZED STEEL");
      c4 = STRING_TYPE("AMERICA");
      c2 = Udate(STRING_TYPE("1995-1-1"));
      c5 = STRING_TYPE("BRAZIL");      
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
    
    void on_insert_REGION(const long regionkey, const STRING_TYPE name, const STRING_TYPE comment) {
      REGION_entry e(regionkey, name, comment, 1L);
      REGION.addOrDelOnZero(e, 1L);
    }
    void on_insert_REGION(REGION_entry &e) {
      e.__av = 1L;
      REGION.addOrDelOnZero(e, 1L);
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
          DELTA_AGG_Lineitem.addOrDelOnZero(se1.modify(l_orderkey, l_partkey, l_suppkey), (v1 * (l_extendedprice * (1L + (-1L * l_discount)))));
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Lineitem_entry* e2 = DELTA_AGG_Lineitem.head;
        while (e2) {
          long l_orderkey = e2->L_ORDERKEY;
          long l_partkey = e2->L_PARTKEY;
          long l_suppkey = e2->L_SUPPKEY;
          DOUBLE_TYPE v2 = e2->__av;
          { //slice
            const SecondaryIdxNode<AGG_Supplier_entry>* n1_head = static_cast<const SecondaryIdxNode<AGG_Supplier_entry>*>(AGG_Supplier.slice(se4.modify0(l_suppkey), 0));
            const SecondaryIdxNode<AGG_Supplier_entry>* n1 = n1_head;
            AGG_Supplier_entry* e3;
            while (n1) {
              e3 = n1->obj;
              STRING_TYPE n_name = e3->N_NAME;
              long v3 = e3->__av;

              DOUBLE_TYPE tmp = v2 * v3 * AGG_Part.getValueOrDefault(se3.modify(l_partkey));
              entry_PAYLOAD.partial_mkt_share = (c5 == n_name) ? tmp : 0;
              entry_PAYLOAD.total_mkt_share = tmp;
              DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se2.modify(l_orderkey), entry_PAYLOAD);

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
          const PAYLOAD_entry& v4 = e4->__av;
          { //slice
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n2_head = static_cast<const SecondaryIdxNode<AGG_OrdersCustomer_entry>*>(AGG_OrdersCustomer.slice(se6.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n2 = n2_head;
            AGG_OrdersCustomer_entry* e5;
            while (n2) {
              e5 = n2->obj;
              long year = e5->YEAR;
              long v5 = e5->__av;

              entry_PAYLOAD.partial_mkt_share = v5 * v4.partial_mkt_share;
              entry_PAYLOAD.total_mkt_share = v5 * v4.total_mkt_share;
              QUERY8.addOrDelOnZero(se5.modify(year), entry_PAYLOAD);
              n2 = (n2 != n2_head ? n2->nxt : n2->child);
            }
          }
          e4 = e4->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e6 = DELTA_AGG_LineitemSupplierPart.head;
        while (e6) {
          long l_orderkey = e6->L_ORDERKEY;
          const PAYLOAD_entry& v6 = e6->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se7.modify(l_orderkey), v6);
          e6 = e6->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Lineitem_entry* e7 = DELTA_AGG_Lineitem.head;
        while (e7) {
          long l_orderkey = e7->L_ORDERKEY;
          long l_partkey = e7->L_PARTKEY;
          long l_suppkey = e7->L_SUPPKEY;
          DOUBLE_TYPE v7 = e7->__av;
          AGG_Lineitem.addOrDelOnZero(se8.modify(l_orderkey, l_partkey, l_suppkey), v7);
          e7 = e7->nxt;
        }
      }
    }
    
    
    void on_batch_update_ORDERS(const std::vector<DELTA_ORDERS_entry>::iterator &begin, const std::vector<DELTA_ORDERS_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Orders.clear();
      { //foreach
        for (std::vector<DELTA_ORDERS_entry>::iterator e8 = begin; e8 != end; e8++) {
          long o_orderkey = e8->orderkey;
          long o_custkey = e8->custkey;
          STRING_TYPE o_orderstatus = e8->orderstatus;
          DOUBLE_TYPE o_totalprice = e8->totalprice;
          date o_orderdate = e8->orderdate;
          STRING_TYPE o_orderpriority = e8->orderpriority;
          STRING_TYPE o_clerk = e8->clerk;
          long o_shippriority = e8->shippriority;
          STRING_TYPE o_comment = e8->comment;
          long v8 = e8->__av;
          if (c1 >= o_orderdate) {
            if (o_orderdate >= c2) {
              long l1 = Udate_year(o_orderdate);
              (/*if */(c1 >= o_orderdate) ? DELTA_AGG_Orders.addOrDelOnZero(se9.modify(o_orderkey, o_custkey, l1), v8) : (void)0);
            }
          }
        }
      }
      
      DELTA_AGG_OrdersCustomer.clear();
      { //foreach
        DELTA_AGG_Orders_entry* e9 = DELTA_AGG_Orders.head;
        while (e9) {
          long o_orderkey = e9->O_ORDERKEY;
          long o_custkey = e9->O_CUSTKEY;
          long year = e9->YEAR;
          long v9 = e9->__av;
          DELTA_AGG_OrdersCustomer.addOrDelOnZero(se10.modify(o_orderkey, year), (v9 * AGG_Customer.getValueOrDefault(se11.modify(o_custkey))));
          e9 = e9->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_OrdersCustomer_entry* e10 = DELTA_AGG_OrdersCustomer.head;
        while (e10) {
          long o_orderkey = e10->O_ORDERKEY;
          long year = e10->YEAR;
          long v10 = e10->__av;

          const PAYLOAD_entry& v11 = AGG_LineitemSupplierPart.getValueOrDefault(se13.modify(o_orderkey));
          entry_PAYLOAD.partial_mkt_share = v10 * v11.partial_mkt_share;
          entry_PAYLOAD.total_mkt_share = v10 * v11.total_mkt_share;
          QUERY8.addOrDelOnZero(se12.modify(year), entry_PAYLOAD);

          e10 = e10->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_OrdersCustomer_entry* e11 = DELTA_AGG_OrdersCustomer.head;
        while (e11) {
          long o_orderkey = e11->O_ORDERKEY;
          long year = e11->YEAR;
          long v11 = e11->__av;
          AGG_OrdersCustomer.addOrDelOnZero(se14.modify(o_orderkey, year), v11);
          e11 = e11->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Orders_entry* e12 = DELTA_AGG_Orders.head;
        while (e12) {
          long o_orderkey = e12->O_ORDERKEY;
          long o_custkey = e12->O_CUSTKEY;
          long year = e12->YEAR;
          long v12 = e12->__av;
          AGG_Orders.addOrDelOnZero(se15.modify(o_orderkey, o_custkey, year), v12);
          e12 = e12->nxt;
        }
      }
    }
    
    
    void on_batch_update_CUSTOMER(const std::vector<DELTA_CUSTOMER_entry>::iterator &begin, const std::vector<DELTA_CUSTOMER_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Customer.clear();
      { //foreach
        for (std::vector<DELTA_CUSTOMER_entry>::iterator e13 = begin; e13 != end; e13++) {
          long c_custkey = e13->custkey;
          STRING_TYPE c_name = e13->name;
          STRING_TYPE c_address = e13->address;
          long c_nationkey = e13->nationkey;
          STRING_TYPE c_phone = e13->phone;
          DOUBLE_TYPE c_acctbal = e13->acctbal;
          STRING_TYPE c_mktsegment = e13->mktsegment;
          STRING_TYPE c_comment = e13->comment;
          long v13 = e13->__av;
          DELTA_AGG_Customer.addOrDelOnZero(se16.modify(c_custkey), (v13 * V_NATION1.getValueOrDefault(se17.modify(c_nationkey))));
        }
      }
      
      DELTA_AGG_OrdersCustomer.clear();
      { //foreach
        DELTA_AGG_Customer_entry* e14 = DELTA_AGG_Customer.head;
        while (e14) {
          long c_custkey = e14->C_CUSTKEY;
          long v14 = e14->__av;
          { //slice
            const SecondaryIdxNode<AGG_Orders_entry>* n3_head = static_cast<const SecondaryIdxNode<AGG_Orders_entry>*>(AGG_Orders.slice(se19.modify1(c_custkey), 0));
            const SecondaryIdxNode<AGG_Orders_entry>* n3 = n3_head;
            AGG_Orders_entry* e15;
            while (n3) {
              e15 = n3->obj;
              long o_orderkey = e15->O_ORDERKEY;
              long year = e15->YEAR;
              long v15 = e15->__av;
              DELTA_AGG_OrdersCustomer.addOrDelOnZero(se18.modify(o_orderkey, year), (v14 * v15));
              n3 = (n3 != n3_head ? n3->nxt : n3->child);
            }
          }
          e14 = e14->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_OrdersCustomer_entry* e16 = DELTA_AGG_OrdersCustomer.head;
        while (e16) {
          long o_orderkey = e16->O_ORDERKEY;
          long year = e16->YEAR;
          long v16 = e16->__av;

          const PAYLOAD_entry& v17 = AGG_LineitemSupplierPart.getValueOrDefault(se21.modify(o_orderkey));
          entry_PAYLOAD.partial_mkt_share = v16 * v17.partial_mkt_share;
          entry_PAYLOAD.total_mkt_share = v16 * v17.total_mkt_share;
          QUERY8.addOrDelOnZero(se20.modify(year), entry_PAYLOAD);

          e16 = e16->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_OrdersCustomer_entry* e17 = DELTA_AGG_OrdersCustomer.head;
        while (e17) {
          long o_orderkey = e17->O_ORDERKEY;
          long year = e17->YEAR;
          long v17 = e17->__av;
          AGG_OrdersCustomer.addOrDelOnZero(se22.modify(o_orderkey, year), v17);
          e17 = e17->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Customer_entry* e18 = DELTA_AGG_Customer.head;
        while (e18) {
          long c_custkey = e18->C_CUSTKEY;
          long v18 = e18->__av;
          AGG_Customer.addOrDelOnZero(se23.modify(c_custkey), v18);
          e18 = e18->nxt;
        }
      }
    }
    
    
    void on_batch_update_SUPPLIER(const std::vector<DELTA_SUPPLIER_entry>::iterator &begin, const std::vector<DELTA_SUPPLIER_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Supplier.clear();
      { //foreach
        for (std::vector<DELTA_SUPPLIER_entry>::iterator e19 = begin; e19 != end; e19++) {
          long s_suppkey = e19->suppkey;
          STRING_TYPE s_name = e19->name;
          STRING_TYPE s_address = e19->address;
          long s_nationkey = e19->nationkey;
          STRING_TYPE s_phone = e19->phone;
          DOUBLE_TYPE s_acctbal = e19->acctbal;
          STRING_TYPE s_comment = e19->comment;
          long v19 = e19->__av;
          { //slice
            const SecondaryIdxNode<V_NATION2_entry>* n4_head = static_cast<const SecondaryIdxNode<V_NATION2_entry>*>(V_NATION2.slice(se25.modify0(s_nationkey), 0));
            const SecondaryIdxNode<V_NATION2_entry>* n4 = n4_head;
            V_NATION2_entry* e20;
            while (n4) {
              e20 = n4->obj;
              STRING_TYPE n_name = e20->N_NAME;
              long v20 = e20->__av;
              DELTA_AGG_Supplier.addOrDelOnZero(se24.modify(s_suppkey, n_name), (v19 * v20));
              n4 = (n4 != n4_head ? n4->nxt : n4->child);
            }
          }
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Supplier_entry* e21 = DELTA_AGG_Supplier.head;
        while (e21) {
          long l_suppkey = e21->S_SUPPKEY;
          STRING_TYPE n_name = e21->N_NAME;
          long v21 = e21->__av;
          { //slice
            const SecondaryIdxNode<AGG_Lineitem_entry>* n5_head = static_cast<const SecondaryIdxNode<AGG_Lineitem_entry>*>(AGG_Lineitem.slice(se28.modify2(l_suppkey), 0));
            const SecondaryIdxNode<AGG_Lineitem_entry>* n5 = n5_head;
            AGG_Lineitem_entry* e22;
            while (n5) {
              e22 = n5->obj;
              long l_orderkey = e22->L_ORDERKEY;
              long l_partkey = e22->L_PARTKEY;
              DOUBLE_TYPE v22 = e22->__av;

              DOUBLE_TYPE tmp = v21 * v22 * AGG_Part.getValueOrDefault(se27.modify(l_partkey));
              entry_PAYLOAD.partial_mkt_share = (c5 == n_name) ? tmp : 0;
              entry_PAYLOAD.total_mkt_share = tmp;
              DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se26.modify(l_orderkey), entry_PAYLOAD);

              n5 = (n5 != n5_head ? n5->nxt : n5->child);
            }
          }
          e21 = e21->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e23 = DELTA_AGG_LineitemSupplierPart.head;
        while (e23) {
          long l_orderkey = e23->L_ORDERKEY;
          const PAYLOAD_entry& v23 = e23->__av;
          { //slice
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n6_head = static_cast<const SecondaryIdxNode<AGG_OrdersCustomer_entry>*>(AGG_OrdersCustomer.slice(se30.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n6 = n6_head;
            AGG_OrdersCustomer_entry* e24;
            while (n6) {
              e24 = n6->obj;
              long year = e24->YEAR;
              long v24 = e24->__av;

              entry_PAYLOAD.partial_mkt_share = v24 * v23.partial_mkt_share;
              entry_PAYLOAD.total_mkt_share = v24 * v23.total_mkt_share;
              QUERY8.addOrDelOnZero(se29.modify(year), entry_PAYLOAD);

              n6 = (n6 != n6_head ? n6->nxt : n6->child);
            }
          }
          e23 = e23->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e25 = DELTA_AGG_LineitemSupplierPart.head;
        while (e25) {
          long l_orderkey = e25->L_ORDERKEY;
          const PAYLOAD_entry& v25 = e25->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se31.modify(l_orderkey), v25);
          e25 = e25->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Supplier_entry* e26 = DELTA_AGG_Supplier.head;
        while (e26) {
          long s_suppkey = e26->S_SUPPKEY;
          STRING_TYPE n_name = e26->N_NAME;
          long v26 = e26->__av;
          AGG_Supplier.addOrDelOnZero(se32.modify(s_suppkey, n_name), v26);
          e26 = e26->nxt;
        }
      }
    }
    
    
    void on_batch_update_PART(const std::vector<DELTA_PART_entry>::iterator &begin, const std::vector<DELTA_PART_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      DELTA_AGG_Part.clear();
      { //foreach
        for (std::vector<DELTA_PART_entry>::iterator e27 = begin; e27 != end; e27++) {
          long p_partkey = e27->partkey;
          STRING_TYPE p_name = e27->name;
          STRING_TYPE p_mfgr = e27->mfgr;
          STRING_TYPE p_brand = e27->brand;
          STRING_TYPE p_type = e27->type;
          long p_size = e27->size;
          STRING_TYPE p_container = e27->container;
          DOUBLE_TYPE p_retailprice = e27->retailprice;
          STRING_TYPE p_comment = e27->comment;
          long v27 = e27->__av;
          (/*if */(p_type == c3) ? DELTA_AGG_Part.addOrDelOnZero(se33.modify(p_partkey), v27) : (void)0);
        }
      }
      
      DELTA_AGG_LineitemSupplierPart.clear();
      { //foreach
        DELTA_AGG_Part_entry* e28 = DELTA_AGG_Part.head;
        while (e28) {
          long l_partkey = e28->P_PARTKEY;
          long v28 = e28->__av;
          { //slice
            const SecondaryIdxNode<AGG_Lineitem_entry>* n8_head = static_cast<const SecondaryIdxNode<AGG_Lineitem_entry>*>(AGG_Lineitem.slice(se36.modify1(l_partkey), 1));
            const SecondaryIdxNode<AGG_Lineitem_entry>* n8 = n8_head;
            AGG_Lineitem_entry* e29;
            while (n8) {
              e29 = n8->obj;
              long l_orderkey = e29->L_ORDERKEY;
              long l_suppkey = e29->L_SUPPKEY;
              DOUBLE_TYPE v29 = e29->__av;
              { //slice
                const SecondaryIdxNode<AGG_Supplier_entry>* n7_head = static_cast<const SecondaryIdxNode<AGG_Supplier_entry>*>(AGG_Supplier.slice(se35.modify0(l_suppkey), 0));
                const SecondaryIdxNode<AGG_Supplier_entry>* n7 = n7_head;
                AGG_Supplier_entry* e30;
                while (n7) {
                  e30 = n7->obj;
                  STRING_TYPE n_name = e30->N_NAME;
                  long v30 = e30->__av;

                  DOUBLE_TYPE tmp = v28 * v29 * v30;
                  entry_PAYLOAD.partial_mkt_share = (c5 == n_name) ? tmp : 0;
                  entry_PAYLOAD.total_mkt_share = tmp;
                  DELTA_AGG_LineitemSupplierPart.addOrDelOnZero(se34.modify(l_orderkey), entry_PAYLOAD);

                  n7 = (n7 != n7_head ? n7->nxt : n7->child);
                }
              }
              n8 = (n8 != n8_head ? n8->nxt : n8->child);
            }
          }
          e28 = e28->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e31 = DELTA_AGG_LineitemSupplierPart.head;
        while (e31) {
          long l_orderkey = e31->L_ORDERKEY;
          const PAYLOAD_entry& v31 = e31->__av;
          { //slice
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n9_head = static_cast<const SecondaryIdxNode<AGG_OrdersCustomer_entry>*>(AGG_OrdersCustomer.slice(se38.modify0(l_orderkey), 0));
            const SecondaryIdxNode<AGG_OrdersCustomer_entry>* n9 = n9_head;
            AGG_OrdersCustomer_entry* e32;
            while (n9) {
              e32 = n9->obj;
              long year = e32->YEAR;
              long v32 = e32->__av;

              entry_PAYLOAD.partial_mkt_share = v32 * v31.partial_mkt_share;
              entry_PAYLOAD.total_mkt_share = v32 * v31.total_mkt_share;
              QUERY8.addOrDelOnZero(se37.modify(year), entry_PAYLOAD);

              n9 = (n9 != n9_head ? n9->nxt : n9->child);
            }
          }
          e31 = e31->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_LineitemSupplierPart_entry* e33 = DELTA_AGG_LineitemSupplierPart.head;
        while (e33) {
          long l_orderkey = e33->L_ORDERKEY;
          const PAYLOAD_entry& v33 = e33->__av;
          AGG_LineitemSupplierPart.addOrDelOnZero(se39.modify(l_orderkey), v33);
          e33 = e33->nxt;
        }
      }
      
      { //foreach
        DELTA_AGG_Part_entry* e34 = DELTA_AGG_Part.head;
        while (e34) {
          long p_partkey = e34->P_PARTKEY;
          long v34 = e34->__av;
          AGG_Part.addOrDelOnZero(se40.modify(p_partkey), v34);
          e34 = e34->nxt;
        }
      }
    }
    
    
    void on_system_ready_event() {
      V_NATION1.clear();
      { //foreach
        NATION_entry* e35 = NATION.head;
        while (e35) {
          long c_nationkey = e35->nationkey;
          STRING_TYPE n_name = e35->name;
          long n_regionkey = e35->regionkey;
          STRING_TYPE n_comment = e35->comment;
          long v35 = e35->__av;
          STRING_TYPE l2 = c4;
          { //slice
            const SecondaryIdxNode<REGION_entry>* n10_head = static_cast<const SecondaryIdxNode<REGION_entry>*>(REGION.slice(se42.modify01(n_regionkey, l2), 0));
            const SecondaryIdxNode<REGION_entry>* n10 = n10_head;
            REGION_entry* e36;
            while (n10) {
              e36 = n10->obj;
              STRING_TYPE r_comment = e36->comment;
              long v36 = e36->__av;
              V_NATION1.addOrDelOnZero(se41.modify(c_nationkey), (v35 * v36));
              n10 = (n10 != n10_head ? n10->nxt : n10->child);
            }
          }
          e35 = e35->nxt;
        }
      }
      
      V_NATION2.clear();
      { //foreach
        NATION_entry* e37 = NATION.head;
        while (e37) {
          long s_nationkey = e37->nationkey;
          STRING_TYPE n_name = e37->name;
          long n_regionkey = e37->regionkey;
          STRING_TYPE n_comment = e37->comment;
          long v37 = e37->__av;
          V_NATION2.addOrDelOnZero(se43.modify(s_nationkey, n_name), v37);
          e37 = e37->nxt;
        }
      }
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */
      DELTA_AGG_Lineitem_entry se1;
      DELTA_AGG_LineitemSupplierPart_entry se2;
      AGG_Part_entry se3;
      AGG_Supplier_entry se4;
      QUERY8_entry se5;
      AGG_OrdersCustomer_entry se6;
      AGG_LineitemSupplierPart_entry se7;
      AGG_Lineitem_entry se8;
      DELTA_AGG_Orders_entry se9;
      DELTA_AGG_OrdersCustomer_entry se10;
      AGG_Customer_entry se11;
      QUERY8_entry se12;
      AGG_LineitemSupplierPart_entry se13;
      AGG_OrdersCustomer_entry se14;
      AGG_Orders_entry se15;
      DELTA_AGG_Customer_entry se16;
      V_NATION1_entry se17;
      DELTA_AGG_OrdersCustomer_entry se18;
      AGG_Orders_entry se19;
      QUERY8_entry se20;
      AGG_LineitemSupplierPart_entry se21;
      AGG_OrdersCustomer_entry se22;
      AGG_Customer_entry se23;
      DELTA_AGG_Supplier_entry se24;
      V_NATION2_entry se25;
      DELTA_AGG_LineitemSupplierPart_entry se26;
      AGG_Part_entry se27;
      AGG_Lineitem_entry se28;
      QUERY8_entry se29;
      AGG_OrdersCustomer_entry se30;
      AGG_LineitemSupplierPart_entry se31;
      AGG_Supplier_entry se32;
      DELTA_AGG_Part_entry se33;
      DELTA_AGG_LineitemSupplierPart_entry se34;
      AGG_Supplier_entry se35;
      AGG_Lineitem_entry se36;
      QUERY8_entry se37;
      AGG_OrdersCustomer_entry se38;
      AGG_LineitemSupplierPart_entry se39;
      AGG_Part_entry se40;
      V_NATION1_entry se41;
      REGION_entry se42;
      V_NATION2_entry se43;
    
      
    
      /* Data structures used for storing materialized views */
      V_NATION1_map V_NATION1;
      V_NATION2_map V_NATION2;
      AGG_Part_map AGG_Part;
      AGG_Lineitem_map AGG_Lineitem;
      AGG_Supplier_map AGG_Supplier;
      AGG_LineitemSupplierPart_map AGG_LineitemSupplierPart;
      AGG_Orders_map AGG_Orders;
      AGG_Customer_map AGG_Customer;
      AGG_OrdersCustomer_map AGG_OrdersCustomer;
      DELTA_AGG_Part_map DELTA_AGG_Part;
      DELTA_AGG_Lineitem_map DELTA_AGG_Lineitem;
      DELTA_AGG_Supplier_map DELTA_AGG_Supplier;
      DELTA_AGG_LineitemSupplierPart_map DELTA_AGG_LineitemSupplierPart;
      DELTA_AGG_Orders_map DELTA_AGG_Orders;
      DELTA_AGG_Customer_map DELTA_AGG_Customer;
      DELTA_AGG_OrdersCustomer_map DELTA_AGG_OrdersCustomer;
      NATION_map NATION;
      REGION_map REGION;
      DELTA_LINEITEM_map DELTA_LINEITEM;
      DELTA_ORDERS_map DELTA_ORDERS;
      DELTA_PART_map DELTA_PART;
      DELTA_CUSTOMER_map DELTA_CUSTOMER;
      DELTA_SUPPLIER_map DELTA_SUPPLIER;
    
      PAYLOAD_entry entry_PAYLOAD;     
    
      /* Constant defitions */
      /* const static */ long c1;
      /* const static */ STRING_TYPE c3;
      /* const static */ STRING_TYPE c4;
      /* const static */ long c2;
      /* const static */ STRING_TYPE c5;
  };

  

}