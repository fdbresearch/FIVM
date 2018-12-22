#ifndef RINGTPCH14_HPP
#define RINGTPCH14_HPP

#include "types.hpp"
#include "serialization.hpp"
#include "functions.hpp"

using namespace dbtoaster;

struct TPCH14Regex {
  regex_t preg1;

  TPCH14Regex() {
    if (regcomp(&preg1, "^PROMO.*$", REG_EXTENDED | REG_NOSUB)) {
      cerr << "Error compiling regular expression: /^PROMO.*$/" << endl;
      exit(-1);
    }
  }
  ~TPCH14Regex() { regfree(&preg1); }
};

struct TPCH14Payload {
  static const TPCH14Regex regex;

  long count;
  DOUBLE_TYPE promo_revenue;
  DOUBLE_TYPE total_revenue;
  DOUBLE_TYPE percent_revenue;

  explicit TPCH14Payload()
      : count(0), promo_revenue(0.0), total_revenue(0.0), percent_revenue(0.0) { }

  explicit TPCH14Payload(long c, DOUBLE_TYPE p, DOUBLE_TYPE t)
      : count(c), promo_revenue(p), total_revenue(t) {
    percent_revenue = (total_revenue != 0.0 ? 100.0 * promo_revenue / total_revenue : 0.0);
  }

  inline bool isZero() const { return count == 0; }

  TPCH14Payload& operator+=(const TPCH14Payload& other) {
    if (other.isZero()) return *this;
    count += other.count;
    promo_revenue += other.promo_revenue;
    total_revenue += other.total_revenue;
    percent_revenue = (total_revenue != 0.0 ? 100.0 * promo_revenue / total_revenue : 0.0);
    return *this;
  }

  TPCH14Payload operator*(const TPCH14Payload& other) const {
    if (isZero() || other.isZero()) return TPCH14Payload();

    TPCH14Payload r;
    r.count = count * other.count;
    r.promo_revenue = promo_revenue * other.promo_revenue;
    r.total_revenue = total_revenue * other.total_revenue;
    r.percent_revenue = (r.total_revenue != 0.0 ? 100.0 * r.promo_revenue / r.total_revenue : 0.0);
    return r;
  }

  TPCH14Payload operator*(long int alpha) const {
      if (alpha == 1L) return *this;
      return multiply(alpha);
  }

  TPCH14Payload multiply(long int alpha) const {
      TPCH14Payload r;
      r.count = alpha * count;
      r.promo_revenue = alpha * promo_revenue;
      r.total_revenue = alpha * total_revenue;
      r.percent_revenue = percent_revenue;
      return r;
  }

  FORCE_INLINE void clear() {
    count = 0L;
    promo_revenue = 0.0;
    total_revenue = 0.0;
    percent_revenue = 0.0;
  }

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) const {
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, promo_revenue);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, total_revenue);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, percent_revenue);
  }
};

const TPCH14Regex TPCH14Payload::regex = TPCH14Regex();

TPCH14Payload operator*(long int alpha, const TPCH14Payload& p) {
    if (alpha == 1L) return p;
    return p.multiply(alpha);
}

TPCH14Payload Uliftpart(const STRING_TYPE& p_type) {
    return TPCH14Payload(1, Upreg_match(TPCH14Payload::regex.preg1, p_type) != 0, 1.0);
}

TPCH14Payload Uliftlineitem(DOUBLE_TYPE revenue) {
    return TPCH14Payload(1, revenue, revenue);
}

#endif /* RINGTPCH14_HPP */