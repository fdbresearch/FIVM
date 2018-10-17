CREATE STREAM R(A int, B float) FROM FILE 'R.dat' LINE DELIMITED CSV;
CREATE TABLE S(A int, C int, E float, extra int) FROM FILE 'S.dat' LINE DELIMITED CSV;
CREATE STREAM T(C int, D float) FROM FILE 'T.dat' LINE DELIMITED CSV;

SELECT SUM((A+B*extra)*B*D) FROM R NATURAL JOIN S NATURAL JOIN T;