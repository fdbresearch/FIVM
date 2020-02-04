#ifndef RINGCOFACTOR_DEGREE2_HPP
#define RINGCOFACTOR_DEGREE2_HPP

#include <array>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename T, size_t IDX, size_t SZ>
struct RingCofactor {
    static constexpr size_t DEG2_SZ = (SZ - 1) * SZ / 2;
    static constexpr size_t DEG3_SZ = (SZ - 1) * SZ;
    static constexpr size_t DEG4_SZ = 3 * (SZ - 1) * SZ / 2;

    int count;
    std::array<T, SZ> sum1;
    std::array<T, SZ> sum2;
    std::array<T, SZ> sum3;
    std::array<T, SZ> sum4;
    std::array<T, DEG2_SZ> degree2;
    std::array<T, DEG3_SZ> degree3;
    std::array<T, DEG4_SZ> degree4;

    explicit RingCofactor() : count(0) {
        memset(sum1.data(), 0, SZ * sizeof(T));
        memset(sum2.data(), 0, SZ * sizeof(T));
        memset(sum3.data(), 0, SZ * sizeof(T));
        memset(sum4.data(), 0, SZ * sizeof(T));
        memset(degree2.data(), 0, DEG2_SZ * sizeof(T));
        memset(degree3.data(), 0, DEG3_SZ * sizeof(T));
        memset(degree4.data(), 0, DEG4_SZ * sizeof(T));
    }

    explicit RingCofactor(T&& t) :  count(1),
                                    sum1 { t },
                                    sum2 { t * t },
                                    sum3 { t * t * t },
                                    sum4 { t * t * t * t } {
        static_assert(SZ == 1, "Incompatible array sizes");
    }

