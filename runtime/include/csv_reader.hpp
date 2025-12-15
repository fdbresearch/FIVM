#ifndef RUNTIME_CSV_READER_HPP
#define RUNTIME_CSV_READER_HPP

#include <fstream>
#include <sstream>

#include "data_source.hpp"

// ---------------------------------------------------------------------------
class CsvReader : public IDataChunkReader {
 public:
  CsvReader(const DataSourceConfig& c, size_t batch_sz)
      : cfg(c), ifs(c.uri), delimiter(parseDelimiter(c)), batch_size(batch_sz) {
    if (!ifs.is_open()) {
      throw std::runtime_error("Cannot open CSV file: " + c.uri);
    }
    // Avoid skipping whitespace
    ifs >> std::noskipws;
  }

  std::shared_ptr<DataChunk> next() override {
    if (!has_next()) return nullptr;

    auto chunk = std::make_shared<DataChunk>(cfg);
    std::string line;
    size_t count = 0;

    while (count < batch_size && std::getline(ifs, line)) {
      if (line.empty()) continue;

      std::stringstream ss(line);
      parse_line_into_chunk(ss, *chunk);
      ++count;
    }
    chunk->row_count = count;

    return (count > 0) ? std::move(chunk) : nullptr;
  }

  bool has_next() override { return ifs.good(); }

  void reset() override {
    ifs.clear();   // Clear eof/fail bits
    ifs.seekg(0);  // Go back to start

    // Avoid skipping whitespace
    ifs >> std::noskipws;
  }

 protected:
  const DataSourceConfig cfg;
  std::ifstream ifs;
  char delimiter;
  size_t batch_size;

  static char parseDelimiter(const DataSourceConfig& c) {
    auto it = c.options.find("delimiter");
    const std::string& del = (it != c.options.end()) ? it->second : ",";

    if (del.size() != 1) {
      throw std::invalid_argument("Invalid delimiter: " + del);
    }
    return del[0];
  }

  void parse_line_into_chunk(std::stringstream& ss, DataChunk& chunk) {
    // Stream the line as CSV fields
    std::string field;

    for (size_t i = 0; i < cfg.schema.size(); ++i) {
      if (!getline(ss, field, delimiter)) {
        field.clear();  // treat missing fields as empty
      }
      chunk.cols[i]->append_from_string(field);
    }

    payload_t payload =
        getline(ss, field, delimiter) ? parse<payload_t>(field) : 1;
    chunk.payload.push_back(payload);
  }
};

class CsvReaderPredefinedBatches : public CsvReader {
 public:
  CsvReaderPredefinedBatches(const DataSourceConfig& c, size_t batch_sz)
      : CsvReader(c, batch_sz) {}

  std::shared_ptr<DataChunk> next() override {
    if (!has_next()) return nullptr;

    auto chunk = std::make_shared<DataChunk>(cfg);
    std::string line;
    size_t count = 0;
    int32_t currBatchId = -1;

    while (count < batch_size) {
      // Save current position
      std::streampos pos = ifs.tellg();

      if (!std::getline(ifs, line)) break;

      if (line.empty()) continue;

      std::stringstream ss(line);
      int32_t batchId = get_batch_id(ss);

      if (currBatchId == -1) {
        currBatchId = batchId;
      }

      if (currBatchId == batchId) {
        parse_line_into_chunk(ss, *chunk);
        ++count;
      } else {
        // Restore the stream to the beginning of this line
        ifs.clear();
        ifs.seekg(pos);
        break;
      }
    }
    chunk->row_count = count;

    return (count > 0) ? std::move(chunk) : nullptr;
  }

 protected:
  int32_t get_batch_id(std::stringstream& ss) {
    std::string field;
    return getline(ss, field, delimiter) ? parse<int32_t>(field) : -1;
  }
};

#endif /* RUNTIME_CSV_READER_HPP */