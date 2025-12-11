#ifndef FIVM_APPLICATION_HPP
#define FIVM_APPLICATION_HPP

#include <sys/time.h>

#include <cstddef>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "common.hpp"
#include "csv_reader.hpp"
#include "data_source.hpp"
#include "stopwatch.hpp"
#include "thread_safe_buffer.hpp"
#include "thread_safe_queue.hpp"

#ifdef LOG_MEMORY_INFO
#include "dbtoaster/memory.hpp"
#define START_HEAP_PROFILE startHeapProfiler("MAIN");
#define STOP_HEAP_PROFILE stopHeapProfiler();
#define DUMP_HEAP_PROFILE dumpHeapProfile("APPLICATION");
#else
#define START_HEAP_PROFILE
#define STOP_HEAP_PROFILE
#define DUMP_HEAP_PROFILE
#endif

namespace dbtoaster {
struct data_t;
}

class Application {
 protected:
  void process_tables(std::vector<DataChunkSource>& sources,
                      dbtoaster::data_t& data);

  void process_on_system_ready(dbtoaster::data_t& data);

  void process_streams(std::vector<DataChunkSource>& sources,
                       dbtoaster::data_t& data);

  void on_snapshot(dbtoaster::data_t& data);

  void on_begin_processing(dbtoaster::data_t& data);

  void on_end_processing(dbtoaster::data_t& data, bool print_result);

  void print_result(dbtoaster::data_t& data);

  void print_checkpoint(dbtoaster::data_t& data);

 public:
  Application() {}

  void run(size_t num_of_runs, bool output_result, size_t batch_sz);
};

std::unique_ptr<IDataChunkReader> createReader(const DataSourceConfig& cfg,
                                               size_t batch_sz) {
  switch (cfg.type) {
    case DataSourceType::CSV:
      return std::make_unique<CsvReader>(cfg, batch_sz);
    case DataSourceType::PARQUET:
      break;
  }

  throw std::runtime_error("Unknown DataSourceFormat");
}

void producer_thread(DataChunkSource& src, size_t batch_sz) {
  auto reader = createReader(src.cfg, batch_sz);
  size_t sz = 0;
  while (auto chunk = reader->next()) {
    sz += chunk->row_count;
    src.source->push(std::move(chunk));
  }
  src.source->close();
  std::cout << "  " << src.cfg.name << " (" << sz << ") completed."
            << std::endl;
}

void round_robin_consumer_thread(std::vector<DataChunkSource>& sources,
                                 dbtoaster::data_t& data) {
  bool any_active = true;
  while (any_active) {
    any_active = false;
    for (auto& s : sources) {
      if (auto chunk = s.source->next()) {
        s.cfg.callback(&data, *chunk);
        any_active = true;  // There was something to process
      }
    }
  }
}

