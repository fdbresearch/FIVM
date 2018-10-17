---------------- TYPE DEFINITIONS ---------------
CREATE TYPE RingCofactor FROM FILE 'ring_cofactor.hpp';

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
DECLARE MAP V_locn_IIWLC1(RingCofactor(43))[][] :=
  AggSum([],
    (V_dateid_IIW1(RingCofactor(13))[][locn]<Local> * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );

DECLARE MAP V_dateid_IIW1(RingCofactor(13))[][locn: long] :=
  AggSum([locn],
    (V_ksn_II1(RingCofactor(6))[][locn, dateid]<Local> * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );

DECLARE MAP V_ksn_II1(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (V_inventoryunits_I1(RingCofactor(1))[][locn, dateid, ksn]<Local> * (V_subcategory_I1(RingCofactor(4))[][ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
  );

DECLARE MAP V_inventoryunits_I1(RingCofactor(1))[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    (INVENTORY(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor(1)](4, inventoryunits))
  );

DECLARE MAP V_subcategory_I1(RingCofactor(4))[][ksn: double] :=
  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (AggSum([ksn, subcategory, category],
        (AggSum([ksn, subcategory, category, categoryCluster],
          (ITEM(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor(1)](36, prize))
        ) * [lift: RingCofactor(1)](35, categoryCluster))
      ) * [lift: RingCofactor(1)](34, category))
    ) * [lift: RingCofactor(1)](33, subcategory))
  );

DECLARE MAP V_rain_W1(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (AggSum([locn, dateid, rain],
      (AggSum([locn, dateid, rain, snow],
        (AggSum([rain, snow, dateid, maxtemp, locn],
          (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
            (AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
              (WEATHER(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor(1)](42, thunder))
            ) * [lift: RingCofactor(1)](41, meanwind))
          ) * [lift: RingCofactor(1)](40, mintemp))
        ) * [lift: RingCofactor(1)](39, maxtemp))
      ) * [lift: RingCofactor(1)](38, snow))
    ) * [lift: RingCofactor(1)](37, rain))
  );

DECLARE MAP V_zip_LC1(RingCofactor(29))[][locn: long] :=
  AggSum([locn],
    (V_rgn_cd_L1(RingCofactor(13))[][locn, zip]<Local> * (V_population_C1(RingCofactor(15))[][zip]<Local> * [lift: RingCofactor(1)](3, zip)))
  );

DECLARE MAP V_rgn_cd_L1(RingCofactor(13))[][locn: long, zip: long] :=
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
                            (LOCATION(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor(1)](17, walmartsupercenterdrivetime))
                          ) * [lift: RingCofactor(1)](16, walmartsupercenterdistance))
                        ) * [lift: RingCofactor(1)](15, walmartdrivetime))
                      ) * [lift: RingCofactor(1)](14, walmartdistance))
                    ) * [lift: RingCofactor(1)](13, targetdrivetime))
                  ) * [lift: RingCofactor(1)](12, targetdistance))
                ) * [lift: RingCofactor(1)](11, supertargetdrivetime))
              ) * [lift: RingCofactor(1)](10, supertargetdistance))
            ) * [lift: RingCofactor(1)](9, avghhi))
          ) * [lift: RingCofactor(1)](8, sell_area_sq_ft))
        ) * [lift: RingCofactor(1)](7, tot_area_sq_ft))
      ) * [lift: RingCofactor(1)](6, clim_zn_nbr))
    ) * [lift: RingCofactor(1)](5, rgn_cd))
  );

DECLARE MAP V_population_C1(RingCofactor(15))[][zip: long] :=
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
                                (CENSUS(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor(1)](32, hispanic))
                              ) * [lift: RingCofactor(1)](31, householdschildren))
                            ) * [lift: RingCofactor(1)](30, females))
                          ) * [lift: RingCofactor(1)](29, males))
                        ) * [lift: RingCofactor(1)](28, husbwife))
                      ) * [lift: RingCofactor(1)](27, households))
                    ) * [lift: RingCofactor(1)](26, families))
                  ) * [lift: RingCofactor(1)](25, houseunits))
                ) * [lift: RingCofactor(1)](24, occupiedhouseunits))
              ) * [lift: RingCofactor(1)](23, medianage))
            ) * [lift: RingCofactor(1)](22, blackafrican))
          ) * [lift: RingCofactor(1)](21, pacific))
        ) * [lift: RingCofactor(1)](20, asian))
      ) * [lift: RingCofactor(1)](19, white))
    ) * [lift: RingCofactor(1)](18, population))
  );

DECLARE MAP TMP_SUM1(RingCofactor(1))[][locn: long, dateid: long, ksn: double] :=
  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor(1)](4, inventoryunits))
  );

