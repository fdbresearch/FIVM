---------------- TYPE DEFINITIONS ---------------


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
DECLARE MAP V_locn_IIWLC1(double)[][] :=
  AggSum([],
    (V_dateid_IIW1(double)[][locn]<Local> * V_zip_LC1(long)[][locn]<Local>)
  );

DECLARE MAP V_dateid_IIW1(double)[][locn: long] :=
  AggSum([locn],
    (V_ksn_II1(double)[][locn, dateid]<Local> * V_rain_W1(long)[][locn, dateid]<Local>)
  );

DECLARE MAP V_ksn_II1(double)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (V_inventoryunits_I1(double)[][locn, dateid, ksn]<Local> * V_subcategory_I1(long)[][ksn]<Local>)
  );

DECLARE MAP V_inventoryunits_I1(double)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    ((INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits) * inventoryunits)
  );

DECLARE MAP V_subcategory_I1(long)[][ksn: double] :=
  AggSum([ksn],
    ITEM(ksn, subcategory, category, categoryCluster, prize)
  );

DECLARE MAP V_rain_W1(long)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder)
  );

DECLARE MAP V_zip_LC1(long)[][locn: long] :=
  AggSum([locn],
    (V_rgn_cd_L1(long)[][locn, zip]<Local> * V_population_C1(long)[][zip]<Local>)
  );

DECLARE MAP V_rgn_cd_L1(long)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)
  );

DECLARE MAP V_population_C1(long)[][zip: long] :=
  AggSum([zip],
    CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
  );

DECLARE MAP TMP_SUM1(double)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    (((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * inventoryunits) * inventoryunits)
  );

DECLARE MAP TMP_SUM2(double)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (TMP_SUM1(double)[][locn, dateid, ksn]<Local> * V_subcategory_I1(long)[][ksn]<Local>)
  );

DECLARE MAP TMP_SUM3(double)[][locn: long] :=
  AggSum([locn],
    (TMP_SUM2(double)[][locn, dateid]<Local> * V_rain_W1(long)[][locn, dateid]<Local>)
  );

DECLARE MAP TMP_SUM4(long)[][walmartdistance: double, zip: long, targetdistance: double, sell_area_sq_ft: double, walmartdrivetime: double, supertargetdrivetime: double, clim_zn_nbr: double, avghhi: double, tot_area_sq_ft: double, supertargetdistance: double, targetdrivetime: double, locn: long, rgn_cd: double] :=
  AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
    (DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)
  );

DECLARE MAP TMP_SUM5(long)[][zip: long, targetdistance: double, sell_area_sq_ft: double, supertargetdrivetime: double, clim_zn_nbr: double, avghhi: double, tot_area_sq_ft: double, supertargetdistance: double, locn: long, rgn_cd: double] :=
  AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
    TMP_SUM4(long)[][walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd]<Local>
  );

DECLARE MAP TMP_SUM6(long)[][zip: long, clim_zn_nbr: double, tot_area_sq_ft: double, locn: long, rgn_cd: double] :=
  AggSum([zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
    TMP_SUM5(long)[][zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd]<Local>
  );

DECLARE MAP TMP_SUM7(long)[][locn: long, zip: long, rgn_cd: double, clim_zn_nbr: double] :=
  AggSum([locn, zip, rgn_cd, clim_zn_nbr],
    TMP_SUM6(long)[][zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd]<Local>
  );

DECLARE MAP TMP_SUM8(long)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    TMP_SUM7(long)[][locn, zip, rgn_cd, clim_zn_nbr]<Local>
  );

DECLARE MAP TMP_SUM9(long)[][locn: long] :=
  AggSum([locn],
    (TMP_SUM8(long)[][locn, zip]<Local> * V_population_C1(long)[][zip]<Local>)
  );

