CREATE STREAM TWITTER1(A float, B float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER2(B float, C float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER3(A float, C float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

SELECT SUM(t1.A * t1.A), SUM(t1.A * t1.B), SUM(t1.A * t2.C), SUM(t1.B * t1.B), SUM(t1.B * t2.C), SUM(t2.C * t2.C)
FROM TWITTER1 t1, TWITTER2 t2, TWITTER3 t3
WHERE t1.B = t2.B AND t2.C = t3.C AND t1.A = t3.A;