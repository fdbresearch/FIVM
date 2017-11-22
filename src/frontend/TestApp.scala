package fivm

import fivm.lib.Utils._
import fivm.frontend._

object TestApp {
 
    def main(args: Array[String]) = {

        println("--------- \nSQL input: \n --------- \n" + testSQL)

        println("--------- \nSQL AST: \n --------- \n" + SQLParser.apply(testSQL))

        println("--------- \nM3 input: \n --------- \n" + testM3)

        println("--------- \nM3 AST: \n --------- \n" + M3Parser.apply(testM3))
    }

    val testSQL = 
        """|CREATE STREAM R(A int, B int)
           |FROM FILE 'experiments/data/r.dat' LINE DELIMITED
           |csv ();

           |CREATE TABLE S(C int, D int)
           |FROM FILE 'experiments/data/s.dat' LINE DELIMITED
           |csv ();

           |SELECT R.* FROM R JOIN S ON R.B = S.C;""".stripMargin

    val testM3 = 
        """ |-- Incremental computation of a sum aggregate for batch updates to all relations
            |--
            |
            |-------------------- SOURCES --------------------
            |CREATE STREAM INVENTORY(locn int, dateid int, ksn float, inventoryunits float)
            |FROM FILE './datasets/usretailer/Inventory.tbl' LINE DELIMITED CSV(delimiter := '|');
            |
            |CREATE STREAM LOCATION(locn int, zip int, rgn_cd float, clim_zn_nbr float, tot_area_sq_ft float, sell_area_sq_ft float, avghhi float, supertargetdistance float, supertargetdrivetime float, targetdistance float, targetdrivetime float, walmartdistance float, walmartdrivetime float, walmartsupercenterdistance float, walmartsupercenterdrivetime float)
            |FROM FILE './datasets/usretailer/Location.tbl' LINE DELIMITED CSV(delimiter := '|');
            |
            |CREATE STREAM CENSUS(zip int, population int, white int, asian int, pacific int, blackafrican int, medianage float, occupiedhouseunits int, houseunits int, families int, households int, husbwife int, males int, females int, householdschildren int, hispanic int)
            |FROM FILE './datasets/usretailer/Census.tbl' LINE DELIMITED CSV(delimiter := '|');
            |
            |CREATE STREAM ITEM(ksn float, subcategory int, category int, categorycluster int, prize float) 
            |FROM FILE './datasets/usretailer/Item.tbl' LINE DELIMITED CSV(delimiter := '|');
            |
            |CREATE STREAM WEATHER(locn int, dateid int, rain int, snow int, maxtemp int, mintemp int, meanwind float, thunder int) 
            |FROM FILE './datasets/usretailer/Weather.tbl' LINE DELIMITED CSV(delimiter := '|');
            |
            |--------------------- MAPS ----------------------
            |DECLARE MAP AGG_Inventory(float)[][locn:int, dateid:int, ksn:float] := 0;
            |DECLARE MAP AGG_Location(float)[][locn:int, zip:int] := 0;
            |DECLARE MAP AGG_Census(float)[][zip:int] := 0;
            |DECLARE MAP AGG_Item(float)[][ksn:float] := 0;
            |DECLARE MAP AGG_Weather(float)[][locn:int, dateid:int] := 0;
            |
            |DECLARE MAP AGG_LC(float)[][locn:int] := 0;
            |DECLARE MAP AGG_InIt(float)[][locn:int, dateid:int] := 0;
            |DECLARE MAP AGG_InItW(float)[][locn:int] := 0;
            |DECLARE MAP AGG(float)[][] := 0;
            |
            |DECLARE MAP DELTA_AGG_Inventory(float)[][locn:int, dateid:int, ksn:float] := 0;
            |DECLARE MAP DELTA_AGG_Location(float)[][locn:int, zip:int] := 0;
            |DECLARE MAP DELTA_AGG_Census(float)[][zip:int] := 0;
            |DECLARE MAP DELTA_AGG_Item(float)[][ksn:float] := 0;
            |DECLARE MAP DELTA_AGG_Weather(float)[][locn:int, dateid:int] := 0;
            |
            |DECLARE MAP DELTA_AGG_LC(float)[][locn:int] := 0;
            |DECLARE MAP DELTA_AGG_InIt(float)[][locn:int, dateid:int] := 0;
            |DECLARE MAP DELTA_AGG_InItW(float)[][locn:int] := 0;
            |
            |-------------------- QUERIES --------------------
            |DECLARE QUERY AGG := AGG(float)[][];
            |
            |------------------- TRIGGERS --------------------
            |ON BATCH UPDATE OF INVENTORY {
            |    
            |    DELTA_AGG_Inventory(float)[][locn, dateid, ksn] := AggSum([locn, dateid, ksn], (DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * inventoryunits * inventoryunits);
            |
            |    DELTA_AGG_InIt(float)[][locn, dateid] := AggSum([locn, dateid], DELTA_AGG_Inventory(float)[][locn, dateid, ksn] * AGG_Item(float)[][ksn]);
            |
            |    DELTA_AGG_InItW(float)[][locn] := AggSum([locn], DELTA_AGG_InIt(float)[][locn, dateid] * AGG_Weather(float)[][locn, dateid]);
            |
            |    AGG(float)[][] += AggSum([], DELTA_AGG_InItW(float)[][locn] * AGG_LC(float)[][locn]);
            |
            |    AGG_InItW(float)[][locn] += DELTA_AGG_InItW(float)[][locn];
            |
            |    AGG_InIt(float)[][locn, dateid] += DELTA_AGG_InIt(float)[][locn, dateid];
            |
            |    AGG_Inventory(float)[][locn, dateid, ksn] += DELTA_AGG_Inventory(float)[][locn, dateid, ksn];
            |}
            |
            |ON BATCH UPDATE OF LOCATION {
            |
            |    DELTA_AGG_Location(float)[][locn, zip] := AggSum([locn, zip], (DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime));
            |
            |    DELTA_AGG_LC(float)[][locn] := AggSum([locn], DELTA_AGG_Location(float)[][locn, zip] * AGG_Census(float)[][zip]);
            |    
            |    AGG(float)[][] += AggSum([], DELTA_AGG_LC(float)[][locn] * AGG_InItW(float)[][locn]);
            |
            |    AGG_LC(float)[][locn] += DELTA_AGG_LC(float)[][locn];
            |
            |    AGG_Location(float)[][locn, zip] += DELTA_AGG_Location(float)[][locn, zip];
            |}
            |
            |ON BATCH UPDATE OF CENSUS {
            |
            |    DELTA_AGG_Census(float)[][zip] := AggSum([zip], (DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic));
            |
            |    DELTA_AGG_LC(float)[][locn] := AggSum([locn], DELTA_AGG_Census(float)[][zip] * AGG_Location(float)[][locn, zip]);
            |
            |    AGG(float)[][] += AggSum([], DELTA_AGG_LC(float)[][locn] * AGG_InItW(float)[][locn]);
            |
            |    AGG_LC(float)[][locn] += DELTA_AGG_LC(float)[][locn];
            |
            |    AGG_Census(float)[][zip] += DELTA_AGG_Census(float)[][zip];
            |}
            |
            |ON BATCH UPDATE OF ITEM {
            |
            |    DELTA_AGG_Item(float)[][ksn] := AggSum([ksn], (DELTA ITEM)(ksn, subcategory, category, categorycluster, prize));
            |
            |    DELTA_AGG_InIt(float)[][locn, dateid] := AggSum([locn, dateid], DELTA_AGG_Item(float)[][ksn] * AGG_Inventory(float)[][locn, dateid, ksn]);
            |
            |    DELTA_AGG_InItW(float)[][locn] := AggSum([locn], DELTA_AGG_InIt(float)[][locn, dateid] * AGG_Weather(float)[][locn, dateid]);
            |
            |    AGG(float)[][] += AggSum([], DELTA_AGG_InItW(float)[][locn] * AGG_LC(float)[][locn]);
            |
            |    AGG_InItW(float)[][locn] += DELTA_AGG_InItW(float)[][locn];
            |
            |    AGG_InIt(float)[][locn, dateid] += DELTA_AGG_InIt(float)[][locn, dateid];
            |
            |    AGG_Item(float)[][ksn] += DELTA_AGG_Item(float)[][ksn];
            |}
            |
            |ON BATCH UPDATE OF WEATHER {
            |
            |    DELTA_AGG_Weather(float)[][locn, dateid] := AggSum([locn, dateid], (DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder));
            |
            |    DELTA_AGG_InItW(float)[][locn] := AggSum([locn], DELTA_AGG_Weather(float)[][locn, dateid] * AGG_InIt(float)[][locn, dateid]);
            |
            |    AGG(float)[][] += AggSum([], DELTA_AGG_InItW(float)[][locn] * AGG_LC(float)[][locn]); 
            |
            |    AGG_InItW(float)[][locn] += DELTA_AGG_InItW(float)[][locn];
            |
            |    AGG_Weather(float)[][locn, dateid] += DELTA_AGG_Weather(float)[][locn, dateid];
            |}
            |
            |ON SYSTEM READY {
            |}""".stripMargin    
}