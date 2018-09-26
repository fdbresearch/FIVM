-- Incremental computation of the cofactor matrix using the pivot method for batch updates to ITEM
--
-- Keeps a sum of degrees in each map schema

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

CREATE TABLE DEGREE15(Deg1 int, Deg2 int, Deg3 int, Deg4 int, Deg5 int, Deg6 int, Deg7 int, Deg8 int, Deg9 int, Deg10 int, Deg11 int, Deg12 int, Deg13 int, Deg14 int, Deg15 int)
FROM FILE './datasets/degree/Degree15.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE13(Deg1 int, Deg2 int, Deg3 int, Deg4 int, Deg5 int, Deg6 int, Deg7 int, Deg8 int, Deg9 int, Deg10 int, Deg11 int, Deg12 int, Deg13 int)
FROM FILE './datasets/degree/Degree13.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE6(Deg1 int, Deg2 int, Deg3 int, Deg4 int, Deg5 int, Deg6 int)
FROM FILE './datasets/degree/Degree6.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE4(Deg1 int, Deg2 int, Deg3 int, Deg4 int)
FROM FILE './datasets/degree/Degree4.tbl' LINE DELIMITED CSV(delimiter := '|');

--CREATE TABLE DEGREE3(Deg1 int, Deg2 int, Deg3 int)
--FROM FILE './datasets/degree/Degree3.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE1(Deg1 int)
FROM FILE './datasets/degree/Degree1.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP AGG_Inventory(float)[][locn:int, dateid:int, ksn:float, Deg:int, Deg_inventoryunits:int] := 0;
DECLARE MAP AGG_Location(float)[][locn:int, zip:int, Deg:int, Deg_rgn_cd:int, Deg_clim_zn_nbr:int, Deg_tot_area_sq_ft:int, Deg_sell_area_sq_ft:int, Deg_avghhi:int, Deg_supertargetdistance:int, Deg_supertargetdrivetime:int, Deg_targetdistance:int, Deg_targetdrivetime:int, Deg_walmartdistance:int, Deg_walmartdrivetime:int, Deg_walmartsupercenterdistance:int, Deg_walmartsupercenterdrivetime:int] := 0;
DECLARE MAP AGG_Census(float)[][zip:int, Deg:int, Deg_population:int, Deg_white:int, Deg_asian:int, Deg_pacific:int, Deg_blackafrican:int, Deg_medianage:int, Deg_occupiedhouseunits:int, Deg_houseunits:int, Deg_families:int, Deg_households:int, Deg_husbwife:int, Deg_males:int, Deg_females:int, Deg_householdschildren:int, Deg_hispanic:int] := 0;
DECLARE MAP AGG_Item(float)[][ksn:float, Deg:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int] := 0;
DECLARE MAP AGG_Weather(float)[][locn:int, dateid:int, Deg:int, Deg_rain:int, Deg_snow:int, Deg_maxtemp:int, Deg_mintemp:int, Deg_meanwind:int, Deg_thunder:int] := 0;

DECLARE MAP AGG_LC(float)[][locn:int, Deg:int, Deg_zip:int, Deg_rgn_cd:int, Deg_clim_zn_nbr:int, Deg_tot_area_sq_ft:int, Deg_sell_area_sq_ft:int, Deg_avghhi:int, Deg_supertargetdistance:int, Deg_supertargetdrivetime:int, Deg_targetdistance:int, Deg_targetdrivetime:int, Deg_walmartdistance:int, Deg_walmartdrivetime:int, Deg_walmartsupercenterdistance:int, Deg_walmartsupercenterdrivetime:int, Deg_population:int, Deg_white:int, Deg_asian:int, Deg_pacific:int, Deg_blackafrican:int, Deg_medianage:int, Deg_occupiedhouseunits:int, Deg_houseunits:int, Deg_families:int, Deg_households:int, Deg_husbwife:int, Deg_males:int, Deg_females:int, Deg_householdschildren:int, Deg_hispanic:int] := 0;
DECLARE MAP AGG_InIt(float)[][locn:int, dateid:int, Deg:int, Deg_ksn:int, Deg_inventoryunits:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int] := 0;
DECLARE MAP AGG_InItW(float)[][locn:int, Deg:int, Deg_dateid:int, Deg_ksn:int, Deg_inventoryunits:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int, Deg_rain:int, Deg_snow:int, Deg_maxtemp:int, Deg_mintemp:int, Deg_meanwind:int, Deg_thunder:int] := 0;
DECLARE MAP AGG(float)[][Deg_locn:int, Deg_dateid:int, Deg_ksn:int, Deg_inventoryunits:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int, Deg_rain:int, Deg_snow:int, Deg_maxtemp:int, Deg_mintemp:int, Deg_meanwind:int, Deg_thunder:int, Deg_zip:int, Deg_rgn_cd:int, Deg_clim_zn_nbr:int, Deg_tot_area_sq_ft:int, Deg_sell_area_sq_ft:int, Deg_avghhi:int, Deg_supertargetdistance:int, Deg_supertargetdrivetime:int, Deg_targetdistance:int, Deg_targetdrivetime:int, Deg_walmartdistance:int, Deg_walmartdrivetime:int, Deg_walmartsupercenterdistance:int, Deg_walmartsupercenterdrivetime:int, Deg_population:int, Deg_white:int, Deg_asian:int, Deg_pacific:int, Deg_blackafrican:int, Deg_medianage:int, Deg_occupiedhouseunits:int, Deg_houseunits:int, Deg_families:int, Deg_households:int, Deg_husbwife:int, Deg_males:int, Deg_females:int, Deg_householdschildren:int, Deg_hispanic:int] := 0;

