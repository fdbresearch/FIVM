#ifndef RINGCOFACTOR_LAZY_HPP
#define RINGCOFACTOR_LAZY_HPP

#include "types.hpp"
#include "serialization.hpp"
#include <vector>
#include "mmap.hpp"

using namespace dbtoaster;

template <typename REF, typename VALUE>
struct Aggregator {
private:
    Aggregator(): scale(1L), pointerIdx(0), valueIdx(0) { }

public:
    static Aggregator& getInstance() {
        static Aggregator instance;
        return instance;
    }

    FORCE_INLINE Aggregator& operator<<(long int a) {
        scale *= a;        
        return *this;
    }

    FORCE_INLINE Aggregator& operator<<(const REF& r) {
        // pointers.push_back(&r);
        pointers[pointerIdx++] = &r;
        return *this;
    }

    FORCE_INLINE Aggregator& operator<<(const VALUE& v) {
        // values.push_back(v);
        values[valueIdx++] = v;
        return *this;
    }

    FORCE_INLINE Aggregator& operator*(const Aggregator&) {
        return *this;
    }

    Aggregator(const Aggregator&) = delete;
    Aggregator& operator=(const Aggregator&) = delete;

    void reset() {
        scale = 1L;
        pointerIdx = 0;
        valueIdx = 0;
        // pointers.clear();
        // values.clear();
    }

    long int scale;
    const REF* pointers[27];
    VALUE values[27];
    size_t pointerIdx;
    size_t valueIdx;
    // std::vector<const REF*> pointers;
    // std::vector<VALUE> values;
};

struct Value {
    DOUBLE_TYPE x;
    uint32_t idx;
    Value() { }
    Value(DOUBLE_TYPE x_, uint32_t idx_) : x(x_), idx(idx_) { }
};

template <typename T>
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
        
static int counter = 0;
        cout << "COPY: " << counter++ << endl; 
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

    CofactorDynamic& operator=(Aggregator<CofactorDynamic,Value>& agg) {
        // TODO

static int counter3 = 0;
        cout << "Assignment: " << counter3++ << endl; 

        auto max = 0;
        auto min = 45;
        for (auto i = 0; i < agg.valueIdx; i++) {
            if (agg.values[i].idx > max) { max = agg.values[i].idx; }
            if (agg.values[i].idx < min) { min = agg.values[i].idx; }
        }
        // size = std::max(max - min, 0);

        count = agg.scale;
        // sums = unique_ptr<T[]>(new T[size]);
        // cofactors = unique_ptr<T[]>(new T[size * size]);
        // memset(sums.get(), 0, size * sizeof(T));
        // memset(cofactors.get(), 0, size * size * sizeof(T));

        agg.reset();
        return *this;
    }

    void operator+=(const CofactorDynamic& other) {
static int counter5 = 0;
        cout << "Increm MENT: " << counter5++ << endl; 

        count += other.count;
        for (size_t i = 0; i < size; i++)
            sums[i] += other.sums[i];
        for (size_t i = 0; i < size * size; i++)
            cofactors[i] += other.cofactors[i];
    }

    void operator+=(Aggregator<CofactorDynamic,Value>& agg) {
static int counter4 = 0;
        cout << "Increment: " << counter4++ << endl; 

        // TODO
        auto max = 0;
        auto min = 45;
        for (auto i = 0; i < agg.valueIdx; i++) {
            if (agg.values[i].idx > max) { max = agg.values[i].idx; }
            if (agg.values[i].idx < min) { min = agg.values[i].idx; }
        }
        // size = std::max(max - min, 0);
        count += agg.scale;

        agg.reset();
    }    

    // CofactorDynamic& expandThenAdd(const CofactorDynamic& other) {
    //     auto begin = std::min(idx, other.idx);
    //     auto end = std::max(idx + size, other.idx + other.size);
    //     auto sz = end - begin;
    //     auto nsums = new T[sz]();
    //     auto ncofactors = new T[sz * sz]();

    //     // Count
    //     count += other.count;
    //     // Sum terms        
    //     auto offset = idx - begin;
    //     auto offsetOther = other.idx - begin;
    //     for (size_t i = 0; i < size; i++)
    //         nsums[i + offset] += sums[i];
    //     for (size_t i = 0; i < other.size; i++)
    //         nsums[i + offsetOther] += other.sums[i];
    //     // Cofactor terms
    //     for (size_t i = 0; i < size; i++)
    //         for (size_t j = 0; j < size; j++)
    //             ncofactors[(i + offset) * sz + j + offset] += cofactors[i * size + j];
    //     for (size_t i = 0; i < other.size; i++)
    //         for (size_t j = 0; j < other.size; j++)
    //             ncofactors[(i + offsetOther) * sz + j + offsetOther] += other.cofactors[i * other.size + j];

    //     idx = begin;
    //     size = sz;
    //     sums = unique_ptr<T[]>(nsums);
    //     cofactors = unique_ptr<T[]>(ncofactors);
    //     return *this;
    // }

    // CofactorDynamic& add(const CofactorDynamic& other) {
    //     count += other.count;
    //     for (size_t i = 0; i < size; i++)
    //         sums[i] += other.sums[i];
    //     for (size_t i = 0; i < size * size; i++)
    //         cofactors[i] += other.cofactors[i];
    //     return *this;
    // }

    // CofactorDynamic operator*(const CofactorDynamic& other) const {
    //     if (*this == zero || other == zero) return zero;

    //     auto begin = std::min(idx, other.idx);
    //     auto end = std::max(idx + size, other.idx + other.size);
    //     CofactorDynamic r(end-begin, begin);

    //     // Count
    //     r.count = count * other.count;
    //     // Sum terms
    //     auto offset = idx - begin;
    //     auto offsetThat = other.idx - begin;
    //     for (size_t i = 0; i < size; i++)
    //         r.sums[i + offset] += other.count * sums[i];
    //     for (size_t i = 0; i < other.size; i++)
    //         r.sums[i + offsetThat] += count * other.sums[i];
    //     // Cofactor terms
    //     for (size_t i = 0; i < size; i++)
    //         for (size_t j = 0; j < size; j++)
    //             r.cofactors[(i + offset) * r.size + j + offset] += other.count * cofactors[i * size + j];
    //     for (size_t i = 0; i < other.size; i++)
    //         for (size_t j = 0; j < other.size; j++)
    //             r.cofactors[(i + offsetThat) * r.size + j + offsetThat] += count * other.cofactors[i * other.size + j];
    //     for (size_t i = 0; i < size; i++)
    //         for (size_t j = 0; j < other.size; j++) {                
    //             r.cofactors[(i + offset) * r.size + j + offsetThat] += sums[i] * other.sums[j];
    //             r.cofactors[(j + offsetThat) * r.size + i + offset] += sums[i] * other.sums[j];
    //         }
    //     return r;        
    // }

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

