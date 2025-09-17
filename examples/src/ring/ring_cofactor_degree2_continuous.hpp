#ifndef RING_COFACTOR_DEGREE2_CONTINUOUS_HPP
#define RING_COFACTOR_DEGREE2_CONTINUOUS_HPP

#include <array>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

constexpr size_t array_size_per_degree(size_t num_vars, size_t deg) {
    return
        (deg == 0 || num_vars == 0) ? 1 :
            (num_vars == 1 ? 1 :
                array_size_per_degree(num_vars - 1, deg) + array_size_per_degree(num_vars, deg - 1));
}

constexpr size_t array_size(size_t num_vars, size_t deg) {
    return
        (deg == 0) ? array_size_per_degree(num_vars, deg) :
            array_size_per_degree(num_vars, deg) + array_size(num_vars, deg - 1);
}

template <size_t IDX, typename T, size_t SZ>
struct RingCofactor {

    static constexpr size_t ARRAY_SZ = array_size(SZ, 4) - 1;

    int count;
    std::array<T, ARRAY_SZ> arr;

    explicit RingCofactor() : count(0) {
        memset(arr.data(), 0, ARRAY_SZ * sizeof(T));
    }

    explicit RingCofactor(T&& t) :  count(1), arr { t, t * t, t * t * t, t * t * t * t } {
        static_assert(SZ == 1, "Incompatible array sizes");
    }

    template <typename... Args>
    explicit RingCofactor(Args&&... args) : count(1), arr { args... } {
        static_assert(SZ == sizeof...(args), "Incompatible array sizes");

        T *out2 = arr.data() + array_size_per_degree(SZ, 1);
        T *out3 = out2 + array_size_per_degree(SZ, 2);
        T *out4 = out3 + array_size_per_degree(SZ, 3);

        for (size_t i = 0; i < SZ; i++) {
            for (size_t j = i; j < SZ; j++) {
                *out2++ = arr[i] * arr[j];
                for (size_t k = j; k < SZ; k++) {
                    *out3++ = arr[i] * arr[j] * arr[k];
                    for (size_t l = k; l < SZ; l++) {
                        *out4++ = arr[i] * arr[j] * arr[k] * arr[l];
                    }
                }
            }
        }
    }

    inline bool isZero() const { return count == 0; }

    RingCofactor& operator+=(const RingCofactor& other) {
        if (other.isZero()) return *this;
        count += other.count;
        for (size_t i = 0; i < ARRAY_SZ; i++) {
            arr[i] += other.arr[i];
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

        // OMITTED MULTIPLICATION

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
        for (size_t i = 0; i < ARRAY_SZ; i++) {
            r.arr[i] = alpha * arr[i];
        }
        return r;
    }

    void clear() {
        count = 0;
        memset(arr.data(), 0, ARRAY_SZ * sizeof(T));
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, IDX);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, arr.data(), ARRAY_SZ);
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

#endif /* RING_COFACTOR_DEGREE2_CONTINUOUS_HPP */