DECLARE MAP DELTA_AGG_Inventory(float)[][locn:int, dateid:int, ksn:float, Deg:int, Deg_inventoryunits:int] := 0;
DECLARE MAP DELTA_AGG_Location(float)[][locn:int, zip:int, Deg:int, Deg_rgn_cd:int, Deg_clim_zn_nbr:int, Deg_tot_area_sq_ft:int, Deg_sell_area_sq_ft:int, Deg_avghhi:int, Deg_supertargetdistance:int, Deg_supertargetdrivetime:int, Deg_targetdistance:int, Deg_targetdrivetime:int, Deg_walmartdistance:int, Deg_walmartdrivetime:int, Deg_walmartsupercenterdistance:int, Deg_walmartsupercenterdrivetime:int] := 0;
DECLARE MAP DELTA_AGG_Census(float)[][zip:int, Deg:int, Deg_population:int, Deg_white:int, Deg_asian:int, Deg_pacific:int, Deg_blackafrican:int, Deg_medianage:int, Deg_occupiedhouseunits:int, Deg_houseunits:int, Deg_families:int, Deg_households:int, Deg_husbwife:int, Deg_males:int, Deg_females:int, Deg_householdschildren:int, Deg_hispanic:int] := 0;
DECLARE MAP DELTA_AGG_Item(float)[][ksn:float, Deg:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int] := 0;
DECLARE MAP DELTA_AGG_Weather(float)[][locn:int, dateid:int, Deg:int, Deg_rain:int, Deg_snow:int, Deg_maxtemp:int, Deg_mintemp:int, Deg_meanwind:int, Deg_thunder:int] := 0;

