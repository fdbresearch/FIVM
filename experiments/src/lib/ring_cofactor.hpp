#ifndef RINGCOFACTOR_HPP
#define RINGCOFACTOR_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename T, size_t SZ>
struct CofactorGeneric {
    static constexpr size_t MSZ = (SZ + 1) * SZ / 2;
    static constexpr size_t ZERO_IDX = 0xffffffff;
    static const CofactorGeneric<T, SZ> zero;
   
    size_t idx;
    int count;
    T sums[SZ];
    T cofactors[MSZ];

    explicit CofactorGeneric() : idx(ZERO_IDX), count(0) {
        memset(sums, 0, SZ * sizeof(T));
        memset(cofactors, 0, MSZ * sizeof(T));
    }

    explicit CofactorGeneric(size_t idx_) : idx(idx_), count(0) { }
    
    template <typename... Args>
    explicit CofactorGeneric(size_t idx_, Args&&... args) : idx(idx_), count(1), sums{ args... } {
        static_assert(SZ == sizeof...(args), "Incompatible array sizes");
        T* out = cofactors;
        for (size_t i = 0; i < SZ; i++)
            for (size_t j = i; j < SZ; j++)
                *out++ = sums[i] * sums[j];
    }

    CofactorGeneric& operator+=(const CofactorGeneric& other) {
        assert(idx == ZERO_IDX || idx == other.idx);
        idx &= other.idx;
        count += other.count;
        for (size_t i = 0; i < SZ; i++)
            sums[i] += other.sums[i];
        for (size_t i = 0; i < MSZ; i++)
            cofactors[i] += other.cofactors[i];
        return *this;
    }

    bool operator==(const CofactorGeneric& other) const {
        return idx == other.idx;
    }

    bool operator!=(const CofactorGeneric& other) const {
        return idx != other.idx;
    }

    bool isZero() const { return idx == ZERO_IDX; }

    template <size_t SZ2>
    CofactorGeneric<T, SZ + SZ2> operator*(const CofactorGeneric<T, SZ2>& other) const {
        if (isZero() || other.isZero()) return CofactorGeneric<T, SZ + SZ2>::zero;
        if (other.idx > idx) return other.multiply(*this);
        return this->multiply(other);
    }

    template <size_t SZ2>
    CofactorGeneric<T, SZ + SZ2> multiply(const CofactorGeneric<T, SZ2>& other) const {
        CofactorGeneric<T, SZ + SZ2> r(other.idx);
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

    CofactorGeneric operator*(long int alpha) const {        
        if (alpha == 1L) return *this;
        return multiply(alpha); 
    }

    CofactorGeneric multiply(long int alpha) const {
        CofactorGeneric<T, SZ> r(idx);
        r.count = alpha * count;
        for (size_t i = 0; i < SZ; i++) 
            r.sums[i] = alpha * sums[i];
        for (size_t i = 0; i < MSZ; i++) 
            r.cofactors[i] = alpha * cofactors[i];
        return r; 
    }

    void clear() {
        idx = 0;
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
const CofactorGeneric<T, SZ> CofactorGeneric<T, SZ>::zero = CofactorGeneric<T, SZ>();

template <typename T, size_t SZ>
CofactorGeneric<T, SZ> operator*(long int alpha, const CofactorGeneric<T, SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <typename T>
FORCE_INLINE DOUBLE_TYPE convert(T x) { return x; }

template <typename... Args>
CofactorGeneric<DOUBLE_TYPE, sizeof...(Args)> Ulift(size_t idx, Args&&... args) {
    return CofactorGeneric<DOUBLE_TYPE, sizeof...(args)>(idx, convert(args)...);
}

typedef CofactorGeneric<DOUBLE_TYPE, 1> RingCofactor1;
typedef CofactorGeneric<DOUBLE_TYPE, 2> RingCofactor2;
typedef CofactorGeneric<DOUBLE_TYPE, 3> RingCofactor3;
typedef CofactorGeneric<DOUBLE_TYPE, 4> RingCofactor4;
typedef CofactorGeneric<DOUBLE_TYPE, 5> RingCofactor5;
typedef CofactorGeneric<DOUBLE_TYPE, 6> RingCofactor6;
typedef CofactorGeneric<DOUBLE_TYPE, 7> RingCofactor7;
typedef CofactorGeneric<DOUBLE_TYPE, 8> RingCofactor8;
typedef CofactorGeneric<DOUBLE_TYPE, 9> RingCofactor9;
typedef CofactorGeneric<DOUBLE_TYPE, 10> RingCofactor10;
typedef CofactorGeneric<DOUBLE_TYPE, 11> RingCofactor11;
typedef CofactorGeneric<DOUBLE_TYPE, 12> RingCofactor12;
typedef CofactorGeneric<DOUBLE_TYPE, 13> RingCofactor13;
typedef CofactorGeneric<DOUBLE_TYPE, 14> RingCofactor14;
typedef CofactorGeneric<DOUBLE_TYPE, 15> RingCofactor15;
typedef CofactorGeneric<DOUBLE_TYPE, 27> RingCofactor27;
typedef CofactorGeneric<DOUBLE_TYPE, 29> RingCofactor29;
typedef CofactorGeneric<DOUBLE_TYPE, 43> RingCofactor43;

#endif /* RINGCOFACTOR_HPP */