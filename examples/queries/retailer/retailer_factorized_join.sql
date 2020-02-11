IMPORT DTREE FROM FILE 'retailer.txt';

CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

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
    [lift<0>: RingFactorizedRelation<[0,int]>](locn) *
    [lift<1>: RingFactorizedRelation<[1,int]>](dateid) *
    [lift<2>: RingFactorizedRelation<[2,int,int]>](ksn, inventoryunits) *
    [lift<14>: RingFactorizedRelation<[14,int]>](zip) *
    [lift<15>: RingFactorizedRelation<[15,int,int,int,int,int,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [lift<28>: RingFactorizedRelation<[28,int,int,int,int,int,double,int,int,int,int,int,int,int,int,int]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
    [lift<4>: RingFactorizedRelation<[4,byte,byte,byte,double]>](subcategory, category, categoryCluster, prize) *
    [lift<8>: RingFactorizedRelation<[8,byte,byte,int,int,double,byte]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingFactorizedRelation<[0,int]>](locn) *
--     [lift<1>: RingFactorizedRelation<[1,int]>](dateid) *
--     [lift<2>: RingFactorizedRelation<[2,int]>](ksn) *
--     [lift<14>: RingFactorizedRelation<[14,int]>](zip) *
--     [lift<3>: RingFactorizedRelation<[3,int]>](inventoryunits) *
--     [lift<15>: RingFactorizedRelation<[15,int,int,int,int,int,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift<28>: RingFactorizedRelation<[28,int,int,int,int,int,double,int,int,int,int,int,int,int,int,int]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
--     [lift<4>: RingFactorizedRelation<[4,byte,byte,byte,double]>](subcategory, category, categoryCluster, prize) *
--     [lift<8>: RingFactorizedRelation<[8,byte,byte,int,int,double,byte]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingFactorizedRelation<[0,int]>](locn) *
--     [lift<1>: RingFactorizedRelation<[1,int]>](dateid) *
--     [lift<2>: RingFactorizedRelation<[2,int]>](ksn) *
--     [lift<14>: RingFactorizedRelation<[14,int]>](zip) *
--     [lift<3>: RingFactorizedRelation<[3,int]>](inventoryunits) *
--     [lift<15>: RingFactorizedRelation<[15,int]>](rgn_cd) *
--     [lift<16>: RingFactorizedRelation<[16,int]>](clim_zn_nbr) *
--     [lift<17>: RingFactorizedRelation<[17,int]>](tot_area_sq_ft) *
--     [lift<18>: RingFactorizedRelation<[18,int]>](sell_area_sq_ft) *
--     [lift<19>: RingFactorizedRelation<[19,int]>](avghhi) *
--     [lift<20>: RingFactorizedRelation<[20,double]>](supertargetdistance) *
--     [lift<21>: RingFactorizedRelation<[21,double]>](supertargetdrivetime) *
--     [lift<22>: RingFactorizedRelation<[22,double]>](targetdistance) *
--     [lift<23>: RingFactorizedRelation<[23,double]>](targetdrivetime) *
--     [lift<24>: RingFactorizedRelation<[24,double]>](walmartdistance) *
--     [lift<25>: RingFactorizedRelation<[25,double]>](walmartdrivetime) *
--     [lift<26>: RingFactorizedRelation<[26,double]>](walmartsupercenterdistance) *
--     [lift<27>: RingFactorizedRelation<[27,double]>](walmartsupercenterdrivetime) *
--     [lift<28>: RingFactorizedRelation<[28,int]>](population) *
--     [lift<29>: RingFactorizedRelation<[29,int]>](white) *
--     [lift<30>: RingFactorizedRelation<[30,int]>](asian) *
--     [lift<31>: RingFactorizedRelation<[31,int]>](pacific) *
--     [lift<32>: RingFactorizedRelation<[32,int]>](blackafrican) *
--     [lift<33>: RingFactorizedRelation<[33,double]>](medianage) *
--     [lift<34>: RingFactorizedRelation<[34,int]>](occupiedhouseunits) *
--     [lift<35>: RingFactorizedRelation<[35,int]>](houseunits) *
--     [lift<36>: RingFactorizedRelation<[36,int]>](families) *
--     [lift<37>: RingFactorizedRelation<[37,int]>](households) *
--     [lift<38>: RingFactorizedRelation<[38,int]>](husbwife) *
--     [lift<39>: RingFactorizedRelation<[39,int]>](males) *
--     [lift<40>: RingFactorizedRelation<[40,int]>](females) *
--     [lift<41>: RingFactorizedRelation<[41,int]>](householdschildren) *
--     [lift<42>: RingFactorizedRelation<[42,int]>](hispanic) *
--     [lift<4>: RingFactorizedRelation<[4,byte]>](subcategory) *
--     [lift<5>: RingFactorizedRelation<[5,byte]>](category) *
--     [lift<6>: RingFactorizedRelation<[6,byte]>](categoryCluster) *
--     [lift<7>: RingFactorizedRelation<[7,double]>](prize) *
--     [lift<8>: RingFactorizedRelation<[8,byte]>](rain) *
--     [lift<9>: RingFactorizedRelation<[9,byte]>](snow) *
--     [lift<10>: RingFactorizedRelation<[10,int]>](maxtemp) *
--     [lift<11>: RingFactorizedRelation<[11,int]>](mintemp) *
--     [lift<12>: RingFactorizedRelation<[12,double]>](meanwind) *
--     [lift<13>: RingFactorizedRelation<[13,byte]>](thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
