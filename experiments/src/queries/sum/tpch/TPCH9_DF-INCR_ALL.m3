-- IVM of TPCH8
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

--------------------- MAPS ----------------------
DECLARE MAP V_NATION(long)[][N_NATIONKEY: long, N_NAME: string] := 0;

DECLARE MAP AGG_Lineitem(float)[][L_ORDERKEY: long, L_PARTKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP AGG_Part(long)[][P_PARTKEY: long] := 0;
DECLARE MAP AGG_Supplier(long)[][S_SUPPKEY: long, N_NATIONKEY: long] := 0;
DECLARE MAP AGG_PartSupp(long)[][PS_PARTKEY: long, PS_SUPPKEY: long] := 0;
DECLARE MAP AGG_LineitemSupplierPart(float)[][L_ORDERKEY: long, N_NATIONKEY: long] := 0;
DECLARE MAP AGG_Orders(long)[][O_ORDERKEY: long, YEAR: long] := 0;

DECLARE MAP DELTA_AGG_Lineitem(float)[][L_ORDERKEY: long, L_PARTKEY: long, L_SUPPKEY: long] := 0;
DECLARE MAP DELTA_AGG_Part(long)[][P_PARTKEY: long] := 0;
DECLARE MAP DELTA_AGG_Supplier(long)[][S_SUPPKEY: long, N_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_PartSupp(long)[][PS_PARTKEY: long, PS_SUPPKEY: long] := 0;
DECLARE MAP DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY: long, N_NATIONKEY: long] := 0;
DECLARE MAP DELTA_AGG_Orders(long)[][O_ORDERKEY: long, YEAR: long] := 0;

DECLARE MAP QUERY9(float)[][N_NAME: string, YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY9 := QUERY9(float)[][N_NAME, YEAR];

------------------- TRIGGERS --------------------


ON BATCH UPDATE OF LINEITEM {
  
  DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] :=
    AggSum([L_ORDERKEY, L_PARTKEY, L_SUPPKEY],
      (DELTA LINEITEM)(L_ORDERKEY, L_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
      (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] :=
    AggSum([L_ORDERKEY, N_NATIONKEY],
      DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_Part(long)[][L_PARTKEY] *
      AGG_PartSupp(long)[][L_PARTKEY, L_SUPPKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, N_NATIONKEY]
    );

  QUERY9(float)[][N_NAME, YEAR] +=
    AggSum([N_NAME, YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] *
      AGG_Orders(long)[][L_ORDERKEY, YEAR] * 
      V_NATION(long)[][N_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY];

  AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] +=
    DELTA_AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY];
}

ON BATCH UPDATE OF ORDERS {

  DELTA_AGG_Orders(long)[][O_ORDERKEY, YEAR] := 
    AggSum([O_ORDERKEY, YEAR],
      (DELTA ORDERS)(O_ORDERKEY, O_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
      (YEAR ^= [date_part: long]('year', O_ORDERDATE))
    );

  QUERY9(float)[][N_NAME, YEAR] +=
    AggSum([N_NAME, YEAR],
      DELTA_AGG_Orders(long)[][O_ORDERKEY, YEAR] *
      AGG_LineitemSupplierPart(float)[][O_ORDERKEY, N_NATIONKEY] *
      V_NATION(long)[][N_NATIONKEY, N_NAME]      
    );

  AGG_Orders(long)[][O_ORDERKEY, YEAR] +=
    DELTA_AGG_Orders(int)[][O_ORDERKEY, YEAR];
}

ON BATCH UPDATE OF PARTSUPP {

  DELTA_AGG_PartSupp(long)[][PS_PARTKEY, PS_SUPPKEY] := 
    AggSum([PS_PARTKEY, PS_SUPPKEY],
      (DELTA PARTSUPP)(PS_PARTKEY, PS_SUPPKEY, PS_AVAILQTY, PS_SUPPLYCOST, PS_COMMENT)
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] :=
    AggSum([L_ORDERKEY, N_NATIONKEY],
      DELTA_AGG_PartSupp(long)[][L_PARTKEY, L_SUPPKEY] *
      AGG_Part(long)[][L_PARTKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, N_NATIONKEY]
    );

  QUERY9(float)[][N_NAME, YEAR] +=
    AggSum([N_NAME, YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] *
      AGG_Orders(long)[][L_ORDERKEY, YEAR] * 
      V_NATION(long)[][N_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY];

  AGG_PartSupp(long)[][PS_PARTKEY, PS_SUPPKEY] +=
    DELTA_AGG_PartSupp(long)[][PS_PARTKEY, PS_SUPPKEY];

}

ON BATCH UPDATE OF SUPPLIER {

  DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] :=
    AggSum([S_SUPPKEY, S_NATIONKEY],
      (DELTA SUPPLIER)(S_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT)
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] :=
    AggSum([L_ORDERKEY, N_NATIONKEY],
      DELTA_AGG_Supplier(long)[][L_SUPPKEY, N_NATIONKEY] *
      AGG_PartSupp(long)[][L_PARTKEY, L_SUPPKEY] *
      AGG_Part(long)[][L_PARTKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY]
    );

  QUERY9(float)[][N_NAME, YEAR] +=
    AggSum([N_NAME, YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] *
      AGG_Orders(long)[][L_ORDERKEY, YEAR] * 
      V_NATION(long)[][N_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY];

  AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY] +=
    DELTA_AGG_Supplier(long)[][S_SUPPKEY, S_NATIONKEY];
}

ON BATCH UPDATE OF PART {

  DELTA_AGG_Part(long)[][P_PARTKEY] :=
    AggSum([P_PARTKEY],
      (DELTA PART)(P_PARTKEY, P_NAME, P_MFGR, P_BRAND, P_TYPE, P_SIZE, P_CONTAINER, P_RETAILPRICE, P_COMMENT) *
      {0 != [regexp_match: long]('^.*green.*$', P_NAME)}
    );

  DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] :=
    AggSum([L_ORDERKEY, N_NATIONKEY],
      DELTA_AGG_Part(long)[][L_PARTKEY] *
      AGG_Lineitem(float)[][L_ORDERKEY, L_PARTKEY, L_SUPPKEY] *
      AGG_PartSupp(long)[][L_PARTKEY, L_SUPPKEY] *
      AGG_Supplier(long)[][L_SUPPKEY, N_NATIONKEY]
    );

  QUERY9(float)[][N_NAME, YEAR] +=
    AggSum([N_NAME, YEAR],
      DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] *
      AGG_Orders(long)[][L_ORDERKEY, YEAR] * 
      V_NATION(long)[][N_NATIONKEY, N_NAME]
    );

  AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY] +=
    DELTA_AGG_LineitemSupplierPart(float)[][L_ORDERKEY, N_NATIONKEY];

  AGG_Part(long)[][P_PARTKEY] +=
    DELTA_AGG_Part(long)[][P_PARTKEY];
}


ON SYSTEM READY {

  V_NATION(long)[][N_NATIONKEY, N_NAME] :=
    AggSum([N_NATIONKEY, N_NAME],
      NATION(N_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT)
    );
}