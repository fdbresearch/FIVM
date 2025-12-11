#ifndef RUNTIME_DATA_SOURCE_HPP
#define RUNTIME_DATA_SOURCE_HPP

#include "common.hpp"

// ---------------------------------------------------------------------------
template <typename T>
class IDataReader {
 public:
  virtual ~IDataReader() = default;

  virtual T next() = 0;
  virtual bool has_next() = 0;
  virtual void reset() = 0;
};

// ---------------------------------------------------------------------------
template <typename T>
class IDataWriter {
 public:
  virtual ~IDataWriter() = default;

  virtual void push(T item) = 0;
  virtual void close() = 0;
};

// ---------------------------------------------------------------------------
template <typename T>
class IDataSource : public IDataReader<T>, public IDataWriter<T> {
 public:
  virtual ~IDataSource() = default;
};

// ---------------------------------------------------------------------------
using IDataChunkReader = IDataReader<DataChunkPtr>;
using IDataChunkWriter = IDataWriter<DataChunkPtr>;
using IDataChunkSource = IDataSource<DataChunkPtr>;

// ---------------------------------------------------------------------------
class DataChunkSource {
 public:
  const DataSourceConfig cfg;
  std::shared_ptr<IDataChunkSource> source;

  DataChunkSource(const DataSourceConfig& c,
                  std::shared_ptr<IDataChunkSource> s)
      : cfg(c), source(std::move(s)) {}
};

#endif /* RUNTIME_DATA_SOURCE_HPP */