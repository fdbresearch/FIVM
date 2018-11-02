---------------- TYPE DEFINITIONS ---------------
CREATE TYPE RingCofactor
  FROM FILE 'ring_cofactor.hpp'
  WITH PARAMETER SCHEMA (static, dynamic_sum);

-------------------- SOURCES --------------------
CREATE STREAM INVENTORY (locn long, dateid long, ksn double, inventoryunits double)
  FROM FILE './datasets/sears/Inventory_toy2.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM LOCATION (locn long, zip long, rgn_cd double, clim_zn_nbr double, tot_area_sq_ft double, sell_area_sq_ft double, avghhi double, supertargetdistance double, supertargetdrivetime double, targetdistance double, targetdrivetime double, walmartdistance double, walmartdrivetime double, walmartsupercenterdistance double, walmartsupercenterdrivetime double)
  FROM FILE './datasets/sears/Location.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM CENSUS (zip long, population long, white long, asian long, pacific long, blackafrican long, medianage double, occupiedhouseunits long, houseunits long, families long, households long, husbwife long, males long, females long, householdschildren long, hispanic long)
  FROM FILE './datasets/sears/Census.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ITEM (ksn double, subcategory long, category long, categoryCluster long, prize double)
  FROM FILE './datasets/sears/Item.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM WEATHER (locn long, dateid long, rain long, snow long, maxtemp long, mintemp long, meanwind double, thunder long)
  FROM FILE './datasets/sears/Weather.tbl' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_locn_IIWLC1(RingCofactor<double, 43>)[][] :=
  AggSum([],
    ((V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );

DECLARE MAP V_dateid_IIW1(RingCofactor<double, 13>)[][locn: long] :=
  AggSum([locn],
    ((V_ksn_II1(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );

DECLARE MAP V_ksn_II1(RingCofactor<double, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((V_inventoryunits_I1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4>)[][ksn]<Local>) * [lift: RingCofactor<double, 1>](2, ksn))
  );

DECLARE MAP V_inventoryunits_I1(RingCofactor<double, 1>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor<double, 1>](3, inventoryunits))
  );

DECLARE MAP V_subcategory_I1(RingCofactor<double, 4>)[][ksn: double] :=
  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (AggSum([ksn, subcategory, category],
        (AggSum([ksn, subcategory, category, categoryCluster],
          (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor<double, 1>](7, prize))
        ) * [lift: RingCofactor<double, 1>](6, categoryCluster))
      ) * [lift: RingCofactor<double, 1>](5, category))
    ) * [lift: RingCofactor<double, 1>](4, subcategory))
  );

DECLARE MAP V_rain_W1(RingCofactor<double, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (AggSum([locn, dateid, rain],
      (AggSum([locn, dateid, rain, snow],
        (AggSum([rain, snow, dateid, maxtemp, locn],
          (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
            (AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
              (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor<double, 1>](13, thunder))
            ) * [lift: RingCofactor<double, 1>](12, meanwind))
          ) * [lift: RingCofactor<double, 1>](11, mintemp))
        ) * [lift: RingCofactor<double, 1>](10, maxtemp))
      ) * [lift: RingCofactor<double, 1>](9, snow))
    ) * [lift: RingCofactor<double, 1>](8, rain))
  );

DECLARE MAP V_zip_LC1(RingCofactor<double, 29>)[][locn: long] :=
  AggSum([locn],
    ((V_rgn_cd_L1(RingCofactor<double, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 15>)[][zip]<Local>) * [lift: RingCofactor<double, 1>](14, zip))
  );

