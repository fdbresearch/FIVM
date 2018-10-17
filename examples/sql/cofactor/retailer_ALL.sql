CREATE TYPE RingCofactor
FROM FILE 'ring_cofactor.hpp';

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
    [lift: RingCofactor(1)](0, locn) *
    [lift: RingCofactor(1)](1, dateid) *
    [lift: RingCofactor(1)](2, ksn) *
    [lift: RingCofactor(1)](3, zip) *
    [lift: RingCofactor(1)](4, inventoryunits) *
    [lift: RingCofactor(1)](5, rgn_cd) *
    [lift: RingCofactor(1)](6, clim_zn_nbr) *
    [lift: RingCofactor(1)](7, tot_area_sq_ft) *
    [lift: RingCofactor(1)](8, sell_area_sq_ft) *
    [lift: RingCofactor(1)](9, avghhi) *
    [lift: RingCofactor(1)](10, supertargetdistance) *
    [lift: RingCofactor(1)](11, supertargetdrivetime) *
    [lift: RingCofactor(1)](12, targetdistance) *
    [lift: RingCofactor(1)](13, targetdrivetime) *
    [lift: RingCofactor(1)](14, walmartdistance) *
    [lift: RingCofactor(1)](15, walmartdrivetime) *
    [lift: RingCofactor(1)](16, walmartsupercenterdistance) *
    [lift: RingCofactor(1)](17, walmartsupercenterdrivetime) *
    [lift: RingCofactor(1)](18, population) *
    [lift: RingCofactor(1)](19, white) *
    [lift: RingCofactor(1)](20, asian) *
    [lift: RingCofactor(1)](21, pacific) *
    [lift: RingCofactor(1)](22, blackafrican) *
    [lift: RingCofactor(1)](23, medianage) *
    [lift: RingCofactor(1)](24, occupiedhouseunits) *
    [lift: RingCofactor(1)](25, houseunits) *
    [lift: RingCofactor(1)](26, families) *
    [lift: RingCofactor(1)](27, households) *
    [lift: RingCofactor(1)](28, husbwife) *
    [lift: RingCofactor(1)](29, males) *
    [lift: RingCofactor(1)](30, females) *
    [lift: RingCofactor(1)](31, householdschildren) *
    [lift: RingCofactor(1)](32, hispanic) *
    [lift: RingCofactor(1)](33, subcategory) *
    [lift: RingCofactor(1)](34, category) *
    [lift: RingCofactor(1)](35, categoryCluster) *
    [lift: RingCofactor(1)](36, prize) *
    [lift: RingCofactor(1)](37, rain) *
    [lift: RingCofactor(1)](38, snow) *
    [lift: RingCofactor(1)](39, maxtemp) *
    [lift: RingCofactor(1)](40, mintemp) *
    [lift: RingCofactor(1)](41, meanwind) *
    [lift: RingCofactor(1)](42, thunder)
)
FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;


-- SELECT SUM(
--     [lift: RingCofactor(1)](0, locn) *
--     [lift: RingCofactor(1)](1, dateid) *
--     [lift: RingCofactor(1)](2, ksn) *
--     [lift: RingCofactor(1)](3, zip) *
--     [lift: RingCofactor(1)](4, inventoryunits) *
--     [lift: RingCofactor(13)](5, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
--     [lift: RingCofactor(15)](18, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
--     [lift: RingCofactor(4)](33, subcategory, category, categoryCluster, prize) *
--     [lift: RingCofactor(6)](37, rain, snow, maxtemp, mintemp, meanwind, thunder)
-- )
-- FROM INVENTORY NATURAL JOIN LOCATION NATURAL JOIN CENSUS NATURAL JOIN ITEM NATURAL JOIN WEATHER;
