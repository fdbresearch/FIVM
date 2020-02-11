IMPORT DTREE FROM FILE 'retailer.txt';

CREATE TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (dynamic_min, static, dynamic_sum);

CREATE STREAM INVENTORY(locn int, dateid int, ksn int, inventoryunits int)
FROM FILE './datasets/retailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM LOCATION(locn int, zip int, rgn_cd int, clim_zn_nbr int, tot_area_sq_ft int, sell_area_sq_ft int, avghhi int, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
FROM FILE './datasets/retailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage double, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/retailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM ITEM(ksn int, subcategory byte, category byte, categoryCluster byte, prize double) 
FROM FILE './datasets/retailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM WEATHER(locn int, dateid int, rain byte, snow byte, maxtemp int, mintemp int, meanwind double, thunder byte) 
FROM FILE './datasets/retailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(
    [lift<0>: RingCofactor<0,double,1>](inventoryunits) *
    [lift<1>: RingCofactor<1,double,4>](subcategory, category, categoryCluster, prize) *
    [lift<5>: RingCofactor<5,double,6>](rain, snow, maxtemp, mintemp, meanwind, thunder) *
    [lift<11>: RingCofactor<11,double,13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [lift<24>: RingCofactor<24,double,15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingCofactor<0,double,1>](locn) *
--     [lift<1>: RingCofactor<1,double,1>](dateid) *
--     [lift<2>: RingCofactor<2,double,1>](ksn) *
--     [lift<3>: RingCofactor<3,double,1>](inventoryunits) *
--     [lift<4>: RingCofactor<4,double,4>](subcategory, category, categoryCluster, prize) *
--     [lift<8>: RingCofactor<8,double,6>](rain, snow, maxtemp, mintemp, meanwind, thunder) *
--     [lift<14>: RingCofactor<14,double,1>](zip) *
--     [lift<15>: RingCofactor<15,double,13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift<28>: RingCofactor<28,double,15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingCofactor<double,0,1>](locn) *
--     [lift<1>: RingCofactor<double,1,1>](dateid) *
--     [lift<2>: RingCofactor<double,2,1>](ksn) *
--     [lift<3>: RingCofactor<double,3,1>](inventoryunits) *
--     [lift<4>: RingCofactor<double,4,1>](subcategory) *
--     [lift<5>: RingCofactor<double,5,1>](category) *
--     [lift<6>: RingCofactor<double,6,1>](categoryCluster) *
--     [lift<7>: RingCofactor<double,7,1>](prize) *
--     [lift<8>: RingCofactor<double,8,1>](rain) *
--     [lift<9>: RingCofactor<double,9,1>](snow) *
--     [lift<10>: RingCofactor<double,10,1>](maxtemp) *
--     [lift<11>: RingCofactor<double,11,1>](mintemp) *
--     [lift<12>: RingCofactor<double,12,1>](meanwind) *
--     [lift<13>: RingCofactor<double,13,1>](thunder) *    
--     [lift<14>: RingCofactor<double,14,1>](zip) *
--     [lift<15>: RingCofactor<double,15,1>](rgn_cd) *
--     [lift<16>: RingCofactor<double,16,1>](clim_zn_nbr) *
--     [lift<17>: RingCofactor<double,17,1>](tot_area_sq_ft) *
--     [lift<18>: RingCofactor<double,18,1>](sell_area_sq_ft) *
--     [lift<19>: RingCofactor<double,19,1>](avghhi) *
--     [lift<20>: RingCofactor<double,20,1>](supertargetdistance) *
--     [lift<21>: RingCofactor<double,21,1>](supertargetdrivetime) *
--     [lift<22>: RingCofactor<double,22,1>](targetdistance) *
--     [lift<23>: RingCofactor<double,23,1>](targetdrivetime) *
--     [lift<24>: RingCofactor<double,24,1>](walmartdistance) *
--     [lift<25>: RingCofactor<double,25,1>](walmartdrivetime) *
--     [lift<26>: RingCofactor<double,26,1>](walmartsupercenterdistance) *
--     [lift<27>: RingCofactor<double,27,1>](walmartsupercenterdrivetime) *
--     [lift<28>: RingCofactor<double,28,1>](population) *
--     [lift<29>: RingCofactor<double,29,1>](white) *
--     [lift<30>: RingCofactor<double,30,1>](asian) *
--     [lift<31>: RingCofactor<double,31,1>](pacific) *
--     [lift<32>: RingCofactor<double,32,1>](blackafrican) *
--     [lift<33>: RingCofactor<double,33,1>](medianage) *
--     [lift<34>: RingCofactor<double,34,1>](occupiedhouseunits) *
--     [lift<35>: RingCofactor<double,35,1>](houseunits) *
--     [lift<36>: RingCofactor<double,36,1>](families) *
--     [lift<37>: RingCofactor<double,37,1>](households) *
--     [lift<38>: RingCofactor<double,38,1>](husbwife) *
--     [lift<39>: RingCofactor<double,39,1>](males) *
--     [lift<40>: RingCofactor<double,40,1>](females) *
--     [lift<41>: RingCofactor<double,41,1>](householdschildren) *
--     [lift<42>: RingCofactor<double,42,1>](hispanic)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
