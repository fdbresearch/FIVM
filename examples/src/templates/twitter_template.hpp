#ifndef TWITTER_TEMPLATE_HPP
#define TWITTER_TEMPLATE_HPP

#include "template.hpp"

#define TEMPLATE_NAME "TWITTER"

const string dataPath = "data/twitter";
const string filename = "higgs-social_network.tbl";

void init_relations() {
    relations.clear();

    #if defined(RELATION_TWITTER1_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER1_entry>(
                "Twitter1", dataPath + "/Twitter1.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER1_entry& t) {
                        data.on_insert_TWITTER1(t);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER1_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_TWITTER1_entry>::iterator CIteratorTwitter1;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_TWITTER1_entry>(
                "Twitter1", dataPath + "/Twitter1.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorTwitter1& begin, CIteratorTwitter1& end) {
                        data.on_batch_update_TWITTER1(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER1_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER1_entry>(
                "Twitter1", dataPath + "/Twitter1.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER1_entry& t) {
                        data.on_insert_TWITTER1(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_TWITTER2_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER2_entry>(
                "Twitter2", dataPath + "/Twitter2.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER2_entry& t) {
                        data.on_insert_TWITTER2(t);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER2_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_TWITTER2_entry>::iterator CIteratorTwitter2;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_TWITTER2_entry>(
                "Twitter2", dataPath + "/Twitter2.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorTwitter2& begin, CIteratorTwitter2& end) {
                        data.on_batch_update_TWITTER2(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER2_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER2_entry>(
                "Twitter2", dataPath + "/Twitter2.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER2_entry& t) {
                        data.on_insert_TWITTER2(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_TWITTER3_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER3_entry>(
                "Twitter3", dataPath + "/Twitter3.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER3_entry& t) {
                        data.on_insert_TWITTER3(t);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER3_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_TWITTER3_entry>::iterator CIteratorTwitter3;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_TWITTER3_entry>(
                "Twitter3", dataPath + "/Twitter3.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorTwitter3& begin, CIteratorTwitter3& end) {
                        data.on_batch_update_TWITTER3(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_TWITTER3_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<TWITTER3_entry>(
                "Twitter3", dataPath + "/Twitter3.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](TWITTER3_entry& t) {
                        data.on_insert_TWITTER3(t);
                    };
                }
        )));
    #endif
}

#endif /* TWITTER_TEMPLATE_HPP */