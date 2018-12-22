IMPORT DTREE FROM FILE 'query10.txt';

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
        custkey          INT,
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

CREATE STREAM CUSTOMER (
        custkey        INT,
        c_name         VARCHAR(25),
        c_address      VARCHAR(40),
        nationkey      INT,
        c_phone        CHAR(15),
        c_acctbal      DECIMAL,
        c_mktsegment   CHAR(10),
        c_comment      VARCHAR(117)
    )
  FROM FILE './datasets/tpch/customer.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE NATION (
        nationkey      INT,
        n_name         CHAR(25),
        n_regionkey    INT,
        n_comment      VARCHAR(152)
    )
  FROM FILE './datasets/tpch/nation.csv'
  LINE DELIMITED CSV (delimiter := '|');

SELECT  custkey, c_name, 
        c_acctbal,
        n_name,
        c_address,
        c_phone,
        c_comment,
        SUM(l_extendedprice * (1 - l_discount))
FROM    customer NATURAL JOIN orders NATURAL JOIN lineitem NATURAL JOIN nation
WHERE   o_orderdate >= DATE('1993-10-01')
  AND   o_orderdate < DATE('1994-01-01')
  AND   l_returnflag = 'R'
GROUP BY custkey, c_name, c_acctbal, c_phone, n_name, c_address, c_comment;
