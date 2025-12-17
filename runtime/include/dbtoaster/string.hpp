//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
//
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_STRING_HPP
#define DBTOASTER_STRING_HPP

#include <cstring>
#include <iostream>
#include <string>

#include "pool.hpp"

namespace dbtoaster {

template <uint64_t SIZE>
struct FixedLengthString {
  uint64_t size_;
  char data_[SIZE + 1];

  FixedLengthString() {
    size_ = 0;
    data_[0] = 0;
  }

  FixedLengthString(const char* str) : FixedLengthString(str, strlen(str)) {}

  FixedLengthString(const char* str, uint64_t length) {
    size_ = (length > SIZE ? SIZE : length);
    memcpy(data_, str, sizeof(char) * size_);
    data_[size_] = 0;
  }

  FixedLengthString(const std::string& str)
      : FixedLengthString(str.c_str(), str.size()) {}

  FixedLengthString(const FixedLengthString& str) {
    size_ = str.size_;
    memcpy(data_, str.data_, sizeof(char) * str.size_);
    data_[size_] = 0;
  }

  FORCE_INLINE uint64_t size() const { return size_; }

  FORCE_INLINE const char* c_str() const { return data_; }

  FORCE_INLINE bool operator==(const char* other) const {
    return (other != nullptr && data_[0] == other[0] &&
            strcmp(data_, other) == 0);
  }

  FORCE_INLINE bool operator==(const FixedLengthString& other) const {
    return (size_ == other.size_ &&
            (size_ == 0 ||
             (data_[0] == other.data_[0] &&
              memcmp(data_, other.data_, sizeof(char) * size_) == 0)));
  }

  FORCE_INLINE bool operator!=(const char* other) const {
    return !(this->operator==(other));
  }

  FORCE_INLINE bool operator!=(const FixedLengthString& other) const {
    return !(this->operator==(other));
  }

  FORCE_INLINE FixedLengthString& operator=(const char* str) {
    uint64_t length = strlen(str);
    size_ = (length > SIZE ? SIZE : length);
    memcpy(data_, str, sizeof(char) * size_);
    data_[size_] = 0;
    return *this;
  }

  FORCE_INLINE FixedLengthString& operator=(const FixedLengthString& other) {
    if (this != &other) {
      size_ = other.size_;
      memcpy(data_, other.data_, sizeof(char) * size_);
      data_[size_] = 0;
    }
    return *this;
  }

  FORCE_INLINE FixedLengthString substr(uint64_t pos, uint64_t len) const {
    return (pos < size_
                ? (len <= (size_ - pos)
                       ? FixedLengthString(this->data_ + pos, len)
                       : FixedLengthString(this->data_ + pos, size_ - pos))
                : FixedLengthString());
  }
};

template <uint64_t SIZE>
std::ostream& operator<<(std::ostream& os, const FixedLengthString<SIZE>& s) {
  os << s.c_str();
  return os;
}

struct VariableLengthString {
 public:
  uint64_t size_;
  char* data_;

 public:
  // default
  VariableLengthString() noexcept : size_(0), data_(nullptr) {}

  // from buffer
  VariableLengthString(const char* str, uint64_t length) {
    size_ = length;
    data_ = new char[size_ + 1];
    memcpy(data_, str, size_);
    data_[size_] = 0;
  }

  // from C string
  VariableLengthString(const char* str)
      : VariableLengthString(str, str ? strlen(str) : 0) {}

  // from std::string
  VariableLengthString(const std::string& str)
      : VariableLengthString(str.c_str(), str.size()) {}

  // copy constructor
  VariableLengthString(const VariableLengthString& other)
      : VariableLengthString(other.data_, other.size_) {}

  // move constructor
  VariableLengthString(VariableLengthString&& other) noexcept
      : size_(other.size_), data_(other.data_) {
    other.size_ = 0;
    other.data_ = nullptr;
  }

  virtual ~VariableLengthString() { delete[] data_; }

  // copy assignment
  VariableLengthString& operator=(const VariableLengthString& other) {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      data_ = new char[size_ + 1];
      memcpy(data_, other.data_, size_);
      data_[size_] = 0;
    }
    return *this;
  }

  // move assignment
  VariableLengthString& operator=(VariableLengthString&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      data_ = other.data_;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }

