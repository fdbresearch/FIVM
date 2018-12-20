---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (static, dynamic_min, dynamic_sum);

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
DECLARE MAP V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][] :=
  AggSum([],
    ((AggSum([locn],
      ((AggSum([locn, dateid],
        ((AggSum([locn, dateid, ksn],
          (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits))
        ) * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn))
      ) * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid))
    ) * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn))
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
    (((LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingCofactor<double, 15, 13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingCofactor<double, 28, 15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    )) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>;

------------------- TRIGGERS --------------------
ON + INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  (((((((1 * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits)) * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn)) * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid)) * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn));
}

ON - INVENTORY (locn, dateid, ksn, inventoryunits) {
  V_locn_IIWLC1(RingCofactor<double, 0, 43>)[][]<Local>  +=  (((((((-1 * [lift<3>: RingCofactor<double, 3, 1>](inventoryunits)) * V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>) * [lift<2>: RingCofactor<double, 2, 1>](ksn)) * V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>) * [lift<1>: RingCofactor<double, 1, 1>](dateid)) * V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](locn));
}

ON SYSTEM READY {
  V_subcategory_I1(RingCofactor<double, 4, 4>)[][ksn]<Local>  :=  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingCofactor<double, 4, 4>](subcategory, category, categoryCluster, prize))
  );
  V_rain_W1(RingCofactor<double, 8, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingCofactor<double, 8, 6>](rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  V_zip_LC1(RingCofactor<double, 14, 29>)[][locn]<Local>  :=  AggSum([locn],
    (((LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingCofactor<double, 15, 13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingCofactor<double, 28, 15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    )) * [lift<14>: RingCofactor<double, 14, 1>](zip))
  );
}