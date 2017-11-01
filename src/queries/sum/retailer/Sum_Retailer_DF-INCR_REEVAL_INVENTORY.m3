-- Incremental computation of a sum aggregate for batch updates to INVENTORY
--


 -------------------- SOURCES --------------------
CREATE STREAM INVENTORY(locn int, dateid int, ksn float, inventoryunits float)
FROM FILE './datasets/usretailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE LOCATION(locn int, zip int, rgn_cd float, clim_zn_nbr float, tot_area_sq_ft float, sell_area_sq_ft float, avghhi float, supertargetdistance float, supertargetdrivetime float, targetdistance float, targetdrivetime float, walmartdistance float, walmartdrivetime float, walmartsupercenterdistance float, walmartsupercenterdrivetime float)
FROM FILE './datasets/usretailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage float, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/usretailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE ITEM(ksn float, subcategory int, category int, categorycluster int, prize float) 
FROM FILE './datasets/usretailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE WEATHER(locn int, dateid int, rain int, snow int, maxtemp int, mintemp int, meanwind float, thunder int) 
FROM FILE './datasets/usretailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP BASE_INVENTORY(int)[][locn:int, dateid:int, ksn:float, inventoryunits:float] := 0;
DECLARE MAP AGG_Location(float)[][locn:int, zip:int] := 0;
DECLARE MAP AGG_Census(float)[][zip:int] := 0;
DECLARE MAP AGG_Item(float)[][ksn:float] := 0;
DECLARE MAP AGG_Weather(float)[][locn:int, dateid:int] := 0;

DECLARE MAP AGG_LC(float)[][locn:int] := 0;
DECLARE MAP AGG(float)[][] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
    
    BASE_INVENTORY(int)[][locn, dateid, ksn, inventoryunits] += (DELTA INVENTORY)(locn, dateid, ksn, inventoryunits);

    AGG(float)[][] += AggSum([], 
        AggSum([locn], 
            AggSum([locn, dateid], 
                AggSum([locn, dateid, ksn], BASE_INVENTORY(int)[][locn, dateid, ksn, inventoryunits] * inventoryunits * inventoryunits) * 
                AGG_Item(float)[][ksn]
            ) * 
            AGG_Weather(float)[][locn, dateid]
        ) * 
        AGG_LC(float)[][locn]
    );
}

ON SYSTEM READY {

    AGG_Location(float)[][locn, zip] := AggSum([locn, zip], LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime));

    AGG_Census(float)[][zip] := AggSum([zip], CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic));

    AGG_Item(float)[][ksn] := AggSum([ksn], ITEM(ksn, subcategory, category, categorycluster, prize));

    AGG_Weather(float)[][locn, dateid] := AggSum([locn, dateid], WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder));

    AGG_LC(float)[][locn] := AggSum([locn], AGG_Location(float)[][locn, zip] * AGG_Census(float)[][zip]);
}