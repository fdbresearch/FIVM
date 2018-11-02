---------------- TYPE DEFINITIONS ---------------
CREATE TYPE RingRelation
  FROM FILE 'ring_relation.hpp'
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
    ((V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );

DECLARE MAP V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );

DECLARE MAP V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((V_inventoryunits_I1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
  );

DECLARE MAP V_inventoryunits_I1(RingRelation<[3, double]>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift<3>: RingRelation<[3, double]>](3, inventoryunits))
  );

DECLARE MAP V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn: double] :=
  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingRelation<[4, long, long, long, double]>](4, subcategory, category, categoryCluster, prize))
  );

DECLARE MAP V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](8, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn: long] :=
  AggSum([locn],
    ((V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );

DECLARE MAP V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](15, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip: long] :=
  AggSum([zip],
    (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](28, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

DECLARE MAP TMP_SUM1(RingRelation<[3, double]>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift<3>: RingRelation<[3, double]>](3, inventoryunits))
  );

DECLARE MAP TMP_SUM2(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
  );

DECLARE MAP TMP_SUM3(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM2(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );

DECLARE MAP TMP_SUM4(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](15, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP TMP_SUM5(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM4(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );

DECLARE MAP TMP_SUM6(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip: long] :=
  AggSum([zip],
    ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](28, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

DECLARE MAP TMP_SUM7(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM6(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local> * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );

DECLARE MAP TMP_SUM8(RingRelation<[4, long, long, long, double]>)[][ksn: double] :=
  AggSum([ksn],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingRelation<[4, long, long, long, double]>](4, subcategory, category, categoryCluster, prize))
  );

DECLARE MAP TMP_SUM9(RingRelation<[2, double, double, long, long, long, double]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM8(RingRelation<[4, long, long, long, double]>)[][ksn]<Local> * V_inventoryunits_I1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
  );

DECLARE MAP TMP_SUM10(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM9(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );

DECLARE MAP TMP_SUM11(RingRelation<[8, long, long, long, long, double, long]>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](8, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP TMP_SUM12(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM11(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local> * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  TMP_SUM1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local>  :=  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift<3>: RingRelation<[3, double]>](3, inventoryunits))
  );
  TMP_SUM2(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
  );
  TMP_SUM3(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM2(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM3(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  TMP_SUM3(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>;
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  TMP_SUM2(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>;
  V_inventoryunits_I1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local>  +=  TMP_SUM1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local>;
}

ON BATCH UPDATE OF LOCATION {
  TMP_SUM4(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>  :=  AggSum([locn, zip],
    ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](15, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );
  TMP_SUM5(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM4(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local> * V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM5(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local> * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  TMP_SUM5(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>;
  V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>  +=  TMP_SUM4(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>;
}

ON BATCH UPDATE OF CENSUS {
  TMP_SUM6(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>  :=  AggSum([zip],
    ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](28, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );
  TMP_SUM7(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM6(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local> * V_rgn_cd_L1(RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>)[][locn, zip]<Local>) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM7(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local> * V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  +=  TMP_SUM7(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>;
  V_population_C1(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>  +=  TMP_SUM6(RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][zip]<Local>;
}

ON BATCH UPDATE OF ITEM {
  TMP_SUM8(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>  :=  AggSum([ksn],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingRelation<[4, long, long, long, double]>](4, subcategory, category, categoryCluster, prize))
  );
  TMP_SUM9(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM8(RingRelation<[4, long, long, long, double]>)[][ksn]<Local> * V_inventoryunits_I1(RingRelation<[3, double]>)[][locn, dateid, ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
  );
  TMP_SUM10(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM9(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local> * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM10(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  TMP_SUM10(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>;
  V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>  +=  TMP_SUM9(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>;
  V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>  +=  TMP_SUM8(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>;
}

ON BATCH UPDATE OF WEATHER {
  TMP_SUM11(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](8, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  TMP_SUM12(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM11(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local> * V_ksn_II1(RingRelation<[2, double, double, long, long, long, double]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
  );
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM12(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local> * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
  V_dateid_IIW1(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>  +=  TMP_SUM12(RingRelation<[1, long, double, double, long, long, long, double, long, long, long, long, double, long]>)[][locn]<Local>;
  V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  +=  TMP_SUM11(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>;
}

ON SYSTEM READY {
  
}