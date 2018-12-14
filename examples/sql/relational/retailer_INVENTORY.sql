CREATE TYPE RingRelation
FROM FILE 'ring/ring_relational_opt.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

CREATE STREAM INVENTORY(locn long, dateid long, ksn double, inventoryunits double)
FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE LOCATION(locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE CENSUS(zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE ITEM(ksn double, subcategory long, category long, categoryCluster long, prize double) 
FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE WEATHER(locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long) 
FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(
    [lift<0>: RingRelation<[0,long]>](locn) *
    [lift<1>: RingRelation<[1,long]>](dateid) *
    [lift<2>: RingRelation<[2,double, double]>](ksn, inventoryunits) *
    [lift<14>: RingRelation<[14,long]>](zip) *
    [lift<15>: RingRelation<[15,double,double,double,double,double,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
    [lift<28>: RingRelation<[28,long,long,long,long,long,double,long,long,long,long,long,long,long,long,long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
    [lift<4>: RingRelation<[4,long,long,long,double]>](subcategory, category, categoryCluster, prize) *
    [lift<8>: RingRelation<[8,long,long,long,long,double,long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingRelation<[0,long]>](locn) *
--     [lift<1>: RingRelation<[1,long]>](dateid) *
--     [lift<2>: RingRelation<[2,double]>](ksn) *
--     [lift<14>: RingRelation<[14,long]>](zip) *
--     [lift<3>: RingRelation<[3,double]>](inventoryunits) *
--     [lift<15>: RingRelation<[15,double,double,double,double,double,double,double,double,double,double,double,double,double]>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift<28>: RingRelation<[28,long,long,long,long,long,double,long,long,long,long,long,long,long,long,long]>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
--     [lift<4>: RingRelation<[4,long,long,long,double]>](subcategory, category, categoryCluster, prize) *
--     [lift<8>: RingRelation<[8,long,long,long,long,double,long]>](rain, snow, maxtemp, mintemp, meanwind, thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift<0>: RingRelation<[0,long]>](locn) *
--     [lift<1>: RingRelation<[1,long]>](dateid) *
--     [lift<2>: RingRelation<[2,double]>](ksn) *
--     [lift<14>: RingRelation<[14,long]>](zip) *
--     [lift<3>: RingRelation<[3,double]>](inventoryunits) *
--     [lift<15>: RingRelation<[15,double]>](rgn_cd) *
--     [lift<16>: RingRelation<[16,double]>](clim_zn_nbr) *
--     [lift<17>: RingRelation<[17,double]>](tot_area_sq_ft) *
--     [lift<18>: RingRelation<[18,double]>](sell_area_sq_ft) *
--     [lift<19>: RingRelation<[19,double]>](avghhi) *
--     [lift<20>: RingRelation<[20,double]>](supertargetdistance) *
--     [lift<21>: RingRelation<[21,double]>](supertargetdrivetime) *
--     [lift<22>: RingRelation<[22,double]>](targetdistance) *
--     [lift<23>: RingRelation<[23,double]>](targetdrivetime) *
--     [lift<24>: RingRelation<[24,double]>](walmartdistance) *
--     [lift<25>: RingRelation<[25,double]>](walmartdrivetime) *
--     [lift<26>: RingRelation<[26,double]>](walmartsupercenterdistance) *
--     [lift<27>: RingRelation<[27,double]>](walmartsupercenterdrivetime) *
--     [lift<28>: RingRelation<[28,long]>](population) *
--     [lift<29>: RingRelation<[29,long]>](white) *
--     [lift<30>: RingRelation<[30,long]>](asian) *
--     [lift<31>: RingRelation<[31,long]>](pacific) *
--     [lift<32>: RingRelation<[32,long]>](blackafrican) *
--     [lift<33>: RingRelation<[33,double]>](medianage) *
--     [lift<34>: RingRelation<[34,long]>](occupiedhouseunits) *
--     [lift<35>: RingRelation<[35,long]>](houseunits) *
--     [lift<36>: RingRelation<[36,long]>](families) *
--     [lift<37>: RingRelation<[37,long]>](households) *
--     [lift<38>: RingRelation<[38,long]>](husbwife) *
--     [lift<39>: RingRelation<[39,long]>](males) *
--     [lift<40>: RingRelation<[40,long]>](females) *
--     [lift<41>: RingRelation<[41,long]>](householdschildren) *
--     [lift<42>: RingRelation<[42,long]>](hispanic) *
--     [lift<4>: RingRelation<[4,long]>](subcategory) *
--     [lift<5>: RingRelation<[5,long]>](category) *
--     [lift<6>: RingRelation<[6,long]>](categoryCluster) *
--     [lift<7>: RingRelation<[7,double]>](prize) *
--     [lift<8>: RingRelation<[8,long]>](rain) *
--     [lift<9>: RingRelation<[9,long]>](snow) *
--     [lift<10>: RingRelation<[10,long]>](maxtemp) *
--     [lift<11>: RingRelation<[11,long]>](mintemp) *
--     [lift<12>: RingRelation<[12,double]>](meanwind) *
--     [lift<13>: RingRelation<[13,long]>](thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
