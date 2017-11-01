-- IVM of TPCH10
--

-------------------- SOURCES --------------------
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


CREATE TABLE NATION (
        nationkey    INT,
        name         CHAR(25),
        regionkey    INT,
        comment      VARCHAR(152)
    )
  FROM FILE './datasets/tpch/nation.csv'
  LINE DELIMITED CSV (delimiter := '|');


--------------------- MAPS ----------------------
DECLARE MAP AGG_Lineitem(float)[][L_ORDERKEY: long] := 0;
DECLARE MAP AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP AGG_Customer(long)[][C_CUSTKEY: long, C_NAME: string, C_ACCTBAL: double, N_NAME: string, C_ADDRESS: string, C_PHONE: string, C_COMMENT: string] := 0;
DECLARE MAP AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_CUSTKEY: long, C_NAME: string, C_ACCTBAL: double, N_NAME: string, C_ADDRESS: string, C_PHONE: string, C_COMMENT: string] := 0;

DECLARE MAP DELTA_AGG_Lineitem(float)[][L_ORDERKEY: long] := 0;
DECLARE MAP DELTA_AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP DELTA_AGG_Customer(long)[][C_CUSTKEY: long, C_NAME: string, C_ACCTBAL: double, N_NAME: string, C_ADDRESS: string, C_PHONE: string, C_COMMENT: string] := 0;
DECLARE MAP DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_CUSTKEY: long, C_NAME: string, C_ACCTBAL: double, N_NAME: string, C_ADDRESS: string, C_PHONE: string, C_COMMENT: string] := 0;

DECLARE MAP QUERY10(float)[][C_CUSTKEY: long, C_NAME: string, C_ACCTBAL: double, N_NAME: string, C_ADDRESS: string, C_PHONE: string, C_COMMENT: string] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY10 := QUERY10(float)[][O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT];

------------------- TRIGGERS --------------------

ON BATCH UPDATE OF LINEITEM {

  DELTA_AGG_Lineitem(float)[][L_ORDERKEY] := 
    AggSum([L_ORDERKEY],
      (DELTA LINEITEM)(L_ORDERKEY, L_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) * 
      (L_RETURNFLAG ^= 'R') *
      (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
    );

  QUERY10(float)[][C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    AggSum([C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      DELTA_AGG_Lineitem(float)[][L_ORDERKEY] *
      AGG_OrdersCustomer(int)[][L_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT]
    );

  AGG_Lineitem(float)[][L_ORDERKEY] += DELTA_AGG_Lineitem(float)[][L_ORDERKEY];
  
}

ON BATCH UPDATE OF ORDERS {

  DELTA_AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] := 
    AggSum([O_ORDERKEY, O_CUSTKEY],
      (DELTA ORDERS)(O_ORDERKEY, O_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
      ({[date: date]('1994-1-1') > O_ORDERDATE} * {O_ORDERDATE >= [date: date]('1993-10-1')})
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] := 
    AggSum([O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      DELTA_AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY] *
      AGG_Customer(long)[][C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT]
    );

  QUERY10(float)[][C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    AggSum([C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] *
      AGG_Lineitem(float)[][O_ORDERKEY]      
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT];

  AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] +=
    DELTA_AGG_Orders(int)[][O_ORDERKEY, O_CUSTKEY];
  
}

ON BATCH UPDATE OF CUSTOMER {

  DELTA_AGG_Customer(long)[][O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] := 
    AggSum([O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      (DELTA CUSTOMER)(O_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT) *
      NATION(C_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT)
    );

  DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY, O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] := 
    AggSum([O_ORDERKEY, O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      DELTA_AGG_Customer(long)[][O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] *
      AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY]
    );

  QUERY10(float)[][C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    AggSum([C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT],
      DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] *
      AGG_Lineitem(float)[][O_ORDERKEY]      
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY, C_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT];

  AGG_Customer(long)[][O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT] +=
    DELTA_AGG_Customer(long)[][O_CUSTKEY, C_NAME, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT]; 
  
}

ON SYSTEM READY {
}