---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE RingRelation
FROM FILE 'ring/ring_relational_opt.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

-------------------- SOURCES --------------------
CREATE STREAM INVENTORY (locn long, dateid long, ksn double, inventoryunits double)
  FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM LOCATION (locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
  FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM CENSUS (zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
  FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ITEM (ksn double, subcategory long, category long, categoryCluster long, prize double)
  FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM WEATHER (locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long)
  FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][] :=
  AggSum([],
    ((V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn))
  );

DECLARE MAP V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid))
  );

DECLARE MAP V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local> * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits))
  );

DECLARE MAP V_inventoryunits_I1(long)[][locn: long, dateid: long, ksn: double, inventoryunits: double] :=
  INVENTORY(locn, dateid, ksn, inventoryunits);

DECLARE MAP V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn: double] :=
  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize))
  );

DECLARE MAP V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip))
  );

DECLARE MAP V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip: long] :=
  AggSum([zip],
    (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

DECLARE MAP TMP_SUM1(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (((1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize)) * V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits))
  );

DECLARE MAP TMP_SUM2(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid))
  );

DECLARE MAP TMP_SUM3(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (((-1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize)) * V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits))
  );

DECLARE MAP TMP_SUM4(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM3(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>;

------------------- TRIGGERS --------------------
ON + INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  ((((((1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid)) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  ((((1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid));
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  ((1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits));
  V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>  +=  1;
}

ON - INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  ((((((-1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid)) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  ((((-1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits)) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid));
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  ((-1 * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits));
  V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>  +=  -1;
}

ON + LOCATION (locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  (((((1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip)) * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  (((1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip));
  V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>  +=  (1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime));
}

ON - LOCATION (locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  (((((-1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip)) * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  (((-1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip));
  V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>  +=  (-1 * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime));
}

ON + CENSUS (zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    (((((1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)) * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip)) * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn))
  );
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  (((1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)) * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip));
  V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>  +=  (1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic));
}

ON - CENSUS (zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    (((((-1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)) * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip)) * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn))
  );
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  (((-1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)) * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](zip));
  V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>  +=  (-1 * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic));
}

ON + ITEM (ksn, subcategory, category, categoryCluster, prize) {
  TMP_SUM1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (((1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize)) * V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits))
  );
  TMP_SUM2(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM2(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn))
  );
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  TMP_SUM2(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>;
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  TMP_SUM1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>;
  V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>  +=  (1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize));
}

ON - ITEM (ksn, subcategory, category, categoryCluster, prize) {
  TMP_SUM3(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (((-1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize)) * V_inventoryunits_I1(long)[][locn, dateid, ksn, inventoryunits]<Local>) * [lift<2>: RingRelation<[2, double, double]>](ksn, inventoryunits))
  );
  TMP_SUM4(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM3(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM4(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn))
  );
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  TMP_SUM4(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>;
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  TMP_SUM3(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>;
  V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>  +=  (-1 * [lift<4>: RingRelation<[4, long, long, long, double]>](subcategory, category, categoryCluster, prize));
}

ON + WEATHER (locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  (((((1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)) * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid)) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  (((1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)) * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid));
  V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  +=  (1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder));
}

ON - WEATHER (locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  (((((-1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)) * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid)) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](locn));
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  (((-1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)) * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](dateid));
  V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  +=  (-1 * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](rain, snow, maxtemp, mintemp, meanwind, thunder));
}

ON SYSTEM READY {
  
}