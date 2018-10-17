CREATE TYPE RingCofactor
FROM FILE 'ring_cofactor.hpp';

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
    [lift: RingCofactor(1)](0, postcode) *
    [lift: RingCofactor(10)](1, livingarea, price, nbbedrooms, nbbathrooms, kitchensize, house, flat, unknown, garden, parking) *
    [lift: RingCofactor(5)](11, openinghoursshop, pricerangeshop, sainsburys, tesco, ms) *
    [lift: RingCofactor(2)](16, typeeducation, sizeinstitution) *
    [lift: RingCofactor(2)](18, openinghoursrest, pricerangerest) *    
    [lift: RingCofactor(4)](20, averagesalary, crimesperyear, unemployment, nbhospitals) *
    [lift: RingCofactor(3)](24, nbbuslines, nbtrainstations, distancecitycentre)
)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;

-- SELECT SUM(
--     [lift: RingCofactor(1)](0, postcode) *
--     [lift: RingCofactor(1)](1, livingarea) *
--     [lift: RingCofactor(1)](2, price) *
--     [lift: RingCofactor(1)](3, nbbedrooms) *
--     [lift: RingCofactor(1)](4, nbbathrooms) *
--     [lift: RingCofactor(1)](5, kitchensize) *
--     [lift: RingCofactor(1)](6, house) *
--     [lift: RingCofactor(1)](7, flat) *
--     [lift: RingCofactor(1)](8, unknown) *
--     [lift: RingCofactor(1)](9, garden) *
--     [lift: RingCofactor(1)](10, parking) *
--     [lift: RingCofactor(1)](11, openinghoursshop) *
--     [lift: RingCofactor(1)](12, pricerangeshop) *
--     [lift: RingCofactor(1)](13, sainsburys) *
--     [lift: RingCofactor(1)](14, tesco) *
--     [lift: RingCofactor(1)](15, ms) *
--     [lift: RingCofactor(1)](16, typeeducation) *
--     [lift: RingCofactor(1)](17, sizeinstitution) *
--     [lift: RingCofactor(1)](18, openinghoursrest) *
--     [lift: RingCofactor(1)](19, pricerangerest) *
--     [lift: RingCofactor(1)](20, averagesalary) *
--     [lift: RingCofactor(1)](21, crimesperyear) *
--     [lift: RingCofactor(1)](22, unemployment) *
--     [lift: RingCofactor(1)](23, nbhospitals) *
--     [lift: RingCofactor(1)](24, nbbuslines) *
--     [lift: RingCofactor(1)](25, nbtrainstations) *
--     [lift: RingCofactor(1)](26, distancecitycentre)
-- )
-- FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;