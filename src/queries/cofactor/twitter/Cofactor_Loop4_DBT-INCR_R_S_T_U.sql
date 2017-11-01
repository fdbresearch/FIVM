CREATE STREAM TWITTER1(A float, B float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER2(B float, C float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER3(C float, D float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE STREAM TWITTER4(A float, D float)
FROM FILE './datasets/twitter/higgs-social_network.tbl' LINE DELIMITED CSV(delimiter := ' ');

SELECT SUM(t1.A * t1.A), SUM(t1.A * t1.B), SUM(t1.A * t2.C), SUM(t1.A * t3.D),
       SUM(t1.B * t1.B), SUM(t1.B * t2.C), SUM(t1.B * t3.D),
       SUM(t2.C * t2.C), SUM(t2.C * t3.D),
       SUM(t3.D * t3.D)
FROM TWITTER1 t1, TWITTER2 t2, TWITTER3 t3, TWITTER4 t4
WHERE t1.B = t2.B AND t2.C = t3.C AND t3.D = t4.D AND t1.A = t4.A;