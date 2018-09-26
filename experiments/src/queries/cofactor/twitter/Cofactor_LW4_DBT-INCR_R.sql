CREATE STREAM TWITTER1(B float, C float, D float)
FROM FILE './datasets/synthetic/input.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER2(A float, C float, D float)
FROM FILE './datasets/synthetic/input.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER3(A float, B float, D float)
FROM FILE './datasets/synthetic/input.tbl' LINE DELIMITED CSV(delimiter := ' ');

CREATE TABLE TWITTER4(A float, B float, C float)
FROM FILE './datasets/synthetic/input.tbl' LINE DELIMITED CSV(delimiter := ' ');

SELECT SUM(s2.A * s2.A), SUM(s2.A * s1.B), SUM(s2.A * s1.C), SUM(s2.A * s1.D),
       SUM(s1.B * s1.B), SUM(s1.B * s1.C), SUM(s1.B * s1.D),
       SUM(s1.C * s1.C), SUM(s1.C * s1.D),
       SUM(s1.D * s1.D)
FROM TWITTER1 s1, TWITTER2 s2, TWITTER3 s3, TWITTER4 s4
WHERE s1.B = s3.B AND s1.B = s4.B AND s3.B = s4.B AND
      s1.C = s2.C AND s1.C = s4.C AND s2.C = s4.C AND
      s1.D = s2.D AND s1.D = s3.D AND s2.D = s3.D AND
      s2.A = s3.A AND s2.A = s4.A AND s3.A = s4.A