-- Recomputing TPCH9
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

--------------------- MAPS ----------------------
DECLARE MAP V_NATION(long)[][N_NATIONKEY: long, N_NAME: string] := 0;

DECLARE MAP QUERY9(float)[][N_NAME: string, YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY9 := QUERY9(float)[][N_NAME, YEAR];

------------------- TRIGGERS --------------------
ON SYSTEM READY {

  V_NATION(long)[][N_NATIONKEY, N_NAME] :=
    AggSum([N_NATIONKEY, N_NAME],
      NATION(N_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT)
    );

  QUERY9(float)[][N_NAME, YEAR] := 
    AggSum([N_NAME, YEAR],
    
      AggSum([O_ORDERKEY, S_NATIONKEY],

        AggSum([P_PARTKEY],
          PART(P_PARTKEY, P_NAME, P_MFGR, P_BRAND, P_TYPE, P_SIZE, P_CONTAINER, P_RETAILPRICE, P_COMMENT) *
          {0 != [regexp_match: long]('^.*green.*$', P_NAME)}
        ) *

        AggSum([O_ORDERKEY, P_PARTKEY, L_SUPPKEY],
          LINEITEM(O_ORDERKEY, P_PARTKEY, L_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
          (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
        ) *

        AggSum([P_PARTKEY, L_SUPPKEY],
          PARTSUPP(P_PARTKEY, L_SUPPKEY, PS_AVAILQTY, PS_SUPPLYCOST, PS_COMMENT)
        ) *
        
        AggSum([L_SUPPKEY, S_NATIONKEY],          
          SUPPLIER(L_SUPPKEY, S_NAME, S_ADDRESS, S_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT)
        )

      ) *

      AggSum([O_ORDERKEY, YEAR],
        ORDERS(O_ORDERKEY, C_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
        (YEAR ^= [date_part: long]('year', O_ORDERDATE))
      ) *

      V_NATION(long)[][S_NATIONKEY, N_NAME]
    );
}