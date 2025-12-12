#ifndef RUNTIME_THREAD_SAFE_BUFFER_HPP
#define RUNTIME_THREAD_SAFE_BUFFER_HPP

#include <condition_variable>
#include <mutex>
#include <utility>
#include <vector>

#include "data_source.hpp"

// ---------------------------------------------------------------------------
// Templated thread-safe buffer
// ---------------------------------------------------------------------------
template <typename T>
class ThreadSafeBuffer : public IDataSource<T> {
 public:
  T next() override {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&] { return read_pos_ < data_.size() || closed_; });

    if (read_pos_ >= data_.size()) {
      return T{};
    }

    return data_[read_pos_++];
  }

  bool has_next() override {
    std::lock_guard<std::mutex> lock(mtx_);
    return read_pos_ < data_.size();
  }

  void reset() override {
    std::lock_guard<std::mutex> lock(mtx_);
    read_pos_ = 0;
    cv_.notify_all();
  }

  void push(T item) override {
    std::lock_guard<std::mutex> lock(mtx_);
    if (closed_) throw std::runtime_error("Buffer is closed");
    data_.push_back(std::move(item));
    cv_.notify_one();
  }

  void close() override {
    std::lock_guard<std::mutex> lock(mtx_);
    closed_ = true;
    cv_.notify_all();
  }

 private:
  std::vector<T> data_;
  std::mutex mtx_;
  std::condition_variable cv_;
  size_t read_pos_ = 0;
  bool closed_ = false;
};

#endif /* RUNTIME_THREAD_SAFE_BUFFER_HPP */