DECLARE MAP TMP_SUM10(long)[][pacific: long, white: long, population: long, zip: long, blackafrican: long, asian: long] :=
  AggSum([pacific, white, population, zip, blackafrican, asian],
    (DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
  );

DECLARE MAP TMP_SUM11(long)[][pacific: long, white: long, population: long, zip: long, asian: long] :=
  AggSum([pacific, white, population, zip, asian],
    TMP_SUM10(long)[][pacific, white, population, zip, blackafrican, asian]<Local>
  );

DECLARE MAP TMP_SUM12(long)[][zip: long, population: long] :=
  AggSum([zip, population],
    TMP_SUM11(long)[][pacific, white, population, zip, asian]<Local>
  );

DECLARE MAP TMP_SUM13(long)[][zip: long] :=
  AggSum([zip],
    TMP_SUM12(long)[][zip, population]<Local>
  );

DECLARE MAP TMP_SUM14(long)[][locn: long] :=
  AggSum([locn],
    (TMP_SUM13(long)[][zip]<Local> * V_rgn_cd_L1(long)[][locn, zip]<Local>)
  );

DECLARE MAP TMP_SUM15(long)[][ksn: double, subcategory: long, category: long] :=
  AggSum([ksn, subcategory, category],
    (DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize)
  );

DECLARE MAP TMP_SUM16(long)[][ksn: double] :=
  AggSum([ksn],
    TMP_SUM15(long)[][ksn, subcategory, category]<Local>
  );

DECLARE MAP TMP_SUM17(double)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (TMP_SUM16(long)[][ksn]<Local> * V_inventoryunits_I1(double)[][locn, dateid, ksn]<Local>)
  );

DECLARE MAP TMP_SUM18(double)[][locn: long] :=
  AggSum([locn],
    (TMP_SUM17(double)[][locn, dateid]<Local> * V_rain_W1(long)[][locn, dateid]<Local>)
  );

DECLARE MAP TMP_SUM19(long)[][rain: long, snow: long, mintemp: long, meanwind: double, dateid: long, maxtemp: long, locn: long] :=
  AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
    (DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder)
  );

DECLARE MAP TMP_SUM20(long)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    TMP_SUM19(long)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local>
  );

DECLARE MAP TMP_SUM21(double)[][locn: long] :=
  AggSum([locn],
    (TMP_SUM20(long)[][locn, dateid]<Local> * V_ksn_II1(double)[][locn, dateid]<Local>)
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(double)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  TMP_SUM1(double)[][locn, dateid, ksn]<Local>  :=  AggSum([locn, dateid, ksn],
    (((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * inventoryunits) * inventoryunits)
  );
  TMP_SUM2(double)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (TMP_SUM1(double)[][locn, dateid, ksn]<Local> * V_subcategory_I1(long)[][ksn]<Local>)
  );
  TMP_SUM3(double)[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM2(double)[][locn, dateid]<Local> * V_rain_W1(long)[][locn, dateid]<Local>)
  );
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    (TMP_SUM3(double)[][locn]<Local> * V_zip_LC1(long)[][locn]<Local>)
  );
  V_dateid_IIW1(double)[][locn]<Local>  +=  TMP_SUM3(double)[][locn]<Local>;
  V_ksn_II1(double)[][locn, dateid]<Local>  +=  TMP_SUM2(double)[][locn, dateid]<Local>;
  V_inventoryunits_I1(double)[][locn, dateid, ksn]<Local>  +=  TMP_SUM1(double)[][locn, dateid, ksn]<Local>;
}

ON BATCH UPDATE OF LOCATION {
  TMP_SUM4(long)[][walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd]<Local>  :=  AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
    (DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)
  );
  TMP_SUM5(long)[][zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd]<Local>  :=  AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
    TMP_SUM4(long)[][walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd]<Local>
  );
  TMP_SUM6(long)[][zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd]<Local>  :=  AggSum([zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
    TMP_SUM5(long)[][zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd]<Local>
  );
  TMP_SUM7(long)[][locn, zip, rgn_cd, clim_zn_nbr]<Local>  :=  AggSum([locn, zip, rgn_cd, clim_zn_nbr],
    TMP_SUM6(long)[][zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd]<Local>
  );
  TMP_SUM8(long)[][locn, zip]<Local>  :=  AggSum([locn, zip],
    TMP_SUM7(long)[][locn, zip, rgn_cd, clim_zn_nbr]<Local>
  );
  TMP_SUM9(long)[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM8(long)[][locn, zip]<Local> * V_population_C1(long)[][zip]<Local>)
  );
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    (TMP_SUM9(long)[][locn]<Local> * V_dateid_IIW1(double)[][locn]<Local>)
  );
  V_zip_LC1(long)[][locn]<Local>  +=  TMP_SUM9(long)[][locn]<Local>;
  V_rgn_cd_L1(long)[][locn, zip]<Local>  +=  TMP_SUM8(long)[][locn, zip]<Local>;
}

