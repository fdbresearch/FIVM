CREATE TYPE RingCofactor
FROM FILE 'ring_cofactor.hpp'
WITH PARAMETER SCHEMA (static, dynamic_sum);

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
--     [lift: RingCofactor<double,1>](postcode) *
--     [lift: RingCofactor<double,1>](livingarea) *
--     [lift: RingCofactor<double,1>](price) *
--     [lift: RingCofactor<double,1>](nbbedrooms) *
--     [lift: RingCofactor<double,1>](nbbathrooms) *
--     [lift: RingCofactor<double,1>](kitchensize) *
--     [lift: RingCofactor<double,1>](house) *
--     [lift: RingCofactor<double,1>](flat) *
--     [lift: RingCofactor<double,1>](unknown) *
--     [lift: RingCofactor<double,1>](garden) *
--     [lift: RingCofactor<double,1>](parking) *
--     [lift: RingCofactor<double,1>](openinghoursshop) *
--     [lift: RingCofactor<double,1>](pricerangeshop) *
--     [lift: RingCofactor<double,1>](sainsburys) *
--     [lift: RingCofactor<double,1>](tesco) *
--     [lift: RingCofactor<double,1>](ms) *
--     [lift: RingCofactor<double,1>](typeeducation) *
--     [lift: RingCofactor<double,1>](sizeinstitution) *
--     [lift: RingCofactor<double,1>](openinghoursrest) *
--     [lift: RingCofactor<double,1>](pricerangerest) *
--     [lift: RingCofactor<double,1>](averagesalary) *
--     [lift: RingCofactor<double,1>](crimesperyear) *
--     [lift: RingCofactor<double,1>](unemployment) *
--     [lift: RingCofactor<double,1>](nbhospitals) *
--     [lift: RingCofactor<double,1>](nbbuslines) *
--     [lift: RingCofactor<double,1>](nbtrainstations) *
--     [lift: RingCofactor<double,1>](distancecitycentre)
-- )
-- FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;

SELECT SUM(
    [lift: RingCofactor<double,1>](postcode) *
    [lift: RingCofactor<double,10>](livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) *
    [lift: RingCofactor<double,5>](openinghoursshop, pricerangeshop, sainsburys, tesco, ms) *
    [lift: RingCofactor<double,2>](typeeducation, sizeinstitution) *
    [lift: RingCofactor<double,2>](openinghoursrest, pricerangerest) *    
    [lift: RingCofactor<double,4>](averagesalary, crimesperyear, unemployment, nbhospitals) *
    [lift: RingCofactor<double,3>](nbbuslines, nbtrainstations, distancecitycentre)
)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;