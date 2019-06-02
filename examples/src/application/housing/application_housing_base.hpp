#ifndef APPLICATION_HOUSING_BASE_HPP
#define APPLICATION_HOUSING_BASE_HPP

#include "../application.hpp"

const string dataPath = "data/housing-1-normalised";

void Application::init_relations() {
    clear_relations();

    #if defined(RELATION_HOUSE_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<HOUSE_entry>(
                "House", dataPath + "/House.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](HOUSE_entry& t) {
                        data.on_insert_HOUSE(t);
                    };
                }
        )));
    #elif defined(RELATION_HOUSE_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_HOUSE_entry>::iterator CIteratorHouse;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_HOUSE_entry>(
                "House", dataPath + "/House.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorHouse& begin, CIteratorHouse& end) {
                        data.on_batch_update_HOUSE(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_HOUSE_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<HOUSE_entry>(
                "House", dataPath + "/House.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](HOUSE_entry& t) {
                        data.on_insert_HOUSE(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_SHOP_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SHOP_entry>(
                "Shop", dataPath + "/Shop.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](SHOP_entry& t) {
                        data.on_insert_SHOP(t);
                    };
                }
        )));
    #elif defined(RELATION_SHOP_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_SHOP_entry>::iterator CIteratorShop;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_SHOP_entry>(
                "Shop", dataPath + "/Shop.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorShop& begin, CIteratorShop& end) { 
                        data.on_batch_update_SHOP(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_SHOP_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<SHOP_entry>(
                "Shop", dataPath + "/Shop.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](SHOP_entry& t) {
                        data.on_insert_SHOP(t); 
                    };
                }
        )));
    #endif

    #if defined(RELATION_INSTITUTION_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<INSTITUTION_entry>(
                "Institution", dataPath + "/Institution.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](INSTITUTION_entry& t) {
                        data.on_insert_INSTITUTION(t);
                    };
                }
        )));
    #elif defined(RELATION_INSTITUTION_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_INSTITUTION_entry>::iterator CIteratorInstitution;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_INSTITUTION_entry>(
                "Institution", dataPath + "/Institution.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorInstitution& begin, CIteratorInstitution& end) { 
                        data.on_batch_update_INSTITUTION(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_INSTITUTION_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<INSTITUTION_entry>(
                "Institution", dataPath + "/Institution.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](INSTITUTION_entry& t) {
                        data.on_insert_INSTITUTION(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_RESTAURANT_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<RESTAURANT_entry>(
                "Restaurant", dataPath + "/Restaurant.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](RESTAURANT_entry& t) {
                        data.on_insert_RESTAURANT(t);
                    };
                }
        )));
    #elif defined(RELATION_RESTAURANT_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_RESTAURANT_entry>::iterator CIteratorRestaurant;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_RESTAURANT_entry>(
                "Restaurant", dataPath + "/Restaurant.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorRestaurant& begin, CIteratorRestaurant& end) { 
                        data.on_batch_update_RESTAURANT(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_RESTAURANT_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<RESTAURANT_entry>(
                "Restaurant", dataPath + "/Restaurant.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](RESTAURANT_entry& t) {
                        data.on_insert_RESTAURANT(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_DEMOGRAPHICS_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<DEMOGRAPHICS_entry>(
                "Demographics", dataPath + "/Demographics.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](DEMOGRAPHICS_entry& t) {
                        data.on_insert_DEMOGRAPHICS(t);
                    };
                }
        )));
    #elif defined(RELATION_DEMOGRAPHICS_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_DEMOGRAPHICS_entry>::iterator CIteratorDemographics;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_DEMOGRAPHICS_entry>(
                "Demographics", dataPath + "/Demographics.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorDemographics& begin, CIteratorDemographics& end) {
                        data.on_batch_update_DEMOGRAPHICS(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_DEMOGRAPHICS_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<DEMOGRAPHICS_entry>(
                "Demographics", dataPath + "/Demographics.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](DEMOGRAPHICS_entry& t) {
                        data.on_insert_DEMOGRAPHICS(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_TRANSPORT_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TRANSPORT_entry>(
                "Transport", dataPath + "/Transport.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](TRANSPORT_entry& t) {
                        data.on_insert_TRANSPORT(t);
                    };
                }
        )));
    #elif defined(RELATION_TRANSPORT_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_TRANSPORT_entry>::iterator CIteratorTransport;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_TRANSPORT_entry>(
                "Transport", dataPath + "/Transport.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorTransport& begin, CIteratorTransport& end) {
                        data.on_batch_update_TRANSPORT(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_TRANSPORT_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TRANSPORT_entry>(
                "Transport", dataPath + "/Transport.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](TRANSPORT_entry& t) {
                        data.on_insert_TRANSPORT(t);
                    };
                }
        )));
    #endif
}

#endif /* APPLICATION_HOUSING_BASE_HPP */