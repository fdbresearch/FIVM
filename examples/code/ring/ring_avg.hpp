#ifndef RINGAVG_HPP
#define RINGAVG_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

struct RingAvg {
    int count;
    DOUBLE_TYPE sum;

    static RingAvg zero;

    explicit RingAvg() : count(0), sum(0.0) { }

    explicit RingAvg(int c, DOUBLE_TYPE s) : count(c), sum(s) { }

    inline bool isZero() const { return count == 0; }

    RingAvg& operator+=(const RingAvg &r) {
        this->count += r.count;
        this->sum += r.sum;
        return *this;
    }

    RingAvg operator*(const RingAvg &other) {
        return RingAvg(count * other.count, sum * other.count + other.sum * count);
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum);
    }
};

RingAvg operator*(int alpha, const RingAvg &r) {
    return RingAvg(alpha * r.count, alpha * r.sum);
}

RingAvg Ulift(DOUBLE_TYPE a) {
    return RingAvg(1, a);
}

#endif /* RINGAVG_HPP */