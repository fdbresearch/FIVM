CREATE TYPE RingCofactor
FROM FILE 'ring_cofactor.hpp'
WITH PARAMETER SCHEMA (static, dynamic_sum);

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
    [lift: RingCofactor<double,1>](locn) *
    [lift: RingCofactor<double,1>](dateid) *
    [lift: RingCofactor<double,1>](ksn) *
    [lift: RingCofactor<double,1>](inventoryunits) *
    [lift: RingCofactor<double,1>](subcategory) *
    [lift: RingCofactor<double,1>](category) *
    [lift: RingCofactor<double,1>](categoryCluster) *
    [lift: RingCofactor<double,1>](prize) *
    [lift: RingCofactor<double,1>](rain) *
    [lift: RingCofactor<double,1>](snow) *
    [lift: RingCofactor<double,1>](maxtemp) *
    [lift: RingCofactor<double,1>](mintemp) *
    [lift: RingCofactor<double,1>](meanwind) *
    [lift: RingCofactor<double,1>](thunder) *    
    [lift: RingCofactor<double,1>](zip) *
    [lift: RingCofactor<double,1>](rgn_cd) *
    [lift: RingCofactor<double,1>](clim_zn_nbr) *
    [lift: RingCofactor<double,1>](tot_area_sq_ft) *
    [lift: RingCofactor<double,1>](sell_area_sq_ft) *
    [lift: RingCofactor<double,1>](avghhi) *
    [lift: RingCofactor<double,1>](supertargetdistance) *
    [lift: RingCofactor<double,1>](supertargetdrivetime) *
    [lift: RingCofactor<double,1>](targetdistance) *
    [lift: RingCofactor<double,1>](targetdrivetime) *
    [lift: RingCofactor<double,1>](walmartdistance) *
    [lift: RingCofactor<double,1>](walmartdrivetime) *
    [lift: RingCofactor<double,1>](walmartsupercenterdistance) *
    [lift: RingCofactor<double,1>](walmartsupercenterdrivetime) *
    [lift: RingCofactor<double,1>](population) *
    [lift: RingCofactor<double,1>](white) *
    [lift: RingCofactor<double,1>](asian) *
    [lift: RingCofactor<double,1>](pacific) *
    [lift: RingCofactor<double,1>](blackafrican) *
    [lift: RingCofactor<double,1>](medianage) *
    [lift: RingCofactor<double,1>](occupiedhouseunits) *
    [lift: RingCofactor<double,1>](houseunits) *
    [lift: RingCofactor<double,1>](families) *
    [lift: RingCofactor<double,1>](households) *
    [lift: RingCofactor<double,1>](husbwife) *
    [lift: RingCofactor<double,1>](males) *
    [lift: RingCofactor<double,1>](females) *
    [lift: RingCofactor<double,1>](householdschildren) *
    [lift: RingCofactor<double,1>](hispanic)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;

-- SELECT SUM(
--     [lift: RingCofactor<double,1>](locn) *
--     [lift: RingCofactor<double,1>](dateid) *
--     [lift: RingCofactor<double,1>](ksn) *
--     [lift: RingCofactor<double,1>](inventoryunits) *
--     [lift: RingCofactor<double,4>](subcategory, category, categoryCluster, prize) *
--     [lift: RingCofactor<double,6>](rain, snow, maxtemp, mintemp, meanwind, thunder) *
--     [lift: RingCofactor<double,1>](zip) *
--     [lift: RingCofactor<double,13>](rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift: RingCofactor<double,15>](population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;