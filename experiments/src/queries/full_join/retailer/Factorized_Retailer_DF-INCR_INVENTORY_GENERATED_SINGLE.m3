---------------- TYPE DEFINITIONS ---------------
CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

-------------------- SOURCES --------------------
CREATE STREAM INVENTORY (locn long, dateid long, ksn double, inventoryunits double)
  FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE LOCATION (locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
  FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE CENSUS (zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
  FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE ITEM (ksn double, subcategory long, category long, categoryCluster long, prize double)
  FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE WEATHER (locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long)
  FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_locn_IIWLC1(RingFactorizedRelation<[0, long]>)[][] :=
  AggSum([],
    ((V_dateid_IIW1(RingFactorizedRelation<[1, long]>)[][locn]<Local> * V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn]<Local>) * [lift<0>: RingFactorizedRelation<[0, long]>](locn))
  );

DECLARE MAP V_dateid_IIW1(RingFactorizedRelation<[1, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_ksn_II1(RingFactorizedRelation<[2, double, double]>)[][locn, dateid]<Local> * V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingFactorizedRelation<[1, long]>](dateid))
  );

DECLARE MAP V_ksn_II1(RingFactorizedRelation<[2, double, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((INVENTORY(locn, dateid, ksn, inventoryunits) * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits))
  );

DECLARE MAP V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn: double] :=
  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingFactorizedRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize))
  );

DECLARE MAP V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingFactorizedRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_rgn_cd_L1(RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingFactorizedRelation<[14, long]>](zip))
  );

DECLARE MAP V_rgn_cd_L1(RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP V_population_C1(RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip: long] :=
  AggSum([zip],
    (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingFactorizedRelation<[0, long]>)[][]<Local>;

DECLARE QUERY V_dateid_IIW1 := V_dateid_IIW1(RingFactorizedRelation<[1, long]>)[][locn]<Local>;

DECLARE QUERY V_ksn_II1 := V_ksn_II1(RingFactorizedRelation<[2, double, double]>)[][locn, dateid]<Local>;

DECLARE QUERY V_subcategory_I1 := V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>;

DECLARE QUERY V_rain_W1 := V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>;

DECLARE QUERY V_zip_LC1 := V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn]<Local>;

DECLARE QUERY V_rgn_cd_L1 := V_rgn_cd_L1(RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>;

DECLARE QUERY V_population_C1 := V_population_C1(RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>;

------------------- TRIGGERS --------------------
ON + INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingFactorizedRelation<[0, long]>)[][]<Local>  +=  ((((((1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingFactorizedRelation<[1, long]>](dateid)) * V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn]<Local>) * [lift<0>: RingFactorizedRelation<[0, long]>](locn));
  V_dateid_IIW1(RingFactorizedRelation<[1, long]>)[][locn]<Local>  +=  ((((1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingFactorizedRelation<[1, long]>](dateid));
  V_ksn_II1(RingFactorizedRelation<[2, double, double]>)[][locn, dateid]<Local>  +=  ((1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits));
}

ON - INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingFactorizedRelation<[0, long]>)[][]<Local>  +=  ((((((-1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingFactorizedRelation<[1, long]>](dateid)) * V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn]<Local>) * [lift<0>: RingFactorizedRelation<[0, long]>](locn));
  V_dateid_IIW1(RingFactorizedRelation<[1, long]>)[][locn]<Local>  +=  ((((-1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingFactorizedRelation<[1, long]>](dateid));
  V_ksn_II1(RingFactorizedRelation<[2, double, double]>)[][locn, dateid]<Local>  +=  ((-1 * V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingFactorizedRelation<[2, double, double]>](ksn, inventoryunits));
}

ON SYSTEM READY {
  V_subcategory_I1(RingFactorizedRelation<[4, long, long, long, double]>)[][ksn]<Local>  :=  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingFactorizedRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize))
  );
  V_rain_W1(RingFactorizedRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingFactorizedRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  V_zip_LC1(RingFactorizedRelation<[14, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((V_rgn_cd_L1(RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingFactorizedRelation<[14, long]>](zip))
  );
  V_rgn_cd_L1(RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>  :=  AggSum([locn, zip],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingFactorizedRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );
  V_population_C1(RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>  :=  AggSum([zip],
    (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingFactorizedRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );
}