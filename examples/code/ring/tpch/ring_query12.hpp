#ifndef RINGTPCH12_HPP
#define RINGTPCH12_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

const STRING_TYPE c1 = STRING_TYPE("1-URGENT");
const STRING_TYPE c2 = STRING_TYPE("2-HIGH");

struct TPCH12Payload {
  long count;
  long high_line_count;
  long low_line_count;

  explicit TPCH12Payload() : count(0), high_line_count(0), low_line_count(0) { }

  explicit TPCH12Payload(long c, const STRING_TYPE& order_priority) : count(c) {
    high_line_count = ((order_priority == c1) || (order_priority == c2));
    low_line_count = !((order_priority == c1) || (order_priority == c2));
  }

  inline bool isZero() const { return count == 0; }

  TPCH12Payload& operator+=(const TPCH12Payload& other) {
    if (other.isZero()) return *this;
    count += other.count;
    high_line_count += other.high_line_count;
    low_line_count += other.low_line_count;
    return *this;
  }

  TPCH12Payload operator*(const TPCH12Payload& other) const {
    if (isZero() || other.isZero()) return TPCH12Payload();

    TPCH12Payload r;
    r.count = count * other.count;
    r.high_line_count = count * other.high_line_count + other.count * high_line_count;
    r.low_line_count = count * other.low_line_count + other.count * low_line_count;
    return r;
  }

  TPCH12Payload operator*(long int alpha) const {
      if (alpha == 1L) return *this;
      return multiply(alpha);
  }

  TPCH12Payload multiply(long int alpha) const {
      TPCH12Payload r;
      r.count = alpha * count;
      r.high_line_count = alpha * high_line_count;
      r.low_line_count = alpha * low_line_count;
      return r;
  }

  FORCE_INLINE void clear() {
    count = 0L;
    high_line_count = 0L;
    low_line_count = 0L;
  }

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) const {
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, high_line_count);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, low_line_count);
  }
};

TPCH12Payload operator*(long int alpha, const TPCH12Payload& p) {
    if (alpha == 1L) return p;
    return p.multiply(alpha);
}

TPCH12Payload Ulift(const STRING_TYPE& order_priority) {
    return TPCH12Payload(1, order_priority);
}

#endif /* RINGTPCH12_HPP */