template <typename T>
CofactorDynamic<T> CofactorDynamic<T>::zero = CofactorDynamic<T>();

typedef CofactorDynamic<DOUBLE_TYPE> RingCofactor;

Aggregator<RingCofactor, Value>& operator*(long int s, const RingCofactor& r) {
    return Aggregator<RingCofactor, Value>::getInstance() << s << r;
}

Aggregator<RingCofactor, Value>& operator*(long int s, const Value& v) {
    return Aggregator<RingCofactor, Value>::getInstance() << s << v;
}

Aggregator<RingCofactor, Value>& operator*(long int s, Aggregator<RingCofactor, Value>& agg) {
    return agg << s;
}

Aggregator<RingCofactor, Value>& operator*(const RingCofactor& r, long int s) {
    return Aggregator<RingCofactor, Value>::getInstance() << r << s;
}

Aggregator<RingCofactor, Value>& operator*(const RingCofactor& r1, const RingCofactor& r2) {
    return Aggregator<RingCofactor, Value>::getInstance() << r1 << r2;
}

Aggregator<RingCofactor, Value>& operator*(const RingCofactor& r, const Value& v) {
    return Aggregator<RingCofactor, Value>::getInstance() << r << v;
}

Aggregator<RingCofactor, Value>& operator*(const RingCofactor& r, Aggregator<RingCofactor, Value>& agg) {
    return agg << r;
}

Aggregator<RingCofactor, Value>& operator*(const Value& v, long int s) {
    return Aggregator<RingCofactor, Value>::getInstance() << v << s;
}

Aggregator<RingCofactor, Value>& operator*(const Value& v, const RingCofactor& r) {
    return Aggregator<RingCofactor, Value>::getInstance() << v << r;
}

Aggregator<RingCofactor, Value>& operator*(const Value& v1, const Value& v2) {
    return Aggregator<RingCofactor, Value>::getInstance() << v1 << v2;
}

Aggregator<RingCofactor, Value>& operator*(const Value& v, Aggregator<RingCofactor, Value>& agg) {
    return agg << v;
}

// Value Ulift(DOUBLE_TYPE a, uint32_t idx) {
//     return Value(a, idx);
// }

Aggregator<RingCofactor, Value>& Ulift(DOUBLE_TYPE a, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a, idx);
}

Aggregator<RingCofactor, Value>& Ulift2(DOUBLE_TYPE a1, DOUBLE_TYPE a2, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1);
}

Aggregator<RingCofactor, Value>& Ulift3(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2);
}

Aggregator<RingCofactor, Value>& Ulift4(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3);
}

Aggregator<RingCofactor, Value>& Ulift5(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3) << Value(a5, idx + 4);
}

Aggregator<RingCofactor, Value>& Ulift6(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3) << Value(a5, idx + 4) << Value(a6, idx + 5);
}

Aggregator<RingCofactor, Value>& Ulift10(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3) << Value(a5, idx + 4) << Value(a6, idx + 5) << Value(a7, idx + 6) << Value(a8, idx + 7) << Value(a9, idx + 8) << Value(a10, idx + 9);
}

Aggregator<RingCofactor, Value>& Ulift13(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3) << Value(a5, idx + 4) << Value(a6, idx + 5) << Value(a7, idx + 6) << Value(a8, idx + 7) << Value(a9, idx + 8) << Value(a10, idx + 9) << Value(a11, idx + 10) << Value(a12, idx + 11) << Value(a13, idx + 12);
}

Aggregator<RingCofactor, Value>& Ulift15(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, DOUBLE_TYPE a14, DOUBLE_TYPE a15, uint32_t idx) {
    return Aggregator<RingCofactor, Value>::getInstance() << Value(a1, idx) << Value(a2, idx + 1) << Value(a3, idx + 2) << Value(a4, idx + 3) << Value(a5, idx + 4) << Value(a6, idx + 5) << Value(a7, idx + 6) << Value(a8, idx + 7) << Value(a9, idx + 8) << Value(a10, idx + 9) << Value(a11, idx + 10) << Value(a12, idx + 11) << Value(a13, idx + 12) << Value(a14, idx + 13) << Value(a15, idx + 14);
}

#endif /* RINGCOFACTOR_LAZY_HPP */