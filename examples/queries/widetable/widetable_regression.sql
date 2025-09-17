IMPORT DTREE FROM FILE 'rwide.txt';

CREATE TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree4_continuous.hpp'
WITH PARAMETER SCHEMA (dynamic_min, static, dynamic_sum);

CREATE STREAM R(A float, B float, C float, D float, E float, FF float, G float, H float, J float)
FROM FILE './datasets/widetable/R.csv' LINE DELIMITED CSV(delimiter := ',');

SELECT SUM(
    [lift<0>: RingCofactor<0,double,9>](A, B, C, D, E, FF, G, H, J)
)
FROM R;