---------------- TYPE DEFINITIONS ---------------


-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (orderkey int, l_partkey int, l_suppkey int, l_linenumber int, l_quantity double, l_extendedprice double, l_discount double, l_tax double, l_returnflag char, l_linestatus char, l_shipdate date, l_commitdate date, l_receiptdate date, l_shipinstruct string, l_shipmode string, l_comment string)
  FROM FILE '../../experiments/data/tpch/standard/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ORDERS (orderkey int, custkey int, o_orderstatus char, o_totalprice double, o_orderdate date, o_orderpriority string, o_clerk string, o_shippriority int, o_comment string)
  FROM FILE '../../experiments/data/tpch/standard/orders.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM CUSTOMER (custkey int, c_name string, c_address string, c_nationkey int, c_phone string, c_acctbal double, c_mktsegment string, c_comment string)
  FROM FILE '../../experiments/data/tpch/standard/customer.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_orderkey_LOC1(double)[][orderkey: int, o_orderdate: date, o_shippriority: int] :=
  (V_l_partkey_L1(double)[][orderkey]<Local> * V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>);

DECLARE MAP V_l_partkey_L1(double)[][orderkey: int] :=
  AggSum([orderkey],
    (((LINEITEM(orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipdate > [date: date]('1995-03-15')}) * (1 + (-1 * l_discount))) * l_extendedprice)
  );

DECLARE MAP V_o_orderdate_OC1(long)[][orderkey: int, o_orderdate: date, o_shippriority: int] :=
  AggSum([orderkey, o_orderdate, o_shippriority],
    (V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local> * V_c_name_C1(long)[][custkey]<Local>)
  );

DECLARE MAP V_o_orderstatus_O1(long)[][orderkey: int, custkey: int, o_orderdate: date, o_shippriority: int] :=
  AggSum([orderkey, custkey, o_orderdate, o_shippriority],
    (ORDERS(orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) * {[date: date]('1995-03-15') > o_orderdate})
  );

DECLARE MAP V_c_name_C1(long)[][custkey: int] :=
  AggSum([custkey],
    (CUSTOMER(custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment) * {c_mktsegment = 'BUILDING'})
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_orderkey_LOC1 := V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>;

------------------- TRIGGERS --------------------
ON + LINEITEM (orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((((1 * {l_shipdate > [date: date]('1995-03-15')}) * (1 + (-1 * l_discount))) * l_extendedprice) * V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>);
  V_l_partkey_L1(double)[][orderkey]<Local>  +=  (((1 * {l_shipdate > [date: date]('1995-03-15')}) * (1 + (-1 * l_discount))) * l_extendedprice);
}

ON - LINEITEM (orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((((-1 * {l_shipdate > [date: date]('1995-03-15')}) * (1 + (-1 * l_discount))) * l_extendedprice) * V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>);
  V_l_partkey_L1(double)[][orderkey]<Local>  +=  (((-1 * {l_shipdate > [date: date]('1995-03-15')}) * (1 + (-1 * l_discount))) * l_extendedprice);
}

ON + ORDERS (orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  (((1 * {[date: date]('1995-03-15') > o_orderdate}) * V_c_name_C1(long)[][custkey]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>);
  V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((1 * {[date: date]('1995-03-15') > o_orderdate}) * V_c_name_C1(long)[][custkey]<Local>);
  V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>  +=  (1 * {[date: date]('1995-03-15') > o_orderdate});
}

ON - ORDERS (orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  (((-1 * {[date: date]('1995-03-15') > o_orderdate}) * V_c_name_C1(long)[][custkey]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>);
  V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((-1 * {[date: date]('1995-03-15') > o_orderdate}) * V_c_name_C1(long)[][custkey]<Local>);
  V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>  +=  (-1 * {[date: date]('1995-03-15') > o_orderdate});
}

ON + CUSTOMER (custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  (((1 * {c_mktsegment = 'BUILDING'}) * V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>);
  V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((1 * {c_mktsegment = 'BUILDING'}) * V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>);
  V_c_name_C1(long)[][custkey]<Local>  +=  (1 * {c_mktsegment = 'BUILDING'});
}

ON - CUSTOMER (custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment) {
  V_orderkey_LOC1(double)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  (((-1 * {c_mktsegment = 'BUILDING'}) * V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>);
  V_o_orderdate_OC1(long)[][orderkey, o_orderdate, o_shippriority]<Local>  +=  ((-1 * {c_mktsegment = 'BUILDING'}) * V_o_orderstatus_O1(long)[][orderkey, custkey, o_orderdate, o_shippriority]<Local>);
  V_c_name_C1(long)[][custkey]<Local>  +=  (-1 * {c_mktsegment = 'BUILDING'});
}

ON SYSTEM READY {
  
}