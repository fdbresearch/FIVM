#include <sys/time.h>
#include <vector>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "hash.hpp"
#include "mmap.hpp"
#include "serialization.hpp"
#include "../templates/cofactor_value.cpp"

#define RELATION_INVENTORY_DYNAMIC
#define RELATION_LOCATION_STATIC
#define RELATION_CENSUS_STATIC
#define RELATION_ITEM_STATIC
#define RELATION_WEATHER_STATIC

#define AGG_INVENTORY_SIZE 1
#define AGG_LOCATION_SIZE 13
#define AGG_CENSUS_SIZE 15
#define AGG_ITEM_SIZE 4
#define AGG_WEATHER_SIZE 6
#define AGG_LC_SIZE 29
#define AGG_INVITEM_SIZE 6
#define AGG_INVITEMWEAT_SIZE 13
#define AGG_SIZE 43

namespace dbtoaster {

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
  typedef INVENTORY_entry DELTA_INVENTORY_entry;
  
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

  
  // CUSTOM DATA STRUCTURES

  typedef AGG_value<AGG_LOCATION_SIZE> AGG_Location_value;
  struct AGG_Location_entry {
    long locn; 
    long zip; 
    AGG_Location_value __av;  
    AGG_Location_entry* nxt; 
    AGG_Location_entry* prv;