DECLARE MAP TMP_SUM2(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (TMP_SUM1(RingCofactor(1))[][locn, dateid, ksn]<Local> * (V_subcategory_I1(RingCofactor(4))[][ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
  );

DECLARE MAP TMP_SUM3(RingCofactor(13))[][locn: long] :=
  AggSum([locn],
    (TMP_SUM2(RingCofactor(6))[][locn, dateid]<Local> * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );

DECLARE MAP TMP_SUM4(RingCofactor(13))[][locn: long, zip: long] :=
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
                            ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor(1)](17, walmartsupercenterdrivetime))
                          ) * [lift: RingCofactor(1)](16, walmartsupercenterdistance))
                        ) * [lift: RingCofactor(1)](15, walmartdrivetime))
                      ) * [lift: RingCofactor(1)](14, walmartdistance))
                    ) * [lift: RingCofactor(1)](13, targetdrivetime))
                  ) * [lift: RingCofactor(1)](12, targetdistance))
                ) * [lift: RingCofactor(1)](11, supertargetdrivetime))
              ) * [lift: RingCofactor(1)](10, supertargetdistance))
            ) * [lift: RingCofactor(1)](9, avghhi))
          ) * [lift: RingCofactor(1)](8, sell_area_sq_ft))
        ) * [lift: RingCofactor(1)](7, tot_area_sq_ft))
      ) * [lift: RingCofactor(1)](6, clim_zn_nbr))
    ) * [lift: RingCofactor(1)](5, rgn_cd))
  );

DECLARE MAP TMP_SUM5(RingCofactor(29))[][locn: long] :=
  AggSum([locn],
    (TMP_SUM4(RingCofactor(13))[][locn, zip]<Local> * (V_population_C1(RingCofactor(15))[][zip]<Local> * [lift: RingCofactor(1)](3, zip)))
  );

DECLARE MAP TMP_SUM6(RingCofactor(15))[][zip: long] :=
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
                                ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor(1)](32, hispanic))
                              ) * [lift: RingCofactor(1)](31, householdschildren))
                            ) * [lift: RingCofactor(1)](30, females))
                          ) * [lift: RingCofactor(1)](29, males))
                        ) * [lift: RingCofactor(1)](28, husbwife))
                      ) * [lift: RingCofactor(1)](27, households))
                    ) * [lift: RingCofactor(1)](26, families))
                  ) * [lift: RingCofactor(1)](25, houseunits))
                ) * [lift: RingCofactor(1)](24, occupiedhouseunits))
              ) * [lift: RingCofactor(1)](23, medianage))
            ) * [lift: RingCofactor(1)](22, blackafrican))
          ) * [lift: RingCofactor(1)](21, pacific))
        ) * [lift: RingCofactor(1)](20, asian))
      ) * [lift: RingCofactor(1)](19, white))
    ) * [lift: RingCofactor(1)](18, population))
  );

DECLARE MAP TMP_SUM7(RingCofactor(29))[][locn: long] :=
  AggSum([locn],
    (TMP_SUM6(RingCofactor(15))[][zip]<Local> * (V_rgn_cd_L1(RingCofactor(13))[][locn, zip]<Local> * [lift: RingCofactor(1)](3, zip)))
  );

DECLARE MAP TMP_SUM8(RingCofactor(4))[][ksn: double] :=
  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (AggSum([ksn, subcategory, category],
        (AggSum([ksn, subcategory, category, categoryCluster],
          ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor(1)](36, prize))
        ) * [lift: RingCofactor(1)](35, categoryCluster))
      ) * [lift: RingCofactor(1)](34, category))
    ) * [lift: RingCofactor(1)](33, subcategory))
  );

DECLARE MAP TMP_SUM9(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (TMP_SUM8(RingCofactor(4))[][ksn]<Local> * (V_inventoryunits_I1(RingCofactor(1))[][locn, dateid, ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
  );

DECLARE MAP TMP_SUM10(RingCofactor(13))[][locn: long] :=
  AggSum([locn],
    (TMP_SUM9(RingCofactor(6))[][locn, dateid]<Local> * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );

DECLARE MAP TMP_SUM11(RingCofactor(6))[][locn: long, dateid: long] :=
  AggSum([locn, dateid],
    (AggSum([locn, dateid, rain],
      (AggSum([locn, dateid, rain, snow],
        (AggSum([rain, snow, dateid, maxtemp, locn],
          (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
            (AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
              ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor(1)](42, thunder))
            ) * [lift: RingCofactor(1)](41, meanwind))
          ) * [lift: RingCofactor(1)](40, mintemp))
        ) * [lift: RingCofactor(1)](39, maxtemp))
      ) * [lift: RingCofactor(1)](38, snow))
    ) * [lift: RingCofactor(1)](37, rain))
  );

