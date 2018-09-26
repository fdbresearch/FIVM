CREATE STREAM Inventory(locn int, dateid int, ksn double, inventoryunits double)
FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Location(locn int, zip int, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Census(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage double, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Item(ksn double, subcategory int, category int, categoryCluster int, prize double) 
FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Weather(locn int, dateid int, rain int, snow int, maxtemp int, mintemp int, meanwind double, thunder int) 
FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(inventoryunits * inventoryunits)
FROM Inventory NATURAL JOIN Location NATURAL JOIN Census NATURAL JOIN Item NATURAL JOIN Weather;
