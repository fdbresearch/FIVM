-- Recomputing TPCH3
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


--------------------- MAPS ----------------------
DECLARE MAP AGG_Lineitem(float)[][orderkey:int] := 0;
DECLARE MAP AGG_Orders(int)[][orderkey:int, orderdate:date, shippriority:int, custkey:int] := 0;
DECLARE MAP AGG_Customer(int)[][custkey:int] := 0;

DECLARE MAP QUERY3(float)[][orderkey:int, orderdate:date, shippriority:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY3 := QUERY3(float)[][orderkey, orderdate, shippriority];

------------------- TRIGGERS --------------------
ON SYSTEM READY {
--    AGG_Customer(int)[][custkey] := 
--      AggSum([custkey], 
--        (mktsegment ^= 'BUILDING') * CUSTOMER(custkey, name, address, nationkey, phone, acctbal, mktsegment, comment)
--      );
--
--    AGG_Orders(int)[][orderkey, orderdate, shippriority, custkey] := 
--      AggSum([orderkey, orderdate, shippriority, custkey], 
--        ORDERS(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, comment) * 
--        {[date: date]('1995-3-15') > orderdate}
--      );
--
--    AGG_Lineitem(float)[][orderkey] := 
--      AggSum([orderkey], 
--        LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, comment) * 
--        ({shipdate > [date: date]('1995-3-15')} * (extendedprice * (1 + (-1 * discount))))
--      );
--
--    QUERY3(float)[][orderkey, orderdate, shippriority] := 
--      AggSum([orderkey, orderdate, shippriority], 
--        AGG_Customer(int)[][custkey] *
--        AGG_Orders(int)[][orderkey, orderdate, shippriority, custkey]
--      ) *
--      AGG_Lineitem(float)[][orderkey];

    QUERY3(float)[][orderkey, orderdate, shippriority] := 
      AggSum([orderkey, orderdate, shippriority],
        AggSum([custkey], 
          (mktsegment ^= 'BUILDING') * CUSTOMER(custkey, name, address, nationkey, phone, acctbal, mktsegment, ccomment)
        ) *
        AggSum([orderkey, orderdate, shippriority, custkey], 
          ORDERS(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, ocomment) * 
          {[date: date]('1995-3-15') > orderdate}
        )
      ) *
      AggSum([orderkey], 
        LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, lcomment) * 
        ({shipdate > [date: date]('1995-3-15')} * (extendedprice * (1 + (-1 * discount))))
      );

}