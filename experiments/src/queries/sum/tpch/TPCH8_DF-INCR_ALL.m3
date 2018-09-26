-- Recomputing TPCH8
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
DECLARE MAP V_NATION1(long)[][C_NATIONKEY: long] := 0;
DECLARE MAP V_NATION2(long)[][S_NATIONKEY: long, N_NAME: string] := 0;

DECLARE MAP AGG_Part(long)[][P_PARTKEY: long] := 0;
DECLARE MAP AGG_Lineitem(float)[][L_ORDERKEY: long, L_PARTKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP AGG_Supplier(long)[][S_SUPPKEY: long, N_NAME: string] := 0;
DECLARE MAP AGG_LineitemSupplierPart(float)[][L_ORDERKEY: long] := 0;
DECLARE MAP AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long, YEAR: long] := 0;
DECLARE MAP AGG_Customer(long)[][C_CUSTKEY: long] := 0;
DECLARE MAP AGG_OrdersCustomer(long)[][O_ORDERKEY: long, YEAR: long] := 0;

DECLARE MAP DELTA_AGG_Part(long)[][P_PARTKEY: long] := 0;
DECLARE MAP DELTA_AGG_Lineitem(float)[][L_ORDERKEY: long, L_PARTKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP DELTA_AGG_Supplier(long)[][S_SUPPKEY: long, N_NAME: string] := 0;
DECLARE MAP DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY: long] := 0;
DECLARE MAP DELTA_AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long, YEAR: long] := 0;
DECLARE MAP DELTA_AGG_Customer(long)[][C_CUSTKEY: long] := 0;
DECLARE MAP DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY: long, YEAR: long] := 0;

DECLARE MAP QUERY8(float)[][YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY8 := QUERY8(float)[][YEAR];

------------------- TRIGGERS --------------------

ON BATCH UPDATE OF LINEITEM {
  
  DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] :=
    AggSum([L_ORDERKEY, L_PARTKEY, L_SUPPKEY],
      (DELTA LINEITEM)(L_ORDERKEY, L_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
      (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] :=
    AggSum([L_ORDERKEY],
      DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_Part(long)[][L_PARTKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, N_NAME]
    );

  QUERY8(float)[][YEAR] +=
    AggSum([YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] *
      AGG_OrdersCustomer(long)[][L_ORDERKEY, YEAR]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY];

  AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] +=
    DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY];
}

ON BATCH UPDATE OF ORDERS {

  DELTA_AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY, YEAR] := 
    AggSum([O_ORDERKEY, O_CUSTKEY, YEAR],
      (DELTA ORDERS)(O_ORDERKEY, O_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
      ({[date: long]('1996-12-31') >= O_ORDERDATE} * {O_ORDERDATE >= [date: long]('1995-1-1')}) *
      (YEAR ^= [date_part: long]('year', O_ORDERDATE))
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, YEAR] := 
    AggSum([O_ORDERKEY, YEAR],
      DELTA_AGG_Orders(int)[][O_ORDERKEY, O_CUSTKEY, YEAR] *
      AGG_Customer(long)[][O_CUSTKEY]
    );

  QUERY8(float)[][YEAR] +=
    AggSum([YEAR],
      DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY, YEAR] *
      AGG_LineitemSupplierPart(float)[][O_ORDERKEY]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, YEAR] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, YEAR];

  AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY, YEAR] +=
    DELTA_AGG_Orders(int)[][O_ORDERKEY, O_CUSTKEY, YEAR];
}

ON BATCH UPDATE OF CUSTOMER {

  DELTA_AGG_Customer(long)[][C_CUSTKEY] := 
    AggSum([C_CUSTKEY],
      (DELTA CUSTOMER)(C_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT) *
      V_NATION1(long)[][C_NATIONKEY]
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, YEAR] := 
    AggSum([O_ORDERKEY, YEAR],
      DELTA_AGG_Customer(long)[][C_CUSTKEY] * 
      AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY, YEAR]
    );

  QUERY8(float)[][YEAR] +=
    AggSum([YEAR],
      DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY, YEAR] *
      AGG_LineitemSupplierPart(float)[][O_ORDERKEY]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, YEAR] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, YEAR];

  AGG_Customer(long)[][C_CUSTKEY] +=
    DELTA_AGG_Customer(long)[][C_CUSTKEY];

}

ON BATCH UPDATE OF SUPPLIER {

  DELTA_AGG_Supplier(long)[][S_SUPPKEY, N_NAME] :=
    AggSum([S_SUPPKEY, N_NAME],
      (DELTA SUPPLIER)(S_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT) *
      V_NATION2(long)[][S_NATIONKEY, N_NAME]
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] :=
    AggSum([L_ORDERKEY],
      DELTA_AGG_Supplier(long)[][L_SUPPKEY, N_NAME] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_Part(long)[][L_PARTKEY]
    );

  QUERY8(float)[][YEAR] +=
    AggSum([YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] *
      AGG_OrdersCustomer(long)[][L_ORDERKEY, YEAR]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY];

  AGG_Supplier(long)[][S_SUPPKEY, N_NAME] +=
    DELTA_AGG_Supplier(long)[][S_SUPPKEY, N_NAME];
}

ON BATCH UPDATE OF PART {

  DELTA_AGG_Part(long)[][P_PARTKEY] :=
    AggSum([P_PARTKEY],
      (DELTA PART)(P_PARTKEY, P_NAME, P_MFGR, P_BRAND, P_TYPE, P_SIZE, P_CONTAINER, P_RETAILPRICE, P_COMMENT) *
      {P_TYPE = 'ECONOMY ANODIZED STEEL'}
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] :=
    AggSum([L_ORDERKEY],
      DELTA_AGG_Part(long)[][L_PARTKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, N_NAME]
    );

  QUERY8(float)[][YEAR] +=
    AggSum([YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY] *
      AGG_OrdersCustomer(long)[][L_ORDERKEY, YEAR]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY];

  AGG_Part(long)[][P_PARTKEY] +=
    DELTA_AGG_Part(long)[][P_PARTKEY];
}


ON SYSTEM READY {

  V_NATION1(long)[][C_NATIONKEY] :=
    AggSum([C_NATIONKEY],
      NATION(C_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT) *
      (R_NAME ^= 'AMERICA') *
      REGION(N_REGIONKEY, R_NAME, R_COMMENT)
    );

  V_NATION2(long)[][S_NATIONKEY, N_NAME] :=
    AggSum([S_NATIONKEY, N_NAME],
      NATION(S_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT)
    );
}