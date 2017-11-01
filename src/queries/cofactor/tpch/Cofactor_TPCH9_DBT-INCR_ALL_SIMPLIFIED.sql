CREATE STREAM LINEITEM (
        orderkey       INT,
        partkey        INT,
        suppkey        INT,
        linenumber     DECIMAL,
        quantity       DECIMAL,
        extendedprice  DECIMAL,
        discount       DECIMAL,
        tax            DECIMAL,
        returnflag     DECIMAL,
        linestatus     DECIMAL,
        shipdate       DECIMAL,
        commitdate     DECIMAL,
        receiptdate    DECIMAL,
        shipinstruct   DECIMAL,
        shipmode       DECIMAL,
        comment        DECIMAL
    )
  FROM FILE './datasets/tpch/lineitem.csv'
  LINE DELIMITED CSV (delimiter := '|');


CREATE STREAM ORDERS (
        orderkey       INT,
        custkey        INT,
        orderstatus    CHAR(1),
        totalprice     DECIMAL,
        orderdate      DATE,
        orderpriority  CHAR(15),
        clerk          CHAR(15),
        shippriority   INT,
        comment        VARCHAR(79)
    )
  FROM FILE './datasets/tpch/orders.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM PART (
        partkey      INT,
        name         VARCHAR(55),
        mfgr         CHAR(25),
        brand        CHAR(10),
        type         VARCHAR(25),
        size         INT,
        container    CHAR(10),
        retailprice  DECIMAL,
        comment      VARCHAR(23)
    )
  FROM FILE './datasets/tpch/part.csv'
  LINE DELIMITED CSV (delimiter := '|');


CREATE STREAM CUSTOMER (
        custkey      INT,
        name         VARCHAR(25),
        address      VARCHAR(40),
        nationkey    INT,
        phone        CHAR(15),
        acctbal      DECIMAL,
        mktsegment   CHAR(10),
        comment      VARCHAR(117)
    )
  FROM FILE './datasets/tpch/customer.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM SUPPLIER (
        suppkey      INT,
        name         CHAR(25),
        address      VARCHAR(40),
        nationkey    INT,
        phone        CHAR(15),
        acctbal      DECIMAL,
        comment      VARCHAR(101)
    )
  FROM FILE './datasets/tpch/supplier.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM PARTSUPP (
        partkey      INT,
        suppkey      INT,
        availqty     INT,
        supplycost   DECIMAL,
        comment      VARCHAR(199)
    )
  FROM FILE './datasets/tpch/partsupp.csv'
  LINE DELIMITED CSV (delimiter := '|');


CREATE TABLE NATION (
        nationkey    INT,
        name         CHAR(25),
        regionkey    INT,
        comment      VARCHAR(152)
    )
  FROM FILE './datasets/tpch/nation.csv'
  LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE REGION (
        regionkey    INT,
        name         CHAR(25),
        comment      VARCHAR(152)
    )
  FROM FILE './datasets/tpch/region.csv'
  LINE DELIMITED CSV (delimiter := '|');


