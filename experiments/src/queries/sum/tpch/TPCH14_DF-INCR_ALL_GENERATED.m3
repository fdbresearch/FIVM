---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE TPCH14Payload FROM FILE 'ring/tpch/ring_query14.hpp';

-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (l_orderkey int, partkey int, l_suppkey int, l_linenumber int, l_quantity double, l_extendedprice double, l_discount double, l_tax double, l_returnflag char, l_linestatus char, l_shipdate date, l_commitdate date, l_receiptdate date, l_shipinstruct string, l_shipmode string, l_comment string)
  FROM FILE './datasets/tpch/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

CREATE STREAM PART (partkey int, p_name string, p_mfgr string, p_brand string, p_type string, p_size int, p_container string, p_retailprice double, p_comment string)
  FROM FILE './datasets/tpch/part.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_partkey_LP1(TPCH14Payload)[][] :=
  AggSum([],
    (V_l_orderkey_L1(TPCH14Payload)[][partkey]<Local> * V_p_name_P1(TPCH14Payload)[][partkey]<Local>)
  );

DECLARE MAP V_l_orderkey_L1(TPCH14Payload)[][partkey: int] :=
  AggSum([partkey],
    (((LINEITEM(l_orderkey, partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipdate >= [date: date]('1995-09-01')}) * {[date: date]('1995-10-01') > l_shipdate}) * [liftlineitem: TPCH14Payload]((l_extendedprice * (1 + (-1 * l_discount)))))
  );

DECLARE MAP V_p_name_P1(TPCH14Payload)[][partkey: int] :=
  AggSum([partkey],
    (PART(partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment) * [liftpart: TPCH14Payload](p_type))
  );

DECLARE MAP TMP_SUM1(TPCH14Payload)[][partkey: int] :=
  AggSum([partkey],
    ((((DELTA LINEITEM)(l_orderkey, partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipdate >= [date: date]('1995-09-01')}) * {[date: date]('1995-10-01') > l_shipdate}) * [liftlineitem: TPCH14Payload]((l_extendedprice * (1 + (-1 * l_discount)))))
  );

DECLARE MAP TMP_SUM2(TPCH14Payload)[][partkey: int] :=
  AggSum([partkey],
    ((DELTA PART)(partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment) * [liftpart: TPCH14Payload](p_type))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_partkey_LP1 := V_partkey_LP1(TPCH14Payload)[][]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {
  TMP_SUM1(TPCH14Payload)[][partkey]<Local>  :=  AggSum([partkey],
    ((((DELTA LINEITEM)(l_orderkey, partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment) * {l_shipdate >= [date: date]('1995-09-01')}) * {[date: date]('1995-10-01') > l_shipdate}) * [liftlineitem: TPCH14Payload]((l_extendedprice * (1 + (-1 * l_discount)))))
  );
  V_partkey_LP1(TPCH14Payload)[][]<Local>  +=  AggSum([],
    (TMP_SUM1(TPCH14Payload)[][partkey]<Local> * V_p_name_P1(TPCH14Payload)[][partkey]<Local>)
  );
  V_l_orderkey_L1(TPCH14Payload)[][partkey]<Local>  +=  TMP_SUM1(TPCH14Payload)[][partkey]<Local>;
}

ON BATCH UPDATE OF PART {
  TMP_SUM2(TPCH14Payload)[][partkey]<Local>  :=  AggSum([partkey],
    ((DELTA PART)(partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice, p_comment) * [liftpart: TPCH14Payload](p_type))
  );
  V_partkey_LP1(TPCH14Payload)[][]<Local>  +=  AggSum([],
    (TMP_SUM2(TPCH14Payload)[][partkey]<Local> * V_l_orderkey_L1(TPCH14Payload)[][partkey]<Local>)
  );
  V_p_name_P1(TPCH14Payload)[][partkey]<Local>  +=  TMP_SUM2(TPCH14Payload)[][partkey]<Local>;
}

ON SYSTEM READY {
  
}