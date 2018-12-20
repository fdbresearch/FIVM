---------------- TYPE DEFINITIONS ---------------
CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

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
DECLARE MAP V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][] :=
  AggSum([],
    ((((((V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local> * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode))
  );

DECLARE MAP V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (HOUSE(postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) * [lift<1>: RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking))
  );

DECLARE MAP V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (SHOP(postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) * [lift<11>: RingFactorizedRelation<[11, double, double, double, double, double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms))
  );

DECLARE MAP V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (INSTITUTION(postcode, typeeducation, sizeinstitution) * [lift<16>: RingFactorizedRelation<[16, double, double]>](typeeducation, sizeinstitution))
  );

DECLARE MAP V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (RESTAURANT(postcode, openinghoursrest, pricerangerest) * [lift<18>: RingFactorizedRelation<[18, double, double]>](openinghoursrest, pricerangerest))
  );

DECLARE MAP V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (DEMOGRAPHICS(postcode, averagesalary, crimesperyear, unemployment, nbhospitals) * [lift<20>: RingFactorizedRelation<[20, double, double, double, double]>](averagesalary, crimesperyear, unemployment, nbhospitals))
  );

DECLARE MAP V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode: double] :=
  AggSum([postcode],
    (TRANSPORT(postcode, nbbuslines, nbtrainstations, distancecitycentre) * [lift<24>: RingFactorizedRelation<[24, double, double, double]>](nbbuslines, nbtrainstations, distancecitycentre))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_postcode_HSIRDT1 := V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>;

DECLARE QUERY V_house_H1 := V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>;

DECLARE QUERY V_sainsburys_S1 := V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>;

DECLARE QUERY V_typeeducation_I1 := V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>;

DECLARE QUERY V_pricerangerest_R1 := V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>;

DECLARE QUERY V_unemployment_D1 := V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>;

DECLARE QUERY V_nbbuslines_T1 := V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>;

------------------- TRIGGERS --------------------
ON + HOUSE (postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<1>: RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking)) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>  +=  (1 * [lift<1>: RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking));
}

ON - HOUSE (postcode, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<1>: RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking)) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<1>: RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking));
}

ON + SHOP (postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<11>: RingFactorizedRelation<[11, double, double, double, double, double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms)) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>  +=  (1 * [lift<11>: RingFactorizedRelation<[11, double, double, double, double, double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms));
}

ON - SHOP (postcode, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<11>: RingFactorizedRelation<[11, double, double, double, double, double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms)) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<11>: RingFactorizedRelation<[11, double, double, double, double, double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms));
}

ON + INSTITUTION (postcode, typeeducation, sizeinstitution) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<16>: RingFactorizedRelation<[16, double, double]>](typeeducation, sizeinstitution)) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>  +=  (1 * [lift<16>: RingFactorizedRelation<[16, double, double]>](typeeducation, sizeinstitution));
}

ON - INSTITUTION (postcode, typeeducation, sizeinstitution) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<16>: RingFactorizedRelation<[16, double, double]>](typeeducation, sizeinstitution)) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<16>: RingFactorizedRelation<[16, double, double]>](typeeducation, sizeinstitution));
}

ON + RESTAURANT (postcode, openinghoursrest, pricerangerest) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<18>: RingFactorizedRelation<[18, double, double]>](openinghoursrest, pricerangerest)) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>  +=  (1 * [lift<18>: RingFactorizedRelation<[18, double, double]>](openinghoursrest, pricerangerest));
}

ON - RESTAURANT (postcode, openinghoursrest, pricerangerest) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<18>: RingFactorizedRelation<[18, double, double]>](openinghoursrest, pricerangerest)) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<18>: RingFactorizedRelation<[18, double, double]>](openinghoursrest, pricerangerest));
}

ON + DEMOGRAPHICS (postcode, averagesalary, crimesperyear, unemployment, nbhospitals) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<20>: RingFactorizedRelation<[20, double, double, double, double]>](averagesalary, crimesperyear, unemployment, nbhospitals)) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>  +=  (1 * [lift<20>: RingFactorizedRelation<[20, double, double, double, double]>](averagesalary, crimesperyear, unemployment, nbhospitals));
}

ON - DEMOGRAPHICS (postcode, averagesalary, crimesperyear, unemployment, nbhospitals) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<20>: RingFactorizedRelation<[20, double, double, double, double]>](averagesalary, crimesperyear, unemployment, nbhospitals)) * V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<20>: RingFactorizedRelation<[20, double, double, double, double]>](averagesalary, crimesperyear, unemployment, nbhospitals));
}

ON + TRANSPORT (postcode, nbbuslines, nbtrainstations, distancecitycentre) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((1 * [lift<24>: RingFactorizedRelation<[24, double, double, double]>](nbbuslines, nbtrainstations, distancecitycentre)) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>  +=  (1 * [lift<24>: RingFactorizedRelation<[24, double, double, double]>](nbbuslines, nbtrainstations, distancecitycentre));
}

ON - TRANSPORT (postcode, nbbuslines, nbtrainstations, distancecitycentre) {
  V_postcode_HSIRDT1(RingFactorizedRelation<[0, double]>)[][]<Local>  +=  (((((((-1 * [lift<24>: RingFactorizedRelation<[24, double, double, double]>](nbbuslines, nbtrainstations, distancecitycentre)) * V_unemployment_D1(RingFactorizedRelation<[20, double, double, double, double]>)[][postcode]<Local>) * V_pricerangerest_R1(RingFactorizedRelation<[18, double, double]>)[][postcode]<Local>) * V_typeeducation_I1(RingFactorizedRelation<[16, double, double]>)[][postcode]<Local>) * V_sainsburys_S1(RingFactorizedRelation<[11, double, double, double, double, double]>)[][postcode]<Local>) * V_house_H1(RingFactorizedRelation<[1, double, double, double, double, double, double, double, double, double, double]>)[][postcode]<Local>) * [lift<0>: RingFactorizedRelation<[0, double]>](postcode));
  V_nbbuslines_T1(RingFactorizedRelation<[24, double, double, double]>)[][postcode]<Local>  +=  (-1 * [lift<24>: RingFactorizedRelation<[24, double, double, double]>](nbbuslines, nbtrainstations, distancecitycentre));
}

ON SYSTEM READY {
  
}