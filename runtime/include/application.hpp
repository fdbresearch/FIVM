#ifndef FIVM_APPLICATION_HPP
#define FIVM_APPLICATION_HPP

#include <sys/time.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "common.hpp"
#include "csv_reader.hpp"
#include "data_source.hpp"
#include "stopwatch.hpp"
#include "thread_safe_buffer.hpp"

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

using SnapshotFn = std::function<void(dbtoaster::data_t&)>;

class Application {
 protected:
  virtual void createDataSources();

  virtual void preloadDataSources(size_t batch_size);

  virtual void resetDataSources();

  virtual void processTables(std::vector<DataChunkSource>& sources,
                             dbtoaster::data_t& data);

  virtual void processOnSystemReady(dbtoaster::data_t& data);

  virtual void processStreams(std::vector<DataChunkSource>& sources,
                              dbtoaster::data_t& data);

  virtual void onSnapshot(const dbtoaster::data_t& data);

  virtual void onBeginProcessing(const dbtoaster::data_t& data);

  virtual void onEndProcessing(const dbtoaster::data_t& data,
                               bool print_result);

  virtual void printCheckpoint(const dbtoaster::data_t& data);

  std::vector<DataChunkSource> static_sources;
  std::vector<DataChunkSource> dynamic_sources;

 public:
  Application() = default;
  virtual ~Application() = default;

  void run(size_t num_runs, bool print_result, size_t batch_size);
};

// -----------------------------------------------------------------------------
// Reader factory
// -----------------------------------------------------------------------------
std::unique_ptr<IDataChunkReader> createReader(const DataSourceConfig& cfg,
                                               size_t batch_size) {
  switch (cfg.type) {
    case DataSourceType::CSV: {
      bool predefined_batches = cfg.options.count("predefined_batches") &&
                                cfg.options.at("predefined_batches") == "true";
      return predefined_batches
                 ? std::make_unique<CsvReaderPredefinedBatches>(cfg, batch_size)
                 : std::make_unique<CsvReader>(cfg, batch_size);
    }
    case DataSourceType::PARQUET:
      // TODO: implement parquet reader
      break;
  }

  throw std::runtime_error("Unknown DataSourceFormat: " + cfg.name);
}

// -----------------------------------------------------------------------------
// Producer thread
// -----------------------------------------------------------------------------
void dataProducer(DataChunkSource& src, size_t batch_size) {
  auto reader = createReader(src.cfg, batch_size);
  size_t total_rows = 0;

  while (auto chunk = reader->next()) {
    total_rows += chunk->row_count;
    src.source->push(std::move(chunk));
  }

  src.source->close();

  std::cout << "  " << src.cfg.name << " (" << total_rows << ") completed.\n";
}

// -----------------------------------------------------------------------------
// Round-robin consumer
// -----------------------------------------------------------------------------
void roundRobinConsumer(std::vector<DataChunkSource>& sources,
                        dbtoaster::data_t& data) {
  bool active = true;

  while (active) {
    active = false;

    for (auto& src : sources) {
      if (auto chunk = src.source->next()) {
        src.cfg.callback(&data, *chunk);
        active = true;
      }
    }
  }
}

void roundRobinConsumerSnapshot(std::vector<DataChunkSource>& sources,
                                dbtoaster::data_t& data, long snapshot_interval,
                                SnapshotFn snapshotFn) {
  long next_snapshot = 0;
  bool any_active = true;

  while (any_active) {
    any_active = false;

    for (auto& src : sources) {
      if (auto chunk = src.source->next()) {
        if (data.tN >= next_snapshot) {
          snapshotFn(data);
          next_snapshot = data.tN + snapshot_interval;
        }
        src.cfg.callback(&data, *chunk);
        any_active = true;
      }
    }
  }

  if (next_snapshot != data.tN + snapshot_interval) {
    snapshotFn(data);
  }
}

// -----------------------------------------------------------------------------
// Processing steps
// -----------------------------------------------------------------------------

void Application::createDataSources() {
  for (const auto& cfg : dbtoaster::Config::DataSources) {
    auto buffer = std::make_shared<ThreadSafeBuffer<DataChunkPtr>>();

    if (cfg.isStream) {
      dynamic_sources.emplace_back(cfg, buffer);
    } else {
      static_sources.emplace_back(cfg, buffer);
    }
  }
}

