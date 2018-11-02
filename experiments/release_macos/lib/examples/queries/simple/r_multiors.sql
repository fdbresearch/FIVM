CREATE STREAM R(A int, B int) 
  FROM FILE 'examples/data/simple/r.dat' LINE DELIMITED csv;
  
SELECT sum(B) AS agg FROM R WHERE (R.A=1) OR (R.A=2) OR (R.A=3);
