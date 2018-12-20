---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (static, dynamic_min, dynamic_sum);

-------------------- SOURCES --------------------
CREATE STREAM HOUSE (postcode double, livingarea double, price double, nbbedrooms double, nbbathrooms double, kitchensize double, house double, flat double, unknown double, garden double, parking double)
  FROM FILE './datasets/housing/House.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM SHOP (postcode double, openinghoursshop double, pricerangeshop double, sainsburys double, tesco double, ms double)
  FROM FILE './datasets/housing/Shop.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM INSTITUTION (postcode double, typeeducation double, sizeinstitution double)
  FROM FILE './datasets/housing/Institution.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM RESTAURANT (postcode double, openinghoursrest double, pricerangerest double)
  FROM FILE './datasets/housing/Restaurant.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM DEMOGRAPHICS (postcode double, averagesalary double, crimesperyear double, unemployment double, nbhospitals double)
  FROM FILE './datasets/housing/Demographics.tbl' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM TRANSPORT (postcode double, nbbuslines double, nbtrainstations double, distancecitycentre double)
  FROM FILE './datasets/housing/Transport.tbl' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][] :=
  AggSum([],
    ((((((V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local> * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );

DECLARE MAP V_house_H1(RingCofactor<double, 1, 10>)[][postcode: double] :=
  AggSum([postcode],
    (HOUSE(postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) * [lift<1>: RingCofactor<double, 1, 10>](house, flat, unknown, parking, nbbedrooms, nbbathrooms, garden, kitchensize, livingarea, price))
  );

DECLARE MAP V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode: double] :=
  AggSum([postcode],
    (SHOP(postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) * [lift<11>: RingCofactor<double, 11, 5>](sainsburys, tesco, ms, pricerangeshop, openinghoursshop))
  );

DECLARE MAP V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode: double] :=
  AggSum([postcode],
    (INSTITUTION(postcode, typeeducation, sizeinstitution) * [lift<16>: RingCofactor<double, 16, 2>](typeeducation, sizeinstitution))
  );

DECLARE MAP V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode: double] :=
  AggSum([postcode],
    (RESTAURANT(postcode, openinghoursrest, pricerangerest) * [lift<18>: RingCofactor<double, 18, 2>](pricerangerest, openinghoursrest))
  );

DECLARE MAP V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode: double] :=
  AggSum([postcode],
    (DEMOGRAPHICS(postcode, averagesalary, crimesperyear, unemployment, nbhospitals) * [lift<20>: RingCofactor<double, 20, 4>](unemployment, nbhospitals, crimesperyear, averagesalary))
  );

DECLARE MAP V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode: double] :=
  AggSum([postcode],
    (TRANSPORT(postcode, nbbuslines, nbtrainstations, distancecitycentre) * [lift<24>: RingCofactor<double, 24, 3>](nbbuslines, nbtrainstations, distancecitycentre))
  );

DECLARE MAP TMP_SUM1(RingCofactor<double, 1, 10>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA HOUSE)(postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) * [lift<1>: RingCofactor<double, 1, 10>](house, flat, unknown, parking, nbbedrooms, nbbathrooms, garden, kitchensize, livingarea, price))
  );

DECLARE MAP TMP_SUM2(RingCofactor<double, 11, 5>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA SHOP)(postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) * [lift<11>: RingCofactor<double, 11, 5>](sainsburys, tesco, ms, pricerangeshop, openinghoursshop))
  );

DECLARE MAP TMP_SUM3(RingCofactor<double, 16, 2>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA INSTITUTION)(postcode, typeeducation, sizeinstitution) * [lift<16>: RingCofactor<double, 16, 2>](typeeducation, sizeinstitution))
  );

DECLARE MAP TMP_SUM4(RingCofactor<double, 18, 2>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA RESTAURANT)(postcode, openinghoursrest, pricerangerest) * [lift<18>: RingCofactor<double, 18, 2>](pricerangerest, openinghoursrest))
  );

DECLARE MAP TMP_SUM5(RingCofactor<double, 20, 4>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA DEMOGRAPHICS)(postcode, averagesalary, crimesperyear, unemployment, nbhospitals) * [lift<20>: RingCofactor<double, 20, 4>](unemployment, nbhospitals, crimesperyear, averagesalary))
  );

