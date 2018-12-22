IMPORT DTREE FROM FILE 'housing.txt';

CREATE TYPE RingAvg 
FROM FILE 'ring/ring_avg.hpp';

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

SELECT SUM([lift: RingAvg](HOUSE.postcode * HOUSE.postcode))
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;