-- Incremental computation of the cofactor matrix using the pivot method for updates to all relations
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

CREATE TABLE DEGREE10(Deg1 int, Deg2 int, Deg3 int, Deg4 int, Deg5 int, Deg6 int, Deg7 int, Deg8 int, Deg9 int, Deg10 int)
FROM FILE './datasets/degree/Degree10.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE5(Deg1 int, Deg2 int, Deg3 int, Deg4 int, Deg5 int)
FROM FILE './datasets/degree/Degree5.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE4(Deg1 int, Deg2 int, Deg3 int, Deg4 int)
FROM FILE './datasets/degree/Degree4.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE3(Deg1 int, Deg2 int, Deg3 int)
FROM FILE './datasets/degree/Degree3.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE2(Deg1 int, Deg2 int)
FROM FILE './datasets/degree/Degree2.tbl' LINE DELIMITED CSV(delimiter := '|');

CREATE TABLE DEGREE1(Deg1 int)
FROM FILE './datasets/degree/Degree1.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP AGG_HOUSE(float)[][postcode:float, Deg_h2:int, Deg_h3:int, Deg_h4:int, Deg_h5:int, Deg_h6:int, Deg_h7:int, Deg_h8:int, Deg_h9:int, Deg_h10:int, Deg_h11:int] := 0;
DECLARE MAP AGG_SHOP(float)[][postcode:float, Deg_s2:int, Deg_s3:int, Deg_s4:int, Deg_s5:int, Deg_s6:int] := 0;
DECLARE MAP AGG_INSTITUTION(float)[][postcode:float, Deg_i2:int, Deg_i3:int] := 0; 
DECLARE MAP AGG_RESTAURANT(float)[][postcode:float, Deg_r2:int, Deg_r3:int] := 0; 
DECLARE MAP AGG_DEMOGRAPHICS(float)[][postcode:float, Deg_d2:int, Deg_d3:int, Deg_d4:int, Deg_d5:int] := 0; 
DECLARE MAP AGG_TRANSPORT(float)[][postcode:float, Deg_t2:int, Deg_t3:int, Deg_t4:int] := 0; 

DECLARE MAP DELTA_AGG_HOUSE(float)[][postcode:float, Deg_h2:int, Deg_h3:int, Deg_h4:int, Deg_h5:int, Deg_h6:int, Deg_h7:int, Deg_h8:int, Deg_h9:int, Deg_h10:int, Deg_h11:int] := 0;
DECLARE MAP DELTA_AGG_SHOP(float)[][postcode:float, Deg_s2:int, Deg_s3:int, Deg_s4:int, Deg_s5:int, Deg_s6:int] := 0;
DECLARE MAP DELTA_AGG_INSTITUTION(float)[][postcode:float, Deg_i2:int, Deg_i3:int] := 0; 
DECLARE MAP DELTA_AGG_RESTAURANT(float)[][postcode:float, Deg_r2:int, Deg_r3:int] := 0; 
DECLARE MAP DELTA_AGG_DEMOGRAPHICS(float)[][postcode:float, Deg_d2:int, Deg_d3:int, Deg_d4:int, Deg_d5:int] := 0; 
DECLARE MAP DELTA_AGG_TRANSPORT(float)[][postcode:float, Deg_t2:int, Deg_t3:int, Deg_t4:int] := 0; 

DECLARE MAP AGG(float)[][Deg_postcode:int, Deg_h2:int, Deg_h3:int, Deg_h4:int, Deg_h5:int, Deg_h6:int, Deg_h7:int, Deg_h8:int, Deg_h9:int, Deg_h10:int, Deg_h11:int, Deg_s2:int, Deg_s3:int, Deg_s4:int, Deg_s5:int, Deg_s6:int, Deg_i2:int, Deg_i3:int, Deg_r2:int, Deg_r3:int, Deg_d2:int, Deg_d3:int, Deg_d4:int, Deg_d5:int, Deg_t2:int, Deg_t3:int, Deg_t4:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF HOUSE {

    DELTA_AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] := 
        AggSum([postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11],

            (DELTA HOUSE)(postcode, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11) *

            DEGREE10(Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11) *

            ( ({Deg_h2 = 0}) + ({Deg_h2 = 1} * h2) + ({Deg_h2 = 2} * h2 * h2) ) *
            ( ({Deg_h3 = 0}) + ({Deg_h3 = 1} * h3) + ({Deg_h3 = 2} * h3 * h3) ) *
            ( ({Deg_h4 = 0}) + ({Deg_h4 = 1} * h4) + ({Deg_h4 = 2} * h4 * h4) ) *
            ( ({Deg_h5 = 0}) + ({Deg_h5 = 1} * h5) + ({Deg_h5 = 2} * h5 * h5) ) *
            ( ({Deg_h6 = 0}) + ({Deg_h6 = 1} * h6) + ({Deg_h6 = 2} * h6 * h6) ) *
            ( ({Deg_h7 = 0}) + ({Deg_h7 = 1} * h7) + ({Deg_h7 = 2} * h7 * h7) ) *
            ( ({Deg_h8 = 0}) + ({Deg_h8 = 1} * h8) + ({Deg_h8 = 2} * h8 * h8) ) *
            ( ({Deg_h9 = 0}) + ({Deg_h9 = 1} * h9) + ({Deg_h9 = 2} * h9 * h9) ) *
            ( ({Deg_h10 = 0}) + ({Deg_h10 = 1} * h10) + ({Deg_h10 = 2} * h10 * h10) ) *
            ( ({Deg_h11 = 0}) + ({Deg_h11 = 1} * h11) + ({Deg_h11 = 2} * h11 * h11) ) 
        );
    
    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_1 ^= {Deg_postcode + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *

            {Deg_1 <= 2} *

            AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_2 ^= {Deg_1 + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_2 <= 2} *

            AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_3 ^= {Deg_2 + Deg_i2 + Deg_i3}) *

            {Deg_3 <= 2} *

            AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_4 ^= {Deg_3 + Deg_r2 + Deg_r3}) *

            {Deg_4 <= 2} *

            AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            (Deg_5 ^= {Deg_4 + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5}) *

            {Deg_5 <= 2} *

            AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            {(Deg_5 + Deg_t2 + Deg_t3 + Deg_t4) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );
    
    AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] +=
        DELTA_AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11];
}

