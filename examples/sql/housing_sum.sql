CREATE STREAM House(postcode double, livingarea double, price double, nbbedrooms double, nbbathrooms double, kitchensize double, house double, flat double, unknown double, garden double, parking double)
FROM FILE './datasets/housing/House.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Shop(postcode double, openinghoursshop double, pricerangeshop double, sainsburys double, tesco double, ms double)
FROM FILE './datasets/housing/Shop.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Institution(postcode double, typeeducation double, sizeinstitution double)
FROM FILE './datasets/housing/Institution.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Restaurant(postcode double, openinghoursrest double, pricerangerest double)
FROM FILE './datasets/housing/Restaurant.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Demographics(postcode double, averagesalary double, crimesperyear double, unemployment double, nbhospitals double)
FROM FILE './datasets/housing/Demographics.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM Transport(postcode double, nbbuslines double, nbtrainstations double, distancecitycentre double)
FROM FILE './datasets/housing/Transport.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(HOUSE.postcode*HOUSE.postcode)
FROM House NATURAL JOIN Shop NATURAL JOIN Institution NATURAL JOIN Restaurant NATURAL JOIN Demographics NATURAL JOIN Transport;