DECLARE MAP V_rgn_cd_L1(RingCofactor<double, 13>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (AggSum([locn, zip, rgn_cd],
      (AggSum([locn, zip, rgn_cd, clim_zn_nbr],
        (AggSum([zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
          (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
            (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, locn, rgn_cd],
              (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                (AggSum([zip, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                  (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                    (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                      (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                        (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                          (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, walmartsupercenterdistance, targetdrivetime, locn, rgn_cd],
                            (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor<double, 1>](27, walmartsupercenterdrivetime))
                          ) * [lift: RingCofactor<double, 1>](26, walmartsupercenterdistance))
                        ) * [lift: RingCofactor<double, 1>](25, walmartdrivetime))
                      ) * [lift: RingCofactor<double, 1>](24, walmartdistance))
                    ) * [lift: RingCofactor<double, 1>](23, targetdrivetime))
                  ) * [lift: RingCofactor<double, 1>](22, targetdistance))
                ) * [lift: RingCofactor<double, 1>](21, supertargetdrivetime))
              ) * [lift: RingCofactor<double, 1>](20, supertargetdistance))
            ) * [lift: RingCofactor<double, 1>](19, avghhi))
          ) * [lift: RingCofactor<double, 1>](18, sell_area_sq_ft))
        ) * [lift: RingCofactor<double, 1>](17, tot_area_sq_ft))
      ) * [lift: RingCofactor<double, 1>](16, clim_zn_nbr))
    ) * [lift: RingCofactor<double, 1>](15, rgn_cd))
  );

DECLARE MAP V_population_C1(RingCofactor<double, 15>)[][zip: long] :=
  AggSum([zip],
    (AggSum([zip, population],
      (AggSum([zip, population, white],
        (AggSum([zip, population, white, asian],
          (AggSum([pacific, white, population, zip, asian],
            (AggSum([pacific, white, population, zip, blackafrican, asian],
              (AggSum([pacific, white, population, zip, blackafrican, asian, medianage],
                (AggSum([pacific, white, population, zip, blackafrican, asian, medianage, occupiedhouseunits],
                  (AggSum([pacific, white, population, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                    (AggSum([pacific, white, population, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                      (AggSum([pacific, white, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                        (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                          (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, males, occupiedhouseunits],
                            (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, medianage, males, occupiedhouseunits],
                              (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, householdschildren, medianage, males, occupiedhouseunits],
                                (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor<double, 1>](42, hispanic))
                              ) * [lift: RingCofactor<double, 1>](41, householdschildren))
                            ) * [lift: RingCofactor<double, 1>](40, females))
                          ) * [lift: RingCofactor<double, 1>](39, males))
                        ) * [lift: RingCofactor<double, 1>](38, husbwife))
                      ) * [lift: RingCofactor<double, 1>](37, households))
                    ) * [lift: RingCofactor<double, 1>](36, families))
                  ) * [lift: RingCofactor<double, 1>](35, houseunits))
                ) * [lift: RingCofactor<double, 1>](34, occupiedhouseunits))
              ) * [lift: RingCofactor<double, 1>](33, medianage))
            ) * [lift: RingCofactor<double, 1>](32, blackafrican))
          ) * [lift: RingCofactor<double, 1>](31, pacific))
        ) * [lift: RingCofactor<double, 1>](30, asian))
      ) * [lift: RingCofactor<double, 1>](29, white))
    ) * [lift: RingCofactor<double, 1>](28, population))
  );

DECLARE MAP TMP_SUM1(RingCofactor<double, 1>)[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor<double, 1>](3, inventoryunits))
  );

DECLARE MAP TMP_SUM2(RingCofactor<double, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4>)[][ksn]<Local>) * [lift: RingCofactor<double, 1>](2, ksn))
  );

DECLARE MAP TMP_SUM3(RingCofactor<double, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM2(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );

DECLARE MAP TMP_SUM4(RingCofactor<double, 12>)[][locn: long, zip: long, rgn_cd: double] :=
  AggSum([locn, zip, rgn_cd],
    (AggSum([locn, zip, rgn_cd, clim_zn_nbr],
      (AggSum([zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
        (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
          (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, locn, rgn_cd],
            (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
              (AggSum([zip, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                  (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                    (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                      (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                        (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, walmartsupercenterdistance, targetdrivetime, locn, rgn_cd],
                          ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor<double, 1>](27, walmartsupercenterdrivetime))
                        ) * [lift: RingCofactor<double, 1>](26, walmartsupercenterdistance))
                      ) * [lift: RingCofactor<double, 1>](25, walmartdrivetime))
                    ) * [lift: RingCofactor<double, 1>](24, walmartdistance))
                  ) * [lift: RingCofactor<double, 1>](23, targetdrivetime))
                ) * [lift: RingCofactor<double, 1>](22, targetdistance))
              ) * [lift: RingCofactor<double, 1>](21, supertargetdrivetime))
            ) * [lift: RingCofactor<double, 1>](20, supertargetdistance))
          ) * [lift: RingCofactor<double, 1>](19, avghhi))
        ) * [lift: RingCofactor<double, 1>](18, sell_area_sq_ft))
      ) * [lift: RingCofactor<double, 1>](17, tot_area_sq_ft))
    ) * [lift: RingCofactor<double, 1>](16, clim_zn_nbr))
  );