    explicit AGG_Location_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct AGG_Location_mapkey01234567891011121314_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Location_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.zip);
      return h;
    }

    FORCE_INLINE static bool equals(const AGG_Location_entry& x, const AGG_Location_entry& y) {
      return x.locn == y.locn && x.zip == y.zip;
    }
  };
  
  typedef MultiHashMap<AGG_Location_entry, AGG_Location_value,
    PrimaryHashIndex<AGG_Location_entry, AGG_Location_mapkey01234567891011121314_idxfn>
  > AGG_Location_map;


  typedef AGG_value<AGG_CENSUS_SIZE> AGG_Census_value;
  struct AGG_Census_entry {
    long zip; 
    AGG_Census_value __av;  
    AGG_Census_entry* nxt; 
    AGG_Census_entry* prv;

    explicit AGG_Census_entry() : nxt(nullptr), prv(nullptr) {  }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, zip);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };
  struct AGG_Census_mapkey0123456789101112131415_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Census_entry& e) {
      size_t h = 0;
      hash_combine(h, e.zip);
      return h;
    }

    FORCE_INLINE static bool equals(const AGG_Census_entry& x, const AGG_Census_entry& y) {
      return x.zip == y.zip;
    }
  };
  
  typedef MultiHashMap<AGG_Census_entry, AGG_Census_value,
    PrimaryHashIndex<AGG_Census_entry, AGG_Census_mapkey0123456789101112131415_idxfn>
  > AGG_Census_map;


  typedef AGG_value<AGG_ITEM_SIZE> AGG_Item_value;
  struct AGG_Item_entry {
    DOUBLE_TYPE ksn;     
    AGG_Item_value __av;  
    AGG_Item_entry* nxt; 
    AGG_Item_entry* prv;

    explicit AGG_Item_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, ksn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct AGG_Item_mapkey01234_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Item_entry& e) {
      size_t h = 0;
      hash_combine(h, e.ksn);
      return h;
    }

    FORCE_INLINE static bool equals(const AGG_Item_entry& x, const AGG_Item_entry& y) {
      return x.ksn == y.ksn;
    }
  };
   
  typedef MultiHashMap<AGG_Item_entry, AGG_Item_value,
    PrimaryHashIndex<AGG_Item_entry, AGG_Item_mapkey01234_idxfn>
  > AGG_Item_map;
  

  typedef AGG_value<AGG_WEATHER_SIZE> AGG_Weather_value;
  struct AGG_Weather_entry {
    long locn; 
    long dateid; 
    AGG_Weather_value __av;  
    AGG_Weather_entry* nxt; 
    AGG_Weather_entry* prv;

    explicit AGG_Weather_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct AGG_Weather_mapkey01234567_idxfn {
    FORCE_INLINE static size_t hash(const AGG_Weather_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      return h;
    }

    FORCE_INLINE static bool equals(const AGG_Weather_entry& x, const AGG_Weather_entry& y) {
      return x.locn == y.locn && x.dateid == y.dateid;
    }
  };
  
  typedef MultiHashMap<AGG_Weather_entry, AGG_Weather_value,
    PrimaryHashIndex<AGG_Weather_entry, AGG_Weather_mapkey01234567_idxfn>
  > AGG_Weather_map;


  typedef AGG_value<AGG_LC_SIZE> AGG_LC_value;
  struct AGG_LC_entry {
    long locn; 
    AGG_LC_value __av;  
    AGG_LC_entry* nxt; 
    AGG_LC_entry* prv;

    explicit AGG_LC_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct AGG_LC_mapkey01234567891011121314151617181920212223242526272829_idxfn {
    FORCE_INLINE static size_t hash(const AGG_LC_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      return h;
    }

    FORCE_INLINE static bool equals(const AGG_LC_entry& x, const AGG_LC_entry& y) {
      return x.locn == y.locn;
    }
  };
   
  typedef MultiHashMap<AGG_LC_entry, AGG_LC_value,
    PrimaryHashIndex<AGG_LC_entry, AGG_LC_mapkey01234567891011121314151617181920212223242526272829_idxfn>
  > AGG_LC_map;


  /* Definitions of auxiliary maps for storing materialized views. */
 

  // CUSTOM VECTOR-BASED DATA STRUCTURES

  typedef AGG_value<AGG_INVENTORY_SIZE> DELTA_AGG_Inventory_value;
  struct DELTA_AGG_Inventory_entry {
    long locn; 
    long dateid; 
    DOUBLE_TYPE ksn; 
    DELTA_AGG_Inventory_value __av;
    DELTA_AGG_Inventory_entry* nxt; 
    DELTA_AGG_Inventory_entry* prv;

    explicit DELTA_AGG_Inventory_entry() : nxt(nullptr), prv(nullptr) { }
    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
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

  struct DELTA_AGG_Inventory_mapkey0123_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_Inventory_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      hash_combine(h, e.ksn);
      return h;
    }

    FORCE_INLINE static bool equals(const DELTA_AGG_Inventory_entry& x, const DELTA_AGG_Inventory_entry& y) {
      return x.locn == y.locn && x.dateid == y.dateid && x.ksn == y.ksn;
    }
  };

  typedef MultiHashMap<DELTA_AGG_Inventory_entry, DELTA_AGG_Inventory_value,
    PrimaryHashIndex<DELTA_AGG_Inventory_entry, DELTA_AGG_Inventory_mapkey0123_idxfn>
  > DELTA_AGG_Inventory_map;


  typedef AGG_value<AGG_INVITEM_SIZE> DELTA_AGG_InvItem_value;
  struct DELTA_AGG_InvItem_entry {
    long locn; 
    long dateid;
    DELTA_AGG_InvItem_value __av;  
    DELTA_AGG_InvItem_entry* nxt; 
    DELTA_AGG_InvItem_entry* prv;

    explicit DELTA_AGG_InvItem_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, dateid);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct DELTA_AGG_InvItem_mapkey01234567891011121314151617181920212223242526272829_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_InvItem_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      hash_combine(h, e.dateid);
      return h;
    }

    FORCE_INLINE static bool equals(const DELTA_AGG_InvItem_entry& x, const DELTA_AGG_InvItem_entry& y) {
      return x.locn == y.locn && x.dateid == y.dateid;
    }
  };
   
  typedef MultiHashMap<DELTA_AGG_InvItem_entry, DELTA_AGG_InvItem_value,
    PrimaryHashIndex<DELTA_AGG_InvItem_entry, DELTA_AGG_InvItem_mapkey01234567891011121314151617181920212223242526272829_idxfn>
  > DELTA_AGG_InvItem_map;


  typedef AGG_value<AGG_INVITEMWEAT_SIZE> DELTA_AGG_InvItemWeat_value;
  struct DELTA_AGG_InvItemWeat_entry {
    long locn; 
    DELTA_AGG_InvItemWeat_value __av;  
    DELTA_AGG_InvItemWeat_entry* nxt; 
    DELTA_AGG_InvItemWeat_entry* prv;

    explicit DELTA_AGG_InvItemWeat_entry() : nxt(nullptr), prv(nullptr) { }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const 
    {
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, locn);
      ar << ELEM_SEPARATOR;
      DBT_SERIALIZATION_NVP(ar, __av);
    }
  };

  struct DELTA_AGG_InvItemWeat_mapkey01234567891011121314151617181920212223242526272829_idxfn {
    FORCE_INLINE static size_t hash(const DELTA_AGG_InvItemWeat_entry& e) {
      size_t h = 0;
      hash_combine(h, e.locn);
      return h;
    }

    FORCE_INLINE static bool equals(const DELTA_AGG_InvItemWeat_entry& x, const DELTA_AGG_InvItemWeat_entry& y) {
      return x.locn == y.locn;
    }
  };
   
  typedef MultiHashMap<DELTA_AGG_InvItemWeat_entry, DELTA_AGG_InvItemWeat_value,
    PrimaryHashIndex<DELTA_AGG_InvItemWeat_entry, DELTA_AGG_InvItemWeat_mapkey01234567891011121314151617181920212223242526272829_idxfn>
  > DELTA_AGG_InvItemWeat_map;

  typedef AGG_value<AGG_SIZE> AGG_Output_value;  
  
  /* Type definition providing a way to access the results of the sql program */
  struct tlq_t {
    struct timeval t0, t; long tT, tN, tS;
    tlq_t(): tN(0), tS(0)  { gettimeofday(&t0, NULL); }
  
    /* Serialization Code */
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
      ar << "\n";
      dbtoaster::serialize_nvp_tabbed(ar, STRING(AGG_Output), AGG_Output, "\t"); 
    }
    
  protected:
    /* Data structures used for storing / computing top level queries */
    AGG_Output_value AGG_Output;
  
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
        for (std::vector<INVENTORY_entry>::iterator e1 = begin; e1 != end; e1++) {
          long locn = e1->locn;
          long dateid = e1->dateid;
          DOUBLE_TYPE ksn = e1->ksn;
          DOUBLE_TYPE inventoryunits = e1->inventoryunits;
          long count = e1->__av;

          se1.locn = locn;
          se1.dateid = dateid;
          se1.ksn = ksn;

          sv1.count = count;
          sv1.sums[0] = count * inventoryunits;
          sv1.cofactors[0] = count * inventoryunits * inventoryunits;

          DELTA_AGG_Inventory.addOrDelOnZero(se1, sv1);
        }
      }

      DELTA_AGG_InvItem.clear();
      { //foreach
        DELTA_AGG_Inventory_entry* e20 = DELTA_AGG_Inventory.head;
        while(e20){
          long locn = e20->locn;
          long dateid = e20->dateid;
          long ksn = e20->ksn;            
          const DELTA_AGG_Inventory_value &vInventory = e20->__av;

          se4.ksn = ksn;
          const AGG_Item_value &vItem = AGG_Item.getValueOrDefault(se4);

          long count1 = vInventory.count;
          long count2 = vItem.count;
          long count = count1 * count2;

          se7.locn = locn;
          se7.dateid = dateid;
          
          size_t indexOutput = 0;
          sv7.count = count;            
          sv7.sums[indexOutput++] = count * ksn;            
          for (size_t i = 0; i < AGG_INVENTORY_SIZE; i++) {
            sv7.sums[indexOutput++] = count2 * vInventory.sums[i];
          }
          for (size_t i = 0; i < AGG_ITEM_SIZE; i++) {
            sv7.sums[indexOutput++] = count1 * vItem.sums[i];
          }
         
          indexOutput = 0;
          sv7.cofactors[indexOutput++] = count * ksn * ksn;
          for (int i = 0; i < AGG_INVENTORY_SIZE; i++) {
            sv7.cofactors[indexOutput++] = count2 * ksn * vInventory.sums[i];
          }
          for (int i = 0; i < AGG_ITEM_SIZE; i++) {
            sv7.cofactors[indexOutput++] = count1 * ksn * vItem.sums[i];
          }

          size_t indexInput = 0;
          for (int i = 0; i < AGG_INVENTORY_SIZE; i++) {
            for (int j = i; j < AGG_INVENTORY_SIZE; j++) {
              sv7.cofactors[indexOutput++] = count2 * vInventory.cofactors[indexInput++];
            }

            for (int j = 0; j < AGG_ITEM_SIZE; j++) {
              sv7.cofactors[indexOutput++] = vInventory.sums[i] * vItem.sums[j];
            }
          }

          indexInput = 0;
          for (int i = 0; i < AGG_ITEM_SIZE; i++) {
            for (int j = i; j < AGG_ITEM_SIZE; j++) {
              sv7.cofactors[indexOutput++] = count1 * vItem.cofactors[indexInput++];
            }
          }

          DELTA_AGG_InvItem.addOrDelOnZero(se7, sv7);

          e20 = e20->nxt;            
        }
      }

      DELTA_AGG_InvItemWeat.clear();
      { //foreach
        DELTA_AGG_InvItem_entry* e20 = DELTA_AGG_InvItem.head;
        while(e20){
          long locn = e20->locn;
          long dateid = e20->dateid;
          const DELTA_AGG_InvItem_value &vInvItem = e20->__av;

          se5.locn = locn;
          se5.dateid = dateid;
          const AGG_Weather_value &vWeather = AGG_Weather.getValueOrDefault(se5);

          long count1 = vInvItem.count;
          long count2 = vWeather.count;
          long count = count1 * count2;

          se8.locn = locn;
          
          size_t indexOutput = 0;
          sv8.count = count;            
          sv8.sums[indexOutput++] = count * dateid;            
          for (size_t i = 0; i < AGG_INVITEM_SIZE; i++) {
            sv8.sums[indexOutput++] = count2 * vInvItem.sums[i];
          }
          for (size_t i = 0; i < AGG_WEATHER_SIZE; i++) {
            sv8.sums[indexOutput++] = count1 * vWeather.sums[i];
          }
         
          indexOutput = 0;
          sv8.cofactors[indexOutput++] = count * dateid * dateid;
          for (int i = 0; i < AGG_INVITEM_SIZE; i++) {
            sv8.cofactors[indexOutput++] = count2 * dateid * vInvItem.sums[i];
          }
          for (int i = 0; i < AGG_WEATHER_SIZE; i++) {
            sv8.cofactors[indexOutput++] = count1 * dateid * vWeather.sums[i];
          }

          size_t indexInput = 0;
          for (int i = 0; i < AGG_INVITEM_SIZE; i++) {
            for (int j = i; j < AGG_INVITEM_SIZE; j++) {
              sv8.cofactors[indexOutput++] = count2 * vInvItem.cofactors[indexInput++];
            }

            for (int j = 0; j < AGG_WEATHER_SIZE; j++) {
              sv8.cofactors[indexOutput++] = vInvItem.sums[i] * vWeather.sums[j];
            }
          }

          indexInput = 0;
          for (int i = 0; i < AGG_WEATHER_SIZE; i++) {
            for (int j = i; j < AGG_WEATHER_SIZE; j++) {
              sv8.cofactors[indexOutput++] = count1 * vWeather.cofactors[indexInput++];
            }
          }

          DELTA_AGG_InvItemWeat.addOrDelOnZero(se8, sv8);

          e20 = e20->nxt;            
        }
      }

      { //foreach
        DELTA_AGG_InvItemWeat_entry* e20 = DELTA_AGG_InvItemWeat.head;
        while(e20){
          long locn = e20->locn;
          const DELTA_AGG_InvItemWeat_value &vInvItemWeat = e20->__av;

          se6.locn = locn;
          const AGG_LC_value &vLC = AGG_LC.getValueOrDefault(se6);

          long count1 = vInvItemWeat.count;
          long count2 = vLC.count;
          long count = count1 * count2;
          
          AGG_Output.count += count;
          
          size_t indexOutput = 0;
          AGG_Output.cofactors[indexOutput++] += count * locn * locn;
          for (int i = 0; i < AGG_INVITEMWEAT_SIZE; i++) {
            AGG_Output.cofactors[indexOutput++] += count2 * locn * vInvItemWeat.sums[i];
          }
          for (int i = 0; i < AGG_LC_SIZE; i++) {
            AGG_Output.cofactors[indexOutput++] += count1 * locn * vLC.sums[i];
          }

          size_t indexInput = 0;
          for (int i = 0; i < AGG_INVITEMWEAT_SIZE; i++) {
            for (int j = i; j < AGG_INVITEMWEAT_SIZE; j++) {
              AGG_Output.cofactors[indexOutput++] += count2 * vInvItemWeat.cofactors[indexInput++];
            }

            for (int j = 0; j < AGG_LC_SIZE; j++) {
              AGG_Output.cofactors[indexOutput++] += vInvItemWeat.sums[i] * vLC.sums[j];
            }
          }

          indexInput = 0;
          for (int i = 0; i < AGG_LC_SIZE; i++) {
            for (int j = i; j < AGG_LC_SIZE; j++) {
              AGG_Output.cofactors[indexOutput++] += count1 * vLC.cofactors[indexInput++];
            }
          }

          e20 = e20->nxt;
        }
      }
    }
    
    void on_system_ready_event() {

        AGG_Location.clear();
        { //foreach
          DOUBLE_TYPE fields[AGG_LOCATION_SIZE];
          LOCATION_entry* e3 = LOCATION.head;
          while(e3){
            long locn = e3->locn;
            long zip = e3->zip;
            DOUBLE_TYPE rgn_cd = e3->rgn_cd;
            DOUBLE_TYPE clim_zn_nbr = e3->clim_zn_nbr;
            DOUBLE_TYPE tot_area_sq_ft = e3->tot_area_sq_ft;
            DOUBLE_TYPE sell_area_sq_ft = e3->sell_area_sq_ft;
            DOUBLE_TYPE avghhi = e3->avghhi;
            DOUBLE_TYPE supertargetdistance = e3->supertargetdistance;
            DOUBLE_TYPE supertargetdrivetime = e3->supertargetdrivetime;
            DOUBLE_TYPE targetdistance = e3->targetdistance;
            DOUBLE_TYPE targetdrivetime = e3->targetdrivetime;
            DOUBLE_TYPE walmartdistance = e3->walmartdistance;
            DOUBLE_TYPE walmartdrivetime = e3->walmartdrivetime;
            DOUBLE_TYPE walmartsupercenterdistance = e3->walmartsupercenterdistance;
            DOUBLE_TYPE walmartsupercenterdrivetime = e3->walmartsupercenterdrivetime;
            long count = e3->__av;

            fields[0] = rgn_cd;
            fields[1] = clim_zn_nbr;
            fields[2] = tot_area_sq_ft;
            fields[3] = sell_area_sq_ft;
            fields[4] = avghhi;
            fields[5] = supertargetdistance;
            fields[6] = supertargetdrivetime;
            fields[7] = targetdistance;
            fields[8] = targetdrivetime;
            fields[9] = walmartdistance;
            fields[10] = walmartdrivetime;
            fields[11] = walmartsupercenterdistance;
            fields[12] = walmartsupercenterdrivetime;

            // KEY
            se2.locn = locn;
            se2.zip = zip;

            // VALUE
            sv2.count = count;
            size_t indexOutput = 0;
            for (size_t i = 0; i < AGG_LOCATION_SIZE; i++) {
              sv2.sums[indexOutput++] = count * fields[i];
            }
            indexOutput = 0;
            for (size_t i = 0; i < AGG_LOCATION_SIZE; i++) {
              for (size_t j = i; j < AGG_LOCATION_SIZE; j++) {
                sv2.cofactors[indexOutput++] = count * fields[i] * fields[j];;
              }
            }

            AGG_Location.addOrDelOnZero(se2, sv2);

            e3 = e3->nxt;
          }
        }

        AGG_Census.clear();
        { //foreach
          DOUBLE_TYPE fields[AGG_CENSUS_SIZE];
          CENSUS_entry* e5 = CENSUS.head;
          while(e5){
            long zip = e5->zip;
            long population = e5->population;
            long white = e5->white;
            long asian = e5->asian;
            long pacific = e5->pacific;
            long blackafrican = e5->blackafrican;
            DOUBLE_TYPE medianage = e5->medianage;
            long occupiedhouseunits = e5->occupiedhouseunits;
            long houseunits = e5->houseunits;
            long families = e5->families;
            long households = e5->households;
            long husbwife = e5->husbwife;
            long males = e5->males;
            long females = e5->females;
            long householdschildren = e5->householdschildren;
            long hispanic = e5->hispanic;
            long count = e5->__av;

            fields[0] = population;
            fields[1] = white;
            fields[2] = asian;
            fields[3] = pacific;
            fields[4] = blackafrican;
            fields[5] = medianage;
            fields[6] = occupiedhouseunits;
            fields[7] = houseunits;
            fields[8] = families;
            fields[9] = households;
            fields[10] = husbwife;
            fields[11] = males;
            fields[12] = females;
            fields[13] = householdschildren;
            fields[14] = hispanic;

            // KEY
            se3.zip = zip;

            // VALUE
            sv3.count = count;
            size_t indexOutput = 0;
            for (size_t i = 0; i < AGG_CENSUS_SIZE; i++) {
              sv3.sums[indexOutput++] = count * fields[i];
            }
            indexOutput = 0;
            for (size_t i = 0; i < AGG_CENSUS_SIZE; i++) {
              for (size_t j = i; j < AGG_CENSUS_SIZE; j++) {
                sv3.cofactors[indexOutput++] = count * fields[i] * fields[j];;
              }
            }

            AGG_Census.addOrDelOnZero(se3, sv3);
            
            e5 = e5->nxt;
          }
        }

        AGG_Item.clear();
        { //foreach
          DOUBLE_TYPE fields[AGG_ITEM_SIZE];          
          ITEM_entry* e7 = ITEM.head;
          while(e7){
            DOUBLE_TYPE ksn = e7->ksn;
            long subcategory = e7->subcategory;
            long category = e7->category;
            long categorycluster = e7->categorycluster;
            DOUBLE_TYPE prize = e7->prize;
            long count = e7->__av;

            fields[0] = subcategory;
            fields[1] = category;
            fields[2] = categorycluster;
            fields[3] = prize;

            // KEY
            se4.ksn = ksn;

            // VALUE
            sv4.count = count;
            size_t indexOutput = 0;
            for (size_t i = 0; i < AGG_ITEM_SIZE; i++) {
              sv4.sums[indexOutput++] = count * fields[i];
            }
            indexOutput = 0;
            for (size_t i = 0; i < AGG_ITEM_SIZE; i++) {
              for (size_t j = i; j < AGG_ITEM_SIZE; j++) {
                sv4.cofactors[indexOutput++] = count * fields[i] * fields[j];;
              }
            }

            AGG_Item.addOrDelOnZero(se4, sv4);

            e7 = e7->nxt;
          }
        }

        AGG_Weather.clear();
        { //foreach
          DOUBLE_TYPE fields[AGG_WEATHER_SIZE];
          WEATHER_entry* e9 = WEATHER.head;
          while(e9){
            long locn = e9->locn;
            long dateid = e9->dateid;
            long rain = e9->rain;
            long snow = e9->snow;
            long maxtemp = e9->maxtemp;
            long mintemp = e9->mintemp;
            DOUBLE_TYPE meanwind = e9->meanwind;
            long thunder = e9->thunder;
            long count = e9->__av;

            fields[0] = rain;
            fields[1] = snow;
            fields[2] = maxtemp;
            fields[3] = mintemp;
            fields[4] = meanwind;
            fields[5] = thunder;

            // KEY
            se5.locn = locn;
            se5.dateid = dateid;

            // VALUE
            sv5.count = count;
            size_t indexOutput = 0;
            for (size_t i = 0; i < AGG_WEATHER_SIZE; i++) {
              sv5.sums[indexOutput++] = count * fields[i];
            }
            indexOutput = 0;
            for (size_t i = 0; i < AGG_WEATHER_SIZE; i++) {
              for (size_t j = i; j < AGG_WEATHER_SIZE; j++) {
                sv5.cofactors[indexOutput++] = count * fields[i] * fields[j];;
              }
            }

            AGG_Weather.addOrDelOnZero(se5, sv5);
            
            e9 = e9->nxt;
          }
        }

        AGG_LC.clear();
        { //foreach
          AGG_Location_entry* e11 = AGG_Location.head;
          while(e11){
            long locn = e11->locn;
            long zip = e11->zip;
            
            const AGG_Location_value &vLocation = e11->__av;          
            se3.zip = zip;            
            const AGG_Census_value &vCensus = AGG_Census.getValueOrDefault(se3);

            long count1 = vLocation.count;
            long count2 = vCensus.count;
            long count = count1 * count2;

            se6.locn = locn;
            
            size_t indexOutput = 0;
            sv6.count = count;            
            sv6.sums[indexOutput++] = count * zip;            
            for (size_t i = 0; i < AGG_LOCATION_SIZE; i++) {
              sv6.sums[indexOutput++] = count2 * vLocation.sums[i];
            }
            for (size_t i = 0; i < AGG_CENSUS_SIZE; i++) {
              sv6.sums[indexOutput++] = count1 * vCensus.sums[i];
            }
           
            indexOutput = 0;
            sv6.cofactors[indexOutput++] = count * zip * zip;
            for (int i = 0; i < AGG_LOCATION_SIZE; i++) {
              sv6.cofactors[indexOutput++] = count2 * zip * vLocation.sums[i];
            }
            for (int i = 0; i < AGG_CENSUS_SIZE; i++) {
              sv6.cofactors[indexOutput++] = count1 * zip * vCensus.sums[i];
            }

            size_t indexInput = 0;
            for (int i = 0; i < AGG_LOCATION_SIZE; i++) {
              for (int j = i; j < AGG_LOCATION_SIZE; j++) {
                sv6.cofactors[indexOutput++] = count2 * vLocation.cofactors[indexInput++];
              }

              for (int j = 0; j < AGG_CENSUS_SIZE; j++) {
                sv6.cofactors[indexOutput++] = vLocation.sums[i] * vCensus.sums[j];
              }
            }

            indexInput = 0;
            for (int i = 0; i < AGG_CENSUS_SIZE; i++) {
              for (int j = i; j < AGG_CENSUS_SIZE; j++) {
                sv6.cofactors[indexOutput++] = count1 * vCensus.cofactors[indexInput++];
              }
            }

            AGG_LC.addOrDelOnZero(se6, sv6);

            e11 = e11->nxt;
          }
        }      
    }
  
  private:
  
    /* Sample entries for avoiding recreation of temporary objects */
    DELTA_AGG_Inventory_entry se1;
    AGG_Location_entry se2;
    AGG_Census_entry se3;
    AGG_Item_entry se4;
    AGG_Weather_entry se5;
    AGG_LC_entry se6;
    DELTA_AGG_InvItem_entry se7;
    DELTA_AGG_InvItemWeat_entry se8;

    DELTA_AGG_Inventory_value sv1;
    AGG_Location_value sv2;
    AGG_Census_value sv3;
    AGG_Item_value sv4;
    AGG_Weather_value sv5;
    AGG_LC_value sv6;
    DELTA_AGG_InvItem_value sv7;
    DELTA_AGG_InvItemWeat_value sv8;

    /* Data structures used for storing materialized views */
    LOCATION_map LOCATION;
    CENSUS_map CENSUS;
    ITEM_map ITEM;
    WEATHER_map WEATHER;
 
    AGG_Location_map AGG_Location;
    AGG_Census_map AGG_Census;
    AGG_Item_map AGG_Item;
    AGG_Weather_map AGG_Weather;
    AGG_LC_map AGG_LC;
    DELTA_AGG_Inventory_map DELTA_AGG_Inventory;
    DELTA_AGG_InvItem_map DELTA_AGG_InvItem;
    DELTA_AGG_InvItemWeat_map DELTA_AGG_InvItemWeat;
  };
}