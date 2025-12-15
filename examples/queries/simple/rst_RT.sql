IMPORT DTREE FROM FILE 'rst.txt';

CREATE STREAM R(A int, B float) FROM FILE './datasets/simple/R.tbl' LINE DELIMITED CSV;
CREATE TABLE S(A int, C int, E float, extra int) FROM FILE './datasets/simple/S.tbl' LINE DELIMITED CSV;
CREATE STREAM T(C int, D float) FROM FILE './datasets/simple/T.tbl' LINE DELIMITED CSV;

SELECT SUM(A*B*C*D) FROM R NATURAL JOIN S NATURAL JOIN T;