#ifndef TPCH_TEMPLATE_HPP
#define TPCH_TEMPLATE_HPP

#include "template.hpp"

#define TEMPLATE_NAME "TPCH"

const string dataPath = "data/tpch0.01";

void init_relations() {
    relations.clear();

    #if defined(RELATION_LINEITEM_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<LINEITEM_entry>(
                "Lineitem", dataPath + "/lineitem.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](LINEITEM_entry& t) {
                        data.on_insert_LINEITEM(t);
                    };
                }
        )));
    #elif defined(RELATION_LINEITEM_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_LINEITEM_entry>::iterator CIteratorLineitem;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_LINEITEM_entry>(
                "Lineitem", dataPath + "/lineitem.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorLineitem& begin, CIteratorLineitem& end) {
                        data.on_batch_update_LINEITEM(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_LINEITEM_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<LINEITEM_entry>(
                "Lineitem", dataPath + "/lineitem.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](LINEITEM_entry& t) {
                        data.on_insert_LINEITEM(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_ORDERS_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ORDERS_entry>(
                "Orders", dataPath + "/orders.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](ORDERS_entry& t) {
                        data.on_insert_ORDERS(t);
                    };
                }
        )));
    #elif defined(RELATION_ORDERS_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_ORDERS_entry>::iterator CIteratorOrders;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_ORDERS_entry>(
                "Orders", dataPath + "/orders.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorOrders& begin, CIteratorOrders& end) { 
                        data.on_batch_update_ORDERS(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_ORDERS_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ORDERS_entry>(
                "Orders", dataPath + "/orders.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](ORDERS_entry& t) {
                        data.on_insert_ORDERS(t); 
                    };
                }
        )));
    #endif

    #if defined(RELATION_CUSTOMER_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<CUSTOMER_entry>(
                "Customer", dataPath + "/customer.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](CUSTOMER_entry& t) {
                        data.on_insert_CUSTOMER(t);
                    };
                }
        )));
    #elif defined(RELATION_CUSTOMER_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_CUSTOMER_entry>::iterator CIteratorCustomer;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_CUSTOMER_entry>(
                "Customer", dataPath + "/customer.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorCustomer& begin, CIteratorCustomer& end) { 
                        data.on_batch_update_CUSTOMER(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_CUSTOMER_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<CUSTOMER_entry>(
                "Customer", dataPath + "/customer.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CUSTOMER_entry& t) {
                        data.on_insert_CUSTOMER(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_PART_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<PART_entry>(
                "Part", dataPath + "/part.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](PART_entry& t) {
                        data.on_insert_PART(t);
                    };
                }
        )));
    #elif defined(RELATION_PART_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_PART_entry>::iterator CIteratorPart;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_PART_entry>(
                "Part", dataPath + "/part.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorPart& begin, CIteratorPart& end) { 
                        data.on_batch_update_PART(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_PART_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<PART_entry>(
                "Part", dataPath + "/part.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](PART_entry& t) {
                        data.on_insert_PART(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_PARTSUPP_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<PARTSUPP_entry>(
                "Partsupp", dataPath + "/partsupp.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](PARTSUPP_entry& t) {
                        data.on_insert_PARTSUPP(t);
                    };
                }
        )));
    #elif defined(RELATION_PARTSUPP_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_PARTSUPP_entry>::iterator CIteratorPartsupp;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_PARTSUPP_entry>(
                "Partsupp", dataPath + "/partsupp.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorPartsupp& begin, CIteratorPartsupp& end) {
                        data.on_batch_update_PARTSUPP(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_PARTSUPP_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<PARTSUPP_entry>(
                "Partsupp", dataPath + "/partsupp.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](PARTSUPP_entry& t) {
                        data.on_insert_PARTSUPP(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_SUPPLIER_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SUPPLIER_entry>(
                "Supplier", dataPath + "/supplier.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](SUPPLIER_entry& t) {
                        data.on_insert_SUPPLIER(t);
                    };
                }
        )));
    #elif defined(RELATION_SUPPLIER_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_SUPPLIER_entry>::iterator CIteratorSupplier;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_SUPPLIER_entry>(
                "Supplier", dataPath + "/supplier.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorSupplier& begin, CIteratorSupplier& end) {
                        data.on_batch_update_SUPPLIER(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_SUPPLIER_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SUPPLIER_entry>(
                "Supplier", dataPath + "/supplier.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](SUPPLIER_entry& t) {
                        data.on_insert_SUPPLIER(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_NATION_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<NATION_entry>(
                "Nation", dataPath + "/nation.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](NATION_entry& t) {
                        data.on_insert_NATION(t);
                    };
                }
        )));
    #elif defined(RELATION_NATION_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_NATION_entry>::iterator CIteratorNation;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_NATION_entry>(
                "Nation", dataPath + "/nation.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorNation& begin, CIteratorNation& end) {
                        data.on_batch_update_NATION(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_NATION_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<NATION_entry>(
                "Nation", dataPath + "/nation.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](NATION_entry& t) {
                        data.on_insert_NATION(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_REGION_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<REGION_entry>(
                "Region", dataPath + "/region.csv", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](REGION_entry& t) {
                        data.on_insert_REGION(t);
                    };
                }
        )));
    #elif defined(RELATION_REGION_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_REGION_entry>::iterator CIteratorRegion;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_REGION_entry>(
                "Region", dataPath + "/region.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorRegion& begin, CIteratorRegion& end) {
                        data.on_batch_update_REGION(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_REGION_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<REGION_entry>(
                "Region", dataPath + "/region.csv", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](REGION_entry& t) {
                        data.on_insert_REGION(t);
                    };
                }
        )));
    #endif        
}

#endif /* TPCH_TEMPLATE_HPP */