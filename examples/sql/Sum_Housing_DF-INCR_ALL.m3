-- Incremental computation of a sum aggregate for updates to all relations
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
DECLARE MAP AGG_HOUSE(float)[][postcode:float] := 0;
DECLARE MAP AGG_SHOP(float)[][postcode:float] := 0;
DECLARE MAP AGG_INSTITUTION(float)[][postcode:float] := 0; 
DECLARE MAP AGG_RESTAURANT(float)[][postcode:float] := 0; 
DECLARE MAP AGG_DEMOGRAPHICS(float)[][postcode:float] := 0; 
DECLARE MAP AGG_TRANSPORT(float)[][postcode:float] := 0; 

DECLARE MAP DELTA_AGG_HOUSE(float)[][postcode:float] := 0;
DECLARE MAP DELTA_AGG_SHOP(float)[][postcode:float] := 0;
DECLARE MAP DELTA_AGG_INSTITUTION(float)[][postcode:float] := 0; 
DECLARE MAP DELTA_AGG_RESTAURANT(float)[][postcode:float] := 0; 
DECLARE MAP DELTA_AGG_DEMOGRAPHICS(float)[][postcode:float] := 0; 
DECLARE MAP DELTA_AGG_TRANSPORT(float)[][postcode:float] := 0; 

DECLARE MAP AGG(float)[][] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF HOUSE {

    DELTA_AGG_HOUSE(float)[][postcode] := AggSum([postcode], (DELTA HOUSE)(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_HOUSE(float)[][postcode] * 
            AGG_SHOP(float)[][postcode] *
            AGG_INSTITUTION(float)[][postcode] *
            AGG_RESTAURANT(float)[][postcode] *
            AGG_DEMOGRAPHICS(float)[][postcode] *
            AGG_TRANSPORT(float)[][postcode] *
            postcode * postcode
        );

    AGG_HOUSE(float)[][postcode] += DELTA_AGG_HOUSE(float)[][postcode];
}

ON BATCH UPDATE OF SHOP {
    
    DELTA_AGG_SHOP(float)[][postcode] := AggSum([postcode], (DELTA SHOP)(postcode, s2, s3, s4, s5, s6));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_SHOP(float)[][postcode] *
            AGG_HOUSE(float)[][postcode] * 
            AGG_INSTITUTION(float)[][postcode] *
            AGG_RESTAURANT(float)[][postcode] *
            AGG_DEMOGRAPHICS(float)[][postcode] *
            AGG_TRANSPORT(float)[][postcode] *
            postcode * postcode
        );

    AGG_SHOP(float)[][postcode] += DELTA_AGG_SHOP(float)[][postcode];
}

ON BATCH UPDATE OF INSTITUTION {

    DELTA_AGG_INSTITUTION(float)[][postcode] := AggSum([postcode], (DELTA INSTITUTION)(postcode, i2, i3));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_INSTITUTION(float)[][postcode] *
            AGG_HOUSE(float)[][postcode] * 
            AGG_SHOP(float)[][postcode] *            
            AGG_RESTAURANT(float)[][postcode] *
            AGG_DEMOGRAPHICS(float)[][postcode] *
            AGG_TRANSPORT(float)[][postcode] *
            postcode * postcode
        );

    AGG_INSTITUTION(float)[][postcode] += DELTA_AGG_INSTITUTION(float)[][postcode];
}

ON BATCH UPDATE OF RESTAURANT {

    DELTA_AGG_RESTAURANT(float)[][postcode] := AggSum([postcode], (DELTA RESTAURANT)(postcode, r2, r3));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_RESTAURANT(float)[][postcode] *
            AGG_HOUSE(float)[][postcode] * 
            AGG_SHOP(float)[][postcode] *
            AGG_INSTITUTION(float)[][postcode] *
            AGG_DEMOGRAPHICS(float)[][postcode] *
            AGG_TRANSPORT(float)[][postcode] *
            postcode * postcode
        );
    
    AGG_RESTAURANT(float)[][postcode] += DELTA_AGG_RESTAURANT(float)[][postcode];
}

ON BATCH UPDATE OF DEMOGRAPHICS {
    
    DELTA_AGG_DEMOGRAPHICS(float)[][postcode] := AggSum([postcode], (DELTA DEMOGRAPHICS)(postcode, d2, d3, d4, d5));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_DEMOGRAPHICS(float)[][postcode] *
            AGG_HOUSE(float)[][postcode] * 
            AGG_SHOP(float)[][postcode] *
            AGG_INSTITUTION(float)[][postcode] *
            AGG_RESTAURANT(float)[][postcode] *            
            AGG_TRANSPORT(float)[][postcode] *
            postcode * postcode
        );

    AGG_DEMOGRAPHICS(float)[][postcode] += DELTA_AGG_DEMOGRAPHICS(float)[][postcode];        
}

ON BATCH UPDATE OF TRANSPORT {

    DELTA_AGG_TRANSPORT(float)[][postcode] := AggSum([postcode], (DELTA TRANSPORT)(postcode, t2, t3, t4));

    AGG(float)[][] += 
        AggSum([], 
            DELTA_AGG_TRANSPORT(float)[][postcode] *
            AGG_HOUSE(float)[][postcode] * 
            AGG_SHOP(float)[][postcode] *
            AGG_INSTITUTION(float)[][postcode] *
            AGG_RESTAURANT(float)[][postcode] *            
            AGG_DEMOGRAPHICS(float)[][postcode] *
            postcode * postcode
        );

    AGG_TRANSPORT(float)[][postcode] += DELTA_AGG_TRANSPORT(float)[][postcode];
}

ON SYSTEM READY {
}