CREATE STREAM R(A int, B int)
FROM FILE 'examples/data/simple/r.dat' LINE DELIMITED csv;

SELECT C
FROM (SELECT R.A, COUNT(*) as C FROM R GROUP BY A) s
WHERE s.A = 3