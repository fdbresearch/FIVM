IMPORT DTREE FROM FILE 'retailer.txt';

CREATE TYPE RingCofactorMixed
FROM FILE 'ring/ring_cofactor_degree1_categorical.hpp'
WITH PARAMETER SCHEMA (dynamic_min, dynamic_sum, dynamic_sum);

CREATE STREAM INVENTORY(locn int, dateid int, ksn int, inventoryunits int)
FROM FILE './datasets/retailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE LOCATION(locn int, zip int, rgn_cd int, clim_zn_nbr int, tot_area_sq_ft int, sell_area_sq_ft int, avghhi int, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
FROM FILE './datasets/retailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage double, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/retailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE ITEM(ksn int, subcategory byte, category byte, categoryCluster byte, prize double) 
FROM FILE './datasets/retailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE WEATHER(locn int, dateid int, rain byte, snow byte, maxtemp int, mintemp int, meanwind double, thunder byte) 
FROM FILE './datasets/retailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(
    [liftCont<0>: RingCofactorMixed<0,1,0>](inventoryunits) *
    [liftCont<1>: RingCofactorMixed<1,1,0>](prize) *
    [liftCat<2>: RingCofactorMixed<2,0,3>](subcategory, category, categoryCluster) *
    [liftCont<5>: RingCofactorMixed<5,3,0>](maxtemp, mintemp, meanwind) *
    [liftCat<8>: RingCofactorMixed<8,0,3>](rain, snow, thunder) *
    [liftCont<11>: RingCofactorMixed<11,13,0>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [liftCont<24>: RingCofactorMixed<24,15,0>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