ON BATCH UPDATE OF SHOP {
    
    DELTA_AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] := 
        AggSum([postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6],

            (DELTA SHOP)(postcode, s2, s3, s4, s5, s6) *

            DEGREE5(Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6) *

            ( ({Deg_s2 = 0}) + ({Deg_s2 = 1} * s2) + ({Deg_s2 = 2} * s2 * s2) ) *
            ( ({Deg_s3 = 0}) + ({Deg_s3 = 1} * s3) + ({Deg_s3 = 2} * s3 * s3) ) *
            ( ({Deg_s4 = 0}) + ({Deg_s4 = 1} * s4) + ({Deg_s4 = 2} * s4 * s4) ) *
            ( ({Deg_s5 = 0}) + ({Deg_s5 = 1} * s5) + ({Deg_s5 = 2} * s5 * s5) ) *
            ( ({Deg_s6 = 0}) + ({Deg_s6 = 1} * s6) + ({Deg_s6 = 2} * s6 * s6) )
        );

    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_1 ^= {Deg_postcode + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_1 <= 2} *

            AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_2 ^= {Deg_1 + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *            

            {Deg_2 <= 2} *

            AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_3 ^= {Deg_2 + Deg_i2 + Deg_i3}) *

            {Deg_3 <= 2} *

            AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_4 ^= {Deg_3 + Deg_r2 + Deg_r3}) *

            {Deg_4 <= 2} *

            AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            (Deg_5 ^= {Deg_4 + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5}) *

            {Deg_5 <= 2} *

            AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            {(Deg_5 + Deg_t2 + Deg_t3 + Deg_t4) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );

    AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] += 
        DELTA_AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6];
}

ON BATCH UPDATE OF INSTITUTION {

    DELTA_AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] := 
        AggSum([postcode, Deg_i2, Deg_i3],

            (DELTA INSTITUTION)(postcode, i2, i3) *

            DEGREE2(Deg_i2, Deg_i3) *

            ( ({Deg_i2 = 0}) + ({Deg_i2 = 1} * i2) + ({Deg_i2 = 2} * i2 * i2) ) *
            ( ({Deg_i3 = 0}) + ({Deg_i3 = 1} * i3) + ({Deg_i3 = 2} * i3 * i3) )
        );

    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_1 ^= {Deg_postcode + Deg_i2 + Deg_i3}) *

            {Deg_1 <= 2} *

            AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_2 ^= {Deg_1 + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *            

            {Deg_2 <= 2} *

            AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_3 ^= {Deg_2 + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_3 <= 2} *

            AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_4 ^= {Deg_3 + Deg_r2 + Deg_r3}) *

            {Deg_4 <= 2} *

            AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            (Deg_5 ^= {Deg_4 + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5}) *

            {Deg_5 <= 2} *

            AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            {(Deg_5 + Deg_t2 + Deg_t3 + Deg_t4) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );

    AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] +=
        DELTA_AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3];
}

ON BATCH UPDATE OF RESTAURANT {

    DELTA_AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] := 
        AggSum([postcode, Deg_r2, Deg_r3],

            (DELTA RESTAURANT)(postcode, r2, r3) *

            DEGREE2(Deg_r2, Deg_r3) *

            ( ({Deg_r2 = 0}) + ({Deg_r2 = 1} * r2) + ({Deg_r2 = 2} * r2 * r2) ) *
            ( ({Deg_r3 = 0}) + ({Deg_r3 = 1} * r3) + ({Deg_r3 = 2} * r3 * r3) )
        );
    
    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_1 ^= {Deg_postcode + Deg_r2 + Deg_r3}) *

            {Deg_1 <= 2} *

            AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_2 ^= {Deg_1 + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *            

            {Deg_2 <= 2} *

            AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_3 ^= {Deg_2 + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_3 <= 2} *

            AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_4 ^= {Deg_3 + Deg_i2 + Deg_i3}) *

            {Deg_4 <= 2} *

            AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            (Deg_5 ^= {Deg_4 + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5}) *

            {Deg_5 <= 2} *

            AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            {(Deg_5 + Deg_t2 + Deg_t3 + Deg_t4) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );    

    AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] +=
        DELTA_AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3];
}

