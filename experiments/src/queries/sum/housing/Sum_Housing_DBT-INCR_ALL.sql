CREATE STREAM HOUSE(postcode float, h2 float, h3 float, h4 float, h5 float, h6 float, h7 float, h8 float, h9 float, h10 float, h11 float)
FROM FILE './datasets/housing/House.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM SHOP(postcode float, s2 float, s3 float, s4 float, s5 float, s6 float)
FROM FILE './datasets/housing/Shop.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM INSTITUTION(postcode float, i2 float, i3 float)
FROM FILE './datasets/housing/Institution.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM RESTAURANT(postcode float, r2 float, r3 float)
FROM FILE './datasets/housing/Restaurant.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM DEMOGRAPHICS(postcode float, d2 float, d3 float, d4 float, d5 float)
FROM FILE './datasets/housing/Demographics.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE STREAM TRANSPORT(postcode float, t2 float, t3 float, t4 float)
FROM FILE './datasets/housing/Transport.tbl' LINE DELIMITED CSV(delimiter := '|');

SELECT SUM(HOUSE.postcode*HOUSE.postcode)
FROM HOUSE NATURAL JOIN SHOP NATURAL JOIN INSTITUTION NATURAL JOIN RESTAURANT NATURAL JOIN DEMOGRAPHICS NATURAL JOIN TRANSPORT;