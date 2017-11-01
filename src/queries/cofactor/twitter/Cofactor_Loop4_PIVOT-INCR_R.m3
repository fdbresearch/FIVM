-- Incremental computation of the cofactor matrix using the pivot method
--

-------------------- SOURCES --------------------
CREATE STREAM TWITTER1(a float, b float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER2(b float, c float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER3(c float, d float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER4(a float, d float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE DEGREE1(Deg1 int)
FROM FILE './datasets/degree/Degree1.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP AGG_D(float)[][a:float, c:float, Deg_d:int] := 0;
DECLARE MAP AGG_C(float)[][a:float, b:float, Deg_c:int, Deg_d:int] := 0;
DECLARE MAP AGG(float)[][Deg_a:int, Deg_b:int, Deg_c:int, Deg_d:int] := 0;
DECLARE MAP DELTA_AGG_B(float)[][a:float, Deg_b:int, Deg_c:int, Deg_d:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][Deg_a, Deg_b, Deg_c, Deg_d];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF TWITTER1 {
    
    DELTA_AGG_B(float)[][a, Deg_b, Deg_c, Deg_d] := 
        AggSum([a, Deg_b, Deg_c, Deg_d],
            (DELTA TWITTER1)(a, b) * AGG_C(float)[][a, b, Deg_c, Deg_d] * DEGREE1(Deg_b) *
            {(Deg_b + Deg_c + Deg_d) <= 2} *
            ( ({Deg_b = 0}) + ({Deg_b = 1} * b) + ({Deg_b = 2} * b * b) )  
        );

    AGG(float)[][Deg_a, Deg_b, Deg_c, Deg_d] += 
        AggSum([Deg_a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_B(float)[][A, Deg_b, Deg_c, Deg_d] * DEGREE1(Deg_a) * 
            {(Deg_a + Deg_b + Deg_c + Deg_d) = 2} *
            ( ({Deg_a = 0}) + ({Deg_a = 1} * a) + ({Deg_a = 2} * a * a) ) 
        );
}

ON SYSTEM READY {

    AGG_D(float)[][a, c, Deg_d] := 
        AggSum([a, c, Deg_d],
            TWITTER3(c, d) * TWITTER4(a, d) * DEGREE1(Deg_d) *
            ( ({Deg_d = 0}) + ({Deg_d = 1} * d) + ({Deg_d = 2} * d * d) )
        );

    AGG_C(float)[][a, b, Deg_c, Deg_d] :=
        AggSum([a, b, Deg_c, Deg_d],
            AGG_D(float)[][a, c, Deg_d] * TWITTER2(b, c) * DEGREE1(Deg_c) *
            {(Deg_c + Deg_d) <= 2} *
            ( ({Deg_c = 0}) + ({Deg_c = 1} * c) + ({Deg_c = 2} * c * c) )
        );
}