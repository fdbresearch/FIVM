-- Incremental computation of A1*A2*A3 for updates to A2
--

 -------------------- SOURCES --------------------
CREATE TABLE A1(i int, j int)
FROM FILE './datasets/mcm/A1.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM A2(i int, j int)
FROM FILE './datasets/mcm/A2.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE TABLE A3(i int, j int)
FROM FILE './datasets/mcm/A3.tbl' LINE DELIMITED CSV(delimiter := ',');

--------------------- MAPS ----------------------
DECLARE MAP A1(int)[][i:int, j:int] := 0;
DECLARE MAP BASE_A2(float)[][i:int, j:int] := 0;
DECLARE MAP A3(int)[][i:int, j:int] := 0;
DECLARE MAP Q(float)[][i:int, j:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY Q := Q(float)[][i,j];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF A2 {

    Q(float)[][i,j] += AggSum([i,j], AggSum([i,l], (DELTA A2)(k,l) * A1(int)[][i,k]) * A3(int)[][l,j]);

    BASE_A2(float)[][i,j] += (DELTA A2)(i,j);
}

ON SYSTEM READY {
    -- generate random data for A1 and (A3*A4)
}