DECLARE MAP TMP_SUM5(RingCofactor<double, 13>)[][locn: long, zip: long] :=
  AggSum([locn, zip],
    (TMP_SUM4(RingCofactor<double, 12>)[][locn, zip, rgn_cd]<Local> * [lift: RingCofactor<double, 1>](15, rgn_cd))
  );

DECLARE MAP TMP_SUM6(RingCofactor<double, 29>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM5(RingCofactor<double, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 15>)[][zip]<Local>) * [lift: RingCofactor<double, 1>](14, zip))
  );

DECLARE MAP TMP_SUM7(RingCofactor<double, 11>)[][pacific: long, white: long, population: long, zip: long, asian: long] :=
  AggSum([pacific, white, population, zip, asian],
    (AggSum([pacific, white, population, zip, blackafrican, asian],
      (AggSum([pacific, white, population, zip, blackafrican, asian, medianage],
        (AggSum([pacific, white, population, zip, blackafrican, asian, medianage, occupiedhouseunits],
          (AggSum([pacific, white, population, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
            (AggSum([pacific, white, population, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
              (AggSum([pacific, white, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                  (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, males, occupiedhouseunits],
                    (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, medianage, males, occupiedhouseunits],
                      (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, householdschildren, medianage, males, occupiedhouseunits],
                        ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor<double, 1>](42, hispanic))
                      ) * [lift: RingCofactor<double, 1>](41, householdschildren))
                    ) * [lift: RingCofactor<double, 1>](40, females))
                  ) * [lift: RingCofactor<double, 1>](39, males))
                ) * [lift: RingCofactor<double, 1>](38, husbwife))
              ) * [lift: RingCofactor<double, 1>](37, households))
            ) * [lift: RingCofactor<double, 1>](36, families))
          ) * [lift: RingCofactor<double, 1>](35, houseunits))
        ) * [lift: RingCofactor<double, 1>](34, occupiedhouseunits))
      ) * [lift: RingCofactor<double, 1>](33, medianage))
    ) * [lift: RingCofactor<double, 1>](32, blackafrican))
  );

DECLARE MAP TMP_SUM8(RingCofactor<double, 13>)[][zip: long, population: long, white: long] :=
  AggSum([zip, population, white],
    (AggSum([zip, population, white, asian],
      (TMP_SUM7(RingCofactor<double, 11>)[][pacific, white, population, zip, asian]<Local> * [lift: RingCofactor<double, 1>](31, pacific))
    ) * [lift: RingCofactor<double, 1>](30, asian))
  );

DECLARE MAP TMP_SUM9(RingCofactor<double, 15>)[][zip: long] :=
  AggSum([zip],
    (AggSum([zip, population],
      (TMP_SUM8(RingCofactor<double, 13>)[][zip, population, white]<Local> * [lift: RingCofactor<double, 1>](29, white))
    ) * [lift: RingCofactor<double, 1>](28, population))
  );

DECLARE MAP TMP_SUM10(RingCofactor<double, 29>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM9(RingCofactor<double, 15>)[][zip]<Local> * V_rgn_cd_L1(RingCofactor<double, 13>)[][locn, zip]<Local>) * [lift: RingCofactor<double, 1>](14, zip))
  );

DECLARE MAP TMP_SUM11(RingCofactor<double, 1>)[][ksn: double, subcategory: long, category: long, categoryCluster: long] :=
  AggSum([ksn, subcategory, category, categoryCluster],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor<double, 1>](7, prize))
  );

DECLARE MAP TMP_SUM12(RingCofactor<double, 2>)[][ksn: double, subcategory: long, category: long] :=
  AggSum([ksn, subcategory, category],
    (TMP_SUM11(RingCofactor<double, 1>)[][ksn, subcategory, category, categoryCluster]<Local> * [lift: RingCofactor<double, 1>](6, categoryCluster))
  );

