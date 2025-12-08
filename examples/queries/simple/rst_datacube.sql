IMPORT DTREE FROM FILE 'rst2.txt';

CREATE TYPE DataCube
FROM FILE 'ring/ring_datacube.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

CREATE STREAM R(A int, B int) FROM FILE './datasets/simple/R.tbl' LINE DELIMITED CSV;
CREATE STREAM S(A int, C int, E int) FROM FILE './datasets/simple/S.tbl' LINE DELIMITED CSV;
CREATE STREAM T(C int, D int) FROM FILE './datasets/simple/T.tbl' LINE DELIMITED CSV;

SELECT SUM(
    [liftgroupby<0>: DataCube<[0,int]>](A) *
    [liftgroupby<1>: DataCube<[1,int]>](C) *
    [liftgroupby<2>: DataCube<[2,int]>](D)
)
FROM R NATURAL JOIN S NATURAL JOIN T;