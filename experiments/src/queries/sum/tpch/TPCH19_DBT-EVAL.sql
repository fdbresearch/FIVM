CREATE TABLE LINEITEM (
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


CREATE TABLE ORDERS (
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

CREATE TABLE PART (
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


CREATE TABLE CUSTOMER (
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

CREATE TABLE SUPPLIER (
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

CREATE TABLE PARTSUPP (
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


SELECT SUM(l.extendedprice * (1 - l.discount) ) AS revenue
FROM lineitem l, part p
WHERE
    (
        p.partkey = l.partkey
        AND p.brand = 'Brand#12'
        AND ( p.container IN LIST ( 'SM CASE', 'SM BOX', 'SM PACK', 'SM PKG') )
        AND l.quantity >= 1 AND l.quantity <= 1 + 10 
        AND ( p.size BETWEEN 1 AND 5 )
        AND (l.shipmode IN LIST ('AIR', 'AIR REG') )
        AND l.shipinstruct = 'DELIVER IN PERSON' 
    )
    OR 
    (
        p.partkey = l.partkey
        AND p.brand = 'Brand#23'
        AND ( p.container IN LIST ('MED BAG', 'MED BOX', 'MED PKG', 'MED PACK') )
        AND l.quantity >= 10 AND l.quantity <= 10 + 10
        AND ( p.size BETWEEN 1 AND 10 )
        AND ( l.shipmode IN LIST ('AIR', 'AIR REG') )
        AND l.shipinstruct = 'DELIVER IN PERSON'
    )
    OR 
    (
        p.partkey = l.partkey
        AND p.brand = 'Brand#34'
        AND ( p.container IN LIST ( 'LG CASE', 'LG BOX', 'LG PACK', 'LG PKG') )
        AND l.quantity >= 20 AND l.quantity <= 20 + 10
        AND ( p.size BETWEEN 1 AND 15 )
        AND ( l.shipmode IN LIST ('AIR', 'AIR REG') )
        AND l.shipinstruct = 'DELIVER IN PERSON'
    );
