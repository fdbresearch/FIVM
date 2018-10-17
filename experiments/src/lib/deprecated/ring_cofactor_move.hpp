#include "types.hpp"
#include "serialization.hpp"
#include <memory>

using namespace std;
using namespace dbtoaster;

template<typename T>
struct CofactorDynamic {
public:
    uint32_t size;
    uint32_t idx;
    long count;
    unique_ptr<T[]> sums;
    unique_ptr<T[]> cofactors;

    static CofactorDynamic zero;

    explicit CofactorDynamic() 
        : size(0), idx(0), count(0), sums(nullptr), cofactors(nullptr) { }

    explicit CofactorDynamic(uint32_t sz, uint32_t ind)
        : size(sz), idx(ind), count(0), sums(new T[sz]()), cofactors(new T[sz * sz]()) { }

   //  // move constructor
   //  CofactorDynamic(CofactorDynamic&& other)
   //      : size(other.size), idx(other.idx), count(other.count)
   //      , sums(std::move(other.sums)), cofactors(std::move(other.cofactors)) { }

   // // move assignment operator
   // CofactorDynamic& operator=(CofactorDynamic&& other) {
   //      // std::cout << "Move assignment" << std::endl;
   //      size = other.size;
   //      idx = other.idx;
   //      count = other.count;
   //      sums = unique_ptr<T[]>(std::move(other.sums));
   //      cofactors = unique_ptr<T[]>(std::move(other.cofactors));
   //      return *this;
   //  }

    CofactorDynamic(CofactorDynamic&& other) = default;

    CofactorDynamic& operator=(CofactorDynamic&& other) = default;

    // copy constructor
    CofactorDynamic(const CofactorDynamic& other)
        : size(other.size), idx(other.idx), count(other.count)
        , sums(new T[other.size]), cofactors(new T[other.size * other.size]()) {
        
        // std::cout << "Copy constructor " << other.size << std::endl;
        memcpy(sums.get(), other.sums.get(), size * sizeof(T));
        memcpy(cofactors.get(), other.cofactors.get(), size * size * sizeof(T));
    }

    // copy assignment
    CofactorDynamic& operator=(const CofactorDynamic& other) {
        // std::cout << "Copy assignment " << other.size << std::endl;
        size = other.size;
        idx = other.idx;
        count = other.count;
        sums = unique_ptr<T[]>(new T[size]);
        cofactors = unique_ptr<T[]>(new T[size * size]);
        memcpy(sums.get(), other.sums.get(), size * sizeof(T));
        memcpy(cofactors.get(), other.cofactors.get(), size * size * sizeof(T));
        return *this;
    }  

    CofactorDynamic& operator+=(CofactorDynamic&& other) {
        if (other == zero) return *this;

        if (size == 0) {
            return this->operator=(std::move(other));
        }
        if (idx != other.idx || size != other.size) {
            return this->expandThenAdd(other);
        }
        return this->add(other);
    }

    CofactorDynamic& operator+=(const CofactorDynamic& other) {
        if (other == zero) return *this;

        if (size == 0) {
            return this->operator=(other);
        }
        if (idx != other.idx || size != other.size) {
            return this->expandThenAdd(other);
        }
        return this->add(other);
    }    

    CofactorDynamic& expandThenAdd(const CofactorDynamic& other) {
        auto begin = std::min(idx, other.idx);
        auto end = std::max(idx + size, other.idx + other.size);
        auto sz = end - begin;
        auto nsums = new T[sz]();
        auto ncofactors = new T[sz * sz]();

        // Count
        count += other.count;
        // Sum terms        
        auto offset = idx - begin;
        auto offsetOther = other.idx - begin;
        for (size_t i = 0; i < size; i++)
            nsums[i + offset] += sums[i];
        for (size_t i = 0; i < other.size; i++)
            nsums[i + offsetOther] += other.sums[i];
        // Cofactor terms
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < size; j++)
                ncofactors[(i + offset) * sz + j + offset] += cofactors[i * size + j];
        for (size_t i = 0; i < other.size; i++)
            for (size_t j = 0; j < other.size; j++)
                ncofactors[(i + offsetOther) * sz + j + offsetOther] += other.cofactors[i * other.size + j];

