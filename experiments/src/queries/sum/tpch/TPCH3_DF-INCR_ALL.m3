-- IVM of TPCH3
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


--------------------- MAPS ----------------------
DECLARE MAP AGG_Lineitem(float)[][orderkey:int] := 0;
DECLARE MAP AGG_Orders(int)[][orderkey:int, orderdate:date, shippriority:int, custkey:int] := 0;
DECLARE MAP AGG_Customer(int)[][custkey:int] := 0;
DECLARE MAP AGG_OrdersCustomer(int)[][orderkey:int, orderdate:date, shippriority:int] := 0;

DECLARE MAP DELTA_AGG_Lineitem(float)[][orderkey:int] := 0;
DECLARE MAP DELTA_AGG_Orders(int)[][orderkey:int, orderdate:date, shippriority:int, custkey:int] := 0;
DECLARE MAP DELTA_AGG_Customer(int)[][custkey:int] := 0;
DECLARE MAP DELTA_AGG_OrdersCustomer(int)[][orderkey:int, orderdate:date, shippriority:int] := 0;

DECLARE MAP QUERY3(float)[][orderkey:int, orderdate:date, shippriority:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY3 := QUERY3(float)[][orderkey, orderdate, shippriority];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {

  DELTA_AGG_Lineitem(float)[][lineitem_orderkey] := 
    AggSum([lineitem_orderkey],
      (DELTA LINEITEM)(lineitem_orderkey, lineitem_partkey, lineitem_suppkey, lineitem_linenumber, lineitem_quantity, lineitem_extendedprice, lineitem_discount, lineitem_tax, lineitem_returnflag, lineitem_linestatus, lineitem_shipdate, lineitem_commitdate, lineitem_receiptdate, lineitem_shipinstruct, lineitem_shipmode, lineitem_comment) * 
      ({lineitem_shipdate > [date: date]('1995-3-15')} * (lineitem_extendedprice * (1 + (-1 * lineitem_discount))))
    );
  
  QUERY3(float)[][lineitem_orderkey, orderdate, shippriority] +=
    DELTA_AGG_Lineitem(float)[][lineitem_orderkey] *
    AGG_OrdersCustomer(int)[][lineitem_orderkey, orderdate, shippriority];

  AGG_Lineitem(float)[][lineitem_orderkey] += DELTA_AGG_Lineitem(float)[][lineitem_orderkey];
}

ON BATCH UPDATE OF ORDERS {
  
  DELTA_AGG_Orders(int)[][orders_orderkey, orders_orderdate, orders_shippriority, orders_custkey] := 
    AggSum([orders_orderkey, orders_orderdate, orders_shippriority, orders_custkey],
      (DELTA ORDERS)(orders_orderkey, orders_custkey, orders_orderstatus, orders_totalprice, orders_orderdate, orders_orderpriority, orders_clerk, orders_shippriority, orders_comment) *
      {[date: date]('1995-3-15') > orders_orderdate}
    );

  DELTA_AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority] := 
    AggSum([orders_orderkey, orders_orderdate, orders_shippriority],
      DELTA_AGG_Orders(int)[][orders_orderkey, orders_orderdate, orders_shippriority, orders_custkey] *
      AGG_Customer(int)[][orders_custkey]
    );

  QUERY3(float)[][lineitem_orderkey, orderdate, shippriority] +=
    DELTA_AGG_OrdersCustomer(int)[][lineitem_orderkey, orderdate, shippriority] *
    AGG_Lineitem(float)[][lineitem_orderkey];  

  AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority] +=
    DELTA_AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority];

  AGG_Orders(int)[][orders_orderkey, orders_orderdate, orders_shippriority, orders_custkey] +=
    DELTA_AGG_Orders(int)[][orders_orderkey, orders_orderdate, orders_shippriority, orders_custkey];

}

ON BATCH UPDATE OF CUSTOMER {

  DELTA_AGG_Customer(int)[][customer_custkey] := 
    AggSum([customer_custkey],
      (DELTA CUSTOMER)(customer_custkey, customer_name, customer_address, customer_nationkey, customer_phone, customer_acctbal, customer_mktsegment, customer_comment) *
      (customer_mktsegment ^= 'BUILDING')
    );

  DELTA_AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority] := 
    AggSum([orders_orderkey, orders_orderdate, orders_shippriority],
      DELTA_AGG_Customer(int)[][customer_custkey] *
      AGG_Orders(int)[][orders_orderkey, orders_orderdate, orders_shippriority, customer_custkey]
    );

  QUERY3(float)[][lineitem_orderkey, orderdate, shippriority] +=
    DELTA_AGG_OrdersCustomer(int)[][lineitem_orderkey, orderdate, shippriority] *
    AGG_Lineitem(float)[][lineitem_orderkey];  

  AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority] +=
    DELTA_AGG_OrdersCustomer(int)[][orders_orderkey, orders_orderdate, orders_shippriority];

  AGG_Customer(int)[][customer_custkey] +=
    DELTA_AGG_Customer(int)[][customer_custkey];

}

ON SYSTEM READY {
}