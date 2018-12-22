IMPORT DTREE FROM FILE 'retailer.txt';

CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

CREATE STREAM INVENTORY(locn long, dateid long, ksn double, inventoryunits double)
FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM LOCATION(locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM CENSUS(zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM ITEM(ksn double, subcategory long, category long, categoryCluster long, prize double) 
FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM WEATHER(locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long) 
FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(
    [lift<0>: RingFactorizedRelation<[0,long]>](locn) *
    [lift<1>: RingFactorizedRelation<[1,long]>](dateid) *
    [lift<2>: RingFactorizedRelation<[2,double, double]>](ksn, inventoryunits) *
    [lift<14>: RingFactorizedRelation<[14,long]>](zip) *
    [lift<15>: RingFactorizedRelation<[15,double,double,double,double,double,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [lift<28>: RingFactorizedRelation<[28,long,long,long,long,long,double,long,long,long,long,long,long,long,long,long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
    [lift<4>: RingFactorizedRelation<[4,long,long,long,double]>](subcategory, category, categoryCluster, prize) *
    [lift<8>: RingFactorizedRelation<[8,long,long,long,long,double,long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingFactorizedRelation<[0,long]>](locn) *
--     [lift<1>: RingFactorizedRelation<[1,long]>](dateid) *
--     [lift<2>: RingFactorizedRelation<[2,double]>](ksn) *
--     [lift<14>: RingFactorizedRelation<[14,long]>](zip) *
--     [lift<3>: RingFactorizedRelation<[3,double]>](inventoryunits) *
--     [lift<15>: RingFactorizedRelation<[15,double,double,double,double,double,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift<28>: RingFactorizedRelation<[28,long,long,long,long,long,double,long,long,long,long,long,long,long,long,long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
--     [lift<4>: RingFactorizedRelation<[4,long,long,long,double]>](subcategory, category, categoryCluster, prize) *
--     [lift<8>: RingFactorizedRelation<[8,long,long,long,long,double,long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingFactorizedRelation<[0,long]>](locn) *
--     [lift<1>: RingFactorizedRelation<[1,long]>](dateid) *
--     [lift<2>: RingFactorizedRelation<[2,double]>](ksn) *
--     [lift<14>: RingFactorizedRelation<[14,long]>](zip) *
--     [lift<3>: RingFactorizedRelation<[3,double]>](inventoryunits) *
--     [lift<15>: RingFactorizedRelation<[15,double]>](rgn_cd) *
--     [lift<16>: RingFactorizedRelation<[16,double]>](clim_zn_nbr) *
--     [lift<17>: RingFactorizedRelation<[17,double]>](tot_area_sq_ft) *
--     [lift<18>: RingFactorizedRelation<[18,double]>](sell_area_sq_ft) *
--     [lift<19>: RingFactorizedRelation<[19,double]>](avghhi) *
--     [lift<20>: RingFactorizedRelation<[20,double]>](supertargetdistance) *
--     [lift<21>: RingFactorizedRelation<[21,double]>](supertargetdrivetime) *
--     [lift<22>: RingFactorizedRelation<[22,double]>](targetdistance) *
--     [lift<23>: RingFactorizedRelation<[23,double]>](targetdrivetime) *
--     [lift<24>: RingFactorizedRelation<[24,double]>](walmartdistance) *
--     [lift<25>: RingFactorizedRelation<[25,double]>](walmartdrivetime) *
--     [lift<26>: RingFactorizedRelation<[26,double]>](walmartsupercenterdistance) *
--     [lift<27>: RingFactorizedRelation<[27,double]>](walmartsupercenterdrivetime) *
--     [lift<28>: RingFactorizedRelation<[28,long]>](population) *
--     [lift<29>: RingFactorizedRelation<[29,long]>](white) *
--     [lift<30>: RingFactorizedRelation<[30,long]>](asian) *
--     [lift<31>: RingFactorizedRelation<[31,long]>](pacific) *
--     [lift<32>: RingFactorizedRelation<[32,long]>](blackafrican) *
--     [lift<33>: RingFactorizedRelation<[33,double]>](medianage) *
--     [lift<34>: RingFactorizedRelation<[34,long]>](occupiedhouseunits) *
--     [lift<35>: RingFactorizedRelation<[35,long]>](houseunits) *
--     [lift<36>: RingFactorizedRelation<[36,long]>](families) *
--     [lift<37>: RingFactorizedRelation<[37,long]>](households) *
--     [lift<38>: RingFactorizedRelation<[38,long]>](husbwife) *
--     [lift<39>: RingFactorizedRelation<[39,long]>](males) *
--     [lift<40>: RingFactorizedRelation<[40,long]>](females) *
--     [lift<41>: RingFactorizedRelation<[41,long]>](householdschildren) *
--     [lift<42>: RingFactorizedRelation<[42,long]>](hispanic) *
--     [lift<4>: RingFactorizedRelation<[4,long]>](subcategory) *
--     [lift<5>: RingFactorizedRelation<[5,long]>](category) *
--     [lift<6>: RingFactorizedRelation<[6,long]>](categoryCluster) *
--     [lift<7>: RingFactorizedRelation<[7,double]>](prize) *
--     [lift<8>: RingFactorizedRelation<[8,long]>](rain) *
--     [lift<9>: RingFactorizedRelation<[9,long]>](snow) *
--     [lift<10>: RingFactorizedRelation<[10,long]>](maxtemp) *
--     [lift<11>: RingFactorizedRelation<[11,long]>](mintemp) *
--     [lift<12>: RingFactorizedRelation<[12,double]>](meanwind) *
--     [lift<13>: RingFactorizedRelation<[13,long]>](thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
