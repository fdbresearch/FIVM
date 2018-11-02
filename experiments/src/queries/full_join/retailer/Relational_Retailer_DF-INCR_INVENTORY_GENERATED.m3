---------------- TYPE DEFINITIONS ---------------
CREATE TYPE RingRelation
  FROM FILE 'ring_relation.hpp'
  WITH PARAMETER SCHEMA (dynamic_concat);

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
DECLARE MAP V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][] :=
  AggSum([],
    ((AggSum([locn],
      ((AggSum([locn, dateid],
        ((AggSum([locn, dateid, ksn],
          (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift<3>: RingRelation<[3, double]>](3, inventoryunits))
        ) * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
      ) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
    ) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
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
    ((AggSum([locn, zip],
      (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](15, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
    ) * AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](28, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    )) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  V_locn_IIWLC1(RingRelation<[0, long, long, double, double, long, long, long, double, long, long, long, long, double, long, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][]<Local>  +=  AggSum([],
    ((AggSum([locn],
      ((AggSum([locn, dateid],
        ((AggSum([locn, dateid, ksn],
          ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift<3>: RingRelation<[3, double]>](3, inventoryunits))
        ) * V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>) * [lift<2>: RingRelation<[2, double]>](2, ksn))
      ) * V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>) * [lift<1>: RingRelation<[1, long]>](1, dateid))
    ) * V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>) * [lift<0>: RingRelation<[0, long]>](0, locn))
  );
}

ON SYSTEM READY {
  V_subcategory_I1(RingRelation<[4, long, long, long, double]>)[][ksn]<Local>  :=  AggSum([ksn],
    (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift<4>: RingRelation<[4, long, long, long, double]>](4, subcategory, category, categoryCluster, prize))
  );
  V_rain_W1(RingRelation<[8, long, long, long, long, double, long]>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift<8>: RingRelation<[8, long, long, long, long, double, long]>](8, rain, snow, maxtemp, mintemp, meanwind, thunder))
  );
  V_zip_LC1(RingRelation<[14, long, double, double, double, double, double, double, double, double, double, double, double, double, double, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>)[][locn]<Local>  :=  AggSum([locn],
    ((AggSum([locn, zip],
      (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift<15>: RingRelation<[15, double, double, double, double, double, double, double, double, double, double, double, double, double]>](15, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime))
    ) * AggSum([zip],
      (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift<28>: RingRelation<[28, long, long, long, long, long, double, long, long, long, long, long, long, long, long, long]>](28, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
    )) * [lift<14>: RingRelation<[14, long]>](14, zip))
  );
}