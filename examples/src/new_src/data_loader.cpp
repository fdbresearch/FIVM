#include <atomic>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "common.hpp"
#include "data_source.hpp"
#include "queue.hpp"

typedef ThreadSafeQueue<std::unique_ptr<DataChunk>> ChunkQueue;

namespace Config {

static const std::vector<DataSourceConfig> DataSources = {
    {"R",
     DataSourceType::CSV,
     "data/simple/R.tbl",
     // schema
     {{"A", PrimitiveType::INT32}, {"B", PrimitiveType::INT32}},
     // options
     {{"delimiter", "|"}, {"header", "true"}}},
    {"S",
     DataSourceType::CSV,
     "data/simple/S.tbl",
     // schema
     {{"C", PrimitiveType::INT32},
      {"D", PrimitiveType::INT32},
      {"E", PrimitiveType::INT32}},
     // options
     {{"delimiter", "|"}}}};

}  // namespace Config

// ---------------------------------------------------------------------------
// ----------------------------- ColumnarLoader (Producer-Consumer)
// ----------------------------- Each reader runs in its own producer thread,
// pushing chunks into a queue. One consumer thread drains all queues and calls
// the processing function.
// class ColumnarLoader {
//  public:
//   vector<unique_ptr<IDataReader>> readers;
//   vector<unique_ptr<ThreadSafeQueue<unique_ptr<Chunk>>>> queues;
//   vector<thread> producer_threads;
//   thread consumer_thread;
//   atomic<bool> stop{false};
//   function<void(shared_ptr<Chunk>, const DataSourceConfig&)> process_fn;

//   ColumnarLoader(vector<unique_ptr<IDataReader>> rs,
//                  function<void(shared_ptr<Chunk>, const DataSourceConfig&)>
//                  fn)
//       : readers(std::move(rs)), process_fn(fn) {
//     for (auto& r : rs) {
//       //   //   SourceConfig sc;
//       //   //   sc.source_name = src["name"].get<string>();
//       //   //   sc.type = src["type"].get<string>();
//       //   //   sc.file_path = src.value("file", "");
//       //   //   sc.sql = src.value("sql", "");

//       //   for (auto& col : src["schema"]) {
//       //     sc.schema.push_back({col["name"], parse_type(col["type"])});
//       //   }

//       //   shared_ptr<IDataReader> reader;
//       //   if (sc.type == "csv")
//       //     reader = make_shared<CsvReader>(sc);
//       //   else if (sc.type == "duckdb")
//       //     reader = make_shared<DuckDBReader>(sc);
//       //   else
//       //     throw runtime_error("Unknown source type: " + sc.type);

//       //   readers.push_back(reader);
//       queues.push_back(make_unique<ThreadSafeQueue<unique_ptr<Chunk>>>());
//     }
//   }

//   void start(size_t batch_rows = 1000) {
//     // Start producer threads
//     for (size_t i = 0; i < readers.size(); ++i) {
//       producer_threads.emplace_back([this, i, batch_rows]() {
//         auto& reader = readers[i];
//         auto& queue = queues[i];
//         while (!stop) {
//           auto chunk = reader->next_chunk(batch_rows);
//           if (!chunk) break;
//           cout << "Chunk rows " << chunk->row_count << " " << endl;
//           queue->push(std::move(chunk));
//         }
//         queue->close();
//       });
//     }

//     // // Start consumer thread
//     // consumer_thread = thread([this]() { consume(); });
//   }

//   void consume() {
//     size_t active = readers.size();
//     while (active > 0) {
//       for (size_t i = 0; i < readers.size(); ++i) {
//         auto& queue = queues[i];
//         auto chunk = queue->pop();
//         if (!chunk) {
//           // Queue closed
//           if (queue->closed.exchange(false)) {
//             --active;
//           }
//           continue;
//         }
//         process_fn(chunk, readers[i]->cfg);
//       }
//     }
//   }

//   void stop_all() {
//     stop = true;
//     for (auto& q : queues) q->close();
//   }

//   void join() {
//     for (auto& t : producer_threads)
//       if (t.joinable()) t.join();
//     if (consumer_thread.joinable()) consumer_thread.join();
//   }
// };

// void producer(IDataReader& r, ThreadSafeQueue<unique_ptr<Chunk>>& q,
//               size_t batch_rows = 1000) {
//   while (true) {
//     auto chunk = r.next_chunk(batch_rows);
//     if (!chunk) break;
//     cout << "Chunk rows " << chunk->row_count << " " << endl;
//     q.push(std::move(chunk));
//   }
//   q.close();
// }

// // Consumer function
// void consumer(ThreadSafeQueue<std::unique_ptr<Task>> &q) {
//     while (true) {
//         auto task = q.pop();
//         if (!task) break; // Queue is done and empty
//         task->execute();
//         std::this_thread::sleep_for(std::chrono::milliseconds(150));
//     }
// }

std::unique_ptr<IDataReader> createReader(const DataSourceConfig& cfg) {
  switch (cfg.type) {
    case DataSourceType::CSV:
      return std::make_unique<CsvReader>(cfg);
    case DataSourceType::PARQUET:
      break;
    case DataSourceType::DUCKDB:
      break;
  }

  throw std::runtime_error("Unknown DataSourceType");
}

int main() {
  std::vector<std::shared_ptr<ChunkQueue>> queues;
  for (const auto& cfg : Config::DataSources) {
    queues.push_back(std::make_shared<ChunkQueue>());
  }

  // Create producers, one per data source
  std::vector<std::thread> threads;
  for (size_t i = 0; i < Config::DataSources.size(); ++i) {
    threads.emplace_back([&queues, i]() {
      const auto& c = Config::DataSources[i];
      const auto& q = queues[i];
      auto reader = createReader(c);

      size_t chunk_index = 0;
      while (auto chunk = reader->next_chunk(2)) {
        std::cout << c.name << "  Chunk # " << chunk_index++
                  << " rows: " << chunk->row_count << std::endl;
        q->push(std::move(chunk));
      }
      q->close();
    });
  }

  // Round-robin consumer
  threads.emplace_back([&queues] {
    std::cout << "Consumer start " << std::endl;
    bool any_active = true;
    while (any_active) {
      any_active = false;
      for (auto& q : queues) {
        while (auto chunk = q->pop()) {
          // chunk->process();   // Assuming Chunk has process() method
          std::cout << "Consumer " << chunk->cols[0]->size() << std::endl;
          any_active = true;  // There was something to process
        }
      }
      // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  });
  // consumer_thread.join();

  // Join producer threads
  for (auto& t : threads) t.join();

  //   //   ColumnarLoader L(std::move(rs), fn);
  //   //   L.start(1024);
  // }
}
