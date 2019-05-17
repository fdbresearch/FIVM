#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <iostream>
#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"
#include "dispatcher.hpp"
#include "relation.hpp"

#ifdef LOG_MEMORY_INFO
    #include "memory.hpp"
    #define PRINT_MEMORY_INFO dumpHeapProfile(TEMPLATE_NAME);
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

using namespace std;
using namespace dbtoaster;

std::vector<std::unique_ptr<IRelation>> relations;

Multiplexer static_multiplexer;
Multiplexer dynamic_multiplexer;

void load_relations() {
    Stopwatch sw;
    for (auto &r : relations) {
        sw.restart();
        r->clear();
        r->load();
        sw.stop();
        std::cout << "Loaded " << r->get_name() << " (" << r->size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    }
}

void destroy_relations() {
    for (auto &r : relations) {
        r->clear();
    }
}

void process_tables(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_tables";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif

    static_multiplexer.clear();

    for (auto &r : relations) {
        if (r->is_static()) {
            static_multiplexer.add_dispatcher(r->create_dispatcher(data));
        }
    }

    while (static_multiplexer.has_next()) {
        static_multiplexer.next();
    }

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

    dynamic_multiplexer.clear();

    for (auto &r : relations) {
        if (!r->is_static()) {
            dynamic_multiplexer.add_dispatcher(r->create_dispatcher(data));
        }
    }

    #ifdef LOG_OUTPUT
        long nextLogPoint = 0;
        std::cout << std::endl;
        PRINT_LOG
    #endif

    while (dynamic_multiplexer.has_next()) {
        dynamic_multiplexer.next();
        PRINT_LOG
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

#endif /* TEMPLATE_HPP */