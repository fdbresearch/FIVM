-- Reevaluation of a sum aggregate for updates to all relations
--
--

-------------------- SOURCES --------------------
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

--------------------- MAPS ----------------------
DECLARE MAP BASE_HOUSE(int)[][postcode:float, h2:float, h3:float, h4:float, h5:float, h6:float, h7:float, h8:float, h9:float, h10:float, h11:float] := 0;
DECLARE MAP BASE_SHOP(int)[][postcode:float, s2:float, s3:float, s4:float, s5:float, s6:float] := 0;
DECLARE MAP BASE_INSTITUTION(int)[][postcode:float, i2:float, i3:float] := 0;
DECLARE MAP BASE_RESTAURANT(int)[][postcode:float, r2:float, r3:float] := 0;
DECLARE MAP BASE_DEMOGRAPHICS(int)[][postcode:float, d2:float, d3:float, d4:float, d5:float] := 0;
DECLARE MAP BASE_TRANSPORT(int)[][postcode:float, t2:float, t3:float, t4:float] := 0;

DECLARE MAP AGG(float)[][] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF HOUSE {

    BASE_HOUSE(int)[][postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11] += (DELTA HOUSE)(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON BATCH UPDATE OF SHOP {
    
    BASE_SHOP(int)[][postcode, s2, s3, s4, s5, s6] += (DELTA SHOP)(postcode, s2, s3, s4, s5, s6);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON BATCH UPDATE OF INSTITUTION {

    BASE_INSTITUTION(int)[][postcode, i2, i3] += (DELTA INSTITUTION)(postcode, i2, i3);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON BATCH UPDATE OF RESTAURANT {

    BASE_RESTAURANT(int)[][postcode, r2, r3] += (DELTA RESTAURANT)(postcode, r2, r3);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON BATCH UPDATE OF DEMOGRAPHICS {
    
    BASE_DEMOGRAPHICS(int)[][postcode, d2, d3, d4, d5] += (DELTA DEMOGRAPHICS)(postcode, d2, d3, d4, d5);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON BATCH UPDATE OF TRANSPORT {

    BASE_TRANSPORT(int)[][postcode, t2, t3, t4] += (DELTA TRANSPORT)(postcode, t2, t3, t4);

    AGG(float)[][] := 
        AggSum([], 
            AggSum([postcode], BASE_HOUSE(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11)) * 
            AggSum([postcode], BASE_SHOP(postcode, s2, s3, s4, s5, s6)) *
            AggSum([postcode], BASE_INSTITUTION(postcode, i2, i3)) *
            AggSum([postcode], BASE_RESTAURANT(postcode, r2, r3)) *
            AggSum([postcode], BASE_DEMOGRAPHICS(postcode, d2, d3, d4, d5)) *
            AggSum([postcode], BASE_TRANSPORT(postcode, t2, t3, t4)) *
            postcode * postcode
        );
}

ON SYSTEM READY {
}