#ifndef RINGCOFACTOR_HPP
#define RINGCOFACTOR_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename T, size_t SZ>
struct RingCofactor {
    static constexpr size_t MSZ = (SZ + 1) * SZ / 2;
    static constexpr size_t ZERO_IDX = 0xffffffff;
    static const RingCofactor<T, SZ> zero;
   
    size_t idx;
    int count;
    T sums[SZ];
    T cofactors[MSZ];

    explicit RingCofactor() : idx(ZERO_IDX), count(0) {
        memset(sums, 0, SZ * sizeof(T));
        memset(cofactors, 0, MSZ * sizeof(T));
    }

    explicit RingCofactor(size_t idx_) : idx(idx_), count(0) { }
    
    template <typename... Args>
    explicit RingCofactor(size_t idx_, Args&&... args) : idx(idx_), count(1), sums{ args... } {
        static_assert(SZ == sizeof...(args), "Incompatible array sizes");
        T* out = cofactors;
        for (size_t i = 0; i < SZ; i++)
            for (size_t j = i; j < SZ; j++)
                *out++ = sums[i] * sums[j];
    }

    inline bool operator==(const RingCofactor& other) const {
        return idx == other.idx;        // shallow test
    }

    inline bool isZero() const { return idx == ZERO_IDX; }

    RingCofactor& operator+=(const RingCofactor& other) {
        if (other.isZero()) return *this;
        assert(idx == ZERO_IDX || idx == other.idx);
        idx &= other.idx;
        count += other.count;
        for (size_t i = 0; i < SZ; i++)
            sums[i] += other.sums[i];
        for (size_t i = 0; i < MSZ; i++)
            cofactors[i] += other.cofactors[i];
        return *this;
    }

    template <size_t SZ2>
    RingCofactor<T, SZ + SZ2> operator*(const RingCofactor<T, SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<T, SZ + SZ2>::zero;
        if (other.idx > idx) return other.multiply(*this);
        return this->multiply(other);
    }

    template <size_t SZ2>
    RingCofactor<T, SZ + SZ2> multiply(const RingCofactor<T, SZ2>& other) const {
        assert((other.idx + SZ2) == idx);
        RingCofactor<T, SZ + SZ2> r(other.idx);
        r.count = other.count * count;
        
        T* out = r.sums;
        for (size_t i = 0; i < SZ2; i++) {
            *out++ = count * other.sums[i];
        }
        for (size_t i = 0; i < SZ; i++) {
            *out++ = other.count * sums[i];
        }

        const T* in = other.cofactors;
        out = r.cofactors;
        for (size_t i = 0; i < SZ2; i++) {
            for (size_t j = i; j < SZ2; j++) {
                *out++ = *in++ * count;
            }
            for (size_t j = 0; j < SZ; j++) {
                *out++ = other.sums[i] * sums[j];
            }
        }
        in = cofactors;
        for (size_t i = 0; i < MSZ; i++) {
            *out++ = *in++ * other.count;
        }
        
        return r;
    }

    RingCofactor operator*(long int alpha) const {        
        if (alpha == 1L) return *this;
        return multiply(alpha); 
    }

    RingCofactor multiply(long int alpha) const {
        RingCofactor<T, SZ> r(idx);
        r.count = alpha * count;
        for (size_t i = 0; i < SZ; i++) 
            r.sums[i] = alpha * sums[i];
        for (size_t i = 0; i < MSZ; i++) 
            r.cofactors[i] = alpha * cofactors[i];
        return r; 
    }

    void clear() {
        idx = ZERO_IDX;
        count = 0;
        memset(sums, 0, SZ * sizeof(T));
        memset(cofactors, 0, MSZ * sizeof(T));
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, idx);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sums, SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, (T*) cofactors, MSZ);
    }
};

template <typename T, size_t SZ>
const RingCofactor<T, SZ> RingCofactor<T, SZ>::zero = RingCofactor<T, SZ>();

template <typename T, size_t SZ>
RingCofactor<T, SZ> operator*(long int alpha, const RingCofactor<T, SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <typename T>
FORCE_INLINE DOUBLE_TYPE convert(T x) { return x; }

template <typename... Args>
RingCofactor<DOUBLE_TYPE, sizeof...(Args)> Ulift(size_t idx, Args&&... args) {
    return RingCofactor<DOUBLE_TYPE, sizeof...(args)>(idx, convert(args)...);
}

#endif /* RINGCOFACTOR_HPP */