DECLARE MAP TMP_SUM13(RingCofactor<double, 4>)[][ksn: double] :=
  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (TMP_SUM12(RingCofactor<double, 2>)[][ksn, subcategory, category]<Local> * [lift: RingCofactor<double, 1>](5, category))
    ) * [lift: RingCofactor<double, 1>](4, subcategory))
  );

DECLARE MAP TMP_SUM14(RingCofactor<double, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    ((TMP_SUM13(RingCofactor<double, 4>)[][ksn]<Local> * V_inventoryunits_I1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local>) * [lift: RingCofactor<double, 1>](2, ksn))
  );

DECLARE MAP TMP_SUM15(RingCofactor<double, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM14(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );

DECLARE MAP TMP_SUM16(RingCofactor<double, 1>)[][rain: long, snow: long, mintemp: long, meanwind: double, dateid: long, maxtemp: long, locn: long] :=
  AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor<double, 1>](13, thunder))
  );

DECLARE MAP TMP_SUM17(RingCofactor<double, 5>)[][locn: long, dateid: long, rain: long] :=
  AggSum([locn, dateid, rain],
    (AggSum([locn, dateid, rain, snow],
      (AggSum([rain, snow, dateid, maxtemp, locn],
        (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
          (TMP_SUM16(RingCofactor<double, 1>)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local> * [lift: RingCofactor<double, 1>](12, meanwind))
        ) * [lift: RingCofactor<double, 1>](11, mintemp))
      ) * [lift: RingCofactor<double, 1>](10, maxtemp))
    ) * [lift: RingCofactor<double, 1>](9, snow))
  );

DECLARE MAP TMP_SUM18(RingCofactor<double, 6>)[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (TMP_SUM17(RingCofactor<double, 5>)[][locn, dateid, rain]<Local> * [lift: RingCofactor<double, 1>](8, rain))
  );

DECLARE MAP TMP_SUM19(RingCofactor<double, 13>)[][locn: long] :=
  AggSum([locn],
    ((TMP_SUM18(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_ksn_II1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  TMP_SUM1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local>  :=  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor<double, 1>](3, inventoryunits))
  );
  TMP_SUM2(RingCofactor<double, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local> * V_subcategory_I1(RingCofactor<double, 4>)[][ksn]<Local>) * [lift: RingCofactor<double, 1>](2, ksn))
  );
  TMP_SUM3(RingCofactor<double, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM2(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM3(RingCofactor<double, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );
  V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local>  +=  TMP_SUM3(RingCofactor<double, 13>)[][locn]<Local>;
  V_ksn_II1(RingCofactor<double, 6>)[][locn, dateid]<Local>  +=  TMP_SUM2(RingCofactor<double, 6>)[][locn, dateid]<Local>;
  V_inventoryunits_I1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local>  +=  TMP_SUM1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local>;
}

ON BATCH UPDATE OF LOCATION {
  TMP_SUM4(RingCofactor<double, 12>)[][locn, zip, rgn_cd]<Local>  :=  AggSum([locn, zip, rgn_cd],
    (AggSum([locn, zip, rgn_cd, clim_zn_nbr],
      (AggSum([zip, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
        (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, tot_area_sq_ft, locn, rgn_cd],
          (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, locn, rgn_cd],
            (AggSum([zip, sell_area_sq_ft, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
              (AggSum([zip, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, locn, rgn_cd],
                  (AggSum([zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                    (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                      (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, targetdrivetime, locn, rgn_cd],
                        (AggSum([walmartdistance, zip, targetdistance, sell_area_sq_ft, walmartdrivetime, supertargetdrivetime, clim_zn_nbr, avghhi, tot_area_sq_ft, supertargetdistance, walmartsupercenterdistance, targetdrivetime, locn, rgn_cd],
                          ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor<double, 1>](27, walmartsupercenterdrivetime))
                        ) * [lift: RingCofactor<double, 1>](26, walmartsupercenterdistance))
                      ) * [lift: RingCofactor<double, 1>](25, walmartdrivetime))
                    ) * [lift: RingCofactor<double, 1>](24, walmartdistance))
                  ) * [lift: RingCofactor<double, 1>](23, targetdrivetime))
                ) * [lift: RingCofactor<double, 1>](22, targetdistance))
              ) * [lift: RingCofactor<double, 1>](21, supertargetdrivetime))
            ) * [lift: RingCofactor<double, 1>](20, supertargetdistance))
          ) * [lift: RingCofactor<double, 1>](19, avghhi))
        ) * [lift: RingCofactor<double, 1>](18, sell_area_sq_ft))
      ) * [lift: RingCofactor<double, 1>](17, tot_area_sq_ft))
    ) * [lift: RingCofactor<double, 1>](16, clim_zn_nbr))
  );
  TMP_SUM5(RingCofactor<double, 13>)[][locn, zip]<Local>  :=  AggSum([locn, zip],
    (TMP_SUM4(RingCofactor<double, 12>)[][locn, zip, rgn_cd]<Local> * [lift: RingCofactor<double, 1>](15, rgn_cd))
  );
  TMP_SUM6(RingCofactor<double, 29>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM5(RingCofactor<double, 13>)[][locn, zip]<Local> * V_population_C1(RingCofactor<double, 15>)[][zip]<Local>) * [lift: RingCofactor<double, 1>](14, zip))
  );
  V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM6(RingCofactor<double, 29>)[][locn]<Local> * V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );
  V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>  +=  TMP_SUM6(RingCofactor<double, 29>)[][locn]<Local>;
  V_rgn_cd_L1(RingCofactor<double, 13>)[][locn, zip]<Local>  +=  TMP_SUM5(RingCofactor<double, 13>)[][locn, zip]<Local>;
}

