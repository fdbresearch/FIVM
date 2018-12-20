#ifndef RINGCOFACTOR_NAIVE_HPP
#define RINGCOFACTOR_NAIVE_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template<int SUM_SIZE>
struct CofactorGeneric {
    
    typedef CofactorGeneric<SUM_SIZE> Cofactor;

    int count;
    DOUBLE_TYPE sums[SUM_SIZE];
    DOUBLE_TYPE cofactors[SUM_SIZE][SUM_SIZE];

    static Cofactor zero;

    explicit CofactorGeneric() {
        count = 0;
        memset(sums, 0, SUM_SIZE * sizeof(DOUBLE_TYPE));
        memset(cofactors, 0, SUM_SIZE * SUM_SIZE * sizeof(DOUBLE_TYPE));
    }

    FORCE_INLINE Cofactor& operator +=(const Cofactor &obj) {
        count += obj.count;
        for (size_t i = 0; i < SUM_SIZE; i++) 
            sums[i] += obj.sums[i];
        for (size_t i = 0; i < SUM_SIZE; i++) 
            for (size_t j = 0; j < SUM_SIZE; j++) 
                cofactors[i][j] += obj.cofactors[i][j];
        return *this;
    }

    FORCE_INLINE bool operator ==(const Cofactor &obj) const {
        return (count == obj.count && sums == obj.sums && cofactors == obj.cofactors);
    }


    FORCE_INLINE bool operator !=(const Cofactor &obj) const {
        return !(count == obj.count && sums == obj.sums && cofactors == obj.cofactors);
    }

    FORCE_INLINE Cofactor operator * (const Cofactor &b) const {
        Cofactor res;
        res.count = count * b.count;        
        for (size_t i = 0; i < SUM_SIZE; i++) 
            res.sums[i] += count * b.sums[i] + b.count * sums[i];
        for (size_t i = 0; i < SUM_SIZE; i++) 
            for (size_t j = 0; j < SUM_SIZE; j++) 
                res.cofactors[i][j] += count * b.cofactors[i][j] +  
                                       b.count * cofactors[i][j] + 
                                       sums[i] * b.sums[j] + 
                                       b.sums[i] * sums[j];

        return res;
    }

    FORCE_INLINE Cofactor operator * (long int alpha) const {
        if (alpha == 1) return *this;
        Cofactor res;
        res.count = alpha * count;
        for (size_t i = 0; i < SUM_SIZE; i++) 
            res.sums[i] += alpha * sums[i];
        for (size_t i = 0; i < SUM_SIZE; i++) 
            for (size_t j = 0; j < SUM_SIZE; j++) 
                res.cofactors[i][j] += alpha * cofactors[i][j];
        return res; 
    }

    FORCE_INLINE void clear() { 
        count = 0;
        memset(sums, 0, SUM_SIZE * sizeof(DOUBLE_TYPE));
        memset(cofactors, 0, SUM_SIZE * SUM_SIZE * sizeof(DOUBLE_TYPE));
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sums, SUM_SIZE);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, (DOUBLE_TYPE*) cofactors, SUM_SIZE * SUM_SIZE);
    }
};

template<int SUM_SIZE>
CofactorGeneric<SUM_SIZE> CofactorGeneric<SUM_SIZE>::zero = CofactorGeneric<SUM_SIZE>();

// typedef CofactorGeneric<27> RingCofactor;
typedef CofactorGeneric<43> RingCofactor;

FORCE_INLINE RingCofactor operator*(long int alpha, const RingCofactor &c) {
    return c * alpha;
}

RingCofactor Ulift(DOUBLE_TYPE a, int idx) {
    RingCofactor res;
    res.count = 1;
    res.sums[idx] = a;
    res.cofactors[idx][idx] = a * a;
    return res;
}

RingCofactor Ulift2(DOUBLE_TYPE a1, DOUBLE_TYPE a2, int idx) {
    RingCofactor res;
    res.count = 1;
    res.sums[idx] = a1;
    res.sums[idx + 1] = a2;
    res.cofactors[idx][idx] = a1 * a1;
    res.cofactors[idx][idx + 1] = a1 * a2;
    res.cofactors[idx + 1][idx] = a2 * a1; 
    res.cofactors[idx + 1][idx + 1] = a2 * a2; 
    return res;
}

