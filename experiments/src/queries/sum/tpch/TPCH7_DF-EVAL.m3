-- Recomputing TPCH7
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

--------------------- MAPS ----------------------
DECLARE MAP V_NATION(long)[][N_NATIONKEY1: long, N_NAME1: string, N_NATIONKEY2: long, N_NAME2: string] := 0;
DECLARE MAP V_NATION_IP1(long)[][N_NATIONKEY1: long] := 0;
DECLARE MAP V_NATION_IP2(long)[][N_NATIONKEY2: long] := 0;
DECLARE MAP QUERY7(float)[][N_NAME1: string, N_NAME2: string, YEAR: long] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY7 := QUERY7(float)[][N_NAME1, N_NAME2, YEAR];

------------------- TRIGGERS --------------------
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

  QUERY7(float)[][N_NAME1, N_NAME2, YEAR] := 
    AggSum([N_NAME1, N_NAME2, YEAR],

      AggSum([N_NATIONKEY1, N_NATIONKEY2, YEAR],

        AggSum([O_ORDERKEY, N_NATIONKEY1, YEAR],

          AggSum([S_SUPPKEY, N_NATIONKEY1],
            V_NATION_IP1(long)[][N_NATIONKEY1] *
            SUPPLIER(S_SUPPKEY, S_NAME, S_ADDRESS, N_NATIONKEY1, S_PHONE, S_ACCTBAL, S_COMMENT)
          ) *

          LINEITEM(O_ORDERKEY, L_PARTKEY, S_SUPPKEY, L_LINENUMBER, L_QUANTITY, L_EXTENDEDPRICE, L_DISCOUNT, L_TAX, L_RETURNFLAG, L_LINESTATUS, L_SHIPDATE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPINSTRUCT, L_SHIPMODE, L_COMMENT) *
          ({[date: long]('1996-12-31') >= L_SHIPDATE} * {L_SHIPDATE >= [date: long]('1995-1-1')}) * 

          (YEAR ^= [date_part: long]('year', L_SHIPDATE)) *
          (L_EXTENDEDPRICE * (1 + (-1 * L_DISCOUNT)))
        ) *

        AggSum([O_ORDERKEY, N_NATIONKEY2],
          ORDERS(O_ORDERKEY, C_CUSTKEY, O_ORDERSTATUS, O_TOTALPRICE, O_ORDERDATE, O_ORDERPRIORITY, O_CLERK, O_SHIPPRIORITY, O_COMMENT) *

          AggSum([C_CUSTKEY, N_NATIONKEY2],
            V_NATION_IP2(long)[][N_NATIONKEY2] *
            CUSTOMER(C_CUSTKEY, C_NAME, C_ADDRESS, N_NATIONKEY2, C_PHONE, C_ACCTBAL, C_MKTSEGMENT, C_COMMENT)
          )
        )
      ) *
      V_NATION(long)[][N_NATIONKEY1, N_NAME1, N_NATIONKEY2, N_NAME2]      
    );
}