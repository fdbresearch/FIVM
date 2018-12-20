---------------- TYPE DEFINITIONS ---------------


-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (l_orderkey int, l_partkey int, l_suppkey int, l_linenumber int, l_quantity double, l_extendedprice double, l_discount double, l_tax double, l_returnflag char, l_linestatus char, l_shipdate date, l_commitdate date, l_receiptdate date, l_shipinstruct string, l_shipmode string, l_comment string)
  FROM FILE './datasets/tpch/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_l_orderkey_L1(double)[][] :=
  AggSum([],
    (((((((LINEITEM(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_shipdate}) * {l_discount >= 0.05}) * {0.07 >= l_discount}) * {24 > l_quantity}) * l_discount) * l_extendedprice)
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_l_orderkey_L1 := V_l_orderkey_L1(double)[][]<Local>;

------------------- TRIGGERS --------------------
ON + LINEITEM (l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_l_orderkey_L1(double)[][]<Local>  +=  (((((((1 * {l_shipdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_shipdate}) * {l_discount >= 0.05}) * {0.07 >= l_discount}) * {24 > l_quantity}) * l_discount) * l_extendedprice);
}

ON - LINEITEM (l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) {
  V_l_orderkey_L1(double)[][]<Local>  +=  (((((((-1 * {l_shipdate >= [date: date]('1994-01-01')}) * {[date: date]('1995-01-01') > l_shipdate}) * {l_discount >= 0.05}) * {0.07 >= l_discount}) * {24 > l_quantity}) * l_discount) * l_extendedprice);
}

ON SYSTEM READY {
  
}