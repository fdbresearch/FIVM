#ifndef FIVM_COMMON_HPP
#define FIVM_COMMON_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "dbtoaster/functions.hpp"
#include "dbtoaster/types.hpp"

struct DataChunk;

using DataChunkPtr = std::shared_ptr<DataChunk>;

using DispatchFn = void (*)(void* ctx, const DataChunk& chunk);

using payload_t = int32_t;

using string_t = dbtoaster::STRING_TYPE;

struct date_t {
  dbtoaster::DATE_TYPE value;
};
static_assert(sizeof(date_t) == sizeof(dbtoaster::DATE_TYPE));

// ---------------------------------------------------------------------------
enum class PrimitiveType : uint8_t {
  INT8,
  INT16,
  INT32,
  INT64,
  FLOAT,
  DOUBLE,
  CHAR,
  STRING,
  DATE
};

enum class DataSourceType : uint8_t { CSV, PARQUET };

struct SchemaField {
  std::string name;
  PrimitiveType type;
};

struct DataSourceConfig {
  std::string name;
  bool isStream;
  DataSourceType type;
  std::string uri;
  std::vector<SchemaField> schema;
  std::map<std::string, std::string> options;
  DispatchFn callback = nullptr;
};

// ---------------------------------------------------------------------------
struct ColumnBase {
 public:
  explicit ColumnBase(PrimitiveType t) : type_(t) {}
  virtual ~ColumnBase() = default;

  PrimitiveType type() const { return type_; }

  virtual void append_from_string(const std::string&) = 0;
  virtual size_t size() const = 0;

 protected:
  PrimitiveType type_;
};

// ---------------------------------------------------------------------------
template <typename T>
T parse(const std::string& s);

template <>
inline int8_t parse<int8_t>(const std::string& s) {
  return static_cast<int8_t>(std::stoi(s));
}

template <>
inline int16_t parse<int16_t>(const std::string& s) {
  return static_cast<int16_t>(std::stoi(s));
}

template <>
inline int32_t parse<int32_t>(const std::string& s) {
  return std::stoi(s);
}

template <>
inline int64_t parse<int64_t>(const std::string& s) {
  return std::stoll(s);
}

template <>
inline float parse<float>(const std::string& s) {
  return std::stof(s);
}

template <>
inline double parse<double>(const std::string& s) {
  return std::stod(s);
}

template <>
inline char parse<char>(const std::string& s) {
  if (s.size() != 1) {
    throw std::invalid_argument(
        "parse<char>: input must be exactly 1 character");
  }
  return s[0];
}

template <>
inline string_t parse<string_t>(const std::string& s) {
  return string_t(s);
}

template <>
inline date_t parse<date_t>(const std::string& s) {
  return date_t{dbtoaster::str2date(s.c_str())};
}

// ---------------------------------------------------------------------------
template <typename T>
struct Column : public ColumnBase {
  std::vector<T> data;

  Column(PrimitiveType t, size_t sz = 0) : ColumnBase(t) { data.reserve(sz); }

  void append_from_string(const std::string& s) override {
    data.push_back(parse<T>(s));
  }

  size_t size() const override { return data.size(); }
};

// ---------------------------------------------------------------------------
struct DataChunk {
  std::vector<std::unique_ptr<ColumnBase>> cols;
  std::vector<payload_t> payload;
  size_t row_count = 0;

  DataChunk(const DataSourceConfig& cfg, size_t sz = 0) {
    cols.reserve(cfg.schema.size());

    for (const auto& c : cfg.schema) {
      cols.push_back(createColumn(c.type, sz));
    }
    payload.reserve(sz);
  }

 private:
  static std::unique_ptr<ColumnBase> createColumn(PrimitiveType type,
                                                  size_t sz) {
    switch (type) {
      case PrimitiveType::INT8:
        return std::make_unique<Column<int8_t>>(type, sz);
      case PrimitiveType::INT16:
        return std::make_unique<Column<int16_t>>(type, sz);
      case PrimitiveType::INT32:
        return std::make_unique<Column<int32_t>>(type, sz);
      case PrimitiveType::INT64:
        return std::make_unique<Column<int64_t>>(type, sz);
      case PrimitiveType::FLOAT:
        return std::make_unique<Column<float>>(type, sz);
      case PrimitiveType::DOUBLE:
        return std::make_unique<Column<double>>(type, sz);
      case PrimitiveType::CHAR:
        return std::make_unique<Column<char>>(type, sz);
      case PrimitiveType::STRING:
        return std::make_unique<Column<string_t>>(type, sz);
      case PrimitiveType::DATE:
        return std::make_unique<Column<date_t>>(type, sz);
    }

    throw std::runtime_error("Unknown PrimitiveType");
  }
};

#endif /* FIVM_COMMON_HPP */