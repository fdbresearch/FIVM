CREATE TYPE TPCH12Payload
FROM FILE 'ring/tpch/ring_query12.hpp';

CREATE STREAM LINEITEM (
        orderkey         INT,
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


CREATE STREAM ORDERS (
        orderkey         INT,
        o_custkey        INT,
        o_orderstatus    CHAR(1),
        o_totalprice     DECIMAL,
        o_orderdate      DATE,
        o_orderpriority  CHAR(15),
        o_clerk          CHAR(15),
        o_shippriority   INT,
        o_comment        VARCHAR(79)
    )
  FROM FILE './datasets/tpch/orders.csv'
  LINE DELIMITED CSV (delimiter := '|');

SELECT l_shipmode, SUM([lift: TPCH12Payload](o_orderpriority))
FROM   lineitem NATURAL JOIN orders
WHERE  l_shipmode INLIST ('MAIL', 'SHIP')
  AND  l_commitdate < l_receiptdate
  AND  l_shipdate < l_commitdate
  AND  l_receiptdate >= DATE('1994-01-01')
  AND  l_receiptdate < DATE('1995-01-01')
GROUP BY l_shipmode;