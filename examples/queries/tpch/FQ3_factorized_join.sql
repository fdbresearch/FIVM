IMPORT DTREE FROM FILE 'FQ3.txt';

CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

CREATE STREAM LINEITEM (
        orderkey         INT,
        partkey          INT,
        suppkey          INT,
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
        c_nationkey    INT,
        c_phone        CHAR(15),
        c_acctbal      DECIMAL,
        c_mktsegment   CHAR(10),
        c_comment      VARCHAR(117)
    )
  FROM FILE './datasets/tpch/customer.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM PARTSUPP (
        partkey         INT,
        suppkey         INT,
        ps_availqty     INT,
        ps_supplycost   DECIMAL,
        ps_comment      VARCHAR(199)
    )
  FROM FILE './datasets/tpch/partsupp.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM SUPPLIER (
        suppkey        INT,
        s_name         CHAR(25),
        s_address      VARCHAR(40),
        s_nationkey    INT,
        s_phone        CHAR(15),
        s_acctbal      DECIMAL,
        s_comment      VARCHAR(101)
    )
  FROM FILE './datasets/tpch/supplier.csv'
  LINE DELIMITED CSV (delimiter := '|');

SELECT SUM(
    [lift<0>: RingFactorizedRelation<[0, INT]>](orderkey) *
    [lift<1>: RingFactorizedRelation<[1, INT]>](partkey) *
    [lift<2>: RingFactorizedRelation<[2, INT]>](suppkey) *
    [lift<3>: RingFactorizedRelation<[3, INT, DECIMAL, DECIMAL, DECIMAL, DECIMAL, CHAR(1), CHAR(1), DATE, DATE, DATE, CHAR(25), CHAR(10), VARCHAR(44)]>](l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) *
    [lift<16>: RingFactorizedRelation<[16, INT, DECIMAL, VARCHAR(199)]>](ps_availqty, ps_supplycost, ps_comment) *
    [lift<19>: RingFactorizedRelation<[19, CHAR(25), VARCHAR(40), INT, CHAR(15), DECIMAL, VARCHAR(101)]>](s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment) *
    [lift<25>: RingFactorizedRelation<[25, INT]>](custkey) *
    [lift<26>: RingFactorizedRelation<[26, VARCHAR(25), VARCHAR(40), INT, CHAR(15), DECIMAL, CHAR(10), VARCHAR(117)]>](c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment) *    
    [lift<33>: RingFactorizedRelation<[33, CHAR(1), DECIMAL, DATE, CHAR(15), CHAR(15), INT, VARCHAR(79)]>](o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment)
)
FROM LINEITEM NATURAL JOIN ORDERS NATURAL JOIN CUSTOMER NATURAL JOIN PARTSUPP NATURAL JOIN SUPPLIER;