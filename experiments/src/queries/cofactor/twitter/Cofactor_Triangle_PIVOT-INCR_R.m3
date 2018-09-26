-- Incrementally computing the cofactor matrix using the pivot method
--

-------------------- SOURCES --------------------
CREATE STREAM TWITTER1(A float, B float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER2(B float, C float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER3(A float, C float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE DEGREE1(Deg1 int)
FROM FILE './datasets/degree/Degree1.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP AGG_C(float)[][A:float, B:float, Deg_c:int] := 0;
DECLARE MAP AGG(float)[][Deg_a:int, Deg_b:int, Deg_c:int] := 0;
DECLARE MAP DELTA_AGG_B(float)[][A:float, Deg_b:int, Deg_c:int] := 0;
-------------------- QUERIES --------------------
DECLARE QUERY AGG := AGG(float)[][Deg_a, Deg_b, Deg_c];

------------------- TRIGGERS --------------------

ON BATCH UPDATE OF TWITTER1 {

     DELTA_AGG_B(float)[][A, Deg_b, Deg_c] := 
        AggSum([A, Deg_b, Deg_c],
            (DELTA TWITTER1)(A, B) * AGG_C(float)[][A, B, Deg_c] * DEGREE1(Deg_b) * 
            {(Deg_b + Deg_c) <= 2} *
            ( ({Deg_b = 0}) + ({Deg_b = 1} * B) + ({Deg_b = 2} * B * B) ) 
        );

    AGG(float)[][Deg_a, Deg_b, Deg_c] += 
        AggSum([Deg_a, Deg_b, Deg_c],
            DELTA_AGG_B(float)[][A, Deg_b, Deg_c] * DEGREE1(Deg_a) * 
            {(Deg_a + Deg_b + Deg_c) = 2} *
            ( ({Deg_a = 0}) + ({Deg_a = 1} * A) + ({Deg_a = 2} * A * A) ) 
        );
}

ON SYSTEM READY {

    AGG_C(float)[][A, B, Deg_c] := 
        AggSum([A, B, Deg_c], 
            TWITTER2(B, C) * TWITTER3(A, C) * DEGREE1(Deg_c) * 
            ( ({Deg_c = 0}) + ({Deg_c = 1} * C) + ({Deg_c = 2} * C * C) )
        );
}