DECLARE MAP DELTA_AGG_LC(float)[][locn:int, Deg:int, Deg_zip:int, Deg_rgn_cd:int, Deg_clim_zn_nbr:int, Deg_tot_area_sq_ft:int, Deg_sell_area_sq_ft:int, Deg_avghhi:int, Deg_supertargetdistance:int, Deg_supertargetdrivetime:int, Deg_targetdistance:int, Deg_targetdrivetime:int, Deg_walmartdistance:int, Deg_walmartdrivetime:int, Deg_walmartsupercenterdistance:int, Deg_walmartsupercenterdrivetime:int, Deg_population:int, Deg_white:int, Deg_asian:int, Deg_pacific:int, Deg_blackafrican:int, Deg_medianage:int, Deg_occupiedhouseunits:int, Deg_houseunits:int, Deg_families:int, Deg_households:int, Deg_husbwife:int, Deg_males:int, Deg_females:int, Deg_householdschildren:int, Deg_hispanic:int] := 0;
DECLARE MAP DELTA_AGG_InIt(float)[][locn:int, dateid:int, Deg:int, Deg_ksn:int, Deg_inventoryunits:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int] := 0;
DECLARE MAP DELTA_AGG_InItW(float)[][locn:int, Deg:int, Deg_dateid:int, Deg_ksn:int, Deg_inventoryunits:int, Deg_subcategory:int, Deg_category:int, Deg_categorycluster:int, Deg_prize:int, Deg_rain:int, Deg_snow:int, Deg_maxtemp:int, Deg_mintemp:int, Deg_meanwind:int, Deg_thunder:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
    
    DELTA_AGG_Inventory(float)[][locn, dateid, ksn, Deg, Deg_inventoryunits] := 
        AggSum([locn, dateid, ksn, Deg, Deg_inventoryunits],

            (DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) *
        
            DEGREE1(Deg_inventoryunits) *
        
            ( ({Deg_inventoryunits = 0}) + ({Deg_inventoryunits = 1} * inventoryunits) + ({Deg_inventoryunits = 2} * inventoryunits * inventoryunits) ) *

            (Deg ^= {Deg_inventoryunits})
        );

    DELTA_AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] :=
        AggSum([locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize],
           
            DELTA_AGG_Inventory(float)[][locn, dateid, ksn, Deg1, Deg_inventoryunits] *

            AGG_Item(float)[][ksn, Deg2, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] *

            DEGREE1(Deg_ksn) *

            (Deg ^= {Deg_ksn + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_ksn = 0}) + ({Deg_ksn = 1} * ksn) + ({Deg_ksn = 2} * ksn * ksn) )
        );

    DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] :=
        AggSum([locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder],
           
            DELTA_AGG_InIt(float)[][locn, dateid, Deg1, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] *

            AGG_Weather(float)[][locn, dateid, Deg2, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *

            DEGREE1(Deg_dateid) *

            (Deg ^= {Deg_dateid + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_dateid = 0}) + ({Deg_dateid = 1} * dateid) + ({Deg_dateid = 2} * dateid * dateid) )
        );

   AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] += 
       AggSum([Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic], 
           
           DELTA_AGG_InItW(float)[][locn, Deg1, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *
           
           AGG_LC(float)[][locn, Deg2, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

           DEGREE1(Deg_locn) *

           (Deg ^= {Deg_locn + Deg1 + Deg2}) *

           {Deg = 2} *

           ( ({Deg_locn = 0}) + ({Deg_locn = 1} * locn) + ({Deg_locn = 2} * locn * locn) )
       );

    AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] +=
        DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder];

    AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] +=
        DELTA_AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize];

    AGG_Inventory(float)[][locn, dateid, ksn, Deg, Deg_inventoryunits] +=
        DELTA_AGG_Inventory(float)[][locn, dateid, ksn, Deg, Deg_inventoryunits];
}

