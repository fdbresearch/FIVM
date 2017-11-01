-- Recomputing TPCH5
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
DECLARE MAP V_NATION_REGION(long)[][N_NATIONKEY: long, N_NAME: string] := 0;
DECLARE MAP V_NATION_REGION_IP(long)[][N_NATIONKEY: long] := 0;
DECLARE MAP QUERY5(float)[][C_NAME: string] := 0;


-------------------- QUERIES --------------------
DECLARE QUERY QUERY5 := QUERY5(float)[][C_NAME];

------------------- TRIGGERS --------------------
ON SYSTEM READY {

  V_NATION_REGION(long)[][N_NATIONKEY, N_NAME] :=
    AggSum([N_NATIONKEY, N_NAME],
      (R_NAME ^= 'ASIA') * 
      NATION(N_NATIONKEY, N_NAME, N_REGIONKEY, N_COMMENT) * 
      REGION(N_REGIONKEY, R_NAME, R_COMMENT)
    );

  V_NATION_REGION_IP(long)[][N_NATIONKEY] :=
    EXISTS(AggSum([N_NATIONKEY], V_NATION_REGION(long)[][N_NATIONKEY, N_NAME]));

  QUERY5(float)[][N_NAME] := 
    AggSum([N_NAME],
      AggSum([C_NATIONKEY],         
        AggSum([O_ORDERKEY, C_NATIONKEY],
          AggSum([C_CUSTKEY, C_NATIONKEY],
            V_NATION_REGION_IP(long)[][C_NATIONKEY] *
            CUSTOMER(C_CUSTKEY, C_NAME, C_ADDRESS, C_NATIONKEY, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT)
          ) *
          ORDERS(O_ORDERKEY, C_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *
          ({O_ORDERDATE >= [date: date]('1994-1-1')} * {[date: date]('1995-1-1') > O_ORDERDATE})
        ) *
        AggSum([O_ORDERKEY, C_NATIONKEY],
          LINEITEM(O_ORDERKEY, L_PARTKEY, S_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *          
          AggSum([S_SUPPKEY, C_NATIONKEY],
            V_NATION_REGION_IP(long)[][C_NATIONKEY] *
            SUPPLIER(S_SUPPKEY, S_NAME, S_ADDRESS, C_NATIONKEY, S_PHONE, S_ACCTBAL, S_COMMENT)
          ) *
          (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
        )
      ) *
      V_NATION_REGION(long)[][C_NATIONKEY, N_NAME]      
    );
}