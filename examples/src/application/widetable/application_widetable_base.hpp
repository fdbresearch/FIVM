#ifndef APPLICATION_WIDETABLE_BASE_HPP
#define APPLICATION_WIDETABLE_BASE_HPP

#include "../application.hpp"

const string dataPath = "data/widetable";

void Application::init_relations() {
    clear_relations();

    #if defined(RELATION_R_STATIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<R_entry>(
                "R", dataPath + "/R.csv", ',', true,
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
                "R", dataPath + "/R.csv", ',', false,
                [](dbtoaster::data_t& data) {
                    return [&](CIteratorR& begin, CIteratorR& end) {
                        data.on_batch_update_R(begin, end);
                    };
                }
        )));
    #elif defined(RELATION_R_DYNAMIC)
        relations.push_back(std::unique_ptr<IRelation>(
            new EventDispatchableRelation<R_entry>(
                "R", dataPath + "/R.csv", ',', false,
                [](dbtoaster::data_t& data) {
                    return [&](R_entry& t) {
                        data.on_insert_R(t);
                    };
                }
        )));
    #endif
}

#endif /* APPLICATION_WIDETABLE_BASE_HPP */