CREATE STREAM R(A float, B float) 
  FROM FILE 'examples/data/simple/r.dat' LINE DELIMITED
  CSV ();

/* We insert a LISTMAX to support incremental computation. */

SELECT SUM(A)/LISTMAX(1, 1+SUM(B)) FROM R

