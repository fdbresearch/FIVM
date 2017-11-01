-- Incremental computation of the cofactor matrix using the pivot method
--

-------------------- SOURCES --------------------
CREATE STREAM TWITTER1(a float, b float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER2(b float, c float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER3(c float, d float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER4(a float, d float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE DEGREE1(Deg1 int)
FROM FILE './datasets/degree/Degree1.tbl' LINE DELIMITED CSV(delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP BASE_TWITTER1(int)[][a:float, b:float] := 0;
DECLARE MAP BASE_TWITTER2(int)[][b:float, c:float] := 0;
DECLARE MAP BASE_TWITTER3(int)[][c:float, d:float] := 0;
DECLARE MAP BASE_TWITTER4(int)[][a:float, d:float] := 0;

DECLARE MAP AGG_D(float)[][a:float, c:float, Deg_d:int] := 0;
DECLARE MAP AGG_C(float)[][a:float, b:float, Deg_c:int, Deg_d:int] := 0;
DECLARE MAP AGG(float)[][Deg_a:int, Deg_b:int, Deg_c:int, Deg_d:int] := 0;

DECLARE MAP DELTA_AGG_D(float)[][a:float, c:float, Deg_d:int] := 0;
DECLARE MAP DELTA_AGG_C(float)[][a:float, b:float, Deg_c:int, Deg_d:int] := 0;
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

    BASE_TWITTER1(int)[][a, b] += (DELTA TWITTER1)(a, b);
}

ON BATCH UPDATE OF TWITTER2 {
    
    DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] :=
        AggSum([a, b, Deg_c, Deg_d],
            (DELTA TWITTER2)(b, c) * AGG_D(float)[][a, c, Deg_d] * DEGREE1(Deg_c) *
            {(Deg_c + Deg_d) <= 2} *
            ( ({Deg_c = 0}) + ({Deg_c = 1} * c) + ({Deg_c = 2} * c * c) )
        );

    DELTA_AGG_B(float)[][a, Deg_b, Deg_c, Deg_d] := 
        AggSum([a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] * BASE_TWITTER1(int)[][a, b] * DEGREE1(Deg_b) *
            {(Deg_b + Deg_c + Deg_d) <= 2} *
            ( ({Deg_b = 0}) + ({Deg_b = 1} * b) + ({Deg_b = 2} * b * b) )  
        );

    AGG(float)[][Deg_a, Deg_b, Deg_c, Deg_d] += 
        AggSum([Deg_a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_B(float)[][A, Deg_b, Deg_c, Deg_d] * DEGREE1(Deg_a) * 
            {(Deg_a + Deg_b + Deg_c + Deg_d) = 2} *
            ( ({Deg_a = 0}) + ({Deg_a = 1} * a) + ({Deg_a = 2} * a * a) ) 
        );


    AGG_C(float)[][a, b, Deg_c, Deg_d] += DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d];
 
    BASE_TWITTER2(int)[][b, c] += (DELTA TWITTER2)(b, c);
}

ON BATCH UPDATE OF TWITTER3 {
 
    DELTA_AGG_D(float)[][a, c, Deg_d] := 
        AggSum([a, c, Deg_d],
            (DELTA TWITTER3)(c, d) * BASE_TWITTER4(a, d) * DEGREE1(Deg_d) *
            ( ({Deg_d = 0}) + ({Deg_d = 1} * d) + ({Deg_d = 2} * d * d) )
        );

    DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] :=
        AggSum([a, b, Deg_c, Deg_d],
            DELTA_AGG_D(float)[][a, c, Deg_d] * BASE_TWITTER2(int)[][b, c] * DEGREE1(Deg_c) *
            {(Deg_c + Deg_d) <= 2} *
            ( ({Deg_c = 0}) + ({Deg_c = 1} * c) + ({Deg_c = 2} * c * c) )
        );

    DELTA_AGG_B(float)[][a, Deg_b, Deg_c, Deg_d] := 
        AggSum([a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] * BASE_TWITTER1(int)[][a, b] * DEGREE1(Deg_b) *
            {(Deg_b + Deg_c + Deg_d) <= 2} *
            ( ({Deg_b = 0}) + ({Deg_b = 1} * b) + ({Deg_b = 2} * b * b) )  
        );

    AGG(float)[][Deg_a, Deg_b, Deg_c, Deg_d] += 
        AggSum([Deg_a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_B(float)[][A, Deg_b, Deg_c, Deg_d] * DEGREE1(Deg_a) * 
            {(Deg_a + Deg_b + Deg_c + Deg_d) = 2} *
            ( ({Deg_a = 0}) + ({Deg_a = 1} * a) + ({Deg_a = 2} * a * a) ) 
        );

    AGG_D(float)[][a, c, Deg_d] += DELTA_AGG_D(float)[][a, c, Deg_d];

    AGG_C(float)[][a, b, Deg_c, Deg_d] += DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d];

    BASE_TWITTER3(int)[][c, d] += (DELTA TWITTER3)(c, d);
}

ON BATCH UPDATE OF TWITTER4 {
    
    DELTA_AGG_D(float)[][a, c, Deg_d] := 
        AggSum([a, c, Deg_d],
            (DELTA TWITTER4)(a, d) * BASE_TWITTER3(c, d) * DEGREE1(Deg_d) *
            ( ({Deg_d = 0}) + ({Deg_d = 1} * d) + ({Deg_d = 2} * d * d) )
        );

    DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] :=
        AggSum([a, b, Deg_c, Deg_d],
            DELTA_AGG_D(float)[][a, c, Deg_d] * BASE_TWITTER2(int)[][b, c] * DEGREE1(Deg_c) *
            {(Deg_c + Deg_d) <= 2} *
            ( ({Deg_c = 0}) + ({Deg_c = 1} * c) + ({Deg_c = 2} * c * c) )
        );

    DELTA_AGG_B(float)[][a, Deg_b, Deg_c, Deg_d] := 
        AggSum([a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d] * BASE_TWITTER1(int)[][a, b] * DEGREE1(Deg_b) *
            {(Deg_b + Deg_c + Deg_d) <= 2} *
            ( ({Deg_b = 0}) + ({Deg_b = 1} * b) + ({Deg_b = 2} * b * b) )  
        );

    AGG(float)[][Deg_a, Deg_b, Deg_c, Deg_d] += 
        AggSum([Deg_a, Deg_b, Deg_c, Deg_d],
            DELTA_AGG_B(float)[][A, Deg_b, Deg_c, Deg_d] * DEGREE1(Deg_a) * 
            {(Deg_a + Deg_b + Deg_c + Deg_d) = 2} *
            ( ({Deg_a = 0}) + ({Deg_a = 1} * a) + ({Deg_a = 2} * a * a) ) 
        );

    AGG_D(float)[][a, c, Deg_d] += DELTA_AGG_D(float)[][a, c, Deg_d];

    AGG_C(float)[][a, b, Deg_c, Deg_d] += DELTA_AGG_C(float)[][a, b, Deg_c, Deg_d];

    BASE_TWITTER4(int)[][a, d] += (DELTA TWITTER4)(a, d);
}

ON SYSTEM READY {
}