ON BATCH UPDATE OF CENSUS {
  TMP_SUM10(long)[][pacific, white, population, zip, blackafrican, asian]<Local>  :=  AggSum([pacific, white, population, zip, blackafrican, asian],
    (DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
  );
  TMP_SUM11(long)[][pacific, white, population, zip, asian]<Local>  :=  AggSum([pacific, white, population, zip, asian],
    TMP_SUM10(long)[][pacific, white, population, zip, blackafrican, asian]<Local>
  );
  TMP_SUM12(long)[][zip, population]<Local>  :=  AggSum([zip, population],
    TMP_SUM11(long)[][pacific, white, population, zip, asian]<Local>
  );
  TMP_SUM13(long)[][zip]<Local>  :=  AggSum([zip],
    TMP_SUM12(long)[][zip, population]<Local>
  );
  TMP_SUM14(long)[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM13(long)[][zip]<Local> * V_rgn_cd_L1(long)[][locn, zip]<Local>)
  );
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    (TMP_SUM14(long)[][locn]<Local> * V_dateid_IIW1(double)[][locn]<Local>)
  );
  V_zip_LC1(long)[][locn]<Local>  +=  TMP_SUM14(long)[][locn]<Local>;
  V_population_C1(long)[][zip]<Local>  +=  TMP_SUM13(long)[][zip]<Local>;
}

ON BATCH UPDATE OF ITEM {
  TMP_SUM15(long)[][ksn, subcategory, category]<Local>  :=  AggSum([ksn, subcategory, category],
    (DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize)
  );
  TMP_SUM16(long)[][ksn]<Local>  :=  AggSum([ksn],
    TMP_SUM15(long)[][ksn, subcategory, category]<Local>
  );
  TMP_SUM17(double)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (TMP_SUM16(long)[][ksn]<Local> * V_inventoryunits_I1(double)[][locn, dateid, ksn]<Local>)
  );
  TMP_SUM18(double)[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM17(double)[][locn, dateid]<Local> * V_rain_W1(long)[][locn, dateid]<Local>)
  );
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    (TMP_SUM18(double)[][locn]<Local> * V_zip_LC1(long)[][locn]<Local>)
  );
  V_dateid_IIW1(double)[][locn]<Local>  +=  TMP_SUM18(double)[][locn]<Local>;
  V_ksn_II1(double)[][locn, dateid]<Local>  +=  TMP_SUM17(double)[][locn, dateid]<Local>;
  V_subcategory_I1(long)[][ksn]<Local>  +=  TMP_SUM16(long)[][ksn]<Local>;
}

ON BATCH UPDATE OF WEATHER {
  TMP_SUM19(long)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local>  :=  AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
    (DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder)
  );
  TMP_SUM20(long)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    TMP_SUM19(long)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local>
  );
  TMP_SUM21(double)[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM20(long)[][locn, dateid]<Local> * V_ksn_II1(double)[][locn, dateid]<Local>)
  );
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    (TMP_SUM21(double)[][locn]<Local> * V_zip_LC1(long)[][locn]<Local>)
  );
  V_dateid_IIW1(double)[][locn]<Local>  +=  TMP_SUM21(double)[][locn]<Local>;
  V_rain_W1(long)[][locn, dateid]<Local>  +=  TMP_SUM20(long)[][locn, dateid]<Local>;
}

ON SYSTEM READY {
  
}