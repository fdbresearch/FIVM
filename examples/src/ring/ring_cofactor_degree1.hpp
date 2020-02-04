#ifndef RINGCOFACTOR_HPP
#define RINGCOFACTOR_HPP

#include <array>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <size_t IDX, typename T, size_t SZ>
struct RingCofactor {
    static constexpr size_t DEG2_SZ = (SZ - 1) * SZ / 2;

    int count;
    std::array<T, SZ> sum1;
    std::array<T, SZ> sum2;
    std::array<T, DEG2_SZ> degree2;

    explicit RingCofactor() : count(0) {
        memset(sum1.data(), 0, SZ * sizeof(T));
        memset(sum2.data(), 0, SZ * sizeof(T));
        memset(degree2.data(), 0, DEG2_SZ * sizeof(T));
    }

    explicit RingCofactor(T&& t) :  count(1),
                                    sum1 { t },
                                    sum2 { t * t } {
        static_assert(SZ == 1, "Incompatible array sizes");
    }

    template <typename... Args>
    explicit RingCofactor(Args&&... args) : count(1), sum1 { args... } {
        static_assert(SZ == sizeof...(args), "Incompatible array sizes");

        T *out2 = degree2.data();
        for (size_t i = 0; i < SZ; i++) {
            sum2[i] = sum1[i] * sum1[i];

            for (size_t j = i + 1; j < SZ; j++) {
                *out2++ = sum1[i] * sum1[j];
            }
        }
    }

    inline bool isZero() const { return count == 0; }

    RingCofactor& operator+=(const RingCofactor& other) {
        if (other.isZero()) return *this;
        count += other.count;
        for (size_t i = 0; i < SZ; i++) {
            sum1[i] += other.sum1[i];
            sum2[i] += other.sum2[i];
        }
        for (size_t i = 0; i < DEG2_SZ; i++) {
            degree2[i] += other.degree2[i];
        }
        return *this;
    }

    template <size_t IDX2, size_t SZ2>
    typename std::enable_if<(IDX < IDX2), RingCofactor<IDX, T, SZ + SZ2>>::type
    operator*(const RingCofactor<IDX2, T, SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX, T, SZ + SZ2>();
        return this->multiply(other);
    }

    template <size_t IDX2, size_t SZ2>
    typename std::enable_if<(IDX > IDX2), RingCofactor<IDX2, T, SZ + SZ2>>::type
    operator*(const RingCofactor<IDX2, T, SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX2, T, SZ + SZ2>();
        return other.multiply(*this);
    }

    template <size_t IDX2, size_t SZ2>
    RingCofactor<IDX, T, SZ + SZ2> multiply(const RingCofactor<IDX2, T, SZ2>& other) const {
        static_assert(IDX + SZ == IDX2, "Cofactor sizes do not match");

        RingCofactor<IDX, T, SZ + SZ2> r;
        r.count = other.count * count;

        for (size_t i = 0; i < SZ; i++) {
            r.sum1[i] = other.count * sum1[i];
            r.sum2[i] = other.count * sum2[i];
        }

        for (size_t i = 0, j = SZ; i < SZ2; i++, j++) {
            r.sum1[j] = count * other.sum1[i];
            r.sum2[j] = count * other.sum2[i];
        }

        const T *in1 = degree2.data();
        T *out2 = r.degree2.data();
        for (size_t i = 0; i < SZ; i++) {
            for (size_t j = i + 1; j < SZ; j++) {
                *out2++ = other.count * (*in1++);
            }
            for (size_t j = 0; j < SZ2; j++) {
                *out2++ = sum1[i] * other.sum1[j];
            }
        }
        for (size_t i = 0; i < RingCofactor<IDX2, T, SZ2>::DEG2_SZ; i++) {
            *out2++ = count * other.degree2[i];
        }

        return r;
    }

    RingCofactor operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    RingCofactor multiply(long alpha) const {
        RingCofactor<IDX, T, SZ> r;
        if (alpha == 0L) return r;

        r.count = alpha * count;
        for (size_t i = 0; i < SZ; i++) {
            r.sum1[i] = alpha * sum1[i];
            r.sum2[i] = alpha * sum2[i];
        }
        for (size_t i = 0; i < DEG2_SZ; i++) {
            r.degree2[i] = alpha * degree2[i];
        }
        return r;
    }

    void clear() {
        count = 0;
        memset(sum1.data(), 0, SZ * sizeof(T));
        memset(sum2.data(), 0, SZ * sizeof(T));
        memset(degree2.data(), 0, DEG2_SZ * sizeof(T));
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, IDX);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sum1.data(), SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sum2.data(), SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, degree2.data(), DEG2_SZ);
    }
};

template <size_t IDX, typename T, size_t SZ>
RingCofactor<IDX, T, SZ> operator*(long int alpha, const RingCofactor<IDX, T, SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <typename T>
FORCE_INLINE DOUBLE_TYPE convert(T x) { return x; }

template <size_t IDX, typename... Args>
RingCofactor<IDX, DOUBLE_TYPE, sizeof...(Args)> Ulift(Args&&... args) {
    return RingCofactor<IDX, DOUBLE_TYPE, sizeof...(args)>(convert(args)...);
}

#endif /* RINGCOFACTOR_HPP */