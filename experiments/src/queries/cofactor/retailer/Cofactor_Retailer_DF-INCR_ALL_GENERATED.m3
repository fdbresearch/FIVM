---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (static, dynamic_min, dynamic_sum);

-------------------- SOURCES --------------------
CREATE STREAM INVENTORY (locn long, dateid long, ksn double, inventoryunits double)
  FROM FILE './datasets/sears/Inventory.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM LOCATION (locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
  FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM CENSUS (zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
  FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ITEM (ksn double, subcategory long, category long, categoryCluster long, prize double)
  FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM WEATHER (locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long)
  FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][] :=
  AggSum([],
    ((V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );

DECLARE MAP V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn: long] :=
  AggSum([locn],
    ((V_ksn_II1(RingCofactor<double, 2, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );

DECLARE MAP V_ksn_II1(RingCofactor<double, 2, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((V_inventoryunits_I1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
  );

DECLARE MAP V_inventoryunits_I1(RingCofactor<double, 3, 1>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits))
  );

DECLARE MAP V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn: double] :=
  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingCofactor<double, 4, 4>](subcategory, category, categoryCluster, prize))
  );

DECLARE MAP V_rain_W1(RingCofactor<double, 8, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingCofactor<double, 8, 6>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP V_zip_LC1(RingCofactor<double, 14, 29>)[][locn: long] :=
  AggSum([locn],
    ((V_rgn_cd_L1(RingCofactor<double, 15, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 28, 15>)[][zip]<Local>) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );

DECLARE MAP V_rgn_cd_L1(RingCofactor<double, 15, 13>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingCofactor<double, 15, 13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP V_population_C1(RingCofactor<double, 28, 15>)[][zip: long] :=
  AggSum([zip],
    (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingCofactor<double, 28, 15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

DECLARE MAP TMP_SUM1(RingCofactor<double, 3, 1>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits))
  );

DECLARE MAP TMP_SUM2(RingCofactor<double, 2, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
  );

DECLARE MAP TMP_SUM3(RingCofactor<double, 1, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM2(RingCofactor<double, 2, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );

DECLARE MAP TMP_SUM4(RingCofactor<double, 15, 13>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingCofactor<double, 15, 13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );

DECLARE MAP TMP_SUM5(RingCofactor<double, 14, 29>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM4(RingCofactor<double, 15, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 28, 15>)[][zip]<Local>) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );

DECLARE MAP TMP_SUM6(RingCofactor<double, 28, 15>)[][zip: long] :=
  AggSum([zip],
    ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingCofactor<double, 28, 15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );

DECLARE MAP TMP_SUM7(RingCofactor<double, 14, 29>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM6(RingCofactor<double, 28, 15>)[][zip]<Local> * V_rgn_cd_L1(RingCofactor<double, 15, 13>)[][locn, zip]<Local>) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );

DECLARE MAP TMP_SUM8(RingCofactor<double, 4, 4>)[][ksn: double] :=
  AggSum([ksn],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingCofactor<double, 4, 4>](subcategory, category, categoryCluster, prize))
  );

DECLARE MAP TMP_SUM9(RingCofactor<double, 2, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM8(RingCofactor<double, 4, 4>)[][ksn]<Local> * V_inventoryunits_I1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
  );

DECLARE MAP TMP_SUM10(RingCofactor<double, 1, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM9(RingCofactor<double, 2, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );

DECLARE MAP TMP_SUM11(RingCofactor<double, 8, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingCofactor<double, 8, 6>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP TMP_SUM12(RingCofactor<double, 1, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM11(RingCofactor<double, 8, 6>)[][locn, dateid]<Local> * V_ksn_II1(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  TMP_SUM1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local>  :=  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits))
  );
  TMP_SUM2(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
  );
  TMP_SUM3(RingCofactor<double, 1, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM2(RingCofactor<double, 2, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM3(RingCofactor<double, 1, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );
  V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local>  +=  TMP_SUM3(RingCofactor<double, 1, 13>)[][locn]<Local>;
  V_ksn_II1(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>  +=  TMP_SUM2(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>;
  V_inventoryunits_I1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local>  +=  TMP_SUM1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local>;
}

ON BATCH UPDATE OF LOCATION {
  TMP_SUM4(RingCofactor<double, 15, 13>)[][locn, zip]<Local>  :=  AggSum([locn, zip],
    ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingCofactor<double, 15, 13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
  );
  TMP_SUM5(RingCofactor<double, 14, 29>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM4(RingCofactor<double, 15, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 28, 15>)[][zip]<Local>) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM5(RingCofactor<double, 14, 29>)[][locn]<Local> * V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );
  V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>  +=  TMP_SUM5(RingCofactor<double, 14, 29>)[][locn]<Local>;
  V_rgn_cd_L1(RingCofactor<double, 15, 13>)[][locn, zip]<Local>  +=  TMP_SUM4(RingCofactor<double, 15, 13>)[][locn, zip]<Local>;
}

ON BATCH UPDATE OF CENSUS {
  TMP_SUM6(RingCofactor<double, 28, 15>)[][zip]<Local>  :=  AggSum([zip],
    ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingCofactor<double, 28, 15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
  );
  TMP_SUM7(RingCofactor<double, 14, 29>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM6(RingCofactor<double, 28, 15>)[][zip]<Local> * V_rgn_cd_L1(RingCofactor<double, 15, 13>)[][locn, zip]<Local>) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM7(RingCofactor<double, 14, 29>)[][locn]<Local> * V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );
  V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>  +=  TMP_SUM7(RingCofactor<double, 14, 29>)[][locn]<Local>;
  V_population_C1(RingCofactor<double, 28, 15>)[][zip]<Local>  +=  TMP_SUM6(RingCofactor<double, 28, 15>)[][zip]<Local>;
}

ON BATCH UPDATE OF ITEM {
  TMP_SUM8(RingCofactor<double, 4, 4>)[][ksn]<Local>  :=  AggSum([ksn],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingCofactor<double, 4, 4>](subcategory, category, categoryCluster, prize))
  );
  TMP_SUM9(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM8(RingCofactor<double, 4, 4>)[][ksn]<Local> * V_inventoryunits_I1(RingCofactor<double, 3, 1>)[][locn, dateid, ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
  );
  TMP_SUM10(RingCofactor<double, 1, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM9(RingCofactor<double, 2, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM10(RingCofactor<double, 1, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );
  V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local>  +=  TMP_SUM10(RingCofactor<double, 1, 13>)[][locn]<Local>;
  V_ksn_II1(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>  +=  TMP_SUM9(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>;
  V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>  +=  TMP_SUM8(RingCofactor<double, 4, 4>)[][ksn]<Local>;
}

ON BATCH UPDATE OF WEATHER {
  TMP_SUM11(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingCofactor<double, 8, 6>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  TMP_SUM12(RingCofactor<double, 1, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM11(RingCofactor<double, 8, 6>)[][locn, dateid]<Local> * V_ksn_II1(RingCofactor<double, 2, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM12(RingCofactor<double, 1, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
  );
  V_dateid_IIW1(RingCofactor<double, 1, 13>)[][locn]<Local>  +=  TMP_SUM12(RingCofactor<double, 1, 13>)[][locn]<Local>;
  V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>  +=  TMP_SUM11(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>;
}

ON SYSTEM READY {
  
}