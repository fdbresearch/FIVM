#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_LINEITEM_DYNAMIC

namespace dbtoaster {

  /* Definitions of maps used for storing materialized views. */
  struct PAYLOAD_entry {
    DOUBLE_TYPE sum_qty;
    DOUBLE_TYPE sum_base_price;
    DOUBLE_TYPE sum_disc_price;
    DOUBLE_TYPE sum_charge;
    DOUBLE_TYPE sum_discount;
    long count_order;
    DOUBLE_TYPE avg_qty;
    DOUBLE_TYPE avg_price;
    DOUBLE_TYPE avg_disc;

    explicit PAYLOAD_entry() { 
      clear();
    }

    FORCE_INLINE bool isZero() const { return count_order == 0; }

    FORCE_INLINE PAYLOAD_entry& operator +=(const PAYLOAD_entry &obj) {
      sum_qty += obj.sum_qty;
      sum_base_price += obj.sum_base_price;
      sum_disc_price += obj.sum_disc_price;
      sum_charge += obj.sum_charge;
      sum_discount += obj.sum_discount;
      count_order += obj.count_order;
      avg_qty = (count_order != 0 ? sum_qty / count_order : 0.0);
      avg_price = (count_order != 0 ? sum_base_price / count_order : 0.0);
      avg_disc = (count_order != 0 ? sum_discount / count_order : 0.0);      
      return *this;
    }

    FORCE_INLINE bool operator ==(const PAYLOAD_entry &obj) const {
      return (count_order == obj.count_order && 
              sum_qty == obj.sum_qty && 
              sum_base_price == obj.sum_base_price && 
              sum_disc_price == obj.sum_disc_price && 
              sum_charge == obj.sum_charge && 
              sum_discount == obj.sum_discount);
    }

    FORCE_INLINE void clear() { 
      sum_qty = 0.0;
      sum_base_price = 0.0;
      sum_disc_price = 0.0;
      sum_charge = 0.0;
      sum_discount = 0.0;
      count_order = 0;
      avg_qty = 0.0;
      avg_price = 0.0;
      avg_disc = 0.0;      
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, sum_qty);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, sum_base_price);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, sum_disc_price);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, sum_charge);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, sum_discount);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, count_order);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, avg_qty);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, avg_price);
      ar << ELEM_SEPARATOR << "\t";
      DBT_SERIALIZATION_NVP(ar, avg_disc);
    }
  };

  struct KEY_entry {
    STRING_TYPE LINEITEM_RETURNFLAG; STRING_TYPE LINEITEM_LINESTATUS; PAYLOAD_entry __av; KEY_entry* nxt; KEY_entry* prv;

    explicit KEY_entry() : nxt(nullptr), prv(nullptr) { }
    explicit KEY_entry(const STRING_TYPE& c0, const STRING_TYPE& c1, const PAYLOAD_entry& c2) { LINEITEM_RETURNFLAG = c0; LINEITEM_LINESTATUS = c1; __av = c2;  }
    KEY_entry(const KEY_entry& other) : LINEITEM_RETURNFLAG(other.LINEITEM_RETURNFLAG), LINEITEM_LINESTATUS(other.LINEITEM_LINESTATUS), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    
    FORCE_INLINE KEY_entry& modify(const STRING_TYPE& c0, const STRING_TYPE& c1) { LINEITEM_RETURNFLAG = c0; LINEITEM_LINESTATUS = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, LINEITEM_RETURNFLAG);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, LINEITEM_LINESTATUS);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }  
  };

  struct KEY_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const KEY_entry& e) {
      size_t h = 0;
      hash_combine(h, e.LINEITEM_RETURNFLAG);
      hash_combine(h, e.LINEITEM_LINESTATUS);
      return h;
    }
    
    FORCE_INLINE static bool equals(const KEY_entry& x, const KEY_entry& y) {
      return x.LINEITEM_RETURNFLAG == y.LINEITEM_RETURNFLAG && x.LINEITEM_LINESTATUS == y.LINEITEM_LINESTATUS;
    }
  };
  
  typedef MultiHashMap<KEY_entry, PAYLOAD_entry, 
    PrimaryHashIndex<KEY_entry, KEY_mapkey01_idxfn>
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
      dbtoaster::serialize_nvp_tabbed(ar, STRING(_AGG), _AGG, "\t");
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
      c1 = Udate(STRING_TYPE("1997-9-1"));
      
    }
  
    
  
    
  
    /* Trigger functions for table relations */
    
    
    /* Trigger functions for stream relations */
    void on_batch_update_LINEITEM(const std::vector<DELTA_LINEITEM_entry>::iterator &begin, const std::vector<DELTA_LINEITEM_entry>::iterator &end) {
      long batchSize = std::distance(begin, end);
      
      tN += batchSize;
      
      { //foreach
        for (std::vector<DELTA_LINEITEM_entry>::iterator e1 = begin; e1 != end; e1++) {
          long lineitem_orderkey = e1->lineitem_orderkey;
          long lineitem_partkey = e1->lineitem_partkey;
          long lineitem_suppkey = e1->lineitem_suppkey;
          long lineitem_linenumber = e1->lineitem_linenumber;
          DOUBLE_TYPE lineitem_quantity = e1->lineitem_quantity;
          DOUBLE_TYPE lineitem_extendedprice = e1->lineitem_extendedprice;
          DOUBLE_TYPE lineitem_discount = e1->lineitem_discount;
          DOUBLE_TYPE lineitem_tax = e1->lineitem_tax;
          STRING_TYPE lineitem_returnflag = e1->lineitem_returnflag;
          STRING_TYPE lineitem_linestatus = e1->lineitem_linestatus;
          date lineitem_shipdate = e1->lineitem_shipdate;
          date lineitem_commitdate = e1->lineitem_commitdate;
          date lineitem_receiptdate = e1->lineitem_receiptdate;
          STRING_TYPE lineitem_shipinstruct = e1->lineitem_shipinstruct;
          STRING_TYPE lineitem_shipmode = e1->lineitem_shipmode;
          STRING_TYPE lineitem_comment = e1->lineitem_comment;
          long v1 = e1->__av;
          if (c1 >= lineitem_shipdate) {

            entry_payload.sum_qty = v1 * lineitem_quantity;
            entry_payload.sum_base_price = v1 * lineitem_extendedprice;
            entry_payload.sum_disc_price = v1 * (lineitem_extendedprice * (1L + (-1L * lineitem_discount)));
            entry_payload.sum_charge = v1 * (lineitem_extendedprice * ((1L + (-1L * lineitem_discount)) * (1L + lineitem_tax)));
            entry_payload.sum_discount = v1 * lineitem_discount;
            entry_payload.count_order = v1;

            AGG.addOrDelOnZero(entry_key.modify(lineitem_returnflag, lineitem_linestatus), entry_payload);
          }
        }
      }
    }
    
    
    void on_system_ready_event() {
      
    }
  
  private:
    
      /* Preallocated map entries (to avoid recreation of temporary objects) */   
      KEY_entry entry_key;
      PAYLOAD_entry entry_payload;
    
      /* Constant defitions */
      /* const static */ long c1;
  };

  

}