ON BATCH UPDATE OF CENSUS {
  TMP_SUM7(RingCofactor<double, 11>)[][pacific, white, population, zip, asian]<Local>  :=  AggSum([pacific, white, population, zip, asian],
    (AggSum([pacific, white, population, zip, blackafrican, asian],
      (AggSum([pacific, white, population, zip, blackafrican, asian, medianage],
        (AggSum([pacific, white, population, zip, blackafrican, asian, medianage, occupiedhouseunits],
          (AggSum([pacific, white, population, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
            (AggSum([pacific, white, population, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
              (AggSum([pacific, white, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, occupiedhouseunits],
                  (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, blackafrican, asian, medianage, males, occupiedhouseunits],
                    (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, medianage, males, occupiedhouseunits],
                      (AggSum([pacific, white, husbwife, population, households, families, zip, houseunits, females, blackafrican, asian, householdschildren, medianage, males, occupiedhouseunits],
                        ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor<double, 1>](42, hispanic))
                      ) * [lift: RingCofactor<double, 1>](41, householdschildren))
                    ) * [lift: RingCofactor<double, 1>](40, females))
                  ) * [lift: RingCofactor<double, 1>](39, males))
                ) * [lift: RingCofactor<double, 1>](38, husbwife))
              ) * [lift: RingCofactor<double, 1>](37, households))
            ) * [lift: RingCofactor<double, 1>](36, families))
          ) * [lift: RingCofactor<double, 1>](35, houseunits))
        ) * [lift: RingCofactor<double, 1>](34, occupiedhouseunits))
      ) * [lift: RingCofactor<double, 1>](33, medianage))
    ) * [lift: RingCofactor<double, 1>](32, blackafrican))
  );
  TMP_SUM8(RingCofactor<double, 13>)[][zip, population, white]<Local>  :=  AggSum([zip, population, white],
    (AggSum([zip, population, white, asian],
      (TMP_SUM7(RingCofactor<double, 11>)[][pacific, white, population, zip, asian]<Local> * [lift: RingCofactor<double, 1>](31, pacific))
    ) * [lift: RingCofactor<double, 1>](30, asian))
  );
  TMP_SUM9(RingCofactor<double, 15>)[][zip]<Local>  :=  AggSum([zip],
    (AggSum([zip, population],
      (TMP_SUM8(RingCofactor<double, 13>)[][zip, population, white]<Local> * [lift: RingCofactor<double, 1>](29, white))
    ) * [lift: RingCofactor<double, 1>](28, population))
  );
  TMP_SUM10(RingCofactor<double, 29>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM9(RingCofactor<double, 15>)[][zip]<Local> * V_rgn_cd_L1(RingCofactor<double, 13>)[][locn, zip]<Local>) * [lift: RingCofactor<double, 1>](14, zip))
  );
  V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM10(RingCofactor<double, 29>)[][locn]<Local> * V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );
  V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>  +=  TMP_SUM10(RingCofactor<double, 29>)[][locn]<Local>;
  V_population_C1(RingCofactor<double, 15>)[][zip]<Local>  +=  TMP_SUM9(RingCofactor<double, 15>)[][zip]<Local>;
}