        idx = begin;
        size = sz;
        sums = unique_ptr<T[]>(nsums);
        cofactors = unique_ptr<T[]>(ncofactors);
        return *this;
    }

    CofactorDynamic& add(const CofactorDynamic& other) {
        count += other.count;
        for (size_t i = 0; i < size; i++)
            sums[i] += other.sums[i];
        for (size_t i = 0; i < size * size; i++)
            cofactors[i] += other.cofactors[i];
        return *this;
    }

    CofactorDynamic operator*(const CofactorDynamic& other) const {
        if (*this == zero || other == zero) return zero;

        auto begin = std::min(idx, other.idx);
        auto end = std::max(idx + size, other.idx + other.size);
        CofactorDynamic r(end-begin, begin);

        // Count
        r.count = count * other.count;
        // Sum terms
        auto offset = idx - begin;
        auto offsetThat = other.idx - begin;
        for (size_t i = 0; i < size; i++)
            r.sums[i + offset] += other.count * sums[i];
        for (size_t i = 0; i < other.size; i++)
            r.sums[i + offsetThat] += count * other.sums[i];
        // Cofactor terms
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < size; j++)
                r.cofactors[(i + offset) * r.size + j + offset] += other.count * cofactors[i * size + j];
        for (size_t i = 0; i < other.size; i++)
            for (size_t j = 0; j < other.size; j++)
                r.cofactors[(i + offsetThat) * r.size + j + offsetThat] += count * other.cofactors[i * other.size + j];
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < other.size; j++) {                
                r.cofactors[(i + offset) * r.size + j + offsetThat] += sums[i] * other.sums[j];
                r.cofactors[(j + offsetThat) * r.size + i + offset] += sums[i] * other.sums[j];
            }
        return r;        
    }

    FORCE_INLINE bool operator==(const CofactorDynamic &other) const {
        return (size == other.size && idx == other.idx && count == other.count && 
                sums == other.sums && cofactors == other.cofactors);
    }

    FORCE_INLINE bool operator!=(const CofactorDynamic &other) const {
        return !(size == other.size && idx == other.idx && count == other.count && 
                 sums == other.sums && cofactors == other.cofactors);
    }

    FORCE_INLINE void clear() {
        count = 0;
        if (sums != nullptr)
            memset(sums.get(), 0, size * sizeof(T));
        if (cofactors != nullptr) 
            memset(cofactors.get(), 0, size * size * sizeof(T));
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, (int)size);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sums.get(), size);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cofactors.get(), size * size);
    }
};

template<typename T>
CofactorDynamic<T> CofactorDynamic<T>::zero = CofactorDynamic<T>();

typedef CofactorDynamic<DOUBLE_TYPE> RingCofactor;

RingCofactor operator*(long int alpha, const RingCofactor& c) {
    if (alpha == 1) return c;
    // std::cout << "Scale 1: " << alpha << std::endl;
    RingCofactor r(c.size, c.idx);
    r.count = alpha * c.count;
    for (size_t i = 0; i < c.size; i++)
        r.sums[i] += alpha * c.sums[i];
    for (size_t i = 0; i < c.size * c.size; i++) 
        r.cofactors[i] += alpha * c.cofactors[i];
    return r;
}

// RingCofactor operator*(long int alpha, RingCofactor&& c) {
//     std::cout << "Scale 3: " << alpha << std::endl;
//     if (alpha == 1) {
//         return RingCofactor(std::move(c));
//     }
//     RingCofactor r(c.size);
//     r.count = alpha * c.count;
//     for (size_t i = 0; i < c.size; i++)
//         r.sums[i] += alpha * c.sums[i];
//     for (size_t i = 0; i < c.size * c.size; i++) 
//         r.cofactors[i] += alpha * c.cofactors[i];
//     return r;
// }


RingCofactor operator*(const RingCofactor& c, long int alpha) {
    if (alpha == 1) return c;
    // std::cout << "Scale 2: " << alpha << std::endl;
    RingCofactor r(c.size, c.idx);
    r.count = alpha * c.count;
    for (size_t i = 0; i < c.size; i++)
        r.sums[i] += alpha * c.sums[i];
    for (size_t i = 0; i < c.size * c.size; i++) 
        r.cofactors[i] += alpha * c.cofactors[i];
    return r;
}

// RingCofactor operator*(RingCofactor&& c, long int alpha) {
//     std::cout << "Scale 4: " << alpha << std::endl;
//     if (alpha == 1) {
//         return RingCofactor(std::move(c));
//     }
//     RingCofactor r(c.size);
//     r.count = alpha * c.count;
//     for (size_t i = 0; i < c.size; i++)
//         r.sums[i] += alpha * c.sums[i];
//     for (size_t i = 0; i < c.size * c.size; i++) 
//         r.cofactors[i] += alpha * c.cofactors[i];
//     return r;
// }



