#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"

#define RELATION_INVENTORY_DYNAMIC
#define RELATION_LOCATION_STATIC
#define RELATION_CENSUS_STATIC
#define RELATION_ITEM_STATIC
#define RELATION_WEATHER_STATIC

namespace dbtoaster {
  
  /* Definitions of auxiliary maps for storing materialized views. */
  struct PAYLOAD_INVENTORYUNITS_INVENTORY {
    DOUBLE_TYPE inventoryunits; long __av; PAYLOAD_INVENTORYUNITS_INVENTORY* nxt; PAYLOAD_INVENTORYUNITS_INVENTORY* prv;
    explicit PAYLOAD_INVENTORYUNITS_INVENTORY() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_INVENTORYUNITS_INVENTORY(const DOUBLE_TYPE c3, const long c4) { inventoryunits = c3; __av = c4;  }
    PAYLOAD_INVENTORYUNITS_INVENTORY(const PAYLOAD_INVENTORYUNITS_INVENTORY& other) : inventoryunits(other.inventoryunits), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_INVENTORYUNITS_INVENTORY& modify(const DOUBLE_TYPE c3) { inventoryunits = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, inventoryunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_INVENTORYUNITS_INVENTORY_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_INVENTORYUNITS_INVENTORY& e) {
      size_t h = 0;
      hash_combine(h, e.inventoryunits);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_INVENTORYUNITS_INVENTORY& x, const PAYLOAD_INVENTORYUNITS_INVENTORY& y) {
      return x.inventoryunits == y.inventoryunits;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_INVENTORYUNITS_INVENTORY, long, 
    PrimaryHashIndex<PAYLOAD_INVENTORYUNITS_INVENTORY, PAYLOAD_INVENTORYUNITS_INVENTORY_mapkey0123_idxfn>
  > PAYLOAD_INVENTORYUNITS_INVENTORY_map;
  
  struct PAYLOAD_RGNCD_LOCATION {
    DOUBLE_TYPE rgn_cd; DOUBLE_TYPE clim_zn_nbr; DOUBLE_TYPE tot_area_sq_ft; DOUBLE_TYPE sell_area_sq_ft; DOUBLE_TYPE avghhi; DOUBLE_TYPE supertargetdistance; DOUBLE_TYPE supertargetdrivetime; DOUBLE_TYPE targetdistance; DOUBLE_TYPE targetdrivetime; DOUBLE_TYPE walmartdistance; DOUBLE_TYPE walmartdrivetime; DOUBLE_TYPE walmartsupercenterdistance; DOUBLE_TYPE walmartsupercenterdrivetime; long __av; PAYLOAD_RGNCD_LOCATION* nxt; PAYLOAD_RGNCD_LOCATION* prv;
    explicit PAYLOAD_RGNCD_LOCATION() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_RGNCD_LOCATION(const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const DOUBLE_TYPE c11, const DOUBLE_TYPE c12, const DOUBLE_TYPE c13, const DOUBLE_TYPE c14, const long c15) { rgn_cd = c2; clim_zn_nbr = c3; tot_area_sq_ft = c4; sell_area_sq_ft = c5; avghhi = c6; supertargetdistance = c7; supertargetdrivetime = c8; targetdistance = c9; targetdrivetime = c10; walmartdistance = c11; walmartdrivetime = c12; walmartsupercenterdistance = c13; walmartsupercenterdrivetime = c14; __av = c15;  }
    PAYLOAD_RGNCD_LOCATION(const PAYLOAD_RGNCD_LOCATION& other) : rgn_cd(other.rgn_cd), clim_zn_nbr(other.clim_zn_nbr), tot_area_sq_ft(other.tot_area_sq_ft), sell_area_sq_ft(other.sell_area_sq_ft), avghhi(other.avghhi), supertargetdistance(other.supertargetdistance), supertargetdrivetime(other.supertargetdrivetime), targetdistance(other.targetdistance), targetdrivetime(other.targetdrivetime), walmartdistance(other.walmartdistance), walmartdrivetime(other.walmartdrivetime), walmartsupercenterdistance(other.walmartsupercenterdistance), walmartsupercenterdrivetime(other.walmartsupercenterdrivetime), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_RGNCD_LOCATION& modify(const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const DOUBLE_TYPE c11, const DOUBLE_TYPE c12, const DOUBLE_TYPE c13, const DOUBLE_TYPE c14) { rgn_cd = c2; clim_zn_nbr = c3; tot_area_sq_ft = c4; sell_area_sq_ft = c5; avghhi = c6; supertargetdistance = c7; supertargetdrivetime = c8; targetdistance = c9; targetdrivetime = c10; walmartdistance = c11; walmartdrivetime = c12; walmartsupercenterdistance = c13; walmartsupercenterdrivetime = c14;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rgn_cd);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, clim_zn_nbr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, tot_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, sell_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, avghhi);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_RGNCD_LOCATION_mapkey01234567891011121314_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_RGNCD_LOCATION& e) {
      size_t h = 0;
      hash_combine(h, e.rgn_cd);
      hash_combine(h, e.clim_zn_nbr);
      hash_combine(h, e.tot_area_sq_ft);
      hash_combine(h, e.sell_area_sq_ft);
      hash_combine(h, e.avghhi);
      hash_combine(h, e.supertargetdistance);
      hash_combine(h, e.supertargetdrivetime);
      hash_combine(h, e.targetdistance);
      hash_combine(h, e.targetdrivetime);
      hash_combine(h, e.walmartdistance);
      hash_combine(h, e.walmartdrivetime);
      hash_combine(h, e.walmartsupercenterdistance);
      hash_combine(h, e.walmartsupercenterdrivetime);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_RGNCD_LOCATION& x, const PAYLOAD_RGNCD_LOCATION& y) {
      return x.rgn_cd == y.rgn_cd && x.clim_zn_nbr == y.clim_zn_nbr && x.tot_area_sq_ft == y.tot_area_sq_ft && x.sell_area_sq_ft == y.sell_area_sq_ft && x.avghhi == y.avghhi && x.supertargetdistance == y.supertargetdistance && x.supertargetdrivetime == y.supertargetdrivetime && x.targetdistance == y.targetdistance && x.targetdrivetime == y.targetdrivetime && x.walmartdistance == y.walmartdistance && x.walmartdrivetime == y.walmartdrivetime && x.walmartsupercenterdistance == y.walmartsupercenterdistance && x.walmartsupercenterdrivetime == y.walmartsupercenterdrivetime;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_RGNCD_LOCATION, long, 
    PrimaryHashIndex<PAYLOAD_RGNCD_LOCATION, PAYLOAD_RGNCD_LOCATION_mapkey01234567891011121314_idxfn>
  > PAYLOAD_RGNCD_LOCATION_map;
  
  
  struct PAYLOAD_POPULATION_CENSUS {
    long population; long white; long asian; long pacific; long blackafrican; DOUBLE_TYPE medianage; long occupiedhouseunits; long houseunits; long families; long households; long husbwife; long males; long females; long householdschildren; long hispanic; long __av; PAYLOAD_POPULATION_CENSUS* nxt; PAYLOAD_POPULATION_CENSUS* prv;
    explicit PAYLOAD_POPULATION_CENSUS() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_POPULATION_CENSUS(const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8, const long c9, const long c10, const long c11, const long c12, const long c13, const long c14, const long c15, const long c16) { population = c1; white = c2; asian = c3; pacific = c4; blackafrican = c5; medianage = c6; occupiedhouseunits = c7; houseunits = c8; families = c9; households = c10; husbwife = c11; males = c12; females = c13; householdschildren = c14; hispanic = c15; __av = c16;  }
    PAYLOAD_POPULATION_CENSUS(const PAYLOAD_POPULATION_CENSUS& other) : population(other.population), white(other.white), asian(other.asian), pacific(other.pacific), blackafrican(other.blackafrican), medianage(other.medianage), occupiedhouseunits(other.occupiedhouseunits), houseunits(other.houseunits), families(other.families), households(other.households), husbwife(other.husbwife), males(other.males), females(other.females), householdschildren(other.householdschildren), hispanic(other.hispanic), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_POPULATION_CENSUS& modify(const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8, const long c9, const long c10, const long c11, const long c12, const long c13, const long c14, const long c15) { population = c1; white = c2; asian = c3; pacific = c4; blackafrican = c5; medianage = c6; occupiedhouseunits = c7; houseunits = c8; families = c9; households = c10; husbwife = c11; males = c12; females = c13; householdschildren = c14; hispanic = c15;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, population);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, white);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, asian);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, pacific);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, blackafrican);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, medianage);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, occupiedhouseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, houseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, families);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, households);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, husbwife);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, males);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, females);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, householdschildren);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, hispanic);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_POPULATION_CENSUS_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_POPULATION_CENSUS& e) {
      size_t h = 0;
      hash_combine(h, e.population);
      hash_combine(h, e.white);
      hash_combine(h, e.asian);
      hash_combine(h, e.pacific);
      hash_combine(h, e.blackafrican);
      hash_combine(h, e.medianage);
      hash_combine(h, e.occupiedhouseunits);
      hash_combine(h, e.houseunits);
      hash_combine(h, e.families);
      hash_combine(h, e.households);
      hash_combine(h, e.husbwife);
      hash_combine(h, e.males);
      hash_combine(h, e.females);
      hash_combine(h, e.householdschildren);
      hash_combine(h, e.hispanic);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_POPULATION_CENSUS& x, const PAYLOAD_POPULATION_CENSUS& y) {
      return x.population == y.population && x.white == y.white && x.asian == y.asian && x.pacific == y.pacific && x.blackafrican == y.blackafrican && x.medianage == y.medianage && x.occupiedhouseunits == y.occupiedhouseunits && x.houseunits == y.houseunits && x.families == y.families && x.households == y.households && x.husbwife == y.husbwife && x.males == y.males && x.females == y.females && x.householdschildren == y.householdschildren && x.hispanic == y.hispanic;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_POPULATION_CENSUS, long, 
    PrimaryHashIndex<PAYLOAD_POPULATION_CENSUS, PAYLOAD_POPULATION_CENSUS_mapkey0123456789101112131415_idxfn>
  > PAYLOAD_POPULATION_CENSUS_map;


  struct PAYLOAD_SUBCATEGORY_ITEM {
    long subcategory; long category; long categorycluster; DOUBLE_TYPE prize; long __av; PAYLOAD_SUBCATEGORY_ITEM* nxt; PAYLOAD_SUBCATEGORY_ITEM* prv;
    explicit PAYLOAD_SUBCATEGORY_ITEM() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_SUBCATEGORY_ITEM(const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const long c5) { subcategory = c1; category = c2; categorycluster = c3; prize = c4; __av = c5;  }
    PAYLOAD_SUBCATEGORY_ITEM(const PAYLOAD_SUBCATEGORY_ITEM& other) : subcategory(other.subcategory), category(other.category), categorycluster(other.categorycluster), prize(other.prize), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_SUBCATEGORY_ITEM& modify(const long c1, const long c2, const long c3, const DOUBLE_TYPE c4) { subcategory = c1; category = c2; categorycluster = c3; prize = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, subcategory);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, category);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, categorycluster);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, prize);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_SUBCATEGORY_ITEM_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_SUBCATEGORY_ITEM& e) {
      size_t h = 0;
      hash_combine(h, e.subcategory);
      hash_combine(h, e.category);
      hash_combine(h, e.categorycluster);
      hash_combine(h, e.prize);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_SUBCATEGORY_ITEM& x, const PAYLOAD_SUBCATEGORY_ITEM& y) {
      return x.subcategory == y.subcategory && x.category == y.category && x.categorycluster == y.categorycluster && x.prize == y.prize;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_SUBCATEGORY_ITEM, long, 
    PrimaryHashIndex<PAYLOAD_SUBCATEGORY_ITEM, PAYLOAD_SUBCATEGORY_ITEM_mapkey01234_idxfn>
  > PAYLOAD_SUBCATEGORY_ITEM_map;

  struct PAYLOAD_RAIN_WEATHER {
    long rain; long snow; long maxtemp; long mintemp; DOUBLE_TYPE meanwind; long thunder; long __av; PAYLOAD_RAIN_WEATHER* nxt; PAYLOAD_RAIN_WEATHER* prv;
    explicit PAYLOAD_RAIN_WEATHER() : nxt(nullptr), prv(nullptr) { }
    explicit PAYLOAD_RAIN_WEATHER(const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8) { rain = c2; snow = c3; maxtemp = c4; mintemp = c5; meanwind = c6; thunder = c7; __av = c8;  }
    PAYLOAD_RAIN_WEATHER(const PAYLOAD_RAIN_WEATHER& other) : rain(other.rain), snow(other.snow), maxtemp(other.maxtemp), mintemp(other.mintemp), meanwind(other.meanwind), thunder(other.thunder), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE PAYLOAD_RAIN_WEATHER& modify(const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7) { rain = c2; snow = c3; maxtemp = c4; mintemp = c5; meanwind = c6; thunder = c7;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rain);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, snow);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, maxtemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mintemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, meanwind);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, thunder);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_RAIN_WEATHER_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_RAIN_WEATHER& e) {
      size_t h = 0;
      hash_combine(h, e.rain);
      hash_combine(h, e.snow);
      hash_combine(h, e.maxtemp);
      hash_combine(h, e.mintemp);
      hash_combine(h, e.meanwind);
      hash_combine(h, e.thunder);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_RAIN_WEATHER& x, const PAYLOAD_RAIN_WEATHER& y) {
      return x.rain == y.rain && x.snow == y.snow && x.maxtemp == y.maxtemp && x.mintemp == y.mintemp && x.meanwind == y.meanwind && x.thunder == y.thunder;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_RAIN_WEATHER, long, 
    PrimaryHashIndex<PAYLOAD_RAIN_WEATHER, PAYLOAD_RAIN_WEATHER_mapkey01234567_idxfn>
  > PAYLOAD_RAIN_WEATHER_map;


  struct PAYLOAD_KSN_INVENTORY_ITEM {
    DOUBLE_TYPE ksn;
    DOUBLE_TYPE inventoryunits; 
    long subcategory; long category; long categorycluster; DOUBLE_TYPE prize;
    long __av; PAYLOAD_KSN_INVENTORY_ITEM* nxt; PAYLOAD_KSN_INVENTORY_ITEM* prv;
    explicit PAYLOAD_KSN_INVENTORY_ITEM() : nxt(nullptr), prv(nullptr) { }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, inventoryunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, subcategory);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, category);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, categorycluster);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, prize);      
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_KSN_INVENTORY_ITEM_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_KSN_INVENTORY_ITEM& e) {
      size_t h = 0;
      hash_combine(h, e.ksn);
      hash_combine(h, e.inventoryunits);
      hash_combine(h, e.subcategory);
      // hash_combine(h, e.category);
      // hash_combine(h, e.categorycluster);
      // hash_combine(h, e.prize);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_KSN_INVENTORY_ITEM& x, const PAYLOAD_KSN_INVENTORY_ITEM& y) {
      return 
        x.ksn == y.ksn &&
        x.inventoryunits == y.inventoryunits &&
        x.subcategory == y.subcategory && x.category == y.category && x.categorycluster == y.categorycluster && x.prize == y.prize;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_KSN_INVENTORY_ITEM, long, 
    PrimaryHashIndex<PAYLOAD_KSN_INVENTORY_ITEM, PAYLOAD_KSN_INVENTORY_ITEM_mapkey0123_idxfn>
  > PAYLOAD_KSN_INVENTORY_ITEM_map;


  struct PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER {
    long dateid; DOUBLE_TYPE ksn;
    DOUBLE_TYPE inventoryunits; 
    long subcategory; long category; long categorycluster; DOUBLE_TYPE prize;
    long rain; long snow; long maxtemp; long mintemp; DOUBLE_TYPE meanwind; long thunder;
    long __av; PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER* nxt; PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER* prv;
    explicit PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER() : nxt(nullptr), prv(nullptr) { }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, inventoryunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, subcategory);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, category);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, categorycluster);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, prize);      
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rain);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, snow);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, maxtemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mintemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, meanwind);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, thunder);      
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER& e) {
      size_t h = 0;
      hash_combine(h, e.dateid);
      hash_combine(h, e.ksn);
      hash_combine(h, e.inventoryunits);
      // hash_combine(h, e.subcategory);
      // hash_combine(h, e.category);
      // hash_combine(h, e.categorycluster);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER& x, const PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER& y) {
      return 
        x.dateid == y.dateid &&  x.ksn == y.ksn &&
        x.inventoryunits == y.inventoryunits &&
        x.subcategory == y.subcategory && x.category == y.category && x.categorycluster == y.categorycluster && x.prize == y.prize &&
        x.rain == y.rain && x.snow == y.snow && x.maxtemp == y.maxtemp && x.mintemp == y.mintemp && x.meanwind == y.meanwind && x.thunder == y.thunder;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER, long, 
    PrimaryHashIndex<PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER, PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_mapkey0123_idxfn>
  > PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map;


  struct PAYLOAD_ZIP_LOCATION_CENSUS {
    long zip; 
    DOUBLE_TYPE rgn_cd; DOUBLE_TYPE clim_zn_nbr; DOUBLE_TYPE tot_area_sq_ft; DOUBLE_TYPE sell_area_sq_ft; DOUBLE_TYPE avghhi; DOUBLE_TYPE supertargetdistance; DOUBLE_TYPE supertargetdrivetime; DOUBLE_TYPE targetdistance; DOUBLE_TYPE targetdrivetime; DOUBLE_TYPE walmartdistance; DOUBLE_TYPE walmartdrivetime; DOUBLE_TYPE walmartsupercenterdistance; DOUBLE_TYPE walmartsupercenterdrivetime; 
    long population; long white; long asian; long pacific; long blackafrican; DOUBLE_TYPE medianage; long occupiedhouseunits; long houseunits; long families; long households; long husbwife; long males; long females; long householdschildren; long hispanic; 
    long __av; PAYLOAD_ZIP_LOCATION_CENSUS* nxt; PAYLOAD_ZIP_LOCATION_CENSUS* prv;
    explicit PAYLOAD_ZIP_LOCATION_CENSUS() : nxt(nullptr), prv(nullptr) { }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rgn_cd);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, clim_zn_nbr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, tot_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, sell_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, avghhi);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, population);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, white);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, asian);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, pacific);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, blackafrican);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, medianage);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, occupiedhouseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, houseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, families);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, households);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, husbwife);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, males);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, females);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, householdschildren);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, hispanic);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_ZIP_LOCATION_CENSUS_mapkey01234567891011121314_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_ZIP_LOCATION_CENSUS& e) {
      size_t h = 0;
      hash_combine(h, e.zip);
      hash_combine(h, e.rgn_cd);
      // hash_combine(h, e.clim_zn_nbr);
      // hash_combine(h, e.population);      
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_ZIP_LOCATION_CENSUS& x, const PAYLOAD_ZIP_LOCATION_CENSUS& y) {
      return 
        x.zip == y.zip && 
        x.rgn_cd == y.rgn_cd && x.clim_zn_nbr == y.clim_zn_nbr && x.tot_area_sq_ft == y.tot_area_sq_ft && x.sell_area_sq_ft == y.sell_area_sq_ft && x.avghhi == y.avghhi && x.supertargetdistance == y.supertargetdistance && x.supertargetdrivetime == y.supertargetdrivetime && x.targetdistance == y.targetdistance && x.targetdrivetime == y.targetdrivetime && x.walmartdistance == y.walmartdistance && x.walmartdrivetime == y.walmartdrivetime && x.walmartsupercenterdistance == y.walmartsupercenterdistance && x.walmartsupercenterdrivetime == y.walmartsupercenterdrivetime &&
        x.population == y.population && x.white == y.white && x.asian == y.asian && x.pacific == y.pacific && x.blackafrican == y.blackafrican && x.medianage == y.medianage && x.occupiedhouseunits == y.occupiedhouseunits && x.houseunits == y.houseunits && x.families == y.families && x.households == y.households && x.husbwife == y.husbwife && x.males == y.males && x.females == y.females && x.householdschildren == y.householdschildren && x.hispanic == y.hispanic;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_ZIP_LOCATION_CENSUS, long, 
    PrimaryHashIndex<PAYLOAD_ZIP_LOCATION_CENSUS, PAYLOAD_ZIP_LOCATION_CENSUS_mapkey01234567891011121314_idxfn>
  > PAYLOAD_ZIP_LOCATION_CENSUS_map;


  struct PAYLOAD_LOCN {
    long locn; long dateid; DOUBLE_TYPE ksn;
    DOUBLE_TYPE inventoryunits;
    long subcategory; long category; long categorycluster; DOUBLE_TYPE prize;
    long rain; long snow; long maxtemp; long mintemp; DOUBLE_TYPE meanwind; long thunder;
    long zip; 
    DOUBLE_TYPE rgn_cd; DOUBLE_TYPE clim_zn_nbr; DOUBLE_TYPE tot_area_sq_ft; DOUBLE_TYPE sell_area_sq_ft; DOUBLE_TYPE avghhi; DOUBLE_TYPE supertargetdistance; DOUBLE_TYPE supertargetdrivetime; DOUBLE_TYPE targetdistance; DOUBLE_TYPE targetdrivetime; DOUBLE_TYPE walmartdistance; DOUBLE_TYPE walmartdrivetime; DOUBLE_TYPE walmartsupercenterdistance; DOUBLE_TYPE walmartsupercenterdrivetime; 
    long population; long white; long asian; long pacific; long blackafrican; DOUBLE_TYPE medianage; long occupiedhouseunits; long houseunits; long families; long households; long husbwife; long males; long females; long householdschildren; long hispanic;     
    long __av; PAYLOAD_LOCN* nxt; PAYLOAD_LOCN* prv;
    explicit PAYLOAD_LOCN() : nxt(nullptr), prv(nullptr) { }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, inventoryunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, subcategory);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, category);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, categorycluster);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, prize);      
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rain);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, snow);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, maxtemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mintemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, meanwind);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, thunder);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rgn_cd);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, clim_zn_nbr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, tot_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, sell_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, avghhi);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, population);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, white);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, asian);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, pacific);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, blackafrican);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, medianage);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, occupiedhouseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, houseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, families);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, households);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, husbwife);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, males);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, females);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, householdschildren);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, hispanic);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct PAYLOAD_LOCN_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const PAYLOAD_LOCN& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      hash_combine(h, e.ksn);
      return h;
    }
    FORCE_INLINE static bool equals(const PAYLOAD_LOCN& x, const PAYLOAD_LOCN& y) {
      return 
        x.locn == y.locn && x.dateid == y.dateid &&  x.ksn == y.ksn &&
        x.inventoryunits == y.inventoryunits &&
        x.subcategory == y.subcategory && x.category == y.category && x.categorycluster == y.categorycluster && x.prize == y.prize &&
        x.rain == y.rain && x.snow == y.snow && x.maxtemp == y.maxtemp && x.mintemp == y.mintemp && x.meanwind == y.meanwind && x.thunder == y.thunder &&
        x.zip == y.zip && 
        x.rgn_cd == y.rgn_cd && x.clim_zn_nbr == y.clim_zn_nbr && x.tot_area_sq_ft == y.tot_area_sq_ft && x.sell_area_sq_ft == y.sell_area_sq_ft && x.avghhi == y.avghhi && x.supertargetdistance == y.supertargetdistance && x.supertargetdrivetime == y.supertargetdrivetime && x.targetdistance == y.targetdistance && x.targetdrivetime == y.targetdrivetime && x.walmartdistance == y.walmartdistance && x.walmartdrivetime == y.walmartdrivetime && x.walmartsupercenterdistance == y.walmartsupercenterdistance && x.walmartsupercenterdrivetime == y.walmartsupercenterdrivetime &&
        x.population == y.population && x.white == y.white && x.asian == y.asian && x.pacific == y.pacific && x.blackafrican == y.blackafrican && x.medianage == y.medianage && x.occupiedhouseunits == y.occupiedhouseunits && x.houseunits == y.houseunits && x.families == y.families && x.households == y.households && x.husbwife == y.husbwife && x.males == y.males && x.females == y.females && x.householdschildren == y.householdschildren && x.hispanic == y.hispanic;
    }
  };
  
  typedef MultiHashMap<PAYLOAD_LOCN, long, 
    PrimaryHashIndex<PAYLOAD_LOCN, PAYLOAD_LOCN_mapkey0123_idxfn>
  > PAYLOAD_LOCN_map;





  /* Definitions of auxiliary maps for storing materialized views. */
  struct INVENTORY_entry {
    long locn; long dateid; DOUBLE_TYPE ksn; DOUBLE_TYPE inventoryunits; long __av; INVENTORY_entry* nxt; INVENTORY_entry* prv;
    explicit INVENTORY_entry() : nxt(nullptr), prv(nullptr) { }
    explicit INVENTORY_entry(const long c0, const long c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const long c4) { locn = c0; dateid = c1; ksn = c2; inventoryunits = c3; __av = c4;  }
    INVENTORY_entry(const INVENTORY_entry& other) : locn(other.locn), dateid(other.dateid), ksn(other.ksn), inventoryunits(other.inventoryunits), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    INVENTORY_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 4) return; */ locn = std::stol(f[0]); dateid = std::stol(f[1]); ksn = std::stod(f[2]); inventoryunits = std::stod(f[3]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE INVENTORY_entry& modify(const long c0, const long c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3) { locn = c0; dateid = c1; ksn = c2; inventoryunits = c3;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, inventoryunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct INVENTORY_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const INVENTORY_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      hash_combine(h, e.ksn);
      hash_combine(h, e.inventoryunits);
      return h;
    }
    FORCE_INLINE static bool equals(const INVENTORY_entry& x, const INVENTORY_entry& y) {
      return x.locn == y.locn && x.dateid == y.dateid && x.ksn == y.ksn && x.inventoryunits == y.inventoryunits;
    }
  };
  
  typedef MultiHashMap<INVENTORY_entry, long, 
    PrimaryHashIndex<INVENTORY_entry, INVENTORY_mapkey0123_idxfn>
  > INVENTORY_map;
  
  typedef INVENTORY_entry DELTA_INVENTORY_entry;
  typedef INVENTORY_map DELTA_INVENTORY_map;
  struct LOCATION_entry {
    long locn; long zip; DOUBLE_TYPE rgn_cd; DOUBLE_TYPE clim_zn_nbr; DOUBLE_TYPE tot_area_sq_ft; DOUBLE_TYPE sell_area_sq_ft; DOUBLE_TYPE avghhi; DOUBLE_TYPE supertargetdistance; DOUBLE_TYPE supertargetdrivetime; DOUBLE_TYPE targetdistance; DOUBLE_TYPE targetdrivetime; DOUBLE_TYPE walmartdistance; DOUBLE_TYPE walmartdrivetime; DOUBLE_TYPE walmartsupercenterdistance; DOUBLE_TYPE walmartsupercenterdrivetime; long __av; LOCATION_entry* nxt; LOCATION_entry* prv;
    explicit LOCATION_entry() : nxt(nullptr), prv(nullptr) { }
    explicit LOCATION_entry(const long c0, const long c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const DOUBLE_TYPE c11, const DOUBLE_TYPE c12, const DOUBLE_TYPE c13, const DOUBLE_TYPE c14, const long c15) { locn = c0; zip = c1; rgn_cd = c2; clim_zn_nbr = c3; tot_area_sq_ft = c4; sell_area_sq_ft = c5; avghhi = c6; supertargetdistance = c7; supertargetdrivetime = c8; targetdistance = c9; targetdrivetime = c10; walmartdistance = c11; walmartdrivetime = c12; walmartsupercenterdistance = c13; walmartsupercenterdrivetime = c14; __av = c15;  }
    LOCATION_entry(const LOCATION_entry& other) : locn(other.locn), zip(other.zip), rgn_cd(other.rgn_cd), clim_zn_nbr(other.clim_zn_nbr), tot_area_sq_ft(other.tot_area_sq_ft), sell_area_sq_ft(other.sell_area_sq_ft), avghhi(other.avghhi), supertargetdistance(other.supertargetdistance), supertargetdrivetime(other.supertargetdrivetime), targetdistance(other.targetdistance), targetdrivetime(other.targetdrivetime), walmartdistance(other.walmartdistance), walmartdrivetime(other.walmartdrivetime), walmartsupercenterdistance(other.walmartsupercenterdistance), walmartsupercenterdrivetime(other.walmartsupercenterdrivetime), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    LOCATION_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 15) return; */ locn = std::stol(f[0]); zip = std::stol(f[1]); rgn_cd = std::stod(f[2]); clim_zn_nbr = std::stod(f[3]); tot_area_sq_ft = std::stod(f[4]); sell_area_sq_ft = std::stod(f[5]); avghhi = std::stod(f[6]); supertargetdistance = std::stod(f[7]); supertargetdrivetime = std::stod(f[8]); targetdistance = std::stod(f[9]); targetdrivetime = std::stod(f[10]); walmartdistance = std::stod(f[11]); walmartdrivetime = std::stod(f[12]); walmartsupercenterdistance = std::stod(f[13]); walmartsupercenterdrivetime = std::stod(f[14]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE LOCATION_entry& modify(const long c0, const long c1, const DOUBLE_TYPE c2, const DOUBLE_TYPE c3, const DOUBLE_TYPE c4, const DOUBLE_TYPE c5, const DOUBLE_TYPE c6, const DOUBLE_TYPE c7, const DOUBLE_TYPE c8, const DOUBLE_TYPE c9, const DOUBLE_TYPE c10, const DOUBLE_TYPE c11, const DOUBLE_TYPE c12, const DOUBLE_TYPE c13, const DOUBLE_TYPE c14) { locn = c0; zip = c1; rgn_cd = c2; clim_zn_nbr = c3; tot_area_sq_ft = c4; sell_area_sq_ft = c5; avghhi = c6; supertargetdistance = c7; supertargetdrivetime = c8; targetdistance = c9; targetdrivetime = c10; walmartdistance = c11; walmartdrivetime = c12; walmartsupercenterdistance = c13; walmartsupercenterdrivetime = c14;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rgn_cd);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, clim_zn_nbr);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, tot_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, sell_area_sq_ft);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, avghhi);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, supertargetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, targetdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdistance);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, walmartsupercenterdrivetime);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct LOCATION_mapkey01234567891011121314_idxfn {
    FORCE_INLINE static size_t hash(const LOCATION_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.zip);
      hash_combine(h, e.rgn_cd);
      hash_combine(h, e.clim_zn_nbr);
      hash_combine(h, e.tot_area_sq_ft);
      hash_combine(h, e.sell_area_sq_ft);
      hash_combine(h, e.avghhi);
      hash_combine(h, e.supertargetdistance);
      hash_combine(h, e.supertargetdrivetime);
      hash_combine(h, e.targetdistance);
      hash_combine(h, e.targetdrivetime);
      hash_combine(h, e.walmartdistance);
      hash_combine(h, e.walmartdrivetime);
      hash_combine(h, e.walmartsupercenterdistance);
      hash_combine(h, e.walmartsupercenterdrivetime);
      return h;
    }
    FORCE_INLINE static bool equals(const LOCATION_entry& x, const LOCATION_entry& y) {
      return x.locn == y.locn && x.zip == y.zip && x.rgn_cd == y.rgn_cd && x.clim_zn_nbr == y.clim_zn_nbr && x.tot_area_sq_ft == y.tot_area_sq_ft && x.sell_area_sq_ft == y.sell_area_sq_ft && x.avghhi == y.avghhi && x.supertargetdistance == y.supertargetdistance && x.supertargetdrivetime == y.supertargetdrivetime && x.targetdistance == y.targetdistance && x.targetdrivetime == y.targetdrivetime && x.walmartdistance == y.walmartdistance && x.walmartdrivetime == y.walmartdrivetime && x.walmartsupercenterdistance == y.walmartsupercenterdistance && x.walmartsupercenterdrivetime == y.walmartsupercenterdrivetime;
    }
  };
  
  typedef MultiHashMap<LOCATION_entry, long, 
    PrimaryHashIndex<LOCATION_entry, LOCATION_mapkey01234567891011121314_idxfn>
  > LOCATION_map;
  
  typedef LOCATION_entry DELTA_LOCATION_entry;
  typedef LOCATION_map DELTA_LOCATION_map;
  struct CENSUS_entry {
    long zip; long population; long white; long asian; long pacific; long blackafrican; DOUBLE_TYPE medianage; long occupiedhouseunits; long houseunits; long families; long households; long husbwife; long males; long females; long householdschildren; long hispanic; long __av; CENSUS_entry* nxt; CENSUS_entry* prv;
    explicit CENSUS_entry() : nxt(nullptr), prv(nullptr) { }
    explicit CENSUS_entry(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8, const long c9, const long c10, const long c11, const long c12, const long c13, const long c14, const long c15, const long c16) { zip = c0; population = c1; white = c2; asian = c3; pacific = c4; blackafrican = c5; medianage = c6; occupiedhouseunits = c7; houseunits = c8; families = c9; households = c10; husbwife = c11; males = c12; females = c13; householdschildren = c14; hispanic = c15; __av = c16;  }
    CENSUS_entry(const CENSUS_entry& other) : zip(other.zip), population(other.population), white(other.white), asian(other.asian), pacific(other.pacific), blackafrican(other.blackafrican), medianage(other.medianage), occupiedhouseunits(other.occupiedhouseunits), houseunits(other.houseunits), families(other.families), households(other.households), husbwife(other.husbwife), males(other.males), females(other.females), householdschildren(other.householdschildren), hispanic(other.hispanic), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    CENSUS_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 16) return; */ zip = std::stol(f[0]); population = std::stol(f[1]); white = std::stol(f[2]); asian = std::stol(f[3]); pacific = std::stol(f[4]); blackafrican = std::stol(f[5]); medianage = std::stod(f[6]); occupiedhouseunits = std::stol(f[7]); houseunits = std::stol(f[8]); families = std::stol(f[9]); households = std::stol(f[10]); husbwife = std::stol(f[11]); males = std::stol(f[12]); females = std::stol(f[13]); householdschildren = std::stol(f[14]); hispanic = std::stol(f[15]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE CENSUS_entry& modify(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8, const long c9, const long c10, const long c11, const long c12, const long c13, const long c14, const long c15) { zip = c0; population = c1; white = c2; asian = c3; pacific = c4; blackafrican = c5; medianage = c6; occupiedhouseunits = c7; houseunits = c8; families = c9; households = c10; husbwife = c11; males = c12; females = c13; householdschildren = c14; hispanic = c15;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, population);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, white);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, asian);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, pacific);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, blackafrican);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, medianage);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, occupiedhouseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, houseunits);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, families);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, households);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, husbwife);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, males);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, females);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, householdschildren);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, hispanic);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct CENSUS_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const CENSUS_entry& e) {
      size_t h = 0;
      hash_combine(h, e.zip);
      hash_combine(h, e.population);
      hash_combine(h, e.white);
      hash_combine(h, e.asian);
      hash_combine(h, e.pacific);
      hash_combine(h, e.blackafrican);
      hash_combine(h, e.medianage);
      hash_combine(h, e.occupiedhouseunits);
      hash_combine(h, e.houseunits);
      hash_combine(h, e.families);
      hash_combine(h, e.households);
      hash_combine(h, e.husbwife);
      hash_combine(h, e.males);
      hash_combine(h, e.females);
      hash_combine(h, e.householdschildren);
      hash_combine(h, e.hispanic);
      return h;
    }
    FORCE_INLINE static bool equals(const CENSUS_entry& x, const CENSUS_entry& y) {
      return x.zip == y.zip && x.population == y.population && x.white == y.white && x.asian == y.asian && x.pacific == y.pacific && x.blackafrican == y.blackafrican && x.medianage == y.medianage && x.occupiedhouseunits == y.occupiedhouseunits && x.houseunits == y.houseunits && x.families == y.families && x.households == y.households && x.husbwife == y.husbwife && x.males == y.males && x.females == y.females && x.householdschildren == y.householdschildren && x.hispanic == y.hispanic;
    }
  };
  
  typedef MultiHashMap<CENSUS_entry, long, 
    PrimaryHashIndex<CENSUS_entry, CENSUS_mapkey0123456789101112131415_idxfn>
  > CENSUS_map;
  
  typedef CENSUS_entry DELTA_CENSUS_entry;
  typedef CENSUS_map DELTA_CENSUS_map;
  struct ITEM_entry {
    DOUBLE_TYPE ksn; long subcategory; long category; long categorycluster; DOUBLE_TYPE prize; long __av; ITEM_entry* nxt; ITEM_entry* prv;
    explicit ITEM_entry() : nxt(nullptr), prv(nullptr) { }
    explicit ITEM_entry(const DOUBLE_TYPE c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4, const long c5) { ksn = c0; subcategory = c1; category = c2; categorycluster = c3; prize = c4; __av = c5;  }
    ITEM_entry(const ITEM_entry& other) : ksn(other.ksn), subcategory(other.subcategory), category(other.category), categorycluster(other.categorycluster), prize(other.prize), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    ITEM_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 5) return; */ ksn = std::stod(f[0]); subcategory = std::stol(f[1]); category = std::stol(f[2]); categorycluster = std::stol(f[3]); prize = std::stod(f[4]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE ITEM_entry& modify(const DOUBLE_TYPE c0, const long c1, const long c2, const long c3, const DOUBLE_TYPE c4) { ksn = c0; subcategory = c1; category = c2; categorycluster = c3; prize = c4;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, subcategory);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, category);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, categorycluster);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, prize);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct ITEM_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const ITEM_entry& e) {
      size_t h = 0;
      hash_combine(h, e.ksn);
      hash_combine(h, e.subcategory);
      hash_combine(h, e.category);
      hash_combine(h, e.categorycluster);
      hash_combine(h, e.prize);
      return h;
    }
    FORCE_INLINE static bool equals(const ITEM_entry& x, const ITEM_entry& y) {
      return x.ksn == y.ksn && x.subcategory == y.subcategory && x.category == y.category && x.categorycluster == y.categorycluster && x.prize == y.prize;
    }
  };
  
  typedef MultiHashMap<ITEM_entry, long, 
    PrimaryHashIndex<ITEM_entry, ITEM_mapkey01234_idxfn>
  > ITEM_map;
  
  typedef ITEM_entry DELTA_ITEM_entry;
  typedef ITEM_map DELTA_ITEM_map;
  struct WEATHER_entry {
    long locn; long dateid; long rain; long snow; long maxtemp; long mintemp; DOUBLE_TYPE meanwind; long thunder; long __av; WEATHER_entry* nxt; WEATHER_entry* prv;
    explicit WEATHER_entry() : nxt(nullptr), prv(nullptr) { }
    explicit WEATHER_entry(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7, const long c8) { locn = c0; dateid = c1; rain = c2; snow = c3; maxtemp = c4; mintemp = c5; meanwind = c6; thunder = c7; __av = c8;  }
    WEATHER_entry(const WEATHER_entry& other) : locn(other.locn), dateid(other.dateid), rain(other.rain), snow(other.snow), maxtemp(other.maxtemp), mintemp(other.mintemp), meanwind(other.meanwind), thunder(other.thunder), __av(other.__av), nxt(nullptr), prv(nullptr) { }
    WEATHER_entry(const std::vector<std::string>& f, const long v) { /* if (f.size() < 8) return; */ locn = std::stol(f[0]); dateid = std::stol(f[1]); rain = std::stol(f[2]); snow = std::stol(f[3]); maxtemp = std::stol(f[4]); mintemp = std::stol(f[5]); meanwind = std::stod(f[6]); thunder = std::stol(f[7]); __av = v; nxt = nullptr; prv = nullptr; }
    FORCE_INLINE WEATHER_entry& modify(const long c0, const long c1, const long c2, const long c3, const long c4, const long c5, const DOUBLE_TYPE c6, const long c7) { locn = c0; dateid = c1; rain = c2; snow = c3; maxtemp = c4; mintemp = c5; meanwind = c6; thunder = c7;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, rain);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, snow);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, maxtemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, mintemp);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, meanwind);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, thunder);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct WEATHER_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const WEATHER_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      hash_combine(h, e.rain);
      hash_combine(h, e.snow);
      hash_combine(h, e.maxtemp);
      hash_combine(h, e.mintemp);
      hash_combine(h, e.meanwind);
      hash_combine(h, e.thunder);
      return h;
    }
    FORCE_INLINE static bool equals(const WEATHER_entry& x, const WEATHER_entry& y) {
      return x.locn == y.locn && x.dateid == y.dateid && x.rain == y.rain && x.snow == y.snow && x.maxtemp == y.maxtemp && x.mintemp == y.mintemp && x.meanwind == y.meanwind && x.thunder == y.thunder;
    }
  };
  
  typedef MultiHashMap<WEATHER_entry, long, 
    PrimaryHashIndex<WEATHER_entry, WEATHER_mapkey01234567_idxfn>
  > WEATHER_map;
  
  typedef WEATHER_entry DELTA_WEATHER_entry;
  typedef WEATHER_map DELTA_WEATHER_map;

  struct KEY_INVENTORYUNITS_INVENTORY {
    long locn; long dateid; DOUBLE_TYPE ksn; PAYLOAD_INVENTORYUNITS_INVENTORY_map __av; KEY_INVENTORYUNITS_INVENTORY* nxt; KEY_INVENTORYUNITS_INVENTORY* prv;
    explicit KEY_INVENTORYUNITS_INVENTORY() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_INVENTORYUNITS_INVENTORY& modify(const long c0, const long c1, const DOUBLE_TYPE c2) { locn = c0; dateid = c1; ksn = c2;  return *this; }
    FORCE_INLINE KEY_INVENTORYUNITS_INVENTORY& modify2(const DOUBLE_TYPE c2) { ksn = c2;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_INVENTORYUNITS_mapkey012_idxfn {
    FORCE_INLINE static size_t hash(const KEY_INVENTORYUNITS_INVENTORY& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      hash_combine(h, e.ksn);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_INVENTORYUNITS_INVENTORY& x, const KEY_INVENTORYUNITS_INVENTORY& y) {
      return x.locn == y.locn && x.dateid == y.dateid && x.ksn == y.ksn;
    }
  };
  
  struct KEY_INVENTORYUNITS_mapkey2_idxfn {
    FORCE_INLINE static size_t hash(const KEY_INVENTORYUNITS_INVENTORY& e) {
      size_t h = 0;
      hash_combine(h, e.ksn);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_INVENTORYUNITS_INVENTORY& x, const KEY_INVENTORYUNITS_INVENTORY& y) {
      return x.ksn == y.ksn;
    }
  };
  
  typedef MultiHashMap<KEY_INVENTORYUNITS_INVENTORY, PAYLOAD_INVENTORYUNITS_INVENTORY_map, 
    PrimaryHashIndex<KEY_INVENTORYUNITS_INVENTORY, KEY_INVENTORYUNITS_mapkey012_idxfn>, 
    SecondaryHashIndex<KEY_INVENTORYUNITS_INVENTORY, KEY_INVENTORYUNITS_mapkey2_idxfn>
  > AGG_INVENTORYUNITS_INVENTORY_map;
  

  struct KEY_RGNCD_LOCATION {
    long locn; long zip; PAYLOAD_RGNCD_LOCATION_map __av; KEY_RGNCD_LOCATION* nxt; KEY_RGNCD_LOCATION* prv;
    explicit KEY_RGNCD_LOCATION() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_RGNCD_LOCATION& modify(const long c0, const long c1) { locn = c0; zip = c1;  return *this; }
    FORCE_INLINE KEY_RGNCD_LOCATION& modify1(const long c1) { zip = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_RGNCD_LOCATION_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const KEY_RGNCD_LOCATION& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.zip);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_RGNCD_LOCATION& x, const KEY_RGNCD_LOCATION& y) {
      return x.locn == y.locn && x.zip == y.zip;
    }
  };
  
  struct KEY_RGNCD_LOCATION_mapkey1_idxfn {
    FORCE_INLINE static size_t hash(const KEY_RGNCD_LOCATION& e) {
      size_t h = 0;
      hash_combine(h, e.zip);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_RGNCD_LOCATION& x, const KEY_RGNCD_LOCATION& y) {
      return x.zip == y.zip;
    }
  };
  
  typedef MultiHashMap<KEY_RGNCD_LOCATION, PAYLOAD_RGNCD_LOCATION_map, 
    PrimaryHashIndex<KEY_RGNCD_LOCATION, KEY_RGNCD_LOCATION_mapkey01_idxfn>, 
    SecondaryHashIndex<KEY_RGNCD_LOCATION, KEY_RGNCD_LOCATION_mapkey1_idxfn>
  > AGG_RGNCD_LOCATION_map;
  

  struct KEY_POPULATION_CENSUS {
    long zip; PAYLOAD_POPULATION_CENSUS_map __av; KEY_POPULATION_CENSUS* nxt; KEY_POPULATION_CENSUS* prv;
    explicit KEY_POPULATION_CENSUS() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_POPULATION_CENSUS& modify(const long c0) { zip = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_POPULATION_CENSUS_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const KEY_POPULATION_CENSUS& e) {
      size_t h = 0;
      hash_combine(h, e.zip);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_POPULATION_CENSUS& x, const KEY_POPULATION_CENSUS& y) {
      return x.zip == y.zip;
    }
  };
  
  typedef MultiHashMap<KEY_POPULATION_CENSUS, PAYLOAD_POPULATION_CENSUS_map, 
    PrimaryHashIndex<KEY_POPULATION_CENSUS, KEY_POPULATION_CENSUS_mapkey0_idxfn>
  > AGG_POPULATION_CENSUS_map;
  

  struct KEY_SUBCATEGORY_ITEM {
    DOUBLE_TYPE ksn; PAYLOAD_SUBCATEGORY_ITEM_map __av; KEY_SUBCATEGORY_ITEM* nxt; KEY_SUBCATEGORY_ITEM* prv;
    explicit KEY_SUBCATEGORY_ITEM() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_SUBCATEGORY_ITEM& modify(const DOUBLE_TYPE c0) { ksn = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_SUBCATEGORY_ITEM_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const KEY_SUBCATEGORY_ITEM& e) {
      size_t h = 0;
      hash_combine(h, e.ksn);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_SUBCATEGORY_ITEM& x, const KEY_SUBCATEGORY_ITEM& y) {
      return x.ksn == y.ksn;
    }
  };
  
  typedef MultiHashMap<KEY_SUBCATEGORY_ITEM, PAYLOAD_SUBCATEGORY_ITEM_map, 
    PrimaryHashIndex<KEY_SUBCATEGORY_ITEM, KEY_SUBCATEGORY_ITEM_mapkey0_idxfn>
  > AGG_SUBCATEGORY_ITEM_map;
  

  struct KEY_RAIN_WEATHER {
    long locn; long dateid; PAYLOAD_RAIN_WEATHER_map __av; KEY_RAIN_WEATHER* nxt; KEY_RAIN_WEATHER* prv;
    explicit KEY_RAIN_WEATHER() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_RAIN_WEATHER& modify(const long c0, const long c1) { locn = c0; dateid = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_RAIN_WEATHER_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const KEY_RAIN_WEATHER& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_RAIN_WEATHER& x, const KEY_RAIN_WEATHER& y) {
      return x.locn == y.locn && x.dateid == y.dateid;
    }
  };
  
  typedef MultiHashMap<KEY_RAIN_WEATHER, PAYLOAD_RAIN_WEATHER_map, 
    PrimaryHashIndex<KEY_RAIN_WEATHER, KEY_RAIN_WEATHER_mapkey01_idxfn>
  > AGG_RAIN_WEATHER_map;

  
  struct KEY_ZIP_LOCATION_CENSUS {
    long locn; PAYLOAD_ZIP_LOCATION_CENSUS_map __av; KEY_ZIP_LOCATION_CENSUS* nxt; KEY_ZIP_LOCATION_CENSUS* prv;
    explicit KEY_ZIP_LOCATION_CENSUS() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_ZIP_LOCATION_CENSUS& modify(const long c0) { locn = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_ZIP_LOCATION_CENSUS_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const KEY_ZIP_LOCATION_CENSUS& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_ZIP_LOCATION_CENSUS& x, const KEY_ZIP_LOCATION_CENSUS& y) {
      return x.locn == y.locn;
    }
  };
  
  typedef MultiHashMap<KEY_ZIP_LOCATION_CENSUS, PAYLOAD_ZIP_LOCATION_CENSUS_map, 
    PrimaryHashIndex<KEY_ZIP_LOCATION_CENSUS, KEY_ZIP_LOCATION_CENSUS_mapkey0_idxfn>
  > AGG_ZIP_LOCATION_CENSUS_map;
  

  struct KEY_KSN_INVENTORY_ITEM {
    long locn; long dateid; PAYLOAD_KSN_INVENTORY_ITEM_map __av; KEY_KSN_INVENTORY_ITEM* nxt; KEY_KSN_INVENTORY_ITEM* prv;
    explicit KEY_KSN_INVENTORY_ITEM() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_KSN_INVENTORY_ITEM& modify(const long c0, const long c1) { locn = c0; dateid = c1;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_KSN_INVENTORY_ITEM_mapkey01_idxfn {
    FORCE_INLINE static size_t hash(const KEY_KSN_INVENTORY_ITEM& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_KSN_INVENTORY_ITEM& x, const KEY_KSN_INVENTORY_ITEM& y) {
      return x.locn == y.locn && x.dateid == y.dateid;
    }
  };
  
  typedef MultiHashMap<KEY_KSN_INVENTORY_ITEM, PAYLOAD_KSN_INVENTORY_ITEM_map, 
    PrimaryHashIndex<KEY_KSN_INVENTORY_ITEM, KEY_KSN_INVENTORY_ITEM_mapkey01_idxfn>
  > AGG_KSN_INVENTORY_ITEM_map;

  
  struct KEY_DATEID_INVENTORY_ITEM_WEATHER {
    long locn; PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map __av; KEY_DATEID_INVENTORY_ITEM_WEATHER* nxt; KEY_DATEID_INVENTORY_ITEM_WEATHER* prv;
    explicit KEY_DATEID_INVENTORY_ITEM_WEATHER() : __av(2), nxt(nullptr), prv(nullptr) { }
    FORCE_INLINE KEY_DATEID_INVENTORY_ITEM_WEATHER& modify(const long c0) { locn = c0;  return *this; }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  
  struct KEY_DATEID_INVENTORY_ITEM_WEATHER_mapkey0_idxfn {
    FORCE_INLINE static size_t hash(const KEY_DATEID_INVENTORY_ITEM_WEATHER& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      return h;
    }
    FORCE_INLINE static bool equals(const KEY_DATEID_INVENTORY_ITEM_WEATHER& x, const KEY_DATEID_INVENTORY_ITEM_WEATHER& y) {
      return x.locn == y.locn;
    }
  };
  
  typedef MultiHashMap<KEY_DATEID_INVENTORY_ITEM_WEATHER, PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map, 
    PrimaryHashIndex<KEY_DATEID_INVENTORY_ITEM_WEATHER, KEY_DATEID_INVENTORY_ITEM_WEATHER_mapkey0_idxfn>
  > AGG_DATEID_INVENTORY_ITEM_WEATHER_map;
  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0) , AGG(16U) { gettimeofday(&t0, NULL); }
  
    /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      // dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG), _AGG, "\t");
      ar << AGG.count();
    }
    
  protected:
    /* Data structures used for storing / computing top level queries */
    PAYLOAD_LOCN_map AGG;
  
  };
  
  /* Type definition providing a way to incrementally maintain the results of the sql program */
  struct data_t : tlq_t{
    data_t(): tlq_t() {
      
    }
  
  
    /* Trigger functions for table relations */
    void on_insert_LOCATION(const long locn, const long zip, const DOUBLE_TYPE rgn_cd, const DOUBLE_TYPE clim_zn_nbr, const DOUBLE_TYPE tot_area_sq_ft, const DOUBLE_TYPE sell_area_sq_ft, const DOUBLE_TYPE avghhi, const DOUBLE_TYPE supertargetdistance, const DOUBLE_TYPE supertargetdrivetime, const DOUBLE_TYPE targetdistance, const DOUBLE_TYPE targetdrivetime, const DOUBLE_TYPE walmartdistance, const DOUBLE_TYPE walmartdrivetime, const DOUBLE_TYPE walmartsupercenterdistance, const DOUBLE_TYPE walmartsupercenterdrivetime) {
      LOCATION_entry e(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime, 1L);
      LOCATION.addOrDelOnZero(e, 1L);
    }
    void on_insert_LOCATION(LOCATION_entry &e) {
      e.__av = 1L;
      LOCATION.addOrDelOnZero(e, 1L);
    }
    void on_insert_CENSUS(const long zip, const long population, const long white, const long asian, const long pacific, const long blackafrican, const DOUBLE_TYPE medianage, const long occupiedhouseunits, const long houseunits, const long families, const long households, const long husbwife, const long males, const long females, const long householdschildren, const long hispanic) {
      CENSUS_entry e(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic, 1L);
      CENSUS.addOrDelOnZero(e, 1L);
    }
    void on_insert_CENSUS(CENSUS_entry &e) {
      e.__av = 1L;
      CENSUS.addOrDelOnZero(e, 1L);
    }
    void on_insert_ITEM(const DOUBLE_TYPE ksn, const long subcategory, const long category, const long categorycluster, const DOUBLE_TYPE prize) {
      ITEM_entry e(ksn, subcategory, category, categorycluster, prize, 1L);
      ITEM.addOrDelOnZero(e, 1L);
    }
    void on_insert_ITEM(ITEM_entry &e) {
      e.__av = 1L;
      ITEM.addOrDelOnZero(e, 1L);
    }
    void on_insert_WEATHER(const long locn, const long dateid, const long rain, const long snow, const long maxtemp, const long mintemp, const DOUBLE_TYPE meanwind, const long thunder) {
      WEATHER_entry e(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder, 1L);
      WEATHER.addOrDelOnZero(e, 1L);
    }
    void on_insert_WEATHER(WEATHER_entry &e) {
      e.__av = 1L;
      WEATHER.addOrDelOnZero(e, 1L);
    }
    
    
    /* Trigger functions for stream relations */
    void on_batch_update_INVENTORY(const std::vector<INVENTORY_entry>::iterator &begin, const std::vector<INVENTORY_entry>::iterator &end) {
      tN += std::distance(begin, end);
      DELTA_AGG_Inventory.clear();
      { //foreach
        for (std::vector<DELTA_INVENTORY_entry>::iterator e1 = begin; e1 != end; e1++) {
          // Create key
          key_inventoryunits_inventory.locn = e1->locn;
          key_inventoryunits_inventory.dateid = e1->dateid;
          key_inventoryunits_inventory.ksn = e1->ksn;

          // Create payload
          payload_inventoryunits_inventory.inventoryunits = e1->inventoryunits;
          payload_inventoryunits_inventory.__av = e1->__av;
          payload_inventoryunits_inventory_map.clear();
          payload_inventoryunits_inventory_map.insert(payload_inventoryunits_inventory);

          DELTA_AGG_Inventory.addOrDelOnZero(key_inventoryunits_inventory, payload_inventoryunits_inventory_map);
        }
      }

      DELTA_AGG_InventoryItem.clear();
      { //foreach
        KEY_INVENTORYUNITS_INVENTORY* e2 = DELTA_AGG_Inventory.head;
        while (e2) {
          long locn = e2->locn;
          long dateid = e2->dateid;
          DOUBLE_TYPE ksn = e2->ksn;
          const PAYLOAD_INVENTORYUNITS_INVENTORY_map &pInventory = e2->__av;
          const PAYLOAD_SUBCATEGORY_ITEM_map & pItem = AGG_Item.getValueOrDefault(key_subcategory_item.modify(ksn));

          if (pItem.count() > 0) {

            key_ksn_inventory_item.locn = locn;
            key_ksn_inventory_item.dateid = dateid;

            payload_ksn_inventory_item.ksn = ksn;

            PAYLOAD_INVENTORYUNITS_INVENTORY* p1 = pInventory.head;
            while (p1) {
              payload_ksn_inventory_item.inventoryunits = p1->inventoryunits;
              payload_ksn_inventory_item.__av = p1->__av;

              PAYLOAD_SUBCATEGORY_ITEM* p2 = pItem.head;
              while (p2) {
                payload_ksn_inventory_item.subcategory = p2->subcategory;
                payload_ksn_inventory_item.category = p2->category;
                payload_ksn_inventory_item.categorycluster = p2->categorycluster;
                payload_ksn_inventory_item.prize = p2->prize;
                payload_ksn_inventory_item.__av *= p2->__av;

                payload_ksn_inventory_item_map.clear();
                payload_ksn_inventory_item_map.insert(payload_ksn_inventory_item);

                DELTA_AGG_InventoryItem.addOrDelOnZero(key_ksn_inventory_item, payload_ksn_inventory_item_map);

                p2 = p2->nxt;
              }
              p1 = p1->nxt;
            }
          }
          e2 = e2->nxt;
        }
      }

      DELTA_AGG_InventoryItemWeather.clear();
      { //foreach
        KEY_KSN_INVENTORY_ITEM* e3 = DELTA_AGG_InventoryItem.head;
        while (e3) {
          long locn = e3->locn;
          long dateid = e3->dateid;
          const PAYLOAD_KSN_INVENTORY_ITEM_map &pInventoryItem = e3->__av;
          const PAYLOAD_RAIN_WEATHER_map &pWeather = AGG_Weather.getValueOrDefault(key_rain_weather.modify(locn, dateid));

          if (pWeather.count() > 0) {

            key_dateid_inventory_item_weather.locn = locn;

            payload_dateid_inventory_item_weather.dateid = dateid;

            PAYLOAD_KSN_INVENTORY_ITEM* p1 = pInventoryItem.head;
            while (p1) {
              payload_dateid_inventory_item_weather.ksn = p1->ksn;
              payload_dateid_inventory_item_weather.inventoryunits = p1->inventoryunits;
              payload_dateid_inventory_item_weather.subcategory = p1->subcategory;
              payload_dateid_inventory_item_weather.category = p1->category;
              payload_dateid_inventory_item_weather.categorycluster = p1->categorycluster;
              payload_dateid_inventory_item_weather.prize = p1->prize;
              payload_dateid_inventory_item_weather.__av = p1->__av;

              PAYLOAD_RAIN_WEATHER* p2 = pWeather.head;
              while (p2) {
                payload_dateid_inventory_item_weather.rain = p2->rain;
                payload_dateid_inventory_item_weather.snow = p2->snow;
                payload_dateid_inventory_item_weather.maxtemp = p2->maxtemp;
                payload_dateid_inventory_item_weather.mintemp = p2->mintemp;
                payload_dateid_inventory_item_weather.meanwind = p2->meanwind;
                payload_dateid_inventory_item_weather.thunder = p2->thunder;
                payload_dateid_inventory_item_weather.__av *= p2->__av;

                payload_dateid_inventory_item_weather_map.clear();
                payload_dateid_inventory_item_weather_map.insert(payload_dateid_inventory_item_weather);

                DELTA_AGG_InventoryItemWeather.addOrDelOnZero(key_dateid_inventory_item_weather, payload_dateid_inventory_item_weather_map);

                p2 = p2->nxt;
              }
              p1 = p1->nxt;
            }
          }
          e3 = e3->nxt;
        }
      }

      {//foreach
        KEY_DATEID_INVENTORY_ITEM_WEATHER* e4 = DELTA_AGG_InventoryItemWeather.head;
        while (e4) {
          long locn = e4->locn;
          const PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map &pInventoryItemWeather = e4->__av;
          const PAYLOAD_ZIP_LOCATION_CENSUS_map &pLocationCensus = AGG_LocationCensus.getValueOrDefault(key_zip_location_census.modify(locn));

          if (pLocationCensus.count() > 0) {

            payload_locn.locn = locn;

            PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER* p1 = pInventoryItemWeather.head;
            while (p1) {
              payload_locn.dateid = p1->dateid;
              payload_locn.ksn = p1->ksn;
              payload_locn.inventoryunits = p1->inventoryunits;
              payload_locn.subcategory = p1->subcategory;
              payload_locn.category = p1->category;
              payload_locn.categorycluster = p1->categorycluster;
              payload_locn.prize = p1->prize;
              payload_locn.rain = p1->rain;
              payload_locn.snow = p1->snow;
              payload_locn.maxtemp = p1->maxtemp;
              payload_locn.mintemp = p1->mintemp;
              payload_locn.meanwind = p1->meanwind;
              payload_locn.thunder = p1->thunder;
              payload_locn.__av = p1->__av;

              PAYLOAD_ZIP_LOCATION_CENSUS* p2 = pLocationCensus.head;
              while (p2) {
                payload_locn.zip = p2->zip;
                payload_locn.rgn_cd = p2->rgn_cd;
                payload_locn.clim_zn_nbr = p2->clim_zn_nbr;
                payload_locn.tot_area_sq_ft = p2->tot_area_sq_ft;
                payload_locn.sell_area_sq_ft = p2->sell_area_sq_ft;
                payload_locn.avghhi = p2->avghhi;
                payload_locn.supertargetdistance = p2->supertargetdistance;
                payload_locn.supertargetdrivetime = p2->supertargetdrivetime;
                payload_locn.targetdistance = p2->targetdistance;
                payload_locn.targetdrivetime = p2->targetdrivetime;
                payload_locn.walmartdistance = p2->walmartdistance;
                payload_locn.walmartdrivetime = p2->walmartdrivetime;
                payload_locn.walmartsupercenterdistance = p2->walmartsupercenterdistance;
                payload_locn.walmartsupercenterdrivetime = p2->walmartsupercenterdrivetime; 
                payload_locn.population = p2->population;
                payload_locn.white = p2->white;
                payload_locn.asian = p2->asian;
                payload_locn.pacific = p2->pacific;
                payload_locn.blackafrican = p2->blackafrican;
                payload_locn.medianage = p2->medianage;
                payload_locn.occupiedhouseunits = p2->occupiedhouseunits;
                payload_locn.houseunits = p2->houseunits;
                payload_locn.families = p2->families;
                payload_locn.households = p2->households;
                payload_locn.husbwife = p2->husbwife;
                payload_locn.males = p2->males;
                payload_locn.females = p2->females;
                payload_locn.householdschildren = p2->householdschildren;
                payload_locn.hispanic = p2->hispanic;
                payload_locn.__av *= p2->__av;

                AGG.addOrDelOnZero(payload_locn, payload_locn.__av);

                p2 = p2->nxt;
              }
              p1 = p1->nxt;
            }
          }
          e4 = e4->nxt;
        }        
      }
    }   
        
    void on_system_ready_event() {

      AGG_Location.clear();
      {
        LOCATION_entry* e8 = LOCATION.head;
        while (e8) {
          // Create key
          key_rgncd_location.locn = e8->locn;
          key_rgncd_location.zip = e8->zip;

          // Create payload
          payload_rgncd_location.rgn_cd = e8->rgn_cd;
          payload_rgncd_location.clim_zn_nbr = e8->clim_zn_nbr;
          payload_rgncd_location.tot_area_sq_ft = e8->tot_area_sq_ft;
          payload_rgncd_location.sell_area_sq_ft = e8->sell_area_sq_ft;
          payload_rgncd_location.avghhi = e8->avghhi;
          payload_rgncd_location.supertargetdistance = e8->supertargetdistance;
          payload_rgncd_location.supertargetdrivetime = e8->supertargetdrivetime;
          payload_rgncd_location.targetdistance = e8->targetdistance;
          payload_rgncd_location.targetdrivetime = e8->targetdrivetime;
          payload_rgncd_location.walmartdistance = e8->walmartdistance;
          payload_rgncd_location.walmartdrivetime = e8->walmartdrivetime;
          payload_rgncd_location.walmartsupercenterdistance = e8->walmartsupercenterdistance;
          payload_rgncd_location.walmartsupercenterdrivetime = e8->walmartsupercenterdrivetime;
          payload_rgncd_location.__av = e8->__av;

          payload_rgncd_location_map.clear();
          payload_rgncd_location_map.insert(payload_rgncd_location);

          AGG_Location.addOrDelOnZero(key_rgncd_location, payload_rgncd_location_map);

          e8 = e8->nxt;
        }
      }

      AGG_Census.clear();
      {
        CENSUS_entry* e13 = CENSUS.head;

        while (e13) {
          // Create key
          key_population_census.zip = e13->zip;

          // Create payload
          payload_population_census.population = e13->population;
          payload_population_census.white = e13->white;
          payload_population_census.asian = e13->asian;
          payload_population_census.pacific = e13->pacific;
          payload_population_census.blackafrican = e13->blackafrican;
          payload_population_census.medianage = e13->medianage;
          payload_population_census.occupiedhouseunits = e13->occupiedhouseunits;
          payload_population_census.houseunits = e13->houseunits;
          payload_population_census.families = e13->families;
          payload_population_census.households = e13->households;
          payload_population_census.husbwife = e13->husbwife;
          payload_population_census.males = e13->males;
          payload_population_census.females = e13->females;
          payload_population_census.householdschildren = e13->householdschildren;
          payload_population_census.hispanic = e13->hispanic;
          payload_population_census.__av = e13->__av;

          payload_population_census_map.clear();
          payload_population_census_map.insert(payload_population_census);

          AGG_Census.addOrDelOnZero(key_population_census, payload_population_census_map);

          e13 = e13->nxt;
        }
      }

      AGG_Item.clear();
      {
        ITEM_entry* e19 = ITEM.head;

        while (e19) {
          // Create key
          key_subcategory_item.ksn = e19->ksn;

          // Create payload
          payload_subcategory_item.subcategory = e19->subcategory;
          payload_subcategory_item.category = e19->category;
          payload_subcategory_item.categorycluster = e19->categorycluster;
          payload_subcategory_item.prize = e19->prize;
          payload_subcategory_item.__av = e19->__av;

          payload_subcategory_item_map.clear();
          payload_subcategory_item_map.insert(payload_subcategory_item);

          AGG_Item.addOrDelOnZero(key_subcategory_item, payload_subcategory_item_map);

          e19 = e19->nxt;
        }
      }

      AGG_Weather.clear();
      {
        WEATHER_entry* e27 = WEATHER.head;

        while (e27) {
          // Create key
          key_rain_weather.locn = e27->locn;
          key_rain_weather.dateid = e27->dateid;

          // Create payload
          payload_rain_weather.rain = e27->rain;
          payload_rain_weather.snow = e27->snow;
          payload_rain_weather.maxtemp = e27->maxtemp;
          payload_rain_weather.mintemp = e27->mintemp;
          payload_rain_weather.meanwind = e27->meanwind;
          payload_rain_weather.thunder = e27->thunder;
          payload_rain_weather.__av = e27->__av;

          payload_rain_weather_map.clear();
          payload_rain_weather_map.insert(payload_rain_weather);          

          AGG_Weather.addOrDelOnZero(key_rain_weather, payload_rain_weather_map);

          e27 = e27->nxt;
        }
      }

      AGG_LocationCensus.clear();
      { //foreach
        KEY_RGNCD_LOCATION* e9 = AGG_Location.head;
        while (e9) {
          long locn = e9->locn;
          long zip = e9->zip;
          const PAYLOAD_RGNCD_LOCATION_map &pLocation = e9->__av;
          const PAYLOAD_POPULATION_CENSUS_map &pCensus = AGG_Census.getValueOrDefault(key_population_census.modify(zip));

          if (pCensus.count() > 0) {

            key_zip_location_census.locn = locn;
            
            payload_zip_location_census.zip = zip;

            PAYLOAD_RGNCD_LOCATION* p1 = pLocation.head;
            while (p1) {
              payload_zip_location_census.rgn_cd = p1->rgn_cd;
              payload_zip_location_census.clim_zn_nbr = p1->clim_zn_nbr;
              payload_zip_location_census.tot_area_sq_ft = p1->tot_area_sq_ft;
              payload_zip_location_census.sell_area_sq_ft = p1->sell_area_sq_ft;
              payload_zip_location_census.avghhi = p1->avghhi;
              payload_zip_location_census.supertargetdistance = p1->supertargetdistance;
              payload_zip_location_census.supertargetdrivetime = p1->supertargetdrivetime;
              payload_zip_location_census.targetdistance = p1->targetdistance;
              payload_zip_location_census.targetdrivetime = p1->targetdrivetime;
              payload_zip_location_census.walmartdistance = p1->walmartdistance;
              payload_zip_location_census.walmartdrivetime = p1->walmartdrivetime;
              payload_zip_location_census.walmartsupercenterdistance = p1->walmartsupercenterdistance;
              payload_zip_location_census.walmartsupercenterdrivetime = p1->walmartsupercenterdrivetime;
              payload_zip_location_census.__av = p1->__av;

              PAYLOAD_POPULATION_CENSUS* p2 = pCensus.head;
              while (p2) {
                payload_zip_location_census.population = p2->population;
                payload_zip_location_census.white = p2->white;
                payload_zip_location_census.asian = p2->asian;
                payload_zip_location_census.pacific = p2->pacific;
                payload_zip_location_census.blackafrican = p2->blackafrican;
                payload_zip_location_census.medianage = p2->medianage;
                payload_zip_location_census.occupiedhouseunits = p2->occupiedhouseunits;
                payload_zip_location_census.houseunits = p2->houseunits;
                payload_zip_location_census.families = p2->families;
                payload_zip_location_census.households = p2->households;
                payload_zip_location_census.husbwife = p2->husbwife;
                payload_zip_location_census.males = p2->males;
                payload_zip_location_census.females = p2->females;
                payload_zip_location_census.householdschildren = p2->householdschildren;
                payload_zip_location_census.hispanic = p2->hispanic;
                payload_zip_location_census.__av *= p2->__av;

                payload_zip_location_census_map.clear();
                payload_zip_location_census_map.insert(payload_zip_location_census);

                AGG_LocationCensus.addOrDelOnZero(key_zip_location_census, payload_zip_location_census_map);

                p2 = p2->nxt;
              }
              p1 = p1->nxt;
            }
          }
          e9 = e9->nxt;
        }
      }
    }
  
  private: 
    LOCATION_map LOCATION;
    CENSUS_map CENSUS;
    ITEM_map ITEM;
    WEATHER_map WEATHER;

    /* Sample entries for avoiding recreation of temporary objects */
    KEY_INVENTORYUNITS_INVENTORY key_inventoryunits_inventory;
    PAYLOAD_INVENTORYUNITS_INVENTORY payload_inventoryunits_inventory;
    PAYLOAD_INVENTORYUNITS_INVENTORY_map payload_inventoryunits_inventory_map = PAYLOAD_INVENTORYUNITS_INVENTORY_map(2);

    KEY_SUBCATEGORY_ITEM key_subcategory_item;
    PAYLOAD_SUBCATEGORY_ITEM payload_subcategory_item;
    PAYLOAD_SUBCATEGORY_ITEM_map payload_subcategory_item_map = PAYLOAD_SUBCATEGORY_ITEM_map(2);

    KEY_RAIN_WEATHER key_rain_weather;
    PAYLOAD_RAIN_WEATHER payload_rain_weather;
    PAYLOAD_RAIN_WEATHER_map payload_rain_weather_map = PAYLOAD_RAIN_WEATHER_map(2);

    KEY_RGNCD_LOCATION key_rgncd_location;
    PAYLOAD_RGNCD_LOCATION payload_rgncd_location;
    PAYLOAD_RGNCD_LOCATION_map payload_rgncd_location_map = PAYLOAD_RGNCD_LOCATION_map(2);

    KEY_POPULATION_CENSUS key_population_census;
    PAYLOAD_POPULATION_CENSUS payload_population_census;
    PAYLOAD_POPULATION_CENSUS_map payload_population_census_map = PAYLOAD_POPULATION_CENSUS_map(2);

    KEY_ZIP_LOCATION_CENSUS key_zip_location_census;
    PAYLOAD_ZIP_LOCATION_CENSUS payload_zip_location_census;
    PAYLOAD_ZIP_LOCATION_CENSUS_map payload_zip_location_census_map = PAYLOAD_ZIP_LOCATION_CENSUS_map(2);
  
    KEY_KSN_INVENTORY_ITEM key_ksn_inventory_item;
    PAYLOAD_KSN_INVENTORY_ITEM payload_ksn_inventory_item;
    PAYLOAD_KSN_INVENTORY_ITEM_map payload_ksn_inventory_item_map = PAYLOAD_KSN_INVENTORY_ITEM_map(2);

    KEY_DATEID_INVENTORY_ITEM_WEATHER key_dateid_inventory_item_weather;
    PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER payload_dateid_inventory_item_weather;
    PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map payload_dateid_inventory_item_weather_map = PAYLOAD_DATEID_INVENTORY_ITEM_WEATHER_map(2);

    /* Data structures used for storing materialized views */

    AGG_SUBCATEGORY_ITEM_map AGG_Item;
    AGG_RAIN_WEATHER_map AGG_Weather;
    AGG_RGNCD_LOCATION_map AGG_Location;
    AGG_POPULATION_CENSUS_map AGG_Census;
    AGG_ZIP_LOCATION_CENSUS_map AGG_LocationCensus;

    AGG_INVENTORYUNITS_INVENTORY_map DELTA_AGG_Inventory;
    AGG_KSN_INVENTORY_ITEM_map DELTA_AGG_InventoryItem;
    AGG_DATEID_INVENTORY_ITEM_WEATHER_map DELTA_AGG_InventoryItemWeather;

    PAYLOAD_LOCN payload_locn;
  
  };
}
        