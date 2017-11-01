-- IVM of TPCH7
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


--------------------- MAPS ----------------------
DECLARE MAP V_NATION(long)[][N_NATIONKEY1: long, N_NAME1: string, N_NATIONKEY2: long, N_NAME2: string] := 0;
DECLARE MAP V_NATION_IP1(long)[][N_NATIONKEY1: long] := 0;
DECLARE MAP V_NATION_IP2(long)[][N_NATIONKEY2: long] := 0;

DECLARE MAP AGG_Lineitem(float)[][L_ORDERKEY: long, L_SUPPKEY: long, YEAR: long] := 0;
DECLARE MAP AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP AGG_Customer(long)[][C_CUSTKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP AGG_Supplier(long)[][S_SUPPKEY: long, S_NATIONKEY: long] := 0;
DECLARE MAP AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP AGG_LineitemSupplier(float)[][L_ORDERKEY: long, S_NATIONKEY: long, YEAR: long] := 0;

DECLARE MAP DELTA_AGG_Lineitem(float)[][L_ORDERKEY: long, L_SUPPKEY: long, YEAR: long] := 0;
DECLARE MAP DELTA_AGG_Orders(long)[][O_ORDERKEY: long, O_CUSTKEY: long] := 0;
DECLARE MAP DELTA_AGG_Customer(long)[][C_CUSTKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_Supplier(long)[][S_SUPPKEY: long, S_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_OrdersCustomer(long)[][O_ORDERKEY: long, C_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY: long, S_NATIONKEY: long, YEAR: long] := 0;

DECLARE MAP QUERY7(float)[][N_NAME1: string, N_NAME2: string, YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY7 := QUERY7(float)[][N_NAME1, N_NAME2, YEAR];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {
  
  DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY, YEAR] :=
    AggSum([L_ORDERKEY, L_SUPPKEY, YEAR],
      (DELTA LINEITEM)(L_ORDERKEY, L_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
      ({[date: long]('1996-12-31') >= L_SHIPDATE} * {L_SHIPDATE >= [date: long]('1995-1-1')}) * 
      (YEAR ^= [date_part: long]('year', L_SHIPDATE)) *
      (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
    );

  DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] :=
    AggSum([L_ORDERKEY, S_NATIONKEY, YEAR],
      DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY, YEAR] *
      AGG_Supplier(long)[][L_SUPPKEY, S_NATIONKEY]
    );

  QUERY7(float)[][S_NAME, C_NAME, YEAR] +=
    AggSum([S_NAME, C_NAME, YEAR],
      AggSum([S_NATIONKEY, C_NATIONKEY, YEAR],
        DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] *
        AGG_OrdersCustomer(long)[][L_ORDERKEY, C_NATIONKEY]
      ) *
      V_NATION(long)[][S_NATIONKEY, S_NAME, C_NATIONKEY, C_NAME]
    );

  AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] +=
    DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR];

  AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY, YEAR] +=
    DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY, YEAR];
}

ON BATCH UPDATE OF ORDERS {

  DELTA_AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] := 
    AggSum([O_ORDERKEY, O_CUSTKEY],
      (DELTA ORDERS)(O_ORDERKEY, O_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT)
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] := 
    AggSum([O_ORDERKEY, C_NATIONKEY],
      DELTA_AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY] *
      AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY]
    );

  QUERY7(float)[][S_NAME, C_NAME, YEAR] +=
    AggSum([S_NAME, C_NAME, YEAR],
      AggSum([S_NATIONKEY, C_NATIONKEY, YEAR],
        DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] *
        AGG_LineitemSupplier(float)[][O_ORDERKEY, S_NATIONKEY, YEAR]
      ) *
      V_NATION(long)[][S_NATIONKEY, S_NAME, C_NATIONKEY, C_NAME]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_NATIONKEY] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY];

  AGG_Orders(long)[][O_ORDERKEY, O_CUSTKEY] +=
    DELTA_AGG_Orders(int)[][O_ORDERKEY, O_CUSTKEY];
}

ON BATCH UPDATE OF CUSTOMER {

  DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] := 
    AggSum([C_CUSTKEY, C_NATIONKEY],
      (DELTA CUSTOMER)(C_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT) *
      V_NATION_IP2(long)[][C_NATIONKEY]
    );

  DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] := 
    AggSum([O_ORDERKEY, C_NATIONKEY],
      DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] *
      AGG_Orders(int)[][O_ORDERKEY, C_CUSTKEY]
    );

  QUERY7(float)[][S_NAME, C_NAME, YEAR] +=
    AggSum([S_NAME, C_NAME, YEAR],
      AggSum([S_NATIONKEY, C_NATIONKEY, YEAR],
        DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY] *
        AGG_LineitemSupplier(float)[][O_ORDERKEY, S_NATIONKEY, YEAR]
      ) *
      V_NATION(long)[][S_NATIONKEY, S_NAME, C_NATIONKEY, C_NAME]
    );

  AGG_OrdersCustomer(long)[][O_ORDERKEY, C_NATIONKEY] +=
    DELTA_AGG_OrdersCustomer(int)[][O_ORDERKEY, C_NATIONKEY];

  AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY] +=
    DELTA_AGG_Customer(long)[][C_CUSTKEY, C_NATIONKEY];

}

ON BATCH UPDATE OF SUPPLIER {

  DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] :=
    AggSum([S_SUPPKEY, S_NATIONKEY],
      (DELTA SUPPLIER)(S_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT) *
      V_NATION_IP1(long)[][S_NATIONKEY]
    );

  DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] :=
    AggSum([L_ORDERKEY, S_NATIONKEY, YEAR],
      DELTA_AGG_Supplier(long)[][L_SUPPKEY, S_NATIONKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_SUPPKEY, YEAR]
    );

  QUERY7(float)[][S_NAME, C_NAME, YEAR] +=
    AggSum([S_NAME, C_NAME, YEAR],
      AggSum([S_NATIONKEY, C_NATIONKEY, YEAR],
        DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] *
        AGG_OrdersCustomer(long)[][L_ORDERKEY, C_NATIONKEY]
      ) *
      V_NATION(long)[][S_NATIONKEY, S_NAME, C_NATIONKEY, C_NAME]
    );

  AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR] +=
    DELTA_AGG_LineitemSupplier(float)[][L_ORDERKEY, S_NATIONKEY, YEAR];

  AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] +=
    DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY];
}

ON SYSTEM READY {

  V_NATION(long)[][N_NATIONKEY1, N_NAME1, N_NATIONKEY2, N_NAME2] :=
    AggSum([N_NATIONKEY1, N_NAME1, N_NATIONKEY2, N_NAME2],
      NATION(N_NATIONKEY1, N_NAME1, N_REGIONKEY1, N_COMMENT1) *
      NATION(N_NATIONKEY2, N_NAME2, N_REGIONKEY2, N_COMMENT2) *
      (lift1 ^= (({N_NAME1 = 'FRANCE'} * {N_NAME2 = 'GERMANY'}) + ({N_NAME1 = 'GERMANY'} * {N_NAME2 = 'FRANCE'}))) * 
      {lift1 > 0} 
    );

  V_NATION_IP1(long)[][N_NATIONKEY1] :=
    EXISTS(AggSum([N_NATIONKEY1], V_NATION(long)[][N_NATIONKEY1, N_NAME1, N_NATIONKEY2, N_NAME2]));

  V_NATION_IP2(long)[][N_NATIONKEY2] :=
    EXISTS(AggSum([N_NATIONKEY2], V_NATION(long)[][N_NATIONKEY1, N_NAME1, N_NATIONKEY2, N_NAME2]));
}