FORCE_INLINE RingCofactor Ulift(DOUBLE_TYPE a, int idx) {
    RingCofactor r(1, idx);
    r.count = 1;
    r.sums[0] = a;
    r.cofactors[0] = a * a;
    return r;
}

FORCE_INLINE RingCofactor Ulift2(DOUBLE_TYPE a1, DOUBLE_TYPE a2, int idx) {
    RingCofactor r(2, idx);
    r.count = 1;
    r.sums[0] = a1;
    r.sums[1] = a2;
    r.cofactors[0] = a1 * a1;
    r.cofactors[1] = a1 * a2;
    r.cofactors[2] = a2 * a1; 
    r.cofactors[3] = a2 * a2; 
    return r;
}

FORCE_INLINE RingCofactor Ulift3(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, int idx) {
    RingCofactor r(3, idx);
    r.count = 1;
    r.sums[0] = a1;
    r.sums[1] = a2;
    r.sums[2] = a3;
    r.cofactors[0] = a1 * a1;
    r.cofactors[1] = a1 * a2;
    r.cofactors[2] = a1 * a3;
    r.cofactors[3] = a2 * a1; 
    r.cofactors[4] = a2 * a2; 
    r.cofactors[5] = a2 * a3; 
    r.cofactors[6] = a3 * a1; 
    r.cofactors[7] = a3 * a2; 
    r.cofactors[8] = a3 * a3; 
    return r;
}

FORCE_INLINE RingCofactor Ulift4(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, int idx) {
    RingCofactor r(4, idx);
    r.count = 1;
    r.sums[0] = a1;
    r.sums[1] = a2;
    r.sums[2] = a3;
    r.sums[3] = a4;
    r.cofactors[0] = a1 * a1;
    r.cofactors[1] = a1 * a2;
    r.cofactors[2] = a1 * a3;
    r.cofactors[3] = a1 * a4;
    r.cofactors[4] = a2 * a1; 
    r.cofactors[5] = a2 * a2; 
    r.cofactors[6] = a2 * a3; 
    r.cofactors[7] = a2 * a4; 
    r.cofactors[8] = a3 * a1; 
    r.cofactors[9] = a3 * a2; 
    r.cofactors[10] = a3 * a3; 
    r.cofactors[11] = a3 * a4; 
    r.cofactors[12] = a4 * a1; 
    r.cofactors[13] = a4 * a2; 
    r.cofactors[14] = a4 * a3; 
    r.cofactors[15] = a4 * a4; 
    return r;
}

FORCE_INLINE RingCofactor Ulift5(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, int idx) {
    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5 };
    RingCofactor r(5, idx);
    r.count = 1;
    for (int i = 0; i < 5; i++)
        r.sums[i] = arr[i];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            r.cofactors[i * 5 + j] = arr[i] * arr[j];
    return r;
}

RingCofactor Ulift6(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, int idx) {
    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6 };

    RingCofactor r(6, idx);
    r.count = 1;
    for (int i = 0; i < 6; i++)
        r.sums[i] = arr[i];
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            r.cofactors[i * 6 + j] = arr[i] * arr[j];
    return r;
}

FORCE_INLINE 
RingCofactor Ulift10(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, int idx) {
    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10 };
    RingCofactor r(10, idx);
    r.count = 1;
    for (int i = 0; i < 10; i++)
        r.sums[i] = arr[i];
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            r.cofactors[i * 10 + j] = arr[i] * arr[j];
    return r;
}

RingCofactor Ulift13(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, 
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, int idx) {

    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13 };
    RingCofactor r(13, idx);
    r.count = 1;
    for (int i = 0; i < 13; i++)
        r.sums[i] = arr[i];
    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 13; j++)
            r.cofactors[i * 13 + j] = arr[i] * arr[j];
    return r;
}

RingCofactor Ulift15(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10,
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, DOUBLE_TYPE a14, DOUBLE_TYPE a15, int idx) {

    DOUBLE_TYPE arr[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 };
    RingCofactor r(15, idx);
    r.count = 1;
    for (int i = 0; i < 15; i++)
        r.sums[i] = arr[i];
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            r.cofactors[i * 15 + j] = arr[i] * arr[j];
    return r;
}