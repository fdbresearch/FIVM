IMPORT DTREE FROM FILE 'retailer.txt';

CREATE TYPE RingRelation
FROM FILE 'ring/ring_relational_opt.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

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
    [lift<0>: RingRelation<[0,int]>](locn) *
    [lift<1>: RingRelation<[1,int]>](dateid) *
    [lift<2>: RingRelation<[2,int,int]>](ksn, inventoryunits) *
    [lift<14>: RingRelation<[14,int]>](zip) *
    [lift<15>: RingRelation<[15,int,int,int,int,int,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [lift<28>: RingRelation<[28,int,int,int,int,int,double,int,int,int,int,int,int,int,int,int]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
    [lift<4>: RingRelation<[4,byte,byte,byte,double]>](subcategory, category, categoryCluster, prize) *
    [lift<8>: RingRelation<[8,byte,byte,int,int,double,byte]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingRelation<[0,int]>](locn) *
--     [lift<1>: RingRelation<[1,int]>](dateid) *
--     [lift<2>: RingRelation<[2,int]>](ksn) *
--     [lift<14>: RingRelation<[14,int]>](zip) *
--     [lift<3>: RingRelation<[3,int]>](inventoryunits) *
--     [lift<15>: RingRelation<[15,int,int,int,int,int,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift<28>: RingRelation<[28,int,int,int,int,int,double,int,int,int,int,int,int,int,int,int]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
--     [lift<4>: RingRelation<[4,byte,byte,byte,double]>](subcategory, category, categoryCluster, prize) *
--     [lift<8>: RingRelation<[8,byte,byte,int,int,double,byte]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingRelation<[0,int]>](locn) *
--     [lift<1>: RingRelation<[1,int]>](dateid) *
--     [lift<2>: RingRelation<[2,int]>](ksn) *
--     [lift<14>: RingRelation<[14,int]>](zip) *
--     [lift<3>: RingRelation<[3,int]>](inventoryunits) *
--     [lift<15>: RingRelation<[15,int]>](rgn_cd) *
--     [lift<16>: RingRelation<[16,int]>](clim_zn_nbr) *
--     [lift<17>: RingRelation<[17,int]>](tot_area_sq_ft) *
--     [lift<18>: RingRelation<[18,int]>](sell_area_sq_ft) *
--     [lift<19>: RingRelation<[19,int]>](avghhi) *
--     [lift<20>: RingRelation<[20,double]>](supertargetdistance) *
--     [lift<21>: RingRelation<[21,double]>](supertargetdrivetime) *
--     [lift<22>: RingRelation<[22,double]>](targetdistance) *
--     [lift<23>: RingRelation<[23,double]>](targetdrivetime) *
--     [lift<24>: RingRelation<[24,double]>](walmartdistance) *
--     [lift<25>: RingRelation<[25,double]>](walmartdrivetime) *
--     [lift<26>: RingRelation<[26,double]>](walmartsupercenterdistance) *
--     [lift<27>: RingRelation<[27,double]>](walmartsupercenterdrivetime) *
--     [lift<28>: RingRelation<[28,int]>](population) *
--     [lift<29>: RingRelation<[29,int]>](white) *
--     [lift<30>: RingRelation<[30,int]>](asian) *
--     [lift<31>: RingRelation<[31,int]>](pacific) *
--     [lift<32>: RingRelation<[32,int]>](blackafrican) *
--     [lift<33>: RingRelation<[33,double]>](medianage) *
--     [lift<34>: RingRelation<[34,int]>](occupiedhouseunits) *
--     [lift<35>: RingRelation<[35,int]>](houseunits) *
--     [lift<36>: RingRelation<[36,int]>](families) *
--     [lift<37>: RingRelation<[37,int]>](households) *
--     [lift<38>: RingRelation<[38,int]>](husbwife) *
--     [lift<39>: RingRelation<[39,int]>](males) *
--     [lift<40>: RingRelation<[40,int]>](females) *
--     [lift<41>: RingRelation<[41,int]>](householdschildren) *
--     [lift<42>: RingRelation<[42,int]>](hispanic) *
--     [lift<4>: RingRelation<[4,byte]>](subcategory) *
--     [lift<5>: RingRelation<[5,byte]>](category) *
--     [lift<6>: RingRelation<[6,byte]>](categoryCluster) *
--     [lift<7>: RingRelation<[7,double]>](prize) *
--     [lift<8>: RingRelation<[8,byte]>](rain) *
--     [lift<9>: RingRelation<[9,byte]>](snow) *
--     [lift<10>: RingRelation<[10,int]>](maxtemp) *
--     [lift<11>: RingRelation<[11,int]>](mintemp) *
--     [lift<12>: RingRelation<[12,double]>](meanwind) *
--     [lift<13>: RingRelation<[13,byte]>](thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
