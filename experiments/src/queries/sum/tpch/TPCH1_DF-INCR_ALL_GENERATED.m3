---------------- TYPE DEFINITIONS ---------------
CREATE  TYPE TPCH1Payload FROM FILE 'ring/tpch/ring_query01.hpp';

-------------------- SOURCES --------------------
CREATE STREAM LINEITEM (orderkey int, partkey int, suppkey int, linenumber int, quantity double, extendedprice double, discount double, tax double, returnflag char, linestatus char, shipdate date, commitdate date, receiptdate date, shipinstruct string, shipmode string, comment string)
  FROM FILE './datasets/tpch/lineitem.csv' LINE DELIMITED CSV (delimiter := '|');

--------------------- MAPS ----------------------
DECLARE MAP V_returnflag_L1(TPCH1Payload)[][returnflag: char, linestatus: char] :=
  AggSum([returnflag, linestatus],
    ((LINEITEM(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, comment) * {[date: date]('1997-09-01') >= shipdate}) * [lift: TPCH1Payload](quantity, extendedprice, (extendedprice * (1 + (-1 * discount))), ((extendedprice * (1 + (-1 * discount))) * (1 + tax)), discount))
  );

-------------------- QUERIES --------------------
DECLARE QUERY V_returnflag_L1 := V_returnflag_L1(TPCH1Payload)[][returnflag, linestatus]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF LINEITEM {
  V_returnflag_L1(TPCH1Payload)[][returnflag, linestatus]<Local>  +=  AggSum([returnflag, linestatus],
    (((DELTA LINEITEM)(orderkey, partkey, suppkey, linenumber, quantity, extendedprice, discount, tax, returnflag, linestatus, shipdate, commitdate, receiptdate, shipinstruct, shipmode, comment) * {[date: date]('1997-09-01') >= shipdate}) * [lift: TPCH1Payload](quantity, extendedprice, (extendedprice * (1 + (-1 * discount))), ((extendedprice * (1 + (-1 * discount))) * (1 + tax)), discount))
  );
}

ON SYSTEM READY {
  
}