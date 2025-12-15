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

      parse_line_into_chunk(line, *chunk);
      ++count;
    }
    chunk->row_count = count;

    return (count != 0) ? std::move(chunk) : nullptr;
  }

  bool has_next() override { return ifs.good(); }

  void reset() override {
    ifs.clear();   // Clear eof/fail bits
    ifs.seekg(0);  // Go back to start

    // Avoid skipping whitespace
    ifs >> std::noskipws;
  }

 private:
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

  void parse_line_into_chunk(const std::string& line, DataChunk& chunk) {
    // Stream the line as CSV fields
    std::stringstream ss(line);
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

#endif /* RUNTIME_CSV_READER_HPP */