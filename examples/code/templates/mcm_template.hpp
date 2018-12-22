#ifndef MCM_TEMPLATE_HPP
#define MCM_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;
using namespace dbtoaster;

class data_t;

void load_relations() { }

void destroy_relations() { }

#ifdef LOG_MEMORY_INFO
    #define PRINT_MEMORY_INFO dumpHeapProfile("MCM");
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

    std::vector<A2_entry>::iterator start;
    std::vector<A2_entry>::iterator end;

    int numBatches = NUMBER_OF_UPDATES;
    for (int i = 0; i < numBatches; i++) {
        data.on_batch_update_A2(start,end);
    }

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

#endif /* MCM_TEMPLATE_HPP */
