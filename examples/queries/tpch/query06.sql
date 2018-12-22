IMPORT DTREE FROM FILE 'query06.txt';

CREATE STREAM LINEITEM (
        l_orderkey       INT,
        l_partkey        INT,
        l_suppkey        INT,
        l_linenumber     INT,
        l_quantity       DECIMAL,
        l_extendedprice  DECIMAL,
        l_discount       DECIMAL,
        l_tax            DECIMAL,
        l_returnflag     CHAR(1),
        l_linestatus     CHAR(1),
        l_shipdate       DATE,
        l_commitdate     DATE,
        l_receiptdate    DATE,
        l_shipinstruct   CHAR(25),
        l_shipmode       CHAR(10),
        l_comment        VARCHAR(44)
    )
  FROM FILE './datasets/tpch/lineitem.csv'
  LINE DELIMITED CSV (delimiter := '|');

SELECT SUM(l_extendedprice * l_discount)
FROM   lineitem
WHERE  l_shipdate >= DATE('1994-01-01')
  AND  l_shipdate < DATE('1995-01-01')
  AND  (l_discount BETWEEN (0.06 - 0.01) AND (0.06 + 0.01))
  AND  l_quantity < 24;