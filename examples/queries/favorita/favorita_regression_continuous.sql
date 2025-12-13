IMPORT DTREE FROM FILE 'favorita.txt';

CREATE TYPE RingCofactor
FROM FILE 'ring/ring_cofactor_degree1.hpp'
WITH PARAMETER SCHEMA (dynamic_min, static, dynamic_sum);

CREATE STREAM SALES(date int, store int, item int, unit_sales double, onpromotion int)
FROM FILE './datasets/favorita/Sales.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM OIL(date int, oilprize double)
FROM FILE './datasets/favorita/Oil.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM HOLIDAY(date int, holiday_type int, locale int, locale_id int, transferred int)
FROM FILE './datasets/favorita/Holiday.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM TRANSACTIONS(date int, store int, transactions int) 
FROM FILE './datasets/favorita/Transactions.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM STORES(store int, city int, state int, store_type int, cluster int) 
FROM FILE './datasets/favorita/Stores.tbl' LINE DELIMITED CSV(delimiter := ',');

CREATE STREAM ITEMS(item int, family int, itemclass int, perishable int) 
FROM FILE './datasets/favorita/Stores.tbl' LINE DELIMITED CSV(delimiter := ',');

SELECT SUM(
    [lift<0>: RingCofactor<0,double,2>](unit_sales, onpromotion) *
    [lift<2>: RingCofactor<2,double,3>](family, itemclass, perishable) *
    [lift<5>: RingCofactor<5,double,1>](transactions) *
    [lift<6>: RingCofactor<6,double,4>](city, state, store_type, cluster) *
    [lift<10>: RingCofactor<10,double,1>](oilprize) *
    [lift<11>: RingCofactor<11,double,4>](holiday_type, locale, locale_id, transferred)    
)
FROM Sales NATURAL JOIN Oil NATURAL JOIN Holiday NATURAL JOIN Transactions NATURAL JOIN Stores NATURAL JOIN Items;
