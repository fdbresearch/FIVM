-- Recomputing TPCH8
--

-------------------- SOURCES --------------------
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

DECLARE MAP QUERY8(float)[][YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY8 := QUERY8(float)[][YEAR];

------------------- TRIGGERS --------------------
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

  QUERY8(float)[][YEAR] := 
    AggSum([YEAR],
    
      AggSum([O_ORDERKEY],

        AggSum([L_PARTKEY],
          (P_TYPE ^= 'ECONOMY ANODIZED STEEL') *
          PART(L_PARTKEY, P_NAME, P_MFGR, P_BRAND, P_TYPE, P_SIZE, P_CONTAINER, P_RETAILPRICE, P_COMMENT)
        ) *

        LINEITEM(O_ORDERKEY, L_PARTKEY, S_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *

        AggSum([S_SUPPKEY, N_NAME],          
          SUPPLIER(S_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT) *
          V_NATION2(long)[][S_NATIONKEY, N_NAME]
        ) *

        (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
      ) *

      AggSum([O_ORDERKEY, YEAR],

        ORDERS(O_ORDERKEY, C_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
        ({[date: long]('1996-12-31') >= O_ORDERDATE} * {O_ORDERDATE >= [date: long]('1995-1-1')}) *
        (YEAR ^= [date_part: long]('year', O_ORDERDATE)) *

        AggSum([C_CUSTKEY],
          V_NATION1(long)[][C_NATIONKEY] *
          CUSTOMER(C_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT)
        )
      )
    );
}