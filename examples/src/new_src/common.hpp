#ifndef APPLICATION_COMMON_HPP
#define APPLICATION_COMMON_HPP

#include <map>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
enum class PrimitiveType : uint8_t { INT32, FLOAT, STRING };

enum class DataSourceType : uint8_t { CSV, PARQUET, DUCKDB };

struct SchemaField {
  std::string name;
  PrimitiveType type;
};

struct DataSourceConfig {
  std::string name;
  DataSourceType type;
  std::string uri;
  std::vector<SchemaField> schema;
  std::map<std::string, std::string> options;
};

// ---------------------------------------------------------------------------
struct ColumnBase {
 public:
  explicit ColumnBase(PrimitiveType t) : type_(t) {}
  virtual ~ColumnBase() = default;

  PrimitiveType type() const { return type_; }

  virtual void append_from_string(const std::string&) = 0;
  virtual size_t size() const = 0;

 private:
  PrimitiveType type_;
};

// ---------------------------------------------------------------------------
template <typename T>
T parse(const std::string& s);

template <>
inline int32_t parse<int32_t>(const std::string& s) {
  return std::stoi(s);
}

template <>
inline float parse<float>(const std::string& s) {
  return std::stof(s);
}

template <>
inline std::string parse<std::string>(const std::string& s) {
  return s;
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
  size_t row_count = 0;

  DataChunk(const DataSourceConfig& cfg, size_t sz = 0) {
    cols.reserve(cfg.schema.size());

    for (const auto& c : cfg.schema) {
      cols.push_back(createColumn(c.type, sz));
    }
  }

 private:
  static std::unique_ptr<ColumnBase> createColumn(PrimitiveType type,
                                                  size_t sz) {
    switch (type) {
      case PrimitiveType::INT32:
        return std::make_unique<Column<int32_t>>(type, sz);
      case PrimitiveType::FLOAT:
        return std::make_unique<Column<float>>(type, sz);
      case PrimitiveType::STRING:
        return std::make_unique<Column<std::string>>(type, sz);
    }

    throw std::runtime_error("Unknown PrimitiveType");
  }
};

#endif /* APPLICATION_COMMON_HPP */