  // assign from C string
  VariableLengthString& operator=(const char* str) {
    delete[] data_;
    size_ = str ? strlen(str) : 0;
    data_ = new char[size_ + 1];
    memcpy(data_, str, size_);
    data_[size_] = 0;
    return *this;
  }

  inline uint64_t size() const noexcept { return size_; }

  inline const char* c_str() const noexcept { return data_ ? data_ : ""; }

  inline bool operator==(const char* other) const {
    if (!other) return size_ == 0;
    if (size_ == 0) return *other == 0;
    return data_[0] == other[0] && strcmp(data_, other) == 0;
  }

  inline bool operator==(const VariableLengthString& other) const {
    return size_ == other.size_ &&
           (size_ == 0 || (data_[0] == other.data_[0] &&
                           memcmp(data_, other.data_, size_) == 0));
  }

  inline bool operator!=(const char* other) const {
    return !(this->operator==(other));
  }

  inline bool operator!=(const VariableLengthString& other) const {
    return !(this->operator==(other));
  }

  VariableLengthString substr(uint64_t pos, uint64_t len) const {
    if (pos >= size_) return VariableLengthString();
    uint64_t n = (len <= size_ - pos) ? len : (size_ - pos);
    return VariableLengthString(data_ + pos, n);
  }
};

std::ostream& operator<<(std::ostream& os, const VariableLengthString& s) {
  os << s.c_str();
  return os;
}

struct RefCountedString : VariableLengthString {
 protected:
  uint64_t* ptr_count_;

 public:
  // default
  RefCountedString() noexcept
      : VariableLengthString(), ptr_count_(new uint64_t(1)) {}

  // from buffer
  RefCountedString(const char* str, uint64_t length)
      : VariableLengthString(str, length), ptr_count_(new uint64_t(1)) {}

  // from C string
  RefCountedString(const char* str)
      : VariableLengthString(str), ptr_count_(new uint64_t(1)) {}

  // from std::string
  RefCountedString(const std::string& str)
      : VariableLengthString(str), ptr_count_(new uint64_t(1)) {}

  // copy constructor
  RefCountedString(const RefCountedString& other) {
    size_ = other.size_;
    data_ = other.data_;
    ptr_count_ = other.ptr_count_;
    (*ptr_count_)++;
  }

  // move constructor
  RefCountedString(RefCountedString&& other) noexcept {
    size_ = other.size_;
    data_ = other.data_;
    ptr_count_ = other.ptr_count_;
    other.size_ = 0;
    other.data_ = nullptr;
    other.ptr_count_ = nullptr;
  }

  virtual ~RefCountedString() {
    if (ptr_count_ && (--(*ptr_count_)) == 0) {
      delete ptr_count_;
      delete[] data_;
    }
    data_ = nullptr;
  }

  // assign from C string
  RefCountedString& operator=(const char* str) {
    if ((--(*ptr_count_)) == 0) {
      delete[] data_;
      *ptr_count_ = 1;  // re-use ptr_count_
    } else {
      ptr_count_ = new uint64_t(1);
    }
    size_ = str ? strlen(str) : 0;
    data_ = new char[size_ + 1];
    memcpy(data_, str, size_);
    data_[size_] = 0;
    return *this;
  }

  // copy assignment
  RefCountedString& operator=(const RefCountedString& other) {
    if (this != &other) {
      if (ptr_count_ && (--(*ptr_count_)) == 0) {
        delete[] data_;
        delete ptr_count_;
      }
      size_ = other.size_;
      data_ = other.data_;
      ptr_count_ = other.ptr_count_;
      (*ptr_count_)++;
    }
    return *this;
  }

  // move assignment
  RefCountedString& operator=(RefCountedString&& other) {
    if (this != &other) {
      if (ptr_count_ && (--(*ptr_count_)) == 0) {
        delete[] data_;
        delete ptr_count_;
      }
      size_ = other.size_;
      data_ = other.data_;
      ptr_count_ = other.ptr_count_;
      other.size_ = 0;
      other.data_ = nullptr;
      other.ptr_count_ = nullptr;
    }
    return *this;
  }

  RefCountedString substr(uint64_t pos, uint64_t len) const {
    if (pos >= size_) return RefCountedString();
    uint64_t n = (len <= size_ - pos) ? len : (size_ - pos);
    return RefCountedString(data_ + pos, n);
  }
};

