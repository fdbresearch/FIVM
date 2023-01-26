#ifndef APPLICATION_FAVORITA_BASE_HPP
#define APPLICATION_FAVORITA_BASE_HPP

#include "../application.hpp"

const string dataPath = "data/favorita";

void Application::init_relations() {
    clear_relations();

    #if defined(RELATION_SALES_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SALES_entry>(
                "Sales", dataPath + "/Sales.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](SALES_entry& t) {
                        data.on_insert_SALES(t);
                    };
                }
        )));
    #elif defined(RELATION_SALES_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_SALES_entry>::iterator CIteratorSales;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_SALES_entry>(
                "Sales", dataPath + "/Sales.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorSales& begin, CIteratorSales& end) {
                        data.on_batch_update_SALES(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_SALES_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SALES_entry>(
                "Sales", dataPath + "/Sales.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](SALES_entry& t) {
                        data.on_insert_SALES(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_OIL_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<OIL_entry>(
                "Oil", dataPath + "/Oil.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](OIL_entry& t) {
                        data.on_insert_OIL(t);
                    };
                }
        )));
    #elif defined(RELATION_OIL_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_OIL_entry>::iterator CIteratorOil;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_OIL_entry>(
                "Oil", dataPath + "/Oil.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorOil& begin, CIteratorOil& end) {
                        data.on_batch_update_OIL(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_OIL_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<OIL_entry>(
                "Oil", dataPath + "/Oil.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](OIL_entry& t) {
                        data.on_insert_OIL(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_HOLIDAY_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<HOLIDAY_entry>(
                "Holiday", dataPath + "/Holiday.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](HOLIDAY_entry& t) {
                        data.on_insert_HOLIDAY(t);
                    };
                }
        )));
    #elif defined(RELATION_HOLIDAY_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_HOLIDAY_entry>::iterator CIteratorHoliday;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_HOLIDAY_entry>(
                "Holiday", dataPath + "/Holiday.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorHoliday& begin, CIteratorHoliday& end) {
                        data.on_batch_update_HOLIDAY(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_HOLIDAY_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<HOLIDAY_entry>(
                "Holiday", dataPath + "/Holiday.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](HOLIDAY_entry& t) {
                        data.on_insert_HOLIDAY(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_TRANSACTIONS_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TRANSACTIONS_entry>(
                "Transactions", dataPath + "/Transactions.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](TRANSACTIONS_entry& t) {
                        data.on_insert_TRANSACTIONS(t);
                    };
                }
        )));
    #elif defined(RELATION_TRANSACTIONS_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_TRANSACTIONS_entry>::iterator CIteratorTransactions;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_TRANSACTIONS_entry>(
                "Transactions", dataPath + "/Transactions.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorTransactions& begin, CIteratorTransactions& end) {
                        data.on_batch_update_TRANSACTIONS(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_TRANSACTIONS_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TRANSACTIONS_entry>(
                "Transactions", dataPath + "/Transactions.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](TRANSACTIONS_entry& t) {
                        data.on_insert_TRANSACTIONS(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_STORES_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<STORES_entry>(
                "Stores", dataPath + "/Stores.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](STORES_entry& t) {
                        data.on_insert_STORES(t);
                    };
                }
        )));
    #elif defined(RELATION_STORES_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_STORES_entry>::iterator CIteratorStores;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_STORES_entry>(
                "Stores", dataPath + "/Stores.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorStores& begin, CIteratorStores& end) {
                        data.on_batch_update_STORES(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_STORES_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<STORES_entry>(
                "Stores", dataPath + "/Stores.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](STORES_entry& t) {
                        data.on_insert_STORES(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_ITEMS_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ITEMS_entry>(
                "Items", dataPath + "/Items.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](ITEMS_entry& t) {
                        data.on_insert_ITEMS(t);
                    };
                }
        )));
    #elif defined(RELATION_ITEMS_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_ITEMS_entry>::iterator CIteratorItems;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_ITEMS_entry>(
                "Items", dataPath + "/Items.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorItems& begin, CIteratorItems& end) {
                        data.on_batch_update_ITEMS(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_ITEMS_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ITEMS_entry>(
                "Items", dataPath + "/Items.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](ITEMS_entry& t) {
                        data.on_insert_ITEMS(t);
                    };
                }
        )));
    #endif
}

#endif /* APPLICATION_FAVORITA_BASE_HPP */