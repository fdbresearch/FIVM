#include <atomic>
#include <memory>

// ---------------------------------------------------------------------------
// Templated thread-safe queue
// ---------------------------------------------------------------------------
template <typename T>
class ThreadSafeQueue {
 public:
  void push(T item) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (closed_) throw std::runtime_error("Queue is closed");
    queue_.push(std::move(item));
    cv_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&] { return !queue_.empty() || closed_; });

    if (queue_.empty()) return T{};

    auto item = std::move(queue_.front());
    queue_.pop();
    return item;
  }

  void close() {
    std::lock_guard<std::mutex> lock(mtx_);
    closed_ = true;
    cv_.notify_all();
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.size();
  }

 private:
  std::queue<T> queue_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;
  bool closed_ = false;
};
