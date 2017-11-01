-- Recomputing a sum aggregate for streams
--
--

-------------------- SOURCES --------------------
CREATE STREAM INVENTORY(locn int, dateid int, ksn float, inventoryunits float)
FROM FILE './datasets/usretailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM LOCATION(locn int, zip int, rgn_cd float, clim_zn_nbr float, tot_area_sq_ft float, sell_area_sq_ft float, avghhi float, supertargetdistance float, supertargetdrivetime float, targetdistance float, targetdrivetime float, walmartdistance float, walmartdrivetime float, walmartsupercenterdistance float, walmartsupercenterdrivetime float)
FROM FILE './datasets/usretailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage float, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
FROM FILE './datasets/usretailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM ITEM(ksn float, subcategory int, category int, categorycluster int, prize float) 
FROM FILE './datasets/usretailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM WEATHER(locn int, dateid int, rain int, snow int, maxtemp int, mintemp int, meanwind float, thunder int) 
FROM FILE './datasets/usretailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP BASE_INVENTORY(int)[][locn:int, dateid:int, ksn:float, inventoryunits:float] := 0;
DECLARE MAP BASE_LOCATION(int)[][locn:int, zip:int, rgn_cd:float, clim_zn_nbr:float, tot_area_sq_ft:float, sell_area_sq_ft:float, avghhi:float, supertargetdistance:float, supertargetdrivetime:float, targetdistance:float, targetdrivetime:float, walmartdistance:float, walmartdrivetime:float, walmartsupercenterdistance:float, walmartsupercenterdrivetime:float] := 0;
DECLARE MAP BASE_CENSUS(int)[][zip:int, population:int, white:int, asian:int, pacific:int, blackafrican:int, medianage:float, occupiedhouseunits:int, houseunits:int, families:int, households:int, husbwife:int, males:int, females:int, householdschildren:int, hispanic:int] := 0;
DECLARE MAP BASE_ITEM(int)[][ksn:float, subcategory:int, category:int, categorycluster:int, prize:float] := 0;
DECLARE MAP BASE_WEATHER(int)[][locn:int, dateid:int, rain:int, snow:int, maxtemp:int, mintemp:int, meanwind:float, thunder:int] := 0;
DECLARE MAP AGG(float)[][] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
    
    BASE_INVENTORY(int)[][locn, dateid, ksn, inventoryunits] += (DELTA INVENTORY)(locn, dateid, ksn, inventoryunits);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([locn],                 
                AggSum([locn, dateid], 
                    AggSum([locn, dateid, ksn], BASE_INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits) * 
                    AggSum([ksn], BASE_ITEM(ksn, subcategory, category, categorycluster, prize))
                ) *
                AggSum([locn, dateid], BASE_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder))
            ) * 
            AggSum([locn], 
                AggSum([locn, zip], BASE_LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * 
                AggSum([zip], BASE_CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
            )
        );
}

ON BATCH UPDATE OF LOCATION {

    BASE_LOCATION(int)[][locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime] += (DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([locn],                 
                AggSum([locn, dateid], 
                    AggSum([locn, dateid, ksn], BASE_INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits) * 
                    AggSum([ksn], BASE_ITEM(ksn, subcategory, category, categorycluster, prize))
                ) *
                AggSum([locn, dateid], BASE_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder))
            ) * 
            AggSum([locn], 
                AggSum([locn, zip], BASE_LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * 
                AggSum([zip], BASE_CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
            )
        );
}

ON BATCH UPDATE OF CENSUS {

    BASE_CENSUS(int)[][zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic] += (DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([locn],                 
                AggSum([locn, dateid], 
                    AggSum([locn, dateid, ksn], BASE_INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits) * 
                    AggSum([ksn], BASE_ITEM(ksn, subcategory, category, categorycluster, prize))
                ) *
                AggSum([locn, dateid], BASE_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder))
            ) * 
            AggSum([locn], 
                AggSum([locn, zip], BASE_LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * 
                AggSum([zip], BASE_CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
            )
        );
}

ON BATCH UPDATE OF ITEM {

    BASE_ITEM(int)[][ksn, subcategory, category, categorycluster, prize] += (DELTA ITEM)(ksn, subcategory, category, categorycluster, prize);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([locn],                 
                AggSum([locn, dateid], 
                    AggSum([locn, dateid, ksn], BASE_INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits) * 
                    AggSum([ksn], BASE_ITEM(ksn, subcategory, category, categorycluster, prize))
                ) *
                AggSum([locn, dateid], BASE_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder))
            ) * 
            AggSum([locn], 
                AggSum([locn, zip], BASE_LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * 
                AggSum([zip], BASE_CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
            )
        );
}

ON BATCH UPDATE OF WEATHER {
    
    BASE_WEATHER(int)[][locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder] += (DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder);
    
    AGG(float)[][] := 
        AggSum([], 
            AggSum([locn],                 
                AggSum([locn, dateid], 
                    AggSum([locn, dateid, ksn], BASE_INVENTORY(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits) * 
                    AggSum([ksn], BASE_ITEM(ksn, subcategory, category, categorycluster, prize))
                ) *
                AggSum([locn, dateid], BASE_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder))
            ) * 
            AggSum([locn], 
                AggSum([locn, zip], BASE_LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)) * 
                AggSum([zip], BASE_CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic))
            )
        );
}

ON SYSTEM READY {
}