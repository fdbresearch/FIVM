---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE TPCH12Payload FROM FILE 'ring/tpch/ring_query12.hpp';

-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (orderkey int, l_partkey int, l_suppkey int, l_linenumber int, l_quantity double, l_extendedprice double, l_discount double, l_tax double, l_returnflag char, l_linestatus char, l_shipdate date, l_commitdate date, l_receiptdate date, l_shipinstruct string, l_shipmode string, l_comment string)
  FROM FILE './datasets/tpch/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ORDERS (orderkey int, o_custkey int, o_orderstatus char, o_totalprice double, o_orderdate date, o_orderpriority string, o_clerk string, o_shippriority int, o_comment string)
  FROM FILE './datasets/tpch/orders.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_orderkey_LO1(TPCH12Payload)[][l_shipmode: string] :=
  AggSum([l_shipmode],
    (V_l_partkey_L1(long)[][orderkey, l_shipmode]<Local> * V_o_orderpriority_O1(TPCH12Payload)[][orderkey]<Local>)
  );

DECLARE MAP V_l_partkey_L1(long)[][orderkey: int, l_shipmode: string] :=
  AggSum([orderkey, l_shipmode],
    (((((LINEITEM(orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipmode IN ['MAIL', 'SHIP']}) * {l_receiptdate > l_commitdate}) * {l_commitdate > l_shipdate}) * {l_receiptdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_receiptdate})
  );

DECLARE MAP V_o_orderpriority_O1(TPCH12Payload)[][orderkey: int] :=
  AggSum([orderkey],
    (ORDERS(orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) * [lift: TPCH12Payload](o_orderpriority))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_orderkey_LO1 := V_orderkey_LO1(TPCH12Payload)[][l_shipmode]<Local>;

------------------- TRIGGERS --------------------
ON + LINEITEM (orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_orderkey_LO1(TPCH12Payload)[][l_shipmode]<Local>  +=  ((((((1 * {l_shipmode IN ['MAIL', 'SHIP']}) * {l_receiptdate > l_commitdate}) * {l_commitdate > l_shipdate}) * {l_receiptdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_receiptdate}) * V_o_orderpriority_O1(TPCH12Payload)[][orderkey]<Local>);
  V_l_partkey_L1(long)[][orderkey, l_shipmode]<Local>  +=  (((((1 * {l_shipmode IN ['MAIL', 'SHIP']}) * {l_receiptdate > l_commitdate}) * {l_commitdate > l_shipdate}) * {l_receiptdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_receiptdate});
}

ON - LINEITEM (orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_orderkey_LO1(TPCH12Payload)[][l_shipmode]<Local>  +=  ((((((-1 * {l_shipmode IN ['MAIL', 'SHIP']}) * {l_receiptdate > l_commitdate}) * {l_commitdate > l_shipdate}) * {l_receiptdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_receiptdate}) * V_o_orderpriority_O1(TPCH12Payload)[][orderkey]<Local>);
  V_l_partkey_L1(long)[][orderkey, l_shipmode]<Local>  +=  (((((-1 * {l_shipmode IN ['MAIL', 'SHIP']}) * {l_receiptdate > l_commitdate}) * {l_commitdate > l_shipdate}) * {l_receiptdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_receiptdate});
}

ON + ORDERS (orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) {
  V_orderkey_LO1(TPCH12Payload)[][l_shipmode]<Local>  +=  ((1 * [lift: TPCH12Payload](o_orderpriority)) * V_l_partkey_L1(long)[][orderkey, l_shipmode]<Local>);
  V_o_orderpriority_O1(TPCH12Payload)[][orderkey]<Local>  +=  (1 * [lift: TPCH12Payload](o_orderpriority));
}

ON - ORDERS (orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) {
  V_orderkey_LO1(TPCH12Payload)[][l_shipmode]<Local>  +=  ((-1 * [lift: TPCH12Payload](o_orderpriority)) * V_l_partkey_L1(long)[][orderkey, l_shipmode]<Local>);
  V_o_orderpriority_O1(TPCH12Payload)[][orderkey]<Local>  +=  (-1 * [lift: TPCH12Payload](o_orderpriority));
}

ON SYSTEM READY {
  
}