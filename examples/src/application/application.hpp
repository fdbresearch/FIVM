#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <vector>
#include <sys/time.h>
#include "stopwatch.hpp"
#include "dispatcher.hpp"
#include "relation.hpp"

#ifdef LOG_MEMORY_INFO
    #include "memory.hpp"
    #define START_HEAP_PROFILE startHeapProfiler("MAIN");
    #define STOP_HEAP_PROFILE stopHeapProfiler();
    #define DUMP_HEAP_PROFILE dumpHeapProfile("APPLICATION");
#else
    #define START_HEAP_PROFILE
    #define STOP_HEAP_PROFILE
    #define DUMP_HEAP_PROFILE
#endif

using namespace dbtoaster;

class Application {
  protected:
    std::vector<std::unique_ptr<IRelation>> relations;
    Multiplexer static_multiplexer;
    Multiplexer dynamic_multiplexer;

    void init_relations();

    void load_relations();

    void clear_relations();

    void init_dispatchers(dbtoaster::data_t& data);

    void clear_dispatchers();

    void process_tables(dbtoaster::data_t& data);

    void process_on_system_ready(dbtoaster::data_t& data);

    void process_streams(dbtoaster::data_t& data);

    void process_streams_snapshot(dbtoaster::data_t& data, long snapshot_interval);

    void process_streams_no_snapshot(dbtoaster::data_t& data);

    void on_snapshot(dbtoaster::data_t& data);

    void on_begin_processing(dbtoaster::data_t& data);

    void on_end_processing(dbtoaster::data_t& data, bool print_result);

    void print_result(dbtoaster::data_t& data);

    void print_checkpoint(dbtoaster::data_t& data);

  public:
    Application() { }

    ~Application() {
        clear_relations();
        clear_dispatchers();
    }

    void run(size_t num_of_runs, bool output_result);
};

void Application::load_relations() {
    for (auto &r : relations) {
        r->clear();
        r->load();
    }
}

void Application::clear_relations() {
    for (auto &r : relations) {
        r->clear();
    }
    relations.clear();
}

void Application::init_dispatchers(dbtoaster::data_t& data) {
    clear_dispatchers();

    for (auto &r : relations) {
        if (r->is_static()) {
            static_multiplexer.add_dispatcher(r->create_dispatcher(data));
        }
        else {
            dynamic_multiplexer.add_dispatcher(r->create_dispatcher(data));
        }
    }
}

void Application::clear_dispatchers() {
    static_multiplexer.clear();
    dynamic_multiplexer.clear();   
}

void Application::process_tables(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_tables" << std::flush;
        DUMP_HEAP_PROFILE
    #endif

    while (static_multiplexer.has_next()) {
        static_multiplexer.next();
    }

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_tables: " << std::flush;
        DUMP_HEAP_PROFILE
    #endif
}

void Application::process_on_system_ready(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before on_system_ready: " << std::flush;
        DUMP_HEAP_PROFILE
    #endif

    data.on_system_ready_event();

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after on_system_ready: " << std::flush;
        DUMP_HEAP_PROFILE
    #endif

}

void Application::process_streams(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_streams: " << std::flush;
        DUMP_HEAP_PROFILE
    #endif

    #ifdef SNAPSHOT_INTERVAL
        process_streams_snapshot(data, SNAPSHOT_INTERVAL);
    #else
        process_streams_no_snapshot(data);
    #endif

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_streams: " << std::flush;
        DUMP_HEAP_PROFILE
    #endif
}

void Application::process_streams_snapshot(dbtoaster::data_t& data, long snapshot_interval) {
    long next_snapshot = 0;

    while (dynamic_multiplexer.has_next()) {
        if (data.tN >= next_snapshot) {
            on_snapshot(data);
            next_snapshot = data.tN + snapshot_interval;
        }
        dynamic_multiplexer.next();
    }

    if (next_snapshot != data.tN + snapshot_interval) {
        on_snapshot(data);
    }
}

void Application::process_streams_no_snapshot(dbtoaster::data_t& data) {
    while (dynamic_multiplexer.has_next()) {
        dynamic_multiplexer.next();
    }
}

void Application::print_checkpoint(dbtoaster::data_t& data) {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    std::cout << data.tN << " tuples processed at "
            << tp.tv_sec * 1000 + tp.tv_usec / 1000
            << " ms" << std::endl;
}

void Application::run(size_t num_of_runs, bool print_result) {
    std::cout << "-------------" << std::endl;

    init_relations();

    Stopwatch local_time, total_time;

    std::cout << "Loading input relations... " << std::flush;
    local_time.restart();    
    load_relations();
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    for (auto &r : relations) {
        std::cout << "  " << r->get_name() << " (" << r->size() << ") " << std::endl;
    }

    for (size_t run = 0; run < num_of_runs; run++) {

        dbtoaster::data_t data;

        std::cout << "-------------" << std::endl;

        init_dispatchers(data);

        std::cout << "1. On begin of processing... " << std::flush;
        local_time.restart();
        on_begin_processing(data);
        local_time.stop();
        std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        std::cout << "2. Processing tables... " << std::flush;
        local_time.restart();
        process_tables(data);
        local_time.stop();
        std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        
        total_time.restart();

        std::cout << "3. On system ready... " << std::flush;
        local_time.restart();
        process_on_system_ready(data);
        local_time.stop();
        std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        std::cout << "4. Processing streams... " << std::flush;;
        local_time.restart();
        process_streams(data);
        local_time.stop();
        std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        std::cout << "5. On end of processing... " << std::flush;
        local_time.restart();
        on_end_processing(data, print_result);
        local_time.stop();
        std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        total_time.stop();

        std::cout << "    Run: " << run
                  << "    Processed: " << data.tN
                  << "    Skipped: " << data.tS
                  << "    Execution time: " << total_time.elapsedTimeInMilliSeconds() << " ms"
#ifdef BATCH_SIZE
                  << "    Batch size: " << BATCH_SIZE
#endif
                  << std::endl
                  << "-------------"
                  << std::endl;

        clear_dispatchers();
    }

    clear_relations();
}

#endif /* APPLICATION_HPP */