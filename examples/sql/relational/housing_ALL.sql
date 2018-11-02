CREATE TYPE RingRelation
FROM FILE 'ring_relation.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

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

-- SELECT SUM(
--     [lift<0>: RingRelation<[0,double]>](postcode) *
--     [lift<1>: RingRelation<[1,double]>](livingarea) *
--     [lift<2>: RingRelation<[2,double]>](price) *
--     [lift<3>: RingRelation<[3,double]>](nbbedrooms) *
--     [lift<4>: RingRelation<[4,double]>](nbbathrooms) *
--     [lift<5>: RingRelation<[5,double]>](kitchensize) *
--     [lift<6>: RingRelation<[6,double]>](house) *
--     [lift<7>: RingRelation<[7,double]>](flat) *
--     [lift<8>: RingRelation<[8,double]>](unknown) *
--     [lift<9>: RingRelation<[9,double]>](garden) *
--     [lift<10>: RingRelation<[10,double]>](parking) *
--     [lift<11>: RingRelation<[11,double]>](openinghoursshop) *
--     [lift<12>: RingRelation<[12,double]>](pricerangeshop) *
--     [lift<13>: RingRelation<[13,double]>](sainsburys) *
--     [lift<14>: RingRelation<[14,double]>](tesco) *
--     [lift<15>: RingRelation<[15,double]>](ms) *
--     [lift<16>: RingRelation<[16,double]>](typeeducation) *
--     [lift<17>: RingRelation<[17,double]>](sizeinstitution) *
--     [lift<18>: RingRelation<[18,double]>](openinghoursrest) *
--     [lift<19>: RingRelation<[19,double]>](pricerangerest) *
--     [lift<20>: RingRelation<[20,double]>](averagesalary) *
--     [lift<21>: RingRelation<[21,double]>](crimesperyear) *
--     [lift<22>: RingRelation<[22,double]>](unemployment) *
--     [lift<23>: RingRelation<[23,double]>](nbhospitals) *
--     [lift<24>: RingRelation<[24,double]>](nbbuslines) *
--     [lift<25>: RingRelation<[25,double]>](nbtrainstations) *
--     [lift<26>: RingRelation<[26,double]>](distancecitycentre)
-- )
-- FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;

SELECT SUM(
    [lift<0>: RingRelation<[0,double]>](postcode) *
    [lift<1>: RingRelation<[1,double,double,double,double,double,double,double,double,double,double]>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) *
    [lift<11>: RingRelation<[11,double,double,double,double,double]>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms) *
    [lift<16>: RingRelation<[16,double,double]>](typeeducation, sizeinstitution) *
    [lift<18>: RingRelation<[18,double,double]>](openinghoursrest, pricerangerest) *    
    [lift<20>: RingRelation<[20,double,double,double,double]>](averagesalary, crimesperyear, unemployment, nbhospitals) *
    [lift<24>: RingRelation<[24,double,double,double]>](nbbuslines, nbtrainstations, distancecitycentre)
)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;