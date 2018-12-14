CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

CREATE STREAM HOUSE(postcode double, livingarea double, price double, nbbedrooms double, nbbathrooms double, kitchensize double, house double, flat double, unknown double, garden double, parking double)
FROM FILE './datasets/housing/House.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM SHOP(postcode double, openinghoursshop double, pricerangeshop double, sainsburys double, tesco double, ms double)
FROM FILE './datasets/housing/Shop.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM INSTITUTION(postcode double, typeeducation double, sizeinstitution double)
FROM FILE './datasets/housing/Institution.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM RESTAURANT(postcode double, openinghoursrest double, pricerangerest double)
FROM FILE './datasets/housing/Restaurant.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM DEMOGRAPHICS(postcode double, averagesalary double, crimesperyear double, unemployment double, nbhospitals double)
FROM FILE './datasets/housing/Demographics.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM TRANSPORT(postcode double, nbbuslines double, nbtrainstations double, distancecitycentre double)
FROM FILE './datasets/housing/Transport.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(
    [lift<0>: RingFactorizedRelation<[0,double]>](postcode) *
    [lift<1>: RingFactorizedRelation<[1,double,double,double,double,double,double,double,double,double,double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) *
    [lift<11>: RingFactorizedRelation<[11,double,double,double,double,double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms) *
    [lift<16>: RingFactorizedRelation<[16,double,double]>](typeeducation, sizeinstitution) *
    [lift<18>: RingFactorizedRelation<[18,double,double]>](openinghoursrest, pricerangerest) *    
    [lift<20>: RingFactorizedRelation<[20,double,double,double,double]>](averagesalary, crimesperyear, unemployment, nbhospitals) *
    [lift<24>: RingFactorizedRelation<[24,double,double,double]>](nbbuslines, nbtrainstations, distancecitycentre)
)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;

-- SELECT SUM(
--     [lift<0>: RingFactorizedRelation<[0,double]>](postcode) *
--     [lift<1>: RingFactorizedRelation<[1,double]>](livingarea) *
--     [lift<2>: RingFactorizedRelation<[2,double]>](price) *
--     [lift<3>: RingFactorizedRelation<[3,double]>](nbbedrooms) *
--     [lift<4>: RingFactorizedRelation<[4,double]>](nbbathrooms) *
--     [lift<5>: RingFactorizedRelation<[5,double]>](kitchensize) *
--     [lift<6>: RingFactorizedRelation<[6,double]>](house) *
--     [lift<7>: RingFactorizedRelation<[7,double]>](flat) *
--     [lift<8>: RingFactorizedRelation<[8,double]>](unknown) *
--     [lift<9>: RingFactorizedRelation<[9,double]>](garden) *
--     [lift<10>: RingFactorizedRelation<[10,double]>](parking) *
--     [lift<11>: RingFactorizedRelation<[11,double]>](openinghoursshop) *
--     [lift<12>: RingFactorizedRelation<[12,double]>](pricerangeshop) *
--     [lift<13>: RingFactorizedRelation<[13,double]>](sainsburys) *
--     [lift<14>: RingFactorizedRelation<[14,double]>](tesco) *
--     [lift<15>: RingFactorizedRelation<[15,double]>](ms) *
--     [lift<16>: RingFactorizedRelation<[16,double]>](typeeducation) *
--     [lift<17>: RingFactorizedRelation<[17,double]>](sizeinstitution) *
--     [lift<18>: RingFactorizedRelation<[18,double]>](openinghoursrest) *
--     [lift<19>: RingFactorizedRelation<[19,double]>](pricerangerest) *
--     [lift<20>: RingFactorizedRelation<[20,double]>](averagesalary) *
--     [lift<21>: RingFactorizedRelation<[21,double]>](crimesperyear) *
--     [lift<22>: RingFactorizedRelation<[22,double]>](unemployment) *
--     [lift<23>: RingFactorizedRelation<[23,double]>](nbhospitals) *
--     [lift<24>: RingFactorizedRelation<[24,double]>](nbbuslines) *
--     [lift<25>: RingFactorizedRelation<[25,double]>](nbtrainstations) *
--     [lift<26>: RingFactorizedRelation<[26,double]>](distancecitycentre)
-- )
-- FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;
