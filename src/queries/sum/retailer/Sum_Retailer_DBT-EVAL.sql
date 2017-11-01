CREATE TABLE INVENTORY(locn int, dateid int, ksn float, inventoryunits float)
FROM FILE './datasets/usretailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE LOCATION(locn int, zip int, rgn_cd float, clim_zn_nbr float, tot_area_sq_ft float, sell_area_sq_ft float, avghhi float, supertargetdistance float, supertargetdrivetime float, targetdistance float, targetdrivetime float, walmartdistance float, walmartdrivetime float, walmartsupercenterdistance float, walmartsupercenterdrivetime float)
FROM FILE './datasets/usretailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage float, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/usretailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE ITEM(ksn float, subcategory int, category int, categoryCluster int, prize float) 
FROM FILE './datasets/usretailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE WEATHER(locn int, dateid int, rain int, snow int, maxtemp int, mintemp int, meanwind float, thunder int) 
FROM FILE './datasets/usretailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(inventoryunits * inventoryunits)
FROM Inventory NATURAL JOIN Location NATURAL JOIN Census NATURAL JOIN Item NATURAL JOIN Weather;