ON BATCH UPDATE OF DEMOGRAPHICS {
    
    DELTA_AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] := 
        AggSum([postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5],

            (DELTA DEMOGRAPHICS)(postcode, d2, d3, d4, d5) *

            DEGREE4(Deg_d2, Deg_d3, Deg_d4, Deg_d5) *

            ( ({Deg_d2 = 0}) + ({Deg_d2 = 1} * d2) + ({Deg_d2 = 2} * d2 * d2) ) *
            ( ({Deg_d3 = 0}) + ({Deg_d3 = 1} * d3) + ({Deg_d3 = 2} * d3 * d3) ) *
            ( ({Deg_d4 = 0}) + ({Deg_d4 = 1} * d4) + ({Deg_d4 = 2} * d4 * d4) ) *
            ( ({Deg_d5 = 0}) + ({Deg_d5 = 1} * d5) + ({Deg_d5 = 2} * d5 * d5) ) 
        );

    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            (Deg_1 ^= {Deg_postcode + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5}) *

            {Deg_1 <= 2} *

            AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_2 ^= {Deg_1 + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *            

            {Deg_2 <= 2} *

            AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_3 ^= {Deg_2 + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_3 <= 2} *

            AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_4 ^= {Deg_3 + Deg_i2 + Deg_i3}) *

            {Deg_4 <= 2} *

            AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_5 ^= {Deg_4 + Deg_r2 + Deg_r3}) *

            {Deg_5 <= 2} *

            AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            {(Deg_5 + Deg_t2 + Deg_t3 + Deg_t4) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );

    AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] +=
        DELTA_AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5];        
}

ON BATCH UPDATE OF TRANSPORT {

    DELTA_AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] := 
        AggSum([postcode, Deg_t2, Deg_t3, Deg_t4],

            (DELTA TRANSPORT)(postcode, t2, t3, t4) *

            DEGREE3(Deg_t2, Deg_t3, Deg_t4) *

            ( ({Deg_t2 = 0}) + ({Deg_t2 = 1} * t2) + ({Deg_t2 = 2} * t2 * t2) ) *
            ( ({Deg_t3 = 0}) + ({Deg_t3 = 1} * t3) + ({Deg_t3 = 2} * t3 * t3) ) *
            ( ({Deg_t4 = 0}) + ({Deg_t4 = 1} * t4) + ({Deg_t4 = 2} * t4 * t4) ) 
        );


    AGG(float)[][Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4] += 
        AggSum([Deg_postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6, Deg_i2, Deg_i3, Deg_r2, Deg_r3, Deg_d2, Deg_d3, Deg_d4, Deg_d5, Deg_t2, Deg_t3, Deg_t4], 
            
            DEGREE1(Deg_postcode) *

            DELTA_AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] *

            (Deg_1 ^= {Deg_postcode + Deg_t2 + Deg_t3 + Deg_t4}) *

            {Deg_1 <= 2} *

            AGG_HOUSE(float)[][postcode, Deg_h2, Deg_h3, Deg_h4, Deg_h5, Deg_h6, Deg_h7, Deg_h8, Deg_h9, Deg_h10, Deg_h11] *

            (Deg_2 ^= {Deg_1 + Deg_h2 + Deg_h3 + Deg_h4 + Deg_h5 + Deg_h6 + Deg_h7 + Deg_h8 + Deg_h9 + Deg_h10 + Deg_h11}) *            

            {Deg_2 <= 2} *

            AGG_SHOP(float)[][postcode, Deg_s2, Deg_s3, Deg_s4, Deg_s5, Deg_s6] *

            (Deg_3 ^= {Deg_2 + Deg_s2 + Deg_s3 + Deg_s4 + Deg_s5 + Deg_s6}) *

            {Deg_3 <= 2} *

            AGG_INSTITUTION(float)[][postcode, Deg_i2, Deg_i3] *

            (Deg_4 ^= {Deg_3 + Deg_i2 + Deg_i3}) *

            {Deg_4 <= 2} *

            AGG_RESTAURANT(float)[][postcode, Deg_r2, Deg_r3] *

            (Deg_5 ^= {Deg_4 + Deg_r2 + Deg_r3}) *

            {Deg_5 <= 2} *

            AGG_DEMOGRAPHICS(float)[][postcode, Deg_d2, Deg_d3, Deg_d4, Deg_d5] *

            {(Deg_5 + Deg_d2 + Deg_d3 + Deg_d4 + Deg_d5) = 2} *
  
            ( ({Deg_postcode = 0}) + ({Deg_postcode = 1} * postcode) + ({Deg_postcode = 2} * postcode * postcode) )
        );

    AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4] +=
        DELTA_AGG_TRANSPORT(float)[][postcode, Deg_t2, Deg_t3, Deg_t4];
}

ON SYSTEM READY {
}