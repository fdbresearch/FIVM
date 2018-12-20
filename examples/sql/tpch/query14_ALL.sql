CREATE TYPE TPCH14Payload
FROM FILE 'ring/tpch/ring_query14.hpp';

CREATE STREAM LINEITEM (
        l_orderkey       INT,
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

SELECT SUM([liftpart: TPCH14Payload](p_type) * 
           [liftlineitem: TPCH14Payload](l_extendedprice * (1 - l_discount)))
FROM lineitem NATURAL JOIN part
WHERE l_shipdate >= DATE('1995-09-01') 
  AND l_shipdate <  DATE('1995-10-01');