DECLARE MAP TMP_SUM6(RingCofactor<double, 24, 3>)[][postcode: double] :=
  AggSum([postcode],
    ((DELTA TRANSPORT)(postcode, nbbuslines, nbtrainstations, distancecitycentre) * [lift<24>: RingCofactor<double, 24, 3>](nbbuslines, nbtrainstations, distancecitycentre))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_postcode_HSIRDT1 := V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF HOUSE {
  TMP_SUM1(RingCofactor<double, 1, 10>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA HOUSE)(postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) * [lift<1>: RingCofactor<double, 1, 10>](house, flat, unknown, parking, nbbedrooms, nbbathrooms, garden, kitchensize, livingarea, price))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM1(RingCofactor<double, 1, 10>)[][postcode]<Local> * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>  +=  TMP_SUM1(RingCofactor<double, 1, 10>)[][postcode]<Local>;
}

ON BATCH UPDATE OF SHOP {
  TMP_SUM2(RingCofactor<double, 11, 5>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA SHOP)(postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) * [lift<11>: RingCofactor<double, 11, 5>](sainsburys, tesco, ms, pricerangeshop, openinghoursshop))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM2(RingCofactor<double, 11, 5>)[][postcode]<Local> * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>  +=  TMP_SUM2(RingCofactor<double, 11, 5>)[][postcode]<Local>;
}

ON BATCH UPDATE OF INSTITUTION {
  TMP_SUM3(RingCofactor<double, 16, 2>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA INSTITUTION)(postcode, typeeducation, sizeinstitution) * [lift<16>: RingCofactor<double, 16, 2>](typeeducation, sizeinstitution))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM3(RingCofactor<double, 16, 2>)[][postcode]<Local> * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>  +=  TMP_SUM3(RingCofactor<double, 16, 2>)[][postcode]<Local>;
}

ON BATCH UPDATE OF RESTAURANT {
  TMP_SUM4(RingCofactor<double, 18, 2>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA RESTAURANT)(postcode, openinghoursrest, pricerangerest) * [lift<18>: RingCofactor<double, 18, 2>](pricerangerest, openinghoursrest))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM4(RingCofactor<double, 18, 2>)[][postcode]<Local> * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>  +=  TMP_SUM4(RingCofactor<double, 18, 2>)[][postcode]<Local>;
}

ON BATCH UPDATE OF DEMOGRAPHICS {
  TMP_SUM5(RingCofactor<double, 20, 4>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA DEMOGRAPHICS)(postcode, averagesalary, crimesperyear, unemployment, nbhospitals) * [lift<20>: RingCofactor<double, 20, 4>](unemployment, nbhospitals, crimesperyear, averagesalary))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM5(RingCofactor<double, 20, 4>)[][postcode]<Local> * V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>) * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>  +=  TMP_SUM5(RingCofactor<double, 20, 4>)[][postcode]<Local>;
}

ON BATCH UPDATE OF TRANSPORT {
  TMP_SUM6(RingCofactor<double, 24, 3>)[][postcode]<Local>  :=  AggSum([postcode],
    ((DELTA TRANSPORT)(postcode, nbbuslines, nbtrainstations, distancecitycentre) * [lift<24>: RingCofactor<double, 24, 3>](nbbuslines, nbtrainstations, distancecitycentre))
  );
  V_postcode_HSIRDT1(RingCofactor<double, 0, 27>)[][]<Local>  +=  AggSum([],
    ((((((TMP_SUM6(RingCofactor<double, 24, 3>)[][postcode]<Local> * V_unemployment_D1(RingCofactor<double, 20, 4>)[][postcode]<Local>) * V_pricerangerest_R1(RingCofactor<double, 18, 2>)[][postcode]<Local>) * V_typeeducation_I1(RingCofactor<double, 16, 2>)[][postcode]<Local>) * V_sainsburys_S1(RingCofactor<double, 11, 5>)[][postcode]<Local>) * V_house_H1(RingCofactor<double, 1, 10>)[][postcode]<Local>) * [lift<0>: RingCofactor<double, 0, 1>](postcode))
  );
  V_nbbuslines_T1(RingCofactor<double, 24, 3>)[][postcode]<Local>  +=  TMP_SUM6(RingCofactor<double, 24, 3>)[][postcode]<Local>;
}

ON SYSTEM READY {
  
}