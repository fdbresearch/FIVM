#ifndef RINGPAIR_HPP
#define RINGPAIR_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

struct RingPair {
    long count;
    DOUBLE_TYPE v1;
    DOUBLE_TYPE v2;

    static RingPair zero;

    explicit RingPair() : count(0), v1(0.0), v2(0.0) { }

    explicit RingPair(long c, DOUBLE_TYPE a, DOUBLE_TYPE b) : count(c), v1(a), v2(b) { }

    inline bool isZero() const { return count == 0; }

    RingPair& operator+=(const RingPair &r) {
        this->count += r.count;
        this->v1 += r.v1;
        this->v2 += r.v2;
        return *this;
    }

    RingPair operator*(const RingPair &other) {
        return RingPair(count * other.count, v1 * other.v1, v2 * other.v2);
    }

    RingPair operator*(long alpha) {
        return RingPair(alpha * count, alpha * v1, alpha * v2);
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, v1);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, v2);
    }
};

RingPair operator*(long alpha, const RingPair &r) {
    return RingPair(alpha * r.count, alpha * r.v1, alpha * r.v2);
}

RingPair Ulift(DOUBLE_TYPE v1, DOUBLE_TYPE v2) {
    return RingPair(1, v1, v2);
}

RingPair RingPair::zero = RingPair();

#endif /* RINGPAIR_HPP */