IMPORT DTREE FROM FILE 'query01.txt';

CREATE TYPE TPCH1Payload
FROM FILE 'ring/tpch/ring_query01.hpp';

CREATE STREAM LINEITEM (
        orderkey       INT,
        partkey        INT,
        suppkey        INT,
        linenumber     INT,
        quantity       DECIMAL,
        extendedprice  DECIMAL,
        discount       DECIMAL,
        tax            DECIMAL,
        returnflag     CHAR(1),
        linestatus     CHAR(1),
        shipdate       DATE,
        commitdate     DATE,
        receiptdate    DATE,
        shipinstruct   CHAR(25),
        shipmode       CHAR(10),
        comment        VARCHAR(44)
    )
  FROM FILE './datasets/tpch/lineitem.csv'
  LINE DELIMITED CSV (delimiter := '|');

SELECT returnflag, linestatus, 
       SUM([lift: TPCH1Payload](
         quantity,
         extendedprice,
         extendedprice * (1-discount),
         extendedprice * (1-discount)*(1+tax),
         discount
       ))
FROM lineitem
WHERE shipdate <= DATE('1997-09-01')
GROUP BY returnflag, linestatus;