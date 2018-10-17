---------------- TYPE DEFINITIONS ---------------


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
DECLARE MAP V_locn_IIWLC1(double)[][] :=
  AggSum([],
    (INVENTORY(locn, dateid, ksn, inventoryunits) * (inventoryunits * (inventoryunits * (V_subcategory_I1(long)[][ksn]<Local> * (V_rain_W1(long)[][locn, dateid]<Local> * V_zip_LC1(long)[][locn]<Local>)))))
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
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * AggSum([zip],
      CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
    ))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(double)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  V_locn_IIWLC1(double)[][]<Local>  +=  AggSum([],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * (inventoryunits * (inventoryunits * (V_subcategory_I1(long)[][ksn]<Local> * (V_rain_W1(long)[][locn, dateid]<Local> * V_zip_LC1(long)[][locn]<Local>)))))
  );
}

ON SYSTEM READY {
  V_subcategory_I1(long)[][ksn]<Local>  :=  AggSum([ksn],
    ITEM(ksn, subcategory, category, categoryCluster, prize)
  );
  V_rain_W1(long)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder)
  );
  V_zip_LC1(long)[][locn]<Local>  :=  AggSum([locn],
    (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * AggSum([zip],
      CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
    ))
  );
}