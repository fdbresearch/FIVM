#ifndef RINGRATIO_HPP
#define RINGRATIO_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template<int SCALE = 1>
struct RingRatio {
    int64_t count;
    double v1;
    double v2;

    static RingRatio zero;

    explicit RingRatio() : count(0), v1(0.0), v2(0.0) { }

    explicit RingRatio(int64_t c, double a, double b) : count(c), v1(a), v2(b) { }

    inline bool isZero() const { return count == 0; }

    RingRatio& operator+=(const RingRatio &r) {
        this->count += r.count;
        this->v1 += r.v1;
        this->v2 += r.v2;
        return *this;
    }

    RingRatio operator*(const RingRatio &other) {
        return RingRatio(count * other.count, v1 * other.v1, v2 * other.v2);
    }

    RingRatio operator*(long alpha) {
        return RingRatio(alpha * count, alpha * v1, alpha * v2);
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        double ratio = getResult();
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, ratio);
    }

    double getResult() const {
        return (v2 != 0.0) ? SCALE * (v1 / v2) : 0.0;
    }
};

template <int SCALE>
RingRatio<SCALE> operator*(long alpha, const RingRatio<SCALE> &r) {
    return RingRatio<SCALE>(alpha * r.count, alpha * r.v1, alpha * r.v2);
}

template <int SCALE>
RingRatio<SCALE> Ulift(double v1, double v2) {
    return RingRatio<SCALE>(1, v1, v2);
}

template <int SCALE>
RingRatio<SCALE> RingRatio<SCALE>::zero = RingRatio<SCALE>();

#endif /* RINGRATIO_HPP */