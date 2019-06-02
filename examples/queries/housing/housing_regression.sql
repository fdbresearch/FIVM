IMPORT DTREE FROM FILE 'housing.txt';

CREATE TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (static, dynamic_min, dynamic_sum);

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
    [lift<0>: RingCofactor<double,0,1>](postcode) *
    [lift<1>: RingCofactor<double,1,10>](house, flat, unknown, parking, nbbedrooms, nbbathrooms, garden, kitchensize, livingarea, price) *
    [lift<11>: RingCofactor<double,11,5>](sainsburys, tesco, ms, pricerangeshop, openinghoursshop) *
    [lift<16>: RingCofactor<double,16,2>](typeeducation, sizeinstitution) *
    [lift<18>: RingCofactor<double,18,2>](pricerangerest, openinghoursrest) *    
    [lift<20>: RingCofactor<double,20,4>](unemployment, nbhospitals, crimesperyear, averagesalary) *
    [lift<24>: RingCofactor<double,24,3>](nbbuslines, nbtrainstations, distancecitycentre)
)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;

-- SELECT SUM(
--     [lift<0>: RingCofactor<double,0,1>](postcode) *
--     [lift<1>: RingCofactor<double,1,1>](house) *
--     [lift<2>: RingCofactor<double,2,1>](flat) *
--     [lift<3>: RingCofactor<double,3,1>](unknown) *
--     [lift<4>: RingCofactor<double,4,1>](parking) *
--     [lift<5>: RingCofactor<double,5,1>](nbbedrooms) *
--     [lift<6>: RingCofactor<double,6,1>](nbbathrooms) *
--     [lift<7>: RingCofactor<double,7,1>](garden) *
--     [lift<8>: RingCofactor<double,8,1>](kitchensize) *
--     [lift<9>: RingCofactor<double,9,1>](livingarea) *
--     [lift<10>: RingCofactor<double,10,1>](price) *
--     [lift<11>: RingCofactor<double,11,1>](sainsburys) *
--     [lift<12>: RingCofactor<double,12,1>](tesco) *
--     [lift<13>: RingCofactor<double,13,1>](ms) *
--     [lift<14>: RingCofactor<double,14,1>](pricerangeshop) *
--     [lift<15>: RingCofactor<double,15,1>](openinghoursshop) *
--     [lift<16>: RingCofactor<double,16,1>](typeeducation) *
--     [lift<17>: RingCofactor<double,17,1>](sizeinstitution) *
--     [lift<18>: RingCofactor<double,18,1>](pricerangerest) *
--     [lift<19>: RingCofactor<double,19,1>](openinghoursrest) *
--     [lift<20>: RingCofactor<double,20,1>](unemployment) *
--     [lift<21>: RingCofactor<double,21,1>](nbhospitals) *
--     [lift<22>: RingCofactor<double,22,1>](crimesperyear) *
--     [lift<23>: RingCofactor<double,23,1>](averagesalary) *
--     [lift<24>: RingCofactor<double,24,1>](nbbuslines) *
--     [lift<25>: RingCofactor<double,25,1>](nbtrainstations) *
--     [lift<26>: RingCofactor<double,26,1>](distancecitycentre)
-- )
-- FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;