ON BATCH UPDATE OF LOCATION {

    DELTA_AGG_Location(float)[][locn, zip, Deg, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime] := 
        AggSum([locn, zip, Deg, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime],
            
            (DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) *
            
            DEGREE13(Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime) *

            ( ({Deg_rgn_cd = 0}) + ({Deg_rgn_cd = 1} * rgn_cd) + ({Deg_rgn_cd = 2} * rgn_cd * rgn_cd) ) *
            ( ({Deg_clim_zn_nbr = 0}) + ({Deg_clim_zn_nbr = 1} * clim_zn_nbr) + ({Deg_clim_zn_nbr = 2} * clim_zn_nbr * clim_zn_nbr) ) *
            ( ({Deg_tot_area_sq_ft = 0}) + ({Deg_tot_area_sq_ft = 1} * tot_area_sq_ft) + ({Deg_tot_area_sq_ft = 2} * tot_area_sq_ft * tot_area_sq_ft) ) *
            ( ({Deg_sell_area_sq_ft = 0}) + ({Deg_sell_area_sq_ft = 1} * sell_area_sq_ft) + ({Deg_sell_area_sq_ft = 2} * sell_area_sq_ft * sell_area_sq_ft) ) *
            ( ({Deg_avghhi = 0}) + ({Deg_avghhi = 1} * avghhi) + ({Deg_avghhi = 2} * avghhi * avghhi) ) *
            ( ({Deg_supertargetdistance = 0}) + ({Deg_supertargetdistance = 1} * supertargetdistance) + ({Deg_supertargetdistance = 2} * supertargetdistance * supertargetdistance) ) *
            ( ({Deg_supertargetdrivetime = 0}) + ({Deg_supertargetdrivetime = 1} * supertargetdrivetime) + ({Deg_supertargetdrivetime = 2} * supertargetdrivetime * supertargetdrivetime) ) *
            ( ({Deg_targetdistance = 0}) + ({Deg_targetdistance = 1} * targetdistance) + ({Deg_targetdistance = 2} * targetdistance * targetdistance) ) *
            ( ({Deg_targetdrivetime = 0}) + ({Deg_targetdrivetime = 1} * targetdrivetime) + ({Deg_targetdrivetime = 2} * targetdrivetime * targetdrivetime) ) *
            ( ({Deg_walmartdistance = 0}) + ({Deg_walmartdistance = 1} * walmartdistance) + ({Deg_walmartdistance = 2} * walmartdistance * walmartdistance) ) *
            ( ({Deg_walmartdrivetime = 0}) + ({Deg_walmartdrivetime = 1} * walmartdrivetime) + ({Deg_walmartdrivetime = 2} * walmartdrivetime * walmartdrivetime) ) *
            ( ({Deg_walmartsupercenterdistance = 0}) + ({Deg_walmartsupercenterdistance = 1} * walmartsupercenterdistance) + ({Deg_walmartsupercenterdistance = 2} * walmartsupercenterdistance * walmartsupercenterdistance) ) *
            ( ({Deg_walmartsupercenterdrivetime = 0}) + ({Deg_walmartsupercenterdrivetime = 1} * walmartsupercenterdrivetime) + ({Deg_walmartsupercenterdrivetime = 2} * walmartsupercenterdrivetime * walmartsupercenterdrivetime) ) *

            (Deg ^= {Deg_rgn_cd + Deg_clim_zn_nbr + Deg_tot_area_sq_ft + Deg_sell_area_sq_ft + Deg_avghhi + Deg_supertargetdistance + Deg_supertargetdrivetime + Deg_targetdistance + Deg_targetdrivetime + Deg_walmartdistance + Deg_walmartdrivetime + Deg_walmartsupercenterdistance + Deg_walmartsupercenterdrivetime})
        );

    DELTA_AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] := 
        AggSum([locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic],

            DELTA_AGG_Location(float)[][locn, zip, Deg1, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime] *
            
            AGG_Census(float)[][zip, Deg2, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

            DEGREE1(Deg_zip) *

            (Deg ^= {Deg_zip + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_zip = 0}) + ({Deg_zip = 1} * zip) + ({Deg_zip = 2} * zip * zip) ) 
        );
    
   AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] += 
       AggSum([Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic], 
           
           DELTA_AGG_LC(float)[][locn, Deg2, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

           AGG_InItW(float)[][locn, Deg1, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *
           
           DEGREE1(Deg_locn) *

           (Deg ^= {Deg_locn + Deg1 + Deg2}) *

           {Deg = 2} *

           ( ({Deg_locn = 0}) + ({Deg_locn = 1} * locn) + ({Deg_locn = 2} * locn * locn) )
       );

    AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] +=
        DELTA_AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic];

    AGG_Location(float)[][locn, zip, Deg, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime] +=
        DELTA_AGG_Location(float)[][locn, zip, Deg, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime];
}

