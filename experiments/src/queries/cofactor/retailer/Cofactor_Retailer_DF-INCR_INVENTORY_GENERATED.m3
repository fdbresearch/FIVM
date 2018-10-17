---------------- TYPE DEFINITIONS ---------------
CREATE TYPE RingCofactor FROM FILE 'ring_cofactor.hpp';

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
DECLARE MAP V_locn_IIWLC1(RingCofactor(43))[][] :=
  AggSum([],
    (AggSum([locn],
      (AggSum([locn, dateid],
        (AggSum([locn, dateid, ksn],
          (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor(1)](4, inventoryunits))
        ) * (V_subcategory_I1(RingCofactor(4))[][ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
      ) * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
    ) * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );

DECLARE MAP V_subcategory_I1(RingCofactor(4))[][ksn: double] :=
  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor(4)](33, subcategory, category, categoryCluster, prize))
  );

DECLARE MAP V_rain_W1(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor(6)](37, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );

DECLARE MAP V_zip_LC1(RingCofactor(29))[][locn: long] :=
  AggSum([locn],
    (AggSum([locn, zip],
      (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor(13)](5, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
    ) * (AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor(15)](18, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    ) * [lift: RingCofactor(1)](3, zip)))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingCofactor(43))[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (AggSum([locn],
      (AggSum([locn, dateid],
        (AggSum([locn, dateid, ksn],
          ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor(1)](4, inventoryunits))
        ) * (V_subcategory_I1(RingCofactor(4))[][ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
      ) * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
    ) * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
}

ON SYSTEM READY {
  V_subcategory_I1(RingCofactor(4))[][ksn]<Local>  :=  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor(4)](33, subcategory, category, categoryCluster, prize))
  );
  V_rain_W1(RingCofactor(6))[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor(6)](37, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  V_zip_LC1(RingCofactor(29))[][locn]<Local>  :=  AggSum([locn],
    (AggSum([locn, zip],
      (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor(13)](5, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
    ) * (AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor(15)](18, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    ) * [lift: RingCofactor(1)](3, zip)))
  );
}