DECLARE MAP TMP_SUM12(RingCofactor(13))[][locn: long] :=
  AggSum([locn],
    (TMP_SUM11(RingCofactor(6))[][locn, dateid]<Local> * (V_ksn_II1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_locn_IIWLC1 := V_locn_IIWLC1(RingCofactor(43))[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF INVENTORY {
  TMP_SUM1(RingCofactor(1))[][locn, dateid, ksn]<Local>  :=  AggSum([locn, dateid, ksn],
    ((DELTA INVENTORY)(locn, dateid, ksn, inventoryunits) * [lift: RingCofactor(1)](4, inventoryunits))
  );
  TMP_SUM2(RingCofactor(6))[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (TMP_SUM1(RingCofactor(1))[][locn, dateid, ksn]<Local> * (V_subcategory_I1(RingCofactor(4))[][ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
  );
  TMP_SUM3(RingCofactor(13))[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM2(RingCofactor(6))[][locn, dateid]<Local> * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (TMP_SUM3(RingCofactor(13))[][locn]<Local> * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
  V_dateid_IIW1(RingCofactor(13))[][locn]<Local>  +=  TMP_SUM3(RingCofactor(13))[][locn]<Local>;
  V_ksn_II1(RingCofactor(6))[][locn, dateid]<Local>  +=  TMP_SUM2(RingCofactor(6))[][locn, dateid]<Local>;
  V_inventoryunits_I1(RingCofactor(1))[][locn, dateid, ksn]<Local>  +=  TMP_SUM1(RingCofactor(1))[][locn, dateid, ksn]<Local>;
}

ON BATCH UPDATE OF LOCATION {
  TMP_SUM4(RingCofactor(13))[][locn, zip]<Local>  :=  AggSum([locn, zip],
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
                            ((DELTA LOCATION)(locn, zip, rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime) * [lift: RingCofactor(1)](17, walmartsupercenterdrivetime))
                          ) * [lift: RingCofactor(1)](16, walmartsupercenterdistance))
                        ) * [lift: RingCofactor(1)](15, walmartdrivetime))
                      ) * [lift: RingCofactor(1)](14, walmartdistance))
                    ) * [lift: RingCofactor(1)](13, targetdrivetime))
                  ) * [lift: RingCofactor(1)](12, targetdistance))
                ) * [lift: RingCofactor(1)](11, supertargetdrivetime))
              ) * [lift: RingCofactor(1)](10, supertargetdistance))
            ) * [lift: RingCofactor(1)](9, avghhi))
          ) * [lift: RingCofactor(1)](8, sell_area_sq_ft))
        ) * [lift: RingCofactor(1)](7, tot_area_sq_ft))
      ) * [lift: RingCofactor(1)](6, clim_zn_nbr))
    ) * [lift: RingCofactor(1)](5, rgn_cd))
  );
  TMP_SUM5(RingCofactor(29))[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM4(RingCofactor(13))[][locn, zip]<Local> * (V_population_C1(RingCofactor(15))[][zip]<Local> * [lift: RingCofactor(1)](3, zip)))
  );
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (TMP_SUM5(RingCofactor(29))[][locn]<Local> * (V_dateid_IIW1(RingCofactor(13))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
  V_zip_LC1(RingCofactor(29))[][locn]<Local>  +=  TMP_SUM5(RingCofactor(29))[][locn]<Local>;
  V_rgn_cd_L1(RingCofactor(13))[][locn, zip]<Local>  +=  TMP_SUM4(RingCofactor(13))[][locn, zip]<Local>;
}

ON BATCH UPDATE OF CENSUS {
  TMP_SUM6(RingCofactor(15))[][zip]<Local>  :=  AggSum([zip],
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
                                ((DELTA CENSUS)(zip, population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic) * [lift: RingCofactor(1)](32, hispanic))
                              ) * [lift: RingCofactor(1)](31, householdschildren))
                            ) * [lift: RingCofactor(1)](30, females))
                          ) * [lift: RingCofactor(1)](29, males))
                        ) * [lift: RingCofactor(1)](28, husbwife))
                      ) * [lift: RingCofactor(1)](27, households))
                    ) * [lift: RingCofactor(1)](26, families))
                  ) * [lift: RingCofactor(1)](25, houseunits))
                ) * [lift: RingCofactor(1)](24, occupiedhouseunits))
              ) * [lift: RingCofactor(1)](23, medianage))
            ) * [lift: RingCofactor(1)](22, blackafrican))
          ) * [lift: RingCofactor(1)](21, pacific))
        ) * [lift: RingCofactor(1)](20, asian))
      ) * [lift: RingCofactor(1)](19, white))
    ) * [lift: RingCofactor(1)](18, population))
  );
  TMP_SUM7(RingCofactor(29))[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM6(RingCofactor(15))[][zip]<Local> * (V_rgn_cd_L1(RingCofactor(13))[][locn, zip]<Local> * [lift: RingCofactor(1)](3, zip)))
  );
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (TMP_SUM7(RingCofactor(29))[][locn]<Local> * (V_dateid_IIW1(RingCofactor(13))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
  V_zip_LC1(RingCofactor(29))[][locn]<Local>  +=  TMP_SUM7(RingCofactor(29))[][locn]<Local>;
  V_population_C1(RingCofactor(15))[][zip]<Local>  +=  TMP_SUM6(RingCofactor(15))[][zip]<Local>;
}

ON BATCH UPDATE OF ITEM {
  TMP_SUM8(RingCofactor(4))[][ksn]<Local>  :=  AggSum([ksn],
    (AggSum([ksn, subcategory],
      (AggSum([ksn, subcategory, category],
        (AggSum([ksn, subcategory, category, categoryCluster],
          ((DELTA ITEM)(ksn, subcategory, category, categoryCluster, prize) * [lift: RingCofactor(1)](36, prize))
        ) * [lift: RingCofactor(1)](35, categoryCluster))
      ) * [lift: RingCofactor(1)](34, category))
    ) * [lift: RingCofactor(1)](33, subcategory))
  );
  TMP_SUM9(RingCofactor(6))[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (TMP_SUM8(RingCofactor(4))[][ksn]<Local> * (V_inventoryunits_I1(RingCofactor(1))[][locn, dateid, ksn]<Local> * [lift: RingCofactor(1)](2, ksn)))
  );
  TMP_SUM10(RingCofactor(13))[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM9(RingCofactor(6))[][locn, dateid]<Local> * (V_rain_W1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (TMP_SUM10(RingCofactor(13))[][locn]<Local> * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
  V_dateid_IIW1(RingCofactor(13))[][locn]<Local>  +=  TMP_SUM10(RingCofactor(13))[][locn]<Local>;
  V_ksn_II1(RingCofactor(6))[][locn, dateid]<Local>  +=  TMP_SUM9(RingCofactor(6))[][locn, dateid]<Local>;
  V_subcategory_I1(RingCofactor(4))[][ksn]<Local>  +=  TMP_SUM8(RingCofactor(4))[][ksn]<Local>;
}

ON BATCH UPDATE OF WEATHER {
  TMP_SUM11(RingCofactor(6))[][locn, dateid]<Local>  :=  AggSum([locn, dateid],
    (AggSum([locn, dateid, rain],
      (AggSum([locn, dateid, rain, snow],
        (AggSum([rain, snow, dateid, maxtemp, locn],
          (AggSum([rain, snow, mintemp, dateid, maxtemp, locn],
            (AggSum([rain, snow, mintemp, meanwind, dateid, maxtemp, locn],
              ((DELTA WEATHER)(locn, dateid, rain, snow, maxtemp, mintemp, meanwind, thunder) * [lift: RingCofactor(1)](42, thunder))
            ) * [lift: RingCofactor(1)](41, meanwind))
          ) * [lift: RingCofactor(1)](40, mintemp))
        ) * [lift: RingCofactor(1)](39, maxtemp))
      ) * [lift: RingCofactor(1)](38, snow))
    ) * [lift: RingCofactor(1)](37, rain))
  );
  TMP_SUM12(RingCofactor(13))[][locn]<Local>  :=  AggSum([locn],
    (TMP_SUM11(RingCofactor(6))[][locn, dateid]<Local> * (V_ksn_II1(RingCofactor(6))[][locn, dateid]<Local> * [lift: RingCofactor(1)](1, dateid)))
  );
  V_locn_IIWLC1(RingCofactor(43))[][]<Local>  +=  AggSum([],
    (TMP_SUM12(RingCofactor(13))[][locn]<Local> * (V_zip_LC1(RingCofactor(29))[][locn]<Local> * [lift: RingCofactor(1)](0, locn)))
  );
  V_dateid_IIW1(RingCofactor(13))[][locn]<Local>  +=  TMP_SUM12(RingCofactor(13))[][locn]<Local>;
  V_rain_W1(RingCofactor(6))[][locn, dateid]<Local>  +=  TMP_SUM11(RingCofactor(6))[][locn, dateid]<Local>;
}

ON SYSTEM READY {
  
}