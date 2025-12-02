#ifndef APPLICATION_DATA_SOURCE_HPP
#define APPLICATION_DATA_SOURCE_HPP

#include <fstream>
#include <sstream>

#include "common.hpp"

// ---------------------------------------------------------------------------
class IDataReader {
 public:
  const DataSourceConfig cfg;
  IDataReader(const DataSourceConfig& c) : cfg(c) {}
  virtual ~IDataReader() {}
  virtual bool has_next() = 0;
  virtual std::unique_ptr<DataChunk> next_chunk(size_t batch_size) = 0;
};

// ---------------------------------------------------------------------------
class CsvReader : public IDataReader {
 public:
  CsvReader(const DataSourceConfig& c)
      : IDataReader(c), ifs(c.uri), delimiter(parseDelimiter(c)) {
    // Check if file opened successfully
    if (!ifs.is_open()) {
      throw std::runtime_error("Cannot open CSV file: " + c.uri);
    }
    // Avoid skipping whitespace in fields (important for CSV)
    ifs >> std::noskipws;
  }

  bool has_next() override { return ifs && !ifs.eof(); }

  std::unique_ptr<DataChunk> next_chunk(size_t batch) override {
    if (!has_next()) return nullptr;

    auto chunk = std::make_unique<DataChunk>(cfg);
    std::string line;
    size_t count = 0;

    while (count < batch && std::getline(ifs, line)) {
      if (line.empty()) continue;

      parse_line_into_chunk(line, *chunk);
      ++count;
    }
    chunk->row_count = count;

    return (count != 0) ? std::move(chunk) : nullptr;
  }

 private:
  std::ifstream ifs;
  char delimiter;

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
  }
};

#endif /* APPLICATION_DATA_SOURCE_HPP */