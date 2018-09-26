-- Recomputing a sum aggregate
--
--

-------------------- SOURCES --------------------
CREATE TABLE HOUSE(postcode float, h2 float, h3 float, h4 float, h5 float, h6 float, h7 float, h8 float, h9 float, h10 float, h11 float)
FROM FILE './datasets/housing/House.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE SHOP(postcode float, s2 float, s3 float, s4 float, s5 float, s6 float)
FROM FILE './datasets/housing/Shop.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE INSTITUTION(postcode float, i2 float, i3 float)
FROM FILE './datasets/housing/Institution.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE RESTAURANT(postcode float, r2 float, r3 float)
FROM FILE './datasets/housing/Restaurant.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEMOGRAPHICS(postcode float, d2 float, d3 float, d4 float, d5 float)
FROM FILE './datasets/housing/Demographics.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE TRANSPORT(postcode float, t2 float, t3 float, t4 float)
FROM FILE './datasets/housing/Transport.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP TMP1(float)[][postcode:float] := 0;
DECLARE MAP TMP2(float)[][postcode:float] := 0;
DECLARE MAP TMP3(float)[][postcode:float] := 0;
DECLARE MAP TMP4(float)[][postcode:float] := 0;
DECLARE MAP TMP5(float)[][postcode:float] := 0;
DECLARE MAP TMP6(float)[][postcode:float] := 0;

DECLARE MAP AGG(float)[][] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][];

------------------- TRIGGERS --------------------
ON SYSTEM READY {

    TMP1(float)[][postcode] := AggSum([postcode], HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11));
    TMP2(float)[][postcode] := AggSum([postcode], SHOP(postcode, s2, s3, s4, s5, s6));
    TMP3(float)[][postcode] := AggSum([postcode], INSTITUTION(postcode, i2, i3));
    TMP4(float)[][postcode] := AggSum([postcode], RESTAURANT(postcode, r2, r3));
    TMP5(float)[][postcode] := AggSum([postcode], DEMOGRAPHICS(postcode, d2, d3, d4, d5));
    TMP6(float)[][postcode] := AggSum([postcode], TRANSPORT(postcode, t2, t3, t4));

    AGG(float)[][] := 
        AggSum([], 
            TMP1(float)[][postcode] * 
            TMP2(float)[][postcode] * 
            TMP3(float)[][postcode] * 
            TMP4(float)[][postcode] * 
            TMP5(float)[][postcode] * 
            TMP6(float)[][postcode] * 
            postcode * postcode
        );

--        AggSum([], 
--            AggSum([postcode], HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
--            AggSum([postcode], SHOP(postcode, s2, s3, s4, s5, s6)) *
--            AggSum([postcode], INSTITUTION(postcode, i2, i3)) *
--            AggSum([postcode], RESTAURANT(postcode, r2, r3)) *
--            AggSum([postcode], DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
--            AggSum([postcode], TRANSPORT(postcode, t2, t3, t4)) *
--            postcode * postcode
--        );
}