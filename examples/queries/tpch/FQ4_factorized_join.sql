IMPORT DTREE FROM FILE 'FQ4.txt';

CREATE DISTRIBUTED TYPE RingFactorizedRelation
FROM FILE 'ring/ring_factorized.hpp'
WITH PARAMETER SCHEMA (dynamic_min);

CREATE STREAM LINEITEM (
        l_orderkey       INT,
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
    [lift<0>: RingFactorizedRelation<[0, INT]>](suppkey) *
    [lift<1>: RingFactorizedRelation<[1, INT]>](partkey) *
    [lift<2>: RingFactorizedRelation<[2, INT, INT, DECIMAL, DECIMAL, DECIMAL, DECIMAL, CHAR(1), CHAR(1), DATE, DATE, DATE, CHAR(25), CHAR(10), VARCHAR(44)]>](l_orderkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) *
    [lift<16>: RingFactorizedRelation<[16, INT, DECIMAL, VARCHAR(199)]>](ps_availqty, ps_supplycost, ps_comment) *
    [lift<19>: RingFactorizedRelation<[19, CHAR(25), VARCHAR(40), INT, CHAR(15), DECIMAL, VARCHAR(101)]>](s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment)
)
FROM LINEITEM NATURAL JOIN PARTSUPP NATURAL JOIN SUPPLIER;