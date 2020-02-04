#ifndef APPLICATION_RETAILER_BASE_HPP
#define APPLICATION_RETAILER_BASE_HPP

#include "../application.hpp"

const string dataPath = "data/retailer";

void Application::init_relations() {
    clear_relations();

    #if defined(RELATION_INVENTORY_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<INVENTORY_entry>(
                "Inventory", dataPath + "/Inventory.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](INVENTORY_entry& t) {
                        data.on_insert_INVENTORY(t);
                    };
                }
        )));
    #elif defined(RELATION_INVENTORY_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_INVENTORY_entry>::iterator CIteratorInventory;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_INVENTORY_entry>(
                "Inventory", dataPath + "/Inventory.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorInventory& begin, CIteratorInventory& end) {
                        data.on_batch_update_INVENTORY(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_INVENTORY_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<INVENTORY_entry>(
                "Inventory", dataPath + "/Inventory.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](INVENTORY_entry& t) {
                        data.on_insert_INVENTORY(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_LOCATION_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<LOCATION_entry>(
                "Location", dataPath + "/Location.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](LOCATION_entry& t) {
                        data.on_insert_LOCATION(t);
                    };
                }
        )));
    #elif defined(RELATION_LOCATION_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_LOCATION_entry>::iterator CIteratorLocation;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_LOCATION_entry>(
                "Location", dataPath + "/Location.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorLocation& begin, CIteratorLocation& end) {
                        data.on_batch_update_LOCATION(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_LOCATION_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<LOCATION_entry>(
                "Location", dataPath + "/Location.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](LOCATION_entry& t) {
                        data.on_insert_LOCATION(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_CENSUS_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<CENSUS_entry>(
                "Census", dataPath + "/Census.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](CENSUS_entry& t) {
                        data.on_insert_CENSUS(t);
                    };
                }
        )));
    #elif defined(RELATION_CENSUS_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_CENSUS_entry>::iterator CIteratorCensus;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_CENSUS_entry>(
                "Census", dataPath + "/Census.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorCensus& begin, CIteratorCensus& end) {
                        data.on_batch_update_CENSUS(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_CENSUS_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<CENSUS_entry>(
                "Census", dataPath + "/Census.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CENSUS_entry& t) {
                        data.on_insert_CENSUS(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_ITEM_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ITEM_entry>(
                "Item", dataPath + "/Item.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](ITEM_entry& t) {
                        data.on_insert_ITEM(t);
                    };
                }
        )));
    #elif defined(RELATION_ITEM_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_ITEM_entry>::iterator CIteratorItem;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_ITEM_entry>(
                "Item", dataPath + "/Item.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorItem& begin, CIteratorItem& end) {
                        data.on_batch_update_ITEM(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_ITEM_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<ITEM_entry>(
                "Item", dataPath + "/Item.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](ITEM_entry& t) {
                        data.on_insert_ITEM(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_WEATHER_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<WEATHER_entry>(
                "Weather", dataPath + "/Weather.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](WEATHER_entry& t) {
                        data.on_insert_WEATHER(t);
                    };
                }
        )));
    #elif defined(RELATION_WEATHER_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_WEATHER_entry>::iterator CIteratorWeather;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_WEATHER_entry>(
                "Weather", dataPath + "/Weather.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorWeather& begin, CIteratorWeather& end) {
                        data.on_batch_update_WEATHER(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_WEATHER_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<WEATHER_entry>(
                "Weather", dataPath + "/Weather.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](WEATHER_entry& t) {
                        data.on_insert_WEATHER(t);
                    };
                }
        )));
    #endif
}

#endif /* APPLICATION_RETAILER_BASE_HPP */