RingCofactor Ulift3(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, int idx) {
    RingCofactor res;
    res.count = 1;
    res.sums[idx] = a1;
    res.sums[idx + 1] = a2;
    res.sums[idx + 2] = a3;
    res.cofactors[idx][idx] = a1 * a1;
    res.cofactors[idx][idx + 1] = a1 * a2;
    res.cofactors[idx][idx + 2] = a1 * a3;
    res.cofactors[idx + 1][idx] = a2 * a1; 
    res.cofactors[idx + 1][idx + 1] = a2 * a2; 
    res.cofactors[idx + 1][idx + 2] = a2 * a3; 
    res.cofactors[idx + 2][idx] = a3 * a1; 
    res.cofactors[idx + 2][idx + 1] = a3 * a2; 
    res.cofactors[idx + 2][idx + 2] = a3 * a3; 
    return res;
}

RingCofactor Ulift4(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, int idx) {
    RingCofactor res;
    res.count = 1;
    res.sums[idx] = a1;
    res.sums[idx + 1] = a2;
    res.sums[idx + 2] = a3;
    res.sums[idx + 3] = a4;
    res.cofactors[idx][idx] = a1 * a1;
    res.cofactors[idx][idx + 1] = a1 * a2;
    res.cofactors[idx][idx + 2] = a1 * a3;
    res.cofactors[idx][idx + 3] = a1 * a4;
    res.cofactors[idx + 1][idx] = a2 * a1; 
    res.cofactors[idx + 1][idx + 1] = a2 * a2; 
    res.cofactors[idx + 1][idx + 2] = a2 * a3; 
    res.cofactors[idx + 1][idx + 3] = a2 * a4; 
    res.cofactors[idx + 2][idx] = a3 * a1; 
    res.cofactors[idx + 2][idx + 1] = a3 * a2; 
    res.cofactors[idx + 2][idx + 2] = a3 * a3; 
    res.cofactors[idx + 2][idx + 3] = a3 * a4; 
    res.cofactors[idx + 3][idx] = a4 * a1; 
    res.cofactors[idx + 3][idx + 1] = a4 * a2; 
    res.cofactors[idx + 3][idx + 2] = a4 * a3; 
    res.cofactors[idx + 3][idx + 3] = a4 * a4; 
    return res;
}

RingCofactor Ulift5(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, int idx) {
    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5 };

    RingCofactor res;
    res.count = 1;
    for (int i = 0; i < 5; i++)
        res.sums[idx + i] = arr[i];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            res.cofactors[idx + i][idx + j] = arr[i] * arr[j];
    return res;
}

RingCofactor Ulift6(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, int idx) {
    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6 };

    RingCofactor res;
    res.count = 1;
    for (int i = 0; i < 6; i++)
        res.sums[idx + i] = arr[i];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            res.cofactors[idx + i][idx + j] = arr[i] * arr[j];
    return res;
}

RingCofactor Ulift10(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, int idx) {

    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 };
    RingCofactor res;
    res.count = 1;
    for (int i = 0; i < 10; i++)
        res.sums[idx + i] = arr[i];
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            res.cofactors[idx + i][idx + j] = arr[i] * arr[j];
    return res;
}

RingCofactor Ulift13(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, 
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, int idx) {

    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13 };
    RingCofactor res;
    res.count = 1;
    for (int i = 0; i < 13; i++)
        res.sums[idx + i] = arr[i];
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 13; j++)
            res.cofactors[idx + i][idx + j] = arr[i] * arr[j];
    return res;
}

RingCofactor Ulift15(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10,
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, DOUBLE_TYPE a14, DOUBLE_TYPE a15, int idx) {

    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 };
    RingCofactor res;
    res.count = 1;
    for (int i = 0; i < 15; i++)
        res.sums[idx + i] = arr[i];
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            res.cofactors[idx + i][idx + j] = arr[i] * arr[j];
    return res;
}
#endif /* RINGCOFACTOR_NAIVE_HPP */