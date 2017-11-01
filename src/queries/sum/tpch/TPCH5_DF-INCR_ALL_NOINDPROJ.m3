-- IVM of TPCH5 w/o indicator projections
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

--------------------- MAPS ----------------------
DECLARE MAP V_NATION_REGION(long)[][N_NATIONKEY: long, N_NAME: string] := 0;

DECLARE MAP AGG_Lineitem(float)[][L_ORDERKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP AGG_Customer(long)[][C_CUSTKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP AGG_Supplier(long)[][S_SUPPKEY: long, S_NATIONKEY: long] := 0;
DECLARE MAP AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP AGG_LineitemSupplier(float)[][L_ORDERKEY: long, S_NATIONKEY: long] := 0;

DECLARE MAP DELTA_AGG_Lineitem(float)[][L_ORDERKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP DELTA_AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP DELTA_AGG_Customer(long)[][C_CUSTKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_Supplier(long)[][S_SUPPKEY: long, S_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY: long, S_NATIONKEY: long] := 0;

DECLARE MAP QUERY5(float)[][C_NAME: string] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY5 := QUERY5(float)[][C_NAME];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {
  
  DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY] :=
    AggSum([L_ORDERKEY, L_SUPPKEY],
      (DELTA LINEITEM)(L_ORDERKEY, L_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
      (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
    );

  DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] :=
    AggSum([L_ORDERKEY, S_NATIONKEY],
      DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, S_NATIONKEY]
    );

  QUERY5(float)[][N_NAME] +=
    AggSum([N_NAME],
      AggSum([S_NATIONKEY],
        DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] *
        AGG_OrdersCustomer(long)[][L_ORDERKEY, S_NATIONKEY]
      ) *
      V_NATION_REGION(long)[][S_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] +=
    DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY];

  AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY] +=
    DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY];
}

ON BATCH UPDATE OF ORDERS {

  DELTA_AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] := 
    AggSum([O_ORDERKEY, O_CUSTKEY],
      (DELTA ORDERS)(O_ORDERKEY, O_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
      ({O_ORDERDATE >= [date: date]('1994-1-1')} * {[date: date]('1995-1-1') > O_ORDERDATE})
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] := 
    AggSum([O_ORDERKEY, C_NATIONKEY],
      DELTA_AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY] *
      AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY]
    );

  QUERY5(float)[][N_NAME] +=
    AggSum([N_NAME],
      AggSum([C_NATIONKEY],
        DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] *
        AGG_LineitemSupplier(float)[][O_ORDERKEY, C_NATIONKEY]
      ) *
      V_NATION_REGION(long)[][C_NATIONKEY, N_NAME]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_NATIONKEY] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY];

  AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] +=
    DELTA_AGG_Orders(int)[][O_ORDERKEY, O_CUSTKEY];
}

ON BATCH UPDATE OF CUSTOMER {

  DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] := 
    AggSum([C_CUSTKEY, C_NATIONKEY],
      (DELTA CUSTOMER)(C_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT)
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] := 
    AggSum([O_ORDERKEY, C_NATIONKEY],
      DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] *
      AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY]
    );

  QUERY5(float)[][N_NAME] +=
    AggSum([N_NAME],
      AggSum([C_NATIONKEY],
        DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] *
        AGG_LineitemSupplier(float)[][O_ORDERKEY, C_NATIONKEY]
      ) *
      V_NATION_REGION(long)[][C_NATIONKEY, N_NAME]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_NATIONKEY] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY];

  AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] +=
    DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY];

}

ON BATCH UPDATE OF SUPPLIER {

  DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] :=
    AggSum([S_SUPPKEY, S_NATIONKEY],
      (DELTA SUPPLIER)(S_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT)
    );

  DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] :=
    AggSum([L_ORDERKEY, S_NATIONKEY],
      DELTA_AGG_Supplier(long)[][L_SUPPKEY, S_NATIONKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY]
    );

  QUERY5(float)[][N_NAME] +=
    AggSum([N_NAME],
      AggSum([S_NATIONKEY],
        DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] *
        AGG_OrdersCustomer(long)[][L_ORDERKEY, S_NATIONKEY]
      ) *
      V_NATION_REGION(long)[][S_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY] +=
    DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY];

  AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] +=
    DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY];
}

ON SYSTEM READY {

  V_NATION_REGION(long)[][N_NATIONKEY, N_NAME] :=
    AggSum([N_NATIONKEY, N_NAME],
      (R_NAME ^= 'ASIA') * 
      NATION(N_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT) * 
      REGION(N_REGIONKEY, R_NAME, R_COMMENT)
    );
}