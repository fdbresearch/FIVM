CREATE STREAM R(A int, B int) 
  FROM FILE 'examples/data/simple/r.dat' LINE DELIMITED csv;

SELECT p.A, sum(p.value) AS Q FROM (
   SELECT R.A, sum(R.B) AS value FROM R GROUP BY R.A
) p 
WHERE p.value > 5
GROUP BY p.A;