void Application::process_tables(std::vector<DataChunkSource>& sources,
                                 dbtoaster::data_t& data) {
#ifdef LOG_MEMORY_INFO
  std::cout << "Memory before process_tables" << std::flush;
  DUMP_HEAP_PROFILE
#endif

  round_robin_consumer_thread(sources, data);

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

void Application::process_streams(std::vector<DataChunkSource>& sources,
                                  dbtoaster::data_t& data) {
#ifdef LOG_MEMORY_INFO
  std::cout << "Memory before process_streams: " << std::flush;
  DUMP_HEAP_PROFILE
#endif

#ifdef SNAPSHOT_INTERVAL
  process_streams_snapshot(data, SNAPSHOT_INTERVAL);
#else
  round_robin_consumer_thread(sources, data);
#endif

#ifdef LOG_MEMORY_INFO
  std::cout << "Memory after process_streams: " << std::flush;
  DUMP_HEAP_PROFILE
#endif
}

// void Application::process_streams_snapshot(const QueueList& queues,
//                                            dbtoaster::data_t& data,
//                                            long snapshot_interval) {
//   long next_snapshot = 0;

//   while (dynamic_multiplexer.has_next()) {
//     if (data.tN >= next_snapshot) {
//       on_snapshot(data);
//       next_snapshot = data.tN + snapshot_interval;
//     }
//     dynamic_multiplexer.next();
//   }

//   if (next_snapshot != data.tN + snapshot_interval) {
//     on_snapshot(data);
//   }
// }

void Application::print_checkpoint(dbtoaster::data_t& data) {
  struct timeval tp;
  gettimeofday(&tp, nullptr);
  std::cout << data.tN << " tuples processed at "
            << tp.tv_sec * 1000 + tp.tv_usec / 1000 << " ms" << std::endl;
}

void Application::on_snapshot(dbtoaster::data_t& data) {
  struct timeval tp;
  gettimeofday(&tp, nullptr);
  std::cout << data.tN << " tuples processed at "
            << tp.tv_sec * 1000 + tp.tv_usec / 1000 << " ms" << std::endl;
  DUMP_HEAP_PROFILE
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
  START_HEAP_PROFILE
}

void Application::on_end_processing(dbtoaster::data_t& data,
                                    bool print_result) {
  STOP_HEAP_PROFILE

  if (print_result) {
    data.serialize(std::cout, 0);
  }
}

void Application::run(size_t num_of_runs, bool print_result, size_t batch_sz) {
  std::cout << "-------------" << std::endl;

  Stopwatch local_time, total_time;

  for (size_t run = 0; run < num_of_runs; ++run) {
    dbtoaster::data_t data;

    std::vector<DataChunkSource> static_table_buffers;
    std::vector<DataChunkSource> dynamic_table_buffers;

    for (const auto& cfg : dbtoaster::Config::DataSources) {
      if (cfg.isStream) {
        dynamic_table_buffers.emplace_back(
            cfg, std::make_shared<ThreadSafeBuffer<DataChunkPtr>>());
      } else {
        static_table_buffers.emplace_back(
            cfg, std::make_shared<ThreadSafeBuffer<DataChunkPtr>>());
      }
    }

    std::cout << "Starting reading from data sources... " << std::endl;

    local_time.restart();

    // Create producers, one per data source
    std::vector<std::thread> static_table_producers;
    for (auto& p : static_table_buffers) {
      static_table_producers.emplace_back(
          [&p, batch_sz]() { producer_thread(p, batch_sz); });
    }
    std::vector<std::thread> dynamic_table_producers;
    for (auto& p : dynamic_table_buffers) {
      dynamic_table_producers.emplace_back(
          [&p, batch_sz]() { producer_thread(p, batch_sz); });
    }

    for (auto& t : static_table_producers)
      if (t.joinable()) t.join();

    for (auto& t : dynamic_table_producers)
      if (t.joinable()) t.join();

    local_time.stop();
    std::cout << "Data sources preloaded in "
              << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "-------------" << std::endl;

    std::cout << "1. On begin of processing... " << std::flush;
    local_time.restart();
    on_begin_processing(data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "2. Processing tables... " << std::flush;
    local_time.restart();
    process_tables(static_table_buffers, data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    total_time.restart();

    std::cout << "3. On system ready... " << std::flush;
    local_time.restart();
    process_on_system_ready(data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "4. Processing streams... " << std::flush;
    ;
    local_time.restart();
    process_streams(dynamic_table_buffers, data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "5. On end of processing... " << std::flush;
    local_time.restart();
    on_end_processing(data, print_result);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    total_time.stop();

    // Join producer threads
    for (auto& t : static_table_producers)
      if (t.joinable()) t.join();

    for (auto& t : dynamic_table_producers)
      if (t.joinable()) t.join();

    std::cout << "    Run: " << run << "    Processed: " << data.tN
              << "    Skipped: " << data.tS << "    Execution time: "
              << total_time.elapsedTimeInMilliSeconds() << " ms"
              << "    Batch size: " << batch_sz << std::endl
              << "-------------" << std::endl;
  }
}

#endif /* FIVM_APPLICATION_HPP */