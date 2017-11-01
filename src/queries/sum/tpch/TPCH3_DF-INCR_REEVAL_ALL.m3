-- Recomputing TPCH3
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
DECLARE MAP BASE_LINEITEM(long)[][ORDERS_ORDERKEY: long, LINEITEM_PARTKEY: long, LINEITEM_SUPPKEY: long, LINEITEM_LINENUMBER: long, LINEITEM_QUANTITY: double, LINEITEM_EXTENDEDPRICE: double, LINEITEM_DISCOUNT: double, LINEITEM_TAX: double, LINEITEM_RETURNFLAG: string, LINEITEM_LINESTATUS: string, LINEITEM_SHIPDATE: date, LINEITEM_COMMITDATE: date, LINEITEM_RECEIPTDATE: date, LINEITEM_SHIPINSTRUCT: string, LINEITEM_SHIPMODE: string, LINEITEM_COMMENT: string] := 0;
DECLARE MAP BASE_ORDERS(long)[][ORDERS_ORDERKEY: long, CUSTOMER_CUSTKEY: long, ORDERS_ORDERSTATUS: string, ORDERS_TOTALPRICE: double, ORDERS_ORDERDATE: date, ORDERS_ORDERPRIORITY: string, ORDERS_CLERK: string, ORDERS_SHIPPRIORITY: long, ORDERS_COMMENT: string] := 0;
DECLARE MAP BASE_CUSTOMER(long)[][CUSTOMER_CUSTKEY: long, CUSTOMER_NAME: string, CUSTOMER_ADDRESS: string, CUSTOMER_NATIONKEY: long, CUSTOMER_PHONE: string, CUSTOMER_ACCTBAL: double, CUSTOMER_MKTSEGMENT: string, CUSTOMER_COMMENT: string] := 0;

DECLARE MAP QUERY3(float)[][orderkey:int, orderdate:date, shippriority:int] := 0;

-------------------- QUERIES --------------------
DECLARE QUERY QUERY3 := QUERY3(float)[][orderkey, orderdate, shippriority];

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {

  BASE_LINEITEM(long)[][lineitem_orderkey, lineitem_partkey, lineitem_suppkey, lineitem_linenumber, lineitem_quantity, lineitem_extendedprice, lineitem_discount, lineitem_tax, lineitem_returnflag, lineitem_linestatus, lineitem_shipdate, lineitem_commitdate, lineitem_receiptdate, lineitem_shipinstruct, lineitem_shipmode, lineitem_comment] +=  
    (DELTA LINEITEM)(lineitem_orderkey, lineitem_partkey, lineitem_suppkey, lineitem_linenumber, lineitem_quantity, lineitem_extendedprice, lineitem_discount, lineitem_tax, lineitem_returnflag, lineitem_linestatus, lineitem_shipdate, lineitem_commitdate, lineitem_receiptdate, lineitem_shipinstruct, lineitem_shipmode, lineitem_comment);

  QUERY3(float)[][orderkey, orderdate, shippriority] := 
    AggSum([orderkey, orderdate, shippriority],
      AggSum([custkey], 
        (mktsegment ^= 'BUILDING') * BASE_CUSTOMER(custkey, name, address, nationkey, phone, acctbal, mktsegment, ccomment)
      ) *
      AggSum([orderkey, orderdate, shippriority, custkey], 
        BASE_ORDERS(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, ocomment) * 
        {[date: date]('1995-3-15') > orderdate}
      )
    ) *
    AggSum([orderkey], 
      BASE_LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, lcomment) * 
      ({shipdate > [date: date]('1995-3-15')} * (extendedprice * (1 + (-1 * discount))))
    );
}

ON BATCH UPDATE OF ORDERS {
  
  BASE_ORDERS(long)[][orders_orderkey, orders_custkey, orders_orderstatus, orders_totalprice, orders_orderdate, orders_orderpriority, orders_clerk, orders_shippriority, orders_comment] +=
    (DELTA ORDERS)(orders_orderkey, orders_custkey, orders_orderstatus, orders_totalprice, orders_orderdate, orders_orderpriority, orders_clerk, orders_shippriority, orders_comment);

  QUERY3(float)[][orderkey, orderdate, shippriority] := 
    AggSum([orderkey, orderdate, shippriority],
      AggSum([custkey], 
        (mktsegment ^= 'BUILDING') * BASE_CUSTOMER(custkey, name, address, nationkey, phone, acctbal, mktsegment, ccomment)
      ) *
      AggSum([orderkey, orderdate, shippriority, custkey], 
        BASE_ORDERS(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, ocomment) * 
        {[date: date]('1995-3-15') > orderdate}
      )
    ) *
    AggSum([orderkey], 
      BASE_LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, lcomment) * 
      ({shipdate > [date: date]('1995-3-15')} * (extendedprice * (1 + (-1 * discount))))
    );
}

ON BATCH UPDATE OF CUSTOMER {
  
  BASE_CUSTOMER(long)[][customer_custkey, customer_name, customer_address, customer_nationkey, customer_phone, customer_acctbal, customer_mktsegment, customer_comment] +=
    (DELTA CUSTOMER)(customer_custkey, customer_name, customer_address, customer_nationkey, customer_phone, customer_acctbal, customer_mktsegment, customer_comment);

  QUERY3(float)[][orderkey, orderdate, shippriority] := 
    AggSum([orderkey, orderdate, shippriority],
      AggSum([custkey], 
        (mktsegment ^= 'BUILDING') * BASE_CUSTOMER(custkey, name, address, nationkey, phone, acctbal, mktsegment, ccomment)
      ) *
      AggSum([orderkey, orderdate, shippriority, custkey], 
        BASE_ORDERS(orderkey, custkey, orderstatus, totalprice, orderdate, orderpriority, clerk, shippriority, ocomment) * 
        {[date: date]('1995-3-15') > orderdate}
      )
    ) *
    AggSum([orderkey], 
      BASE_LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, lcomment) * 
      ({shipdate > [date: date]('1995-3-15')} * (extendedprice * (1 + (-1 * discount))))
    );    
}

ON SYSTEM READY {
}