std::ostream& operator<<(std::ostream& os, const RefCountedString& s) {
  os << s.c_str();
  return os;
}

ThreadSafeValuePool<uint64_t> pool;

struct PooledRefCountedString {
  uint64_t* ptr_count_;
  uint64_t size_;
  char* data_;

  // default
  PooledRefCountedString() {
    ptr_count_ = pool.add();
    *ptr_count_ = 1;
    size_ = 0;
    data_ = nullptr;
  }

  // from buffer
  PooledRefCountedString(const char* str, uint64_t length) {
    ptr_count_ = pool.add();
    *ptr_count_ = 1;
    size_ = length;
    data_ = new char[size_ + 1];
    memcpy(data_, str, size_);
    data_[size_] = 0;
  }

  // from C string
  PooledRefCountedString(const char* str)
      : PooledRefCountedString(str, str ? strlen(str) : 0) {}

  // from std::string
  PooledRefCountedString(const std::string& str)
      : PooledRefCountedString(str.c_str(), str.size()) {}

  // copy constructor
  PooledRefCountedString(const PooledRefCountedString& other) {
    ptr_count_ = other.ptr_count_;
    size_ = other.size_;
    data_ = other.data_;
    (*ptr_count_)++;
  }

  // move constructor
  PooledRefCountedString(PooledRefCountedString&& other) noexcept {
    ptr_count_ = other.ptr_count_;
    size_ = other.size_;
    data_ = other.data_;
    other.ptr_count_ = nullptr;
    other.size_ = 0;
    other.data_ = nullptr;
  }

  ~PooledRefCountedString() {
    if (ptr_count_ && (--(*ptr_count_)) == 0) {
      pool.del(ptr_count_);
      delete[] data_;
    }
  }

  // assign from C string
  PooledRefCountedString& operator=(const char* str) {
    if (ptr_count_ && (--(*ptr_count_)) == 0) {
      delete[] data_;
      *ptr_count_ = 1;  // re-use ptr_count_
    } else {
      ptr_count_ = pool.add();
      *ptr_count_ = 1;
    }
    size_ = str ? strlen(str) : 0;
    data_ = new char[size_ + 1];
    memcpy(data_, str, size_);
    data_[size_] = 0;
    return *this;
  }

  // copy assignment
  PooledRefCountedString& operator=(const PooledRefCountedString& other) {
    if (this != &other) {
      if (ptr_count_ && (--(*ptr_count_)) == 0) {
        pool.del(ptr_count_);
        delete[] data_;
      }
      ptr_count_ = other.ptr_count_;
      size_ = other.size_;
      data_ = other.data_;
      (*ptr_count_)++;
    }
    return *this;
  }

  // move assignment
  PooledRefCountedString& operator=(PooledRefCountedString&& other) noexcept {
    if (this != &other) {
      if (ptr_count_ && (--(*ptr_count_)) == 0) {
        pool.del(ptr_count_);
        delete[] data_;
      }
      ptr_count_ = other.ptr_count_;
      size_ = other.size_;
      data_ = other.data_;
      other.ptr_count_ = nullptr;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }

  uint64_t size() const noexcept { return size_; }

  const char* c_str() const noexcept { return data_ ? data_ : ""; }

  bool operator==(const char* other) const { return strcmp(data_, other) == 0; }

  bool operator==(const PooledRefCountedString& other) const {
    return (size_ == other.size_ &&
            (size_ == 0 || (data_[0] == other.data_[0] &&
                            memcmp(data_, other.data_, size_) == 0)));
  }

  bool operator!=(const char* other) const { return !(*this == other); }

  bool operator!=(const PooledRefCountedString& other) const {
    return !(*this == other);
  }

  PooledRefCountedString substr(uint64_t pos, uint64_t len) const {
    if (pos >= size_) return PooledRefCountedString();
    uint64_t n = (len <= size_ - pos) ? len : (size_ - pos);
    return PooledRefCountedString(data_ + pos, n);
  }
};

std::ostream& operator<<(std::ostream& os, const PooledRefCountedString& s) {
  os << s.c_str();
  return os;
}
}  // namespace dbtoaster

#endif /* DBTOASTER_STRING_HPP */
