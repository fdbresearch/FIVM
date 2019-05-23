#ifndef APPLICATION_SIMPLE_BASE_HPP
#define APPLICATION_SIMPLE_BASE_HPP

#include "../application.hpp"

const string dataPath = "data/simple";

void Application::init_relations() {
    clear_relations();

    #if defined(RELATION_R_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<R_entry>(
                "R", dataPath + "/R.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](R_entry& t) {
                        data.on_insert_R(t);
                    };
                }
        )));
    #elif defined(RELATION_R_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_R_entry>::iterator CIteratorR;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_R_entry>(
                "R", dataPath + "/R.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorR& begin, CIteratorR& end) {
                        data.on_batch_update_R(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_R_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<R_entry>(
                "R", dataPath + "/R.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](R_entry& t) {
                        data.on_insert_R(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_S_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<S_entry>(
                "S", dataPath + "/S.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](S_entry& t) {
                        data.on_insert_S(t);
                    };
                }
        )));
    #elif defined(RELATION_S_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_S_entry>::iterator CIteratorS;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_S_entry>(
                "S", dataPath + "/S.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorS& begin, CIteratorS& end) {
                        data.on_batch_update_S(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_S_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<S_entry>(
                "S", dataPath + "/S.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](S_entry& t) {
                        data.on_insert_S(t);
                    };
                }
        )));
    #endif

    #if defined(RELATION_T_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<T_entry>(
                "T", dataPath + "/T.tbl", '|', true,
                [](dbtoaster::data_t& data) {
                    return [&](T_entry& t) {
                        data.on_insert_T(t);
                    };
                }
        )));
    #elif defined(RELATION_T_DYNAMIC) && defined(BATCH_SIZE)
        typedef const std::vector<DELTA_T_entry>::iterator CIteratorT;
        relations.push_back(std::unique_ptr<IRelation>(
            new BatchDispatchableRelation<DELTA_T_entry>(
                "T", dataPath + "/T.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorT& begin, CIteratorT& end) {
                        data.on_batch_update_T(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_T_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<T_entry>(
                "T", dataPath + "/T.tbl", '|', false,
                [](dbtoaster::data_t& data) {
                    return [&](T_entry& t) {
                        data.on_insert_T(t);
                    };
                }
        )));
    #endif
}

#endif /* APPLICATION_SIMPLE_BASE_HPP */