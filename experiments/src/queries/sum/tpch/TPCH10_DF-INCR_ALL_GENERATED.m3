---------------- TYPE DEFINITIONS ---------------


-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (orderkey int, l_partkey int, l_suppkey int, l_linenumber int, l_quantity double, l_extendedprice double, l_discount double, l_tax double, l_returnflag char, l_linestatus char, l_shipdate date, l_commitdate date, l_receiptdate date, l_shipinstruct string, l_shipmode string, l_comment string)
  FROM FILE './datasets/tpch/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM ORDERS (orderkey int, custkey int, o_orderstatus char, o_totalprice double, o_orderdate date, o_orderpriority string, o_clerk string, o_shippriority int, o_comment string)
  FROM FILE './datasets/tpch/orders.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM CUSTOMER (custkey int, c_name string, c_address string, nationkey int, c_phone string, c_acctbal double, c_mktsegment string, c_comment string)
  FROM FILE './datasets/tpch/customer.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE TABLE NATION (nationkey int, n_name string, n_regionkey int, n_comment string)
  FROM FILE './datasets/tpch/nation.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_orderkey_LOCN1(double)[][custkey: int, c_name: string, c_address: string, c_phone: string, c_acctbal: double, c_comment: string, n_name: string] :=
  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    (V_l_partkey_L1(double)[][orderkey]<Local> * V_custkey_OCN1(long)[][orderkey, custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>)
  );

DECLARE MAP V_l_partkey_L1(double)[][orderkey: int] :=
  AggSum([orderkey],
    (((LINEITEM(orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_returnflag = 'R'}) * (1 + (-1 * l_discount))) * l_extendedprice)
  );

DECLARE MAP V_custkey_OCN1(long)[][orderkey: int, custkey: int, c_name: string, c_address: string, c_phone: string, c_acctbal: double, c_comment: string, n_name: string] :=
  (V_o_orderstatus_O1(long)[][orderkey, custkey]<Local> * V_nationkey_CN1(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>);

DECLARE MAP V_o_orderstatus_O1(long)[][orderkey: int, custkey: int] :=
  AggSum([orderkey, custkey],
    ((ORDERS(orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) * {o_orderdate >= [date: date]('1993-10-01')}) * {[date: date]('1994-01-01') > o_orderdate})
  );

DECLARE MAP V_nationkey_CN1(long)[][custkey: int, c_name: string, c_address: string, c_phone: string, c_acctbal: double, c_comment: string, n_name: string] :=
  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    (AggSum([custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment],
      CUSTOMER(custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
    ) * V_n_name_N1(long)[][nationkey, n_name]<Local>)
  );

DECLARE MAP V_n_name_N1(long)[][nationkey: int, n_name: string] :=
  AggSum([nationkey, n_name],
    NATION(nationkey, n_name, n_regionkey, n_comment)
  );

DECLARE MAP TMP_SUM1(double)[][orderkey: int] :=
  AggSum([orderkey],
    ((((DELTA LINEITEM)(orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_returnflag = 'R'}) * (1 + (-1 * l_discount))) * l_extendedprice)
  );

DECLARE MAP TMP_SUM2(long)[][orderkey: int, custkey: int] :=
  AggSum([orderkey, custkey],
    (((DELTA ORDERS)(orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) * {o_orderdate >= [date: date]('1993-10-01')}) * {[date: date]('1994-01-01') > o_orderdate})
  );

DECLARE MAP TMP_SUM3(long)[][custkey: int, c_name: string, c_address: string, nationkey: int, c_phone: string, c_acctbal: double, c_comment: string] :=
  AggSum([custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment],
    (DELTA CUSTOMER)(custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
  );

DECLARE MAP TMP_SUM4(long)[][custkey: int, c_name: string, c_address: string, c_phone: string, c_acctbal: double, c_comment: string, n_name: string] :=
  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    (TMP_SUM3(long)[][custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment]<Local> * V_n_name_N1(long)[][nationkey, n_name]<Local>)
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_orderkey_LOCN1 := V_orderkey_LOCN1(double)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {
  TMP_SUM1(double)[][orderkey]<Local>  :=  AggSum([orderkey],
    ((((DELTA LINEITEM)(orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_returnflag = 'R'}) * (1 + (-1 * l_discount))) * l_extendedprice)
  );
  V_orderkey_LOCN1(double)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    (TMP_SUM1(double)[][orderkey]<Local> * V_custkey_OCN1(long)[][orderkey, custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>)
  );
  V_l_partkey_L1(double)[][orderkey]<Local>  +=  TMP_SUM1(double)[][orderkey]<Local>;
}

ON BATCH UPDATE OF ORDERS {
  TMP_SUM2(long)[][orderkey, custkey]<Local>  :=  AggSum([orderkey, custkey],
    (((DELTA ORDERS)(orderkey, custkey, o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment) * {o_orderdate >= [date: date]('1993-10-01')}) * {[date: date]('1994-01-01') > o_orderdate})
  );
  V_orderkey_LOCN1(double)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    ((TMP_SUM2(long)[][orderkey, custkey]<Local> * V_nationkey_CN1(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>)
  );
  V_custkey_OCN1(long)[][orderkey, custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  (TMP_SUM2(long)[][orderkey, custkey]<Local> * V_nationkey_CN1(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>);
  V_o_orderstatus_O1(long)[][orderkey, custkey]<Local>  +=  TMP_SUM2(long)[][orderkey, custkey]<Local>;
}

ON BATCH UPDATE OF CUSTOMER {
  TMP_SUM3(long)[][custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment]<Local>  :=  AggSum([custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment],
    (DELTA CUSTOMER)(custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
  );
  TMP_SUM4(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  :=  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    (TMP_SUM3(long)[][custkey, c_name, c_address, nationkey, c_phone, c_acctbal, c_comment]<Local> * V_n_name_N1(long)[][nationkey, n_name]<Local>)
  );
  V_orderkey_LOCN1(double)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  AggSum([custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name],
    ((TMP_SUM4(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local> * V_o_orderstatus_O1(long)[][orderkey, custkey]<Local>) * V_l_partkey_L1(double)[][orderkey]<Local>)
  );
  V_custkey_OCN1(long)[][orderkey, custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  (TMP_SUM4(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local> * V_o_orderstatus_O1(long)[][orderkey, custkey]<Local>);
  V_nationkey_CN1(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>  +=  TMP_SUM4(long)[][custkey, c_name, c_address, c_phone, c_acctbal, c_comment, n_name]<Local>;
}

ON SYSTEM READY {
  V_n_name_N1(long)[][nationkey, n_name]<Local>  :=  AggSum([nationkey, n_name],
    NATION(nationkey, n_name, n_regionkey, n_comment)
  );
}