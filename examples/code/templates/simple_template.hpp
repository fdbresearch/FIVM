#ifndef SIMPLE_TEMPLATE_HPP
#define SIMPLE_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"

using namespace std;
using namespace dbtoaster;

const string dataPath = "data/simple";
const string dataset = "";

#if defined(RELATION_R_STATIC) || (defined(RELATION_R_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<R_entry> rRelation;
    std::vector<R_entry>::iterator rRelationIt;
#elif defined(RELATION_R_DYNAMIC)
    std::vector<DELTA_R_entry> rRelation;
    std::vector<DELTA_R_entry>::iterator rRelationIt;
#endif

#if defined(RELATION_S_STATIC) || (defined(RELATION_S_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<S_entry> sRelation;
    std::vector<S_entry>::iterator sRelationIt;
#elif defined(RELATION_S_DYNAMIC)
    std::vector<DELTA_S_entry> sRelation;
    std::vector<DELTA_S_entry>::iterator sRelationIt;
#endif

#if defined(RELATION_T_STATIC) || (defined(RELATION_T_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<T_entry> tRelation;
    std::vector<T_entry>::iterator tRelationIt;
#elif defined(RELATION_T_DYNAMIC)
    std::vector<DELTA_T_entry> tRelation;
    std::vector<DELTA_T_entry>::iterator tRelationIt;
#endif

void load_relations() {
    Stopwatch sw;

    #if defined(RELATION_R_STATIC) || defined(RELATION_R_DYNAMIC)
        sw.restart();
        rRelation.clear();
        readFromFile(rRelation, dataPath + "/" + dataset + "/R.tbl", '|');
        sw.stop();
        std::cout << "Loaded R (" << rRelation.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_S_STATIC) || defined(RELATION_S_DYNAMIC)
        sw.restart();
        sRelation.clear();
        readFromFile(sRelation, dataPath + "/" + dataset + "/S.tbl", '|');
        sw.stop();
        std::cout << "Loaded S (" << sRelation.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_T_STATIC) || defined(RELATION_T_DYNAMIC)
        sw.restart();
        tRelation.clear();
        readFromFile(tRelation, dataPath + "/" + dataset + "/T.tbl", '|');
        sw.stop();
        std::cout << "Loaded T (" << tRelation.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif
}

void destroy_relations() {
    #if defined(RELATION_R_STATIC) || defined(RELATION_R_DYNAMIC)
        rRelation.clear();
    #endif

    #if defined(RELATION_S_STATIC) || defined(RELATION_S_DYNAMIC)
        sRelation.clear();
    #endif

    #if defined(RELATION_T_STATIC) || defined(RELATION_T_DYNAMIC)
        tRelation.clear();
    #endif
}

#ifdef LOG_MEMORY_INFO
    #define PRINT_MEMORY_INFO dumpHeapProfile("SIMPLE");
#else
    #define PRINT_MEMORY_INFO
#endif

#ifdef LOG_OUTPUT
    #define PRINT_LOG \
    { \
        if (data.tN >= nextLogPoint) { \
            timeval current_time; \
            gettimeofday(&current_time, NULL); \
            long endTime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000; \
            std::cout << "Processing " << data.tN << " tuples at " << endTime << "    "; \
            PRINT_MEMORY_INFO \
            std::cout << std::endl; \
            nextLogPoint = data.tN + LOG_OUTPUT; \
        } \
    }
#else
    #define PRINT_LOG
#endif

void process_tables(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_tables";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif

    #ifdef RELATION_R_STATIC
        for (size_t i = 0; i < rRelation.size(); i++) {
            data.on_insert_R(rRelation[i]);
        }
    #endif

    #ifdef RELATION_S_STATIC
        for (size_t i = 0; i < sRelation.size(); i++) {
            data.on_insert_S(sRelation[i]);
        }
    #endif

    #ifdef RELATION_T_STATIC
        for (size_t i = 0; i < tRelation.size(); i++) {
            data.on_insert_T(tRelation[i]);
        }
    #endif

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_tables: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif
}

void process_streams(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_streams: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif

    size_t i = 0, count = 0;

    #ifdef LOG_OUTPUT
        long nextLogPoint = 0;
        cout << endl;
        PRINT_LOG
    #endif

    #ifdef RELATION_R_DYNAMIC
        rRelationIt = rRelation.begin();
    #endif
    #ifdef RELATION_S_DYNAMIC
        sRelationIt = sRelation.begin();
    #endif
    #ifdef RELATION_T_DYNAMIC
        tRelationIt = tRelation.begin();
    #endif

    #ifdef BATCH_SIZE
        size_t batchSize = BATCH_SIZE;
        bool changed;
        do {
            changed = false;

            #ifdef RELATION_R_DYNAMIC
                size_t rDist = std::distance(rRelationIt, rRelation.end());
                if (rDist > 0) {
                    if (rDist >= batchSize) {
                        data.on_batch_update_R(rRelationIt, rRelationIt + batchSize);
                        rRelationIt += batchSize;
                    }
                    else {
                        data.on_batch_update_R(rRelationIt, rRelation.end());
                        rRelationIt = rRelation.end();
                    }
                    changed = true;
                }
            #endif

            #ifdef RELATION_S_DYNAMIC
                size_t sDist = std::distance(sRelationIt, sRelation.end());
                if (sDist > 0) {
                    if (sDist >= batchSize) {
                        data.on_batch_update_S(sRelationIt, sRelationIt + batchSize);
                        sRelationIt += batchSize;
                    }
                    else {
                        data.on_batch_update_S(sRelationIt, sRelation.end());
                        sRelationIt = sRelation.end();
                    }
                    changed = true;
                }
            #endif

            #ifdef RELATION_T_DYNAMIC
                size_t tDist = std::distance(tRelationIt, tRelation.end());
                if (tDist > 0) {
                    if (tDist >= batchSize) {
                        data.on_batch_update_T(tRelationIt, tRelationIt + batchSize);
                        tRelationIt += batchSize;
                    }
                    else {
                        data.on_batch_update_T(tRelationIt, tRelation.end());
                        tRelationIt = tRelation.end();
                    }
                    changed = true;
                }
            #endif
        } while (changed);

    #else

        bool changed;
        do {
            changed = false;

            #ifdef RELATION_R_DYNAMIC
                if (rRelationIt != rRelation.end()) {
                    data.on_insert_R(*rRelationIt);
                    rRelationIt++;
                    changed = true;
                }
            #endif

            #ifdef RELATION_S_DYNAMIC
                if (sRelationIt != sRelation.end()) {
                    data.on_insert_S(*sRelationIt);
                    sRelationIt++;
                    changed = true;
                }
            #endif

            #ifdef RELATION_T_DYNAMIC
                if (tRelationIt != tRelation.end()) {
                    data.on_insert_T(*tRelationIt);
                    tRelationIt++;
                    changed = true;
                }
            #endif
        } while (changed);
    #endif

    #ifdef LOG_OUTPUT
        nextLogPoint = data.tN;
        PRINT_LOG
    #endif

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_streams: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif
}

void print_result(dbtoaster::data_t& data) {
    data.serialize(std::cout, 0);
}

#endif /* SIMPLE_TEMPLATE_HPP */