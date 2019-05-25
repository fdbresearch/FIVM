IMPORT DTREE FROM FILE 'FQ2.txt';

CREATE TYPE RingRelation
FROM FILE 'ring/ring_relational_opt.hpp'
WITH PARAMETER SCHEMA (dynamic_concat);

CREATE STREAM LINEITEM (
        orderkey         INT,
        partkey          INT,
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

CREATE STREAM PART (
        partkey        INT,
        p_name         VARCHAR(55),
        p_mfgr         CHAR(25),
        p_brand        CHAR(10),
        p_type         VARCHAR(25),
        p_size         INT,
        p_container    CHAR(10),
        p_retailprice  DECIMAL,
        p_comment      VARCHAR(23)
    )
  FROM FILE './datasets/tpch/part.csv'
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

SELECT SUM(
    [lift<0>: RingRelation<[0, INT]>](orderkey) *
    [lift<1>: RingRelation<[1, INT]>](partkey) *
    [lift<2>: RingRelation<[2, INT, INT, DECIMAL, DECIMAL, DECIMAL, DECIMAL, CHAR(1), CHAR(1), DATE, DATE, DATE, CHAR(25), CHAR(10), VARCHAR(44)]>](l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) *
    [lift<16>: RingRelation<[16, VARCHAR(55), CHAR(25), CHAR(10), VARCHAR(25), INT, CHAR(10), DECIMAL, VARCHAR(23)]>](p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment) *
    [lift<24>: RingRelation<[24, INT]>](custkey) *
    [lift<25>: RingRelation<[25, INT]>](nationkey) *
    [lift<26>: RingRelation<[26, VARCHAR(25), VARCHAR(40), CHAR(15), DECIMAL, CHAR(10), VARCHAR(117)]>](c_name, c_address, c_phone, c_acctbal, c_mktsegment, c_comment) *
    [lift<32>: RingRelation<[32, CHAR(25), INT, VARCHAR(152)]>](n_name, n_regionkey, n_comment) *
    [lift<35>: RingRelation<[35, CHAR(1), DECIMAL, DATE, CHAR(15), CHAR(15), INT, VARCHAR(79)]>](o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment)
)
FROM LINEITEM NATURAL JOIN ORDERS NATURAL JOIN PART NATURAL JOIN CUSTOMER NATURAL JOIN NATION;