ON BATCH UPDATE OF CENSUS {

    DELTA_AGG_Census(float)[][zip, Deg, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] := 
        AggSum([zip, Deg, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic],

            (DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) *
        
            DEGREE15(Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic) *
        
            ( ({Deg_population = 0}) + ({Deg_population = 1} * population) + ({Deg_population = 2} * population * population) ) *
            ( ({Deg_white = 0}) + ({Deg_white = 1} * white) + ({Deg_white = 2} * white * white) ) *
            ( ({Deg_asian = 0}) + ({Deg_asian = 1} * asian) + ({Deg_asian = 2} * asian * asian) ) *
            ( ({Deg_pacific = 0}) + ({Deg_pacific = 1} * pacific) + ({Deg_pacific = 2} * pacific * pacific) ) *
            ( ({Deg_blackafrican = 0}) + ({Deg_blackafrican = 1} * blackafrican) + ({Deg_blackafrican = 2} * blackafrican * blackafrican) ) *
            ( ({Deg_medianage = 0}) + ({Deg_medianage = 1} * medianage) + ({Deg_medianage = 2} * medianage * medianage) ) *
            ( ({Deg_occupiedhouseunits = 0}) + ({Deg_occupiedhouseunits = 1} * occupiedhouseunits) + ({Deg_occupiedhouseunits = 2} * occupiedhouseunits * occupiedhouseunits) ) *
            ( ({Deg_houseunits = 0}) + ({Deg_houseunits = 1} * houseunits) + ({Deg_houseunits = 2} * houseunits * houseunits) ) *
            ( ({Deg_families = 0}) + ({Deg_families = 1} * families) + ({Deg_families = 2} * families * families) ) *
            ( ({Deg_households = 0}) + ({Deg_households = 1} * households) + ({Deg_households = 2} * households * households) ) *
            ( ({Deg_husbwife = 0}) + ({Deg_husbwife = 1} * husbwife) + ({Deg_husbwife = 2} * husbwife * husbwife) ) *
            ( ({Deg_males = 0}) + ({Deg_males = 1} * males) + ({Deg_males = 2} * males * males) ) *
            ( ({Deg_females = 0}) + ({Deg_females = 1} * females) + ({Deg_females = 2} * females * females) ) *
            ( ({Deg_householdschildren = 0}) + ({Deg_householdschildren = 1} * householdschildren) + ({Deg_householdschildren = 2} * householdschildren * householdschildren) ) *
            ( ({Deg_hispanic = 0}) + ({Deg_hispanic = 1} * hispanic) + ({Deg_hispanic = 2} * hispanic * hispanic) ) *

            (Deg ^= {Deg_population + Deg_white + Deg_asian + Deg_pacific + Deg_blackafrican + Deg_medianage + Deg_occupiedhouseunits + Deg_houseunits + Deg_families + Deg_households + Deg_husbwife + Deg_males + Deg_females + Deg_householdschildren + Deg_hispanic})
        );
    
    DELTA_AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] := 
        AggSum([locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic],

            DELTA_AGG_Census(float)[][zip, Deg2, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

            AGG_Location(float)[][locn, zip, Deg1, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime] *
            
            DEGREE1(Deg_zip) *

            (Deg ^= {Deg_zip + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_zip = 0}) + ({Deg_zip = 1} * zip) + ({Deg_zip = 2} * zip * zip) ) 
        );

    AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] += 
       AggSum([Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic], 
           
           DELTA_AGG_LC(float)[][locn, Deg2, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

           AGG_InItW(float)[][locn, Deg1, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *
           
           DEGREE1(Deg_locn) *

           (Deg ^= {Deg_locn + Deg1 + Deg2}) *

           {Deg = 2} *

           ( ({Deg_locn = 0}) + ({Deg_locn = 1} * locn) + ({Deg_locn = 2} * locn * locn) )
       );

    AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] +=
        DELTA_AGG_LC(float)[][locn, Deg, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic];

    AGG_Census(float)[][zip, Deg, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] +=
        DELTA_AGG_Census(float)[][zip, Deg, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic];
}

ON BATCH UPDATE OF ITEM {

    DELTA_AGG_Item(float)[][ksn, Deg, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] :=
        AggSum([ksn, Deg, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize],
           
            (DELTA ITEM)(ksn, subcategory, category, categorycluster, prize) *
           
            DEGREE4(Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize) *
           
            ( ({Deg_subcategory = 0}) + ({Deg_subcategory = 1} * subcategory) + ({Deg_subcategory = 2} * subcategory * subcategory) ) *
            ( ({Deg_category = 0}) + ({Deg_category = 1} * category) + ({Deg_category = 2} * category * category) ) *
            ( ({Deg_categorycluster = 0}) + ({Deg_categorycluster = 1} * categorycluster) + ({Deg_categorycluster = 2} * categorycluster * categorycluster) ) *
            ( ({Deg_prize = 0}) + ({Deg_prize = 1} * prize) + ({Deg_prize = 2} * prize * prize) ) *

            (Deg ^= {Deg_subcategory + Deg_category + Deg_categorycluster + Deg_prize})
        );

    DELTA_AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] :=
        AggSum([locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize],
           
            DELTA_AGG_Item(float)[][ksn, Deg2, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] *

            AGG_Inventory(float)[][locn, dateid, ksn, Deg1, Deg_inventoryunits] *

            DEGREE1(Deg_ksn) *

            (Deg ^= {Deg_ksn + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_ksn = 0}) + ({Deg_ksn = 1} * ksn) + ({Deg_ksn = 2} * ksn * ksn) )
        );

    DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] :=
        AggSum([locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder],
           
            DELTA_AGG_InIt(float)[][locn, dateid, Deg1, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] *

            AGG_Weather(float)[][locn, dateid, Deg2, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *

            DEGREE1(Deg_dateid) *

            (Deg ^= {Deg_dateid + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_dateid = 0}) + ({Deg_dateid = 1} * dateid) + ({Deg_dateid = 2} * dateid * dateid) )
        );

   AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] += 
       AggSum([Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic], 
           
           DELTA_AGG_InItW(float)[][locn, Deg1, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *
           
           AGG_LC(float)[][locn, Deg2, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

           DEGREE1(Deg_locn) *

           (Deg ^= {Deg_locn + Deg1 + Deg2}) *

           {Deg = 2} *

           ( ({Deg_locn = 0}) + ({Deg_locn = 1} * locn) + ({Deg_locn = 2} * locn * locn) )
       );

    AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] +=
        DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder];

    AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] +=
        DELTA_AGG_InIt(float)[][locn, dateid, Deg, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize];

    AGG_Item(float)[][ksn, Deg, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] += 
        DELTA_AGG_Item(float)[][ksn, Deg, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize];
}