SELECT n.name AS nation, 
       EXTRACT(year from o.orderdate) AS o_year,
       -- SUM(l.orderkey*l.orderkey), SUM(l.orderkey*l.partkey), SUM(l.orderkey*l.suppkey), SUM(l.shipmode*l.comment)
       SUM(l.orderkey*l.orderkey), SUM(l.orderkey*l.partkey), SUM(l.orderkey*l.suppkey), SUM(l.orderkey*l.linenumber), SUM(l.orderkey*l.quantity), SUM(l.orderkey*l.extendedprice), SUM(l.orderkey*l.discount), SUM(l.orderkey*l.tax), SUM(l.orderkey*l.returnflag), SUM(l.orderkey*l.linestatus), SUM(l.orderkey*l.shipdate), SUM(l.orderkey*l.commitdate), SUM(l.orderkey*l.receiptdate), SUM(l.orderkey*l.shipinstruct), SUM(l.orderkey*l.shipmode), SUM(l.orderkey*l.comment), SUM(l.partkey*l.partkey), SUM(l.partkey*l.suppkey), SUM(l.partkey*l.linenumber), SUM(l.partkey*l.quantity), SUM(l.partkey*l.extendedprice), SUM(l.partkey*l.discount), SUM(l.partkey*l.tax), SUM(l.partkey*l.returnflag), SUM(l.partkey*l.linestatus), SUM(l.partkey*l.shipdate), SUM(l.partkey*l.commitdate), SUM(l.partkey*l.receiptdate), SUM(l.partkey*l.shipinstruct), SUM(l.partkey*l.shipmode), SUM(l.partkey*l.comment), SUM(l.suppkey*l.suppkey), SUM(l.suppkey*l.linenumber), SUM(l.suppkey*l.quantity), SUM(l.suppkey*l.extendedprice), SUM(l.suppkey*l.discount), SUM(l.suppkey*l.tax), SUM(l.suppkey*l.returnflag), SUM(l.suppkey*l.linestatus), SUM(l.suppkey*l.shipdate), SUM(l.suppkey*l.commitdate), SUM(l.suppkey*l.receiptdate), SUM(l.suppkey*l.shipinstruct), SUM(l.suppkey*l.shipmode), SUM(l.suppkey*l.comment), SUM(l.linenumber*l.linenumber), SUM(l.linenumber*l.quantity), SUM(l.linenumber*l.extendedprice), SUM(l.linenumber*l.discount), SUM(l.linenumber*l.tax), SUM(l.linenumber*l.returnflag), SUM(l.linenumber*l.linestatus), SUM(l.linenumber*l.shipdate), SUM(l.linenumber*l.commitdate), SUM(l.linenumber*l.receiptdate), SUM(l.linenumber*l.shipinstruct), SUM(l.linenumber*l.shipmode), SUM(l.linenumber*l.comment), SUM(l.quantity*l.quantity), SUM(l.quantity*l.extendedprice), SUM(l.quantity*l.discount), SUM(l.quantity*l.tax), SUM(l.quantity*l.returnflag), SUM(l.quantity*l.linestatus), SUM(l.quantity*l.shipdate), SUM(l.quantity*l.commitdate), SUM(l.quantity*l.receiptdate), SUM(l.quantity*l.shipinstruct), SUM(l.quantity*l.shipmode), SUM(l.quantity*l.comment), SUM(l.extendedprice*l.extendedprice), SUM(l.extendedprice*l.discount), SUM(l.extendedprice*l.tax), SUM(l.extendedprice*l.returnflag), SUM(l.extendedprice*l.linestatus), SUM(l.extendedprice*l.shipdate), SUM(l.extendedprice*l.commitdate), SUM(l.extendedprice*l.receiptdate), SUM(l.extendedprice*l.shipinstruct), SUM(l.extendedprice*l.shipmode), SUM(l.extendedprice*l.comment), SUM(l.discount*l.discount), SUM(l.discount*l.tax), SUM(l.discount*l.returnflag), SUM(l.discount*l.linestatus), SUM(l.discount*l.shipdate), SUM(l.discount*l.commitdate), SUM(l.discount*l.receiptdate), SUM(l.discount*l.shipinstruct), SUM(l.discount*l.shipmode), SUM(l.discount*l.comment), SUM(l.tax*l.tax), SUM(l.tax*l.returnflag), SUM(l.tax*l.linestatus), SUM(l.tax*l.shipdate), SUM(l.tax*l.commitdate), SUM(l.tax*l.receiptdate), SUM(l.tax*l.shipinstruct), SUM(l.tax*l.shipmode), SUM(l.tax*l.comment), SUM(l.returnflag*l.returnflag), SUM(l.returnflag*l.linestatus), SUM(l.returnflag*l.shipdate), SUM(l.returnflag*l.commitdate), SUM(l.returnflag*l.receiptdate), SUM(l.returnflag*l.shipinstruct), SUM(l.returnflag*l.shipmode), SUM(l.returnflag*l.comment), SUM(l.linestatus*l.linestatus), SUM(l.linestatus*l.shipdate), SUM(l.linestatus*l.commitdate), SUM(l.linestatus*l.receiptdate), SUM(l.linestatus*l.shipinstruct), SUM(l.linestatus*l.shipmode), SUM(l.linestatus*l.comment), SUM(l.shipdate*l.shipdate), SUM(l.shipdate*l.commitdate), SUM(l.shipdate*l.receiptdate), SUM(l.shipdate*l.shipinstruct), SUM(l.shipdate*l.shipmode), SUM(l.shipdate*l.comment), SUM(l.commitdate*l.commitdate), SUM(l.commitdate*l.receiptdate), SUM(l.commitdate*l.shipinstruct), SUM(l.commitdate*l.shipmode), SUM(l.commitdate*l.comment), SUM(l.receiptdate*l.receiptdate), SUM(l.receiptdate*l.shipinstruct), SUM(l.receiptdate*l.shipmode), SUM(l.receiptdate*l.comment), SUM(l.shipinstruct*l.shipinstruct), SUM(l.shipinstruct*l.shipmode), SUM(l.shipinstruct*l.comment), SUM(l.shipmode*l.shipmode), SUM(l.shipmode*l.comment), SUM(l.comment*l.comment)
  FROM  part p, supplier s, lineitem l, partsupp ps, orders o, nation n
 WHERE  s.suppkey = l.suppkey
   AND  ps.suppkey = l.suppkey 
   AND  ps.partkey = l.partkey
   AND  p.partkey = l.partkey
   AND  o.orderkey = l.orderkey 
   AND  s.nationkey = n.nationkey 
   AND  (p.name LIKE '%green%')
GROUP BY nation, o_year;