void Application::preloadDataSources(size_t batch_size) {
  // Create producers, one per data source
  std::vector<std::thread> static_producers;
  for (auto& p : static_sources) {
    static_producers.emplace_back(
        [&p, batch_size]() { dataProducer(p, batch_size); });
  }
  std::vector<std::thread> dynamic_producers;
  for (auto& p : dynamic_sources) {
    dynamic_producers.emplace_back(
        [&p, batch_size]() { dataProducer(p, batch_size); });
  }

  for (auto& t : static_producers)
    if (t.joinable()) t.join();

  for (auto& t : dynamic_producers)
    if (t.joinable()) t.join();
}

void Application::resetDataSources() {
  for (auto& b : static_sources) b.source->reset();
  for (auto& b : dynamic_sources) b.source->reset();
}

void Application::processTables(std::vector<DataChunkSource>& sources,
                                dbtoaster::data_t& data) {
#ifdef LOG_MEMORY_INFO
  std::cout << "Memory before process_tables" << std::flush;
  DUMP_HEAP_PROFILE
#endif

  roundRobinConsumer(sources, data);

#ifdef LOG_MEMORY_INFO
  std::cout << "Memory after process_tables: " << std::flush;
  DUMP_HEAP_PROFILE
#endif
}

void Application::processOnSystemReady(dbtoaster::data_t& data) {
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

void Application::processStreams(std::vector<DataChunkSource>& sources,
                                 dbtoaster::data_t& data) {
#ifdef LOG_MEMORY_INFO
  std::cout << "Memory before process_streams: " << std::flush;
  DUMP_HEAP_PROFILE
#endif

#ifdef SNAPSHOT_INTERVAL
  roundRobinConsumerSnapshot(
      sources, data, SNAPSHOT_INTERVAL,
      [this](dbtoaster::data_t& d) { this->onSnapshot(d); });
#else
  roundRobinConsumer(sources, data);
#endif

#ifdef LOG_MEMORY_INFO
  std::cout << "Memory after process_streams: " << std::flush;
  DUMP_HEAP_PROFILE
#endif
}

void Application::onSnapshot(const dbtoaster::data_t& data) {
  printCheckpoint(data);
  DUMP_HEAP_PROFILE
}

void Application::onBeginProcessing(const dbtoaster::data_t& data) {
  START_HEAP_PROFILE
}

void Application::onEndProcessing(const dbtoaster::data_t& data,
                                  bool print_result) {
  STOP_HEAP_PROFILE

  if (print_result) {
    data.serialize(std::cout, 0);
  }
}

void Application::printCheckpoint(const dbtoaster::data_t& data) {
  struct timeval tp;
  gettimeofday(&tp, nullptr);

  const long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  std::cout << data.tN << " tuples processed at " << ms << " ms" << std::endl;
}

void Application::run(size_t num_of_runs, bool print_result,
                      size_t batch_size) {
  std::cout << "-------------" << std::endl;

  createDataSources();

  std::cout << "Starting reading from data sources... " << std::endl;

  Stopwatch preload_timer;
  preload_timer.restart();
  preloadDataSources(batch_size);
  preload_timer.stop();

  std::cout << "Data sources preloaded in "
            << preload_timer.elapsedTimeInMilliSeconds() << " ms\n"
            << "-------------\n";

  for (size_t run = 0; run < num_of_runs; ++run) {
    resetDataSources();

    Stopwatch local_time, total_time;
    dbtoaster::data_t data;

    std::cout << "1. On begin of processing... " << std::flush;
    local_time.restart();
    onBeginProcessing(data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "2. Processing tables... " << std::flush;
    local_time.restart();
    processTables(static_sources, data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    total_time.restart();

    std::cout << "3. On system ready... " << std::flush;
    local_time.restart();
    processOnSystemReady(data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "4. Processing streams... " << std::flush;
    ;
    local_time.restart();
    processStreams(dynamic_sources, data);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    std::cout << "5. On end of processing... " << std::flush;
    local_time.restart();
    onEndProcessing(data, print_result);
    local_time.stop();
    std::cout << local_time.elapsedTimeInMilliSeconds() << " ms" << std::endl;

    total_time.stop();

    std::cout << "    Run: " << run << "    Processed: " << data.tN
              << "    Skipped: " << data.tS << "    Execution time: "
              << total_time.elapsedTimeInMilliSeconds() << " ms"
              << "    Batch size: " << batch_size << std::endl
              << "-------------" << std::endl;
  }
}

#endif /* FIVM_APPLICATION_HPP */