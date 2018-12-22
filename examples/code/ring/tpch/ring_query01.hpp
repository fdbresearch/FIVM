#ifndef RINGTPCH1_HPP
#define RINGTPCH1_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

struct TPCH1Payload {
  long count_order;
  DOUBLE_TYPE sum_qty;
  DOUBLE_TYPE sum_base_price;
  DOUBLE_TYPE sum_disc_price;
  DOUBLE_TYPE sum_charge;
  DOUBLE_TYPE sum_discount;
  DOUBLE_TYPE avg_qty;
  DOUBLE_TYPE avg_price;
  DOUBLE_TYPE avg_disc;

  explicit TPCH1Payload() : count_order(0),
                            sum_qty(0.0),
                            sum_base_price(0.0),
                            sum_disc_price(0.0),
                            sum_charge(0.0),
                            sum_discount(0.0),
                            avg_qty(0.0),
                            avg_price(0.0),
                            avg_disc(0.0) { }

  explicit TPCH1Payload(int _count_order, DOUBLE_TYPE _sum_qty,
                        DOUBLE_TYPE _sum_base_price, DOUBLE_TYPE _sum_disc_price,
                        DOUBLE_TYPE _sum_charge, DOUBLE_TYPE _sum_discount)
    : count_order(_count_order),
      sum_qty(_sum_qty),
      sum_base_price(_sum_base_price),
      sum_disc_price(_sum_disc_price),
      sum_charge(_sum_charge),
      sum_discount(_sum_discount) {

    avg_qty = (count_order != 0 ? sum_qty / count_order : 0.0);
    avg_price = (count_order != 0 ? sum_base_price / count_order : 0.0);
    avg_disc = (count_order != 0 ? sum_discount / count_order : 0.0);
  }

  inline bool isZero() const { return count_order == 0; }

  TPCH1Payload& operator+=(const TPCH1Payload& other) {
    if (other.isZero()) return *this;
    count_order += other.count_order;
    sum_qty += other.sum_qty;
    sum_base_price += other.sum_base_price;
    sum_disc_price += other.sum_disc_price;
    sum_charge += other.sum_charge;
    sum_discount += other.sum_discount;
    avg_qty = (count_order != 0 ? sum_qty / count_order : 0.0);
    avg_price = (count_order != 0 ? sum_base_price / count_order : 0.0);
    avg_disc = (count_order != 0 ? sum_discount / count_order : 0.0);
    return *this;
  }

  TPCH1Payload operator*(const TPCH1Payload& other) const {
    if (isZero() || other.isZero()) return TPCH1Payload();

    TPCH1Payload r;
    r.count_order = count_order * other.count_order;
    r.sum_qty = count_order * other.sum_qty + other.count_order * sum_qty;
    r.sum_base_price = count_order * other.sum_base_price + other.count_order * sum_base_price;
    r.sum_disc_price = count_order * other.sum_disc_price + other.count_order * sum_disc_price;
    r.sum_charge = count_order * other.sum_charge + other.count_order * sum_charge;
    r.sum_discount = count_order * other.sum_discount + other.count_order * sum_discount;
    r.avg_qty = (r.count_order != 0 ? r.sum_qty / r.count_order : 0.0);
    r.avg_price = (r.count_order != 0 ? r.sum_base_price / r.count_order : 0.0);
    r.avg_disc = (r.count_order != 0 ? r.sum_discount / r.count_order : 0.0);
    return r;
  }

  TPCH1Payload operator*(long int alpha) const {
      if (alpha == 1L) return *this;
      return multiply(alpha);
  }

  TPCH1Payload multiply(long int alpha) const {
      TPCH1Payload r;
      r.count_order = alpha * count_order;
      r.sum_qty = alpha * sum_qty;
      r.sum_base_price = alpha * sum_base_price;
      r.sum_disc_price = alpha * sum_disc_price;
      r.sum_charge = alpha * sum_charge;
      r.sum_discount = alpha * sum_discount;
      r.avg_qty = (r.count_order != 0 ? r.sum_qty / r.count_order : 0.0);
      r.avg_price = (r.count_order != 0 ? r.sum_base_price / r.count_order : 0.0);
      r.avg_disc = (r.count_order != 0 ? r.sum_discount / r.count_order : 0.0);
      return r;
  }

  FORCE_INLINE void clear() {
    count_order = 0;
    sum_qty = 0.0;
    sum_base_price = 0.0;
    sum_disc_price = 0.0;
    sum_charge = 0.0;
    sum_discount = 0.0;
    avg_qty = 0.0;
    avg_price = 0.0;
    avg_disc = 0.0;
  }

  template<class Archive>
  void serialize(Archive& ar, const unsigned int version) const {
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, sum_qty);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, sum_base_price);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, sum_disc_price);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, sum_charge);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, avg_qty);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, avg_price);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, avg_disc);
    ar << ELEM_SEPARATOR << "\t";
    DBT_SERIALIZATION_NVP(ar, count_order);
  }
};

TPCH1Payload operator*(long int alpha, const TPCH1Payload& p) {
    if (alpha == 1L) return p;
    return p.multiply(alpha);
}

TPCH1Payload Ulift(DOUBLE_TYPE sum_qty, DOUBLE_TYPE sum_base_price,
                   DOUBLE_TYPE sum_disc_price, DOUBLE_TYPE sum_charge,
                   DOUBLE_TYPE sum_disc) {
    return TPCH1Payload(1, sum_qty, sum_base_price, sum_disc_price, sum_charge, sum_disc);
}

#endif /* RINGTPCH1_HPP */