ON BATCH UPDATE OF WEATHER {

    DELTA_AGG_Weather(float)[][locn, dateid, Deg, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] :=
        AggSum([locn, dateid, Deg, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder],
           
            DELTA_WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) *
           
            DEGREE6(Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder) *
           
            ( ({Deg_rain = 0}) + ({Deg_rain = 1} * rain) + ({Deg_rain = 2} * rain * rain) ) *
            ( ({Deg_snow = 0}) + ({Deg_snow = 1} * snow) + ({Deg_snow = 2} * snow * snow) ) *
            ( ({Deg_maxtemp = 0}) + ({Deg_maxtemp = 1} * maxtemp) + ({Deg_maxtemp = 2} * maxtemp * maxtemp) ) *
            ( ({Deg_mintemp = 0}) + ({Deg_mintemp = 1} * mintemp) + ({Deg_mintemp = 2} * mintemp * mintemp) ) *
            ( ({Deg_meanwind = 0}) + ({Deg_meanwind = 1} * meanwind) + ({Deg_meanwind = 2} * meanwind * meanwind) ) *
            ( ({Deg_thunder = 0}) + ({Deg_thunder = 1} * thunder) + ({Deg_thunder = 2} * thunder * thunder) ) *

            (Deg ^= {Deg_rain + Deg_snow + Deg_maxtemp + Deg_mintemp + Deg_meanwind + Deg_thunder})
        );

    DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] :=
        AggSum([locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder],
           
            DELTA_AGG_Weather(float)[][locn, dateid, Deg2, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *

            AGG_InIt(float)[][locn, dateid, Deg1, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize] *           

            DEGREE1(Deg_dateid) *

            (Deg ^= {Deg_dateid + Deg1 + Deg2}) *

            {Deg <= 2} *

            ( ({Deg_dateid = 0}) + ({Deg_dateid = 1} * dateid) + ({Deg_dateid = 2} * dateid * dateid) )
        );

    AGG(float)[][Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] += 
       AggSum([Deg_locn, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic], 
           
           DELTA_AGG_InItW(float)[][locn, Deg1, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] *
           
           AGG_LC(float)[][locn, Deg2, Deg_zip, Deg_rgn_cd, Deg_clim_zn_nbr, Deg_tot_area_sq_ft, Deg_sell_area_sq_ft, Deg_avghhi, Deg_supertargetdistance, Deg_supertargetdrivetime, Deg_targetdistance, Deg_targetdrivetime, Deg_walmartdistance, Deg_walmartdrivetime, Deg_walmartsupercenterdistance, Deg_walmartsupercenterdrivetime, Deg_population, Deg_white, Deg_asian, Deg_pacific, Deg_blackafrican, Deg_medianage, Deg_occupiedhouseunits, Deg_houseunits, Deg_families, Deg_households, Deg_husbwife, Deg_males, Deg_females, Deg_householdschildren, Deg_hispanic] *

           DEGREE1(Deg_locn) *

           (Deg ^= {Deg_locn + Deg1 + Deg2}) *

           {Deg = 2} *

           ( ({Deg_locn = 0}) + ({Deg_locn = 1} * locn) + ({Deg_locn = 2} * locn * locn) )
       );

    AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] +=
        DELTA_AGG_InItW(float)[][locn, Deg, Deg_dateid, Deg_ksn, Deg_inventoryunits, Deg_subcategory, Deg_category, Deg_categorycluster, Deg_prize, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder];

    AGG_Weather(float)[][locn, dateid, Deg, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder] +=
        DELTA_AGG_Weather(float)[][locn, dateid, Deg, Deg_rain, Deg_snow, Deg_maxtemp, Deg_mintemp, Deg_meanwind, Deg_thunder];

}

ON SYSTEM READY {
}