    template <typename... Args>
    explicit RingCofactor(Args&&... args) : count(1), sum1 { args... } {
        static_assert(SZ == sizeof...(args), "Incompatible array sizes");

        T *out2 = degree2.data();
        T *out3 = degree3.data();
        T *out4 = degree4.data();
        for (size_t i = 0; i < SZ; i++) {
            sum2[i] = sum1[i] * sum1[i];
            sum3[i] = sum2[i] * sum1[i];
            sum4[i] = sum3[i] * sum1[i];

            for (size_t j = i + 1; j < SZ; j++) {
                *out2++ = sum1[i] * sum1[j];
                *out3++ = sum1[i] * sum2[j];
                *out3++ = sum2[i] * sum1[j];
                *out4++ = sum1[i] * sum3[j];
                *out4++ = sum2[i] * sum2[j];
                *out4++ = sum3[i] * sum1[j];
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
            sum3[i] += other.sum3[i];
            sum4[i] += other.sum4[i];
        }
        for (size_t i = 0; i < DEG2_SZ; i++) {
            degree2[i] += other.degree2[i];
        }
        for (size_t i = 0; i < DEG3_SZ; i++) {
            degree3[i] += other.degree3[i];
        }
        for (size_t i = 0; i < DEG4_SZ; i++) {
            degree4[i] += other.degree4[i];
        }
        return *this;
    }

    template <size_t IDX2, size_t SZ2>
    typename std::enable_if<(IDX < IDX2), RingCofactor<T, IDX, SZ + SZ2>>::type
    operator*(const RingCofactor<T, IDX2, SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<T, IDX, SZ + SZ2>();
        return this->multiply(other);
    }

    template <size_t IDX2, size_t SZ2>
    typename std::enable_if<(IDX > IDX2), RingCofactor<T, IDX2, SZ + SZ2>>::type
    operator*(const RingCofactor<T, IDX2, SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<T, IDX2, SZ + SZ2>();
        return other.multiply(*this);
    }

    template <size_t IDX2, size_t SZ2>
    RingCofactor<T, IDX, SZ + SZ2> multiply(const RingCofactor<T, IDX2, SZ2>& other) const {
        static_assert(IDX + SZ == IDX2, "Cofactor sizes do not match");

        RingCofactor<T, IDX, SZ + SZ2> r;
        r.count = other.count * count;

        for (size_t i = 0; i < SZ; i++) {
            r.sum1[i] = other.count * sum1[i];
            r.sum2[i] = other.count * sum2[i];
            r.sum3[i] = other.count * sum3[i];
            r.sum4[i] = other.count * sum4[i];
        }

        for (size_t i = 0, j = SZ; i < SZ2; i++, j++) {
            r.sum1[j] = count * other.sum1[i];
            r.sum2[j] = count * other.sum2[i];
            r.sum3[j] = count * other.sum3[i];
            r.sum4[j] = count * other.sum4[i];
        }

        T *out2 = r.degree2.data();
        for (size_t i = 0; i < DEG2_SZ; i++) {
            *out2++ = degree2[i] * other.count;
        }
        for (size_t i = 0; i < RingCofactor<T, IDX2, SZ2>::DEG2_SZ; i++) {
            *out2++ = count * other.degree2[i];
        }

        T *out3 = r.degree3.data();
        for (size_t i = 0; i < DEG3_SZ; i++) {
            *out3++ = degree3[i] * other.count;
        }
        for (size_t i = 0; i < RingCofactor<T, IDX2, SZ2>::DEG3_SZ; i++) {
            *out3++ = count * other.degree3[i];
        }

        T *out4 = r.degree4.data();
        for (size_t i = 0; i < DEG4_SZ; i++) {
            *out4++ = degree4[i] * other.count;
        }
        for (size_t i = 0; i < RingCofactor<T, IDX2, SZ2>::DEG4_SZ; i++) {
            *out4++ = count * other.degree4[i];
        }

        for (size_t i = 0; i < SZ; i++) {
            for (size_t j = 0; j < SZ2; j++) {
                *out2++ = sum1[i] * other.sum1[j];

                *out3++ = sum2[i] * other.sum1[j];
                *out3++ = sum1[i] * other.sum2[j];

                *out4++ = sum3[i] * other.sum1[j];
                *out4++ = sum2[i] * other.sum2[j];
                *out4++ = sum1[i] * other.sum3[j];
            }
        }
        return r;
    }

    RingCofactor operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    RingCofactor multiply(long alpha) const {
        RingCofactor<T, IDX, SZ> r;
        if (alpha == 0L) return r;

        r.count = alpha * count;
        for (size_t i = 0; i < SZ; i++) {
            r.sum1[i] = alpha * sum1[i];
            r.sum2[i] = alpha * sum2[i];
            r.sum3[i] = alpha * sum3[i];
            r.sum4[i] = alpha * sum4[i];
        }
        for (size_t i = 0; i < DEG2_SZ; i++) {
            r.degree2[i] = alpha * degree2[i];
        }
        for (size_t i = 0; i < DEG3_SZ; i++) {
            r.degree3[i] = alpha * degree3[i];
        }
        for (size_t i = 0; i < DEG4_SZ; i++) {
            r.degree4[i] = alpha * degree4[i];
        }
        return r;
    }

    void clear() {
        count = 0;
        memset(sum1.data(), 0, SZ * sizeof(T));
        memset(sum2.data(), 0, SZ * sizeof(T));
        memset(sum3.data(), 0, SZ * sizeof(T));
        memset(sum4.data(), 0, SZ * sizeof(T));
        memset(degree2.data(), 0, DEG2_SZ * sizeof(T));
        memset(degree3.data(), 0, DEG3_SZ * sizeof(T));
        memset(degree4.data(), 0, DEG4_SZ * sizeof(T));
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
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sum3.data(), SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sum4.data(), SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, degree2.data(), DEG2_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, degree3.data(), DEG3_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, degree4.data(), DEG4_SZ);
    }
};

template <typename T, size_t IDX, size_t SZ>
RingCofactor<T, IDX, SZ> operator*(long int alpha, const RingCofactor<T, IDX, SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <typename T>
FORCE_INLINE DOUBLE_TYPE convert(T x) { return x; }

template <size_t IDX, typename... Args>
RingCofactor<DOUBLE_TYPE, IDX, sizeof...(Args)> Ulift(Args&&... args) {
    return RingCofactor<DOUBLE_TYPE, IDX, sizeof...(args)>(convert(args)...);
}

#endif /* RINGCOFACTOR_DEGREE2_HPP */