ON BATCH UPDATE OF ITEM {
  TMP_SUM11(RingCofactor<double, 1>)[][ksn, subcategory, category, categoryCluster]<Local>  :=  AggSum([ksn, subcategory, category, categoryCluster],
    ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor<double, 1>](7, prize))
  );
  TMP_SUM12(RingCofactor<double, 2>)[][ksn, subcategory, category]<Local>  :=  AggSum([ksn, subcategory, category],
    (TMP_SUM11(RingCofactor<double, 1>)[][ksn, subcategory, category, categoryCluster]<Local> * [lift: RingCofactor<double, 1>](6, categoryCluster))
  );
  TMP_SUM13(RingCofactor<double, 4>)[][ksn]<Local>  :=  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (TMP_SUM12(RingCofactor<double, 2>)[][ksn, subcategory, category]<Local> * [lift: RingCofactor<double, 1>](5, category))
    ) * [lift: RingCofactor<double, 1>](4, subcategory))
  );
  TMP_SUM14(RingCofactor<double, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    ((TMP_SUM13(RingCofactor<double, 4>)[][ksn]<Local> * V_inventoryunits_I1(RingCofactor<double, 1>)[][locn, dateid, ksn]<Local>) * [lift: RingCofactor<double, 1>](2, ksn))
  );
  TMP_SUM15(RingCofactor<double, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM14(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM15(RingCofactor<double, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );
  V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local>  +=  TMP_SUM15(RingCofactor<double, 13>)[][locn]<Local>;
  V_ksn_II1(RingCofactor<double, 6>)[][locn, dateid]<Local>  +=  TMP_SUM14(RingCofactor<double, 6>)[][locn, dateid]<Local>;
  V_subcategory_I1(RingCofactor<double, 4>)[][ksn]<Local>  +=  TMP_SUM13(RingCofactor<double, 4>)[][ksn]<Local>;
}

ON BATCH UPDATE OF WEATHER {
  TMP_SUM16(RingCofactor<double, 1>)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local>  :=  AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
    ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor<double, 1>](13, thunder))
  );
  TMP_SUM17(RingCofactor<double, 5>)[][locn, dateid, rain]<Local>  :=  AggSum([locn, dateid, rain],
    (AggSum([locn, dateid, rain, snow],
      (AggSum([rain, snow, dateid, maxtemp, locn],
        (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
          (TMP_SUM16(RingCofactor<double, 1>)[][rain, snow, mintemp, meanwind, dateid, maxtemp, locn]<Local> * [lift: RingCofactor<double, 1>](12, meanwind))
        ) * [lift: RingCofactor<double, 1>](11, mintemp))
      ) * [lift: RingCofactor<double, 1>](10, maxtemp))
    ) * [lift: RingCofactor<double, 1>](9, snow))
  );
  TMP_SUM18(RingCofactor<double, 6>)[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (TMP_SUM17(RingCofactor<double, 5>)[][locn, dateid, rain]<Local> * [lift: RingCofactor<double, 1>](8, rain))
  );
  TMP_SUM19(RingCofactor<double, 13>)[][locn]<Local>  :=  AggSum([locn],
    ((TMP_SUM18(RingCofactor<double, 6>)[][locn, dateid]<Local> * V_ksn_II1(RingCofactor<double, 6>)[][locn, dateid]<Local>) * [lift: RingCofactor<double, 1>](1, dateid))
  );
  V_locn_IIWLC1(RingCofactor<double, 43>)[][]<Local>  +=  AggSum([],
    ((TMP_SUM19(RingCofactor<double, 13>)[][locn]<Local> * V_zip_LC1(RingCofactor<double, 29>)[][locn]<Local>) * [lift: RingCofactor<double, 1>](0, locn))
  );
  V_dateid_IIW1(RingCofactor<double, 13>)[][locn]<Local>  +=  TMP_SUM19(RingCofactor<double, 13>)[][locn]<Local>;
  V_rain_W1(RingCofactor<double, 6>)[][locn, dateid]<Local>  +=  TMP_SUM18(RingCofactor<double, 6>)[][locn, dateid]<Local>;
}

ON SYSTEM READY {
  
}