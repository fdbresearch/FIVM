#ifndef RUNTIME_THREAD_SAFE_BUFFER_HPP
#define RUNTIME_THREAD_SAFE_BUFFER_HPP

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

#include "data_source.hpp"

// ---------------------------------------------------------------------------
// Templated thread-safe queue
// ---------------------------------------------------------------------------
template <typename T>
class ThreadSafeQueue : public IDataSource<T> {
 public:
  T next() override {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&] { return !queue_.empty() || closed_; });

    if (queue_.empty()) return T{};

    T item = std::move(queue_.front());
    queue_.pop();
    return item;
  }

  bool has_next() override {
    std::lock_guard<std::mutex> lock(mtx_);
    return !queue_.empty();
  }

  void reset() override {
    std::lock_guard<std::mutex> lock(mtx_);
    closed_ = false;
    cv_.notify_all();
  }

  void push(T item) override {
    std::lock_guard<std::mutex> lock(mtx_);
    if (closed_) throw std::runtime_error("Queue is closed");
    queue_.push(std::move(item));
    cv_.notify_one();
  }

  void close() override {
    std::lock_guard<std::mutex> lock(mtx_);
    closed_ = true;
    cv_.notify_all();
  }

 private:
  std::queue<T> queue_;
  std::mutex mtx_;
  std::condition_variable cv_;
  bool closed_ = false;
};

#endif /* RUNTIME_THREAD_SAFE_BUFFER_HPP */