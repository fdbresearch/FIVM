#ifndef RING_COFACTOR_DEGREE1_CATEGORICAL_HPP
#define RING_COFACTOR_DEGREE1_CATEGORICAL_HPP

#include <array>
#include <unordered_map>
#include "types.hpp"
#include "serialization.hpp"
#include "hash.hpp"
#include "container.hpp"

using namespace dbtoaster;

template <typename K, typename V, typename H>
void operator+=(std::unordered_map<K, V, H>& m1, const std::unordered_map<K, V, H>& m2) {
    for (auto &it : m2) {
        m1[it.first] += it.second;
    }
}

template <typename K1, typename K2, typename V>
std::unordered_map<std::tuple<K1, K2>, V, hash_tuple::hash<std::tuple<long, long>>> 
operator*(const std::unordered_map<K1, V>& m1, const std::unordered_map<K2, V>& m2) {
    std::unordered_map<std::tuple<K1, K2>, V, hash_tuple::hash<std::tuple<long, long>>> r;
    if (m1.empty() || m2.empty()) return r;

    for (auto &it1 : m1) {
        for (auto &it2 : m2) {
            r[std::make_tuple(it1.first, it2.first)] = it1.second * it2.second;
        }
    }
    return r;
}

template <typename K, typename V, typename H>
std::unordered_map<K, V, H> operator*(long int alpha, const std::unordered_map<K, V, H>& m) {
    std::unordered_map<K, V, H> r;
    for (auto &it : m) {
        r[it.first] = it.second * alpha;
    }
    return r;
}

template <size_t IDX, size_t CONT_SZ, size_t CAT_SZ>
struct RingCofactor {
    static constexpr size_t CONT_DEG2_SZ = (CONT_SZ - 1) * CONT_SZ / 2;
    static constexpr size_t CAT_DEG2_SZ = (CAT_SZ - 1) * CAT_SZ / 2;
    static constexpr size_t CONT_CAT_DEG2_SZ = CONT_SZ * CAT_SZ;

    long count;
    // Aggregates over continuous variables
    std::array<double, CONT_SZ> cont_sum1;
    std::array<double, CONT_SZ> cont_sum2;
    std::array<double, CONT_DEG2_SZ> cont_degree2;
    // Aggregates over categorical variables
    std::array<std::unordered_map<long, double>, CAT_SZ> cat_sum1;
    std::array<std::unordered_map<long, double>, CAT_SZ> cat_sum2;
    std::array<std::unordered_map<std::tuple<long, long>, double, hash_tuple::hash<std::tuple<long, long>>>, CAT_DEG2_SZ> cat_degree2;
    // Aggregates over continuous and categorical variables
    std::array<std::unordered_map<long, double>, CONT_CAT_DEG2_SZ> cont_cat_degree2;
    
    explicit RingCofactor() : count(0) { }

    template <typename... Args>
    explicit RingCofactor(Args&&... args): count(1), cont_sum1 { args... } {
        static_assert(CONT_SZ == sizeof...(args) && CAT_SZ == 0, "Incompatible array sizes");

        auto *cont_out2 = cont_degree2.data();
        for (size_t i = 0; i < CONT_SZ; i++) {
            cont_sum2[i] = cont_sum1[i] * cont_sum1[i];

            for (size_t j = i + 1; j < CONT_SZ; j++) {
                *cont_out2++ = cont_sum1[i] * cont_sum1[j];
            }
        }
    }

    template <typename... Args>
    explicit RingCofactor(bool, Args&&... args) : count(1), cat_sum1 { std::unordered_map<long, double>{{ args, 1.0 }}... } {
        static_assert(CAT_SZ == sizeof...(args) && CONT_SZ == 0, "Incompatible array sizes");

        auto *cat_out2 = cat_degree2.data();
        for (size_t i = 0; i < CAT_SZ; i++) {
            cat_sum2[i] = cat_sum1[i];

            for (size_t j = i + 1; j < CAT_SZ; j++) {
                *cat_out2++ = cat_sum1[i] * cat_sum1[j];
            }
        }
    }

    inline bool isZero() const { return count == 0; }

    RingCofactor& operator+=(const RingCofactor& other) {
        if (other.isZero()) return *this;
        count += other.count;
        for (size_t i = 0; i < CONT_SZ; i++) {
            cont_sum1[i] += other.cont_sum1[i];
            cont_sum2[i] += other.cont_sum2[i];
        }
        for (size_t i = 0; i < CONT_DEG2_SZ; i++) {
            cont_degree2[i] += other.cont_degree2[i];
        }
        for (size_t i = 0; i < CAT_SZ; i++) {
            cat_sum1[i] += other.cat_sum1[i];
            cat_sum2[i] += other.cat_sum2[i];
        }
        for (size_t i = 0; i < CAT_DEG2_SZ; i++) {
            cat_degree2[i] += other.cat_degree2[i];
        }
        for (size_t i = 0; i < CONT_CAT_DEG2_SZ; i++) {
            cont_cat_degree2[i] += other.cont_cat_degree2[i];
        }
        return *this;
    }

    template <size_t IDX2, size_t CONT_SZ2, size_t CAT_SZ2>
    typename std::enable_if<(IDX < IDX2), RingCofactor<IDX, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2>>::type
    operator*(const RingCofactor<IDX2, CONT_SZ2, CAT_SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2>();
        return this->multiply(other);
    }

    template <size_t IDX2, size_t CONT_SZ2, size_t CAT_SZ2>
    typename std::enable_if<(IDX > IDX2), RingCofactor<IDX2, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2>>::type
    operator*(const RingCofactor<IDX2, CONT_SZ2, CAT_SZ2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX2, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2>();
        return other.multiply(*this);
    }

    template <size_t IDX2, size_t CONT_SZ2, size_t CAT_SZ2>
    RingCofactor<IDX, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2>
    multiply(const RingCofactor<IDX2, CONT_SZ2, CAT_SZ2>& other) const {
        static_assert(IDX + CAT_SZ + CONT_SZ == IDX2, "Cofactor sizes do not match");

        RingCofactor<IDX, CONT_SZ + CONT_SZ2, CAT_SZ + CAT_SZ2> r;
        r.count = other.count * count;

        // CONT, CONT_i * CONT_i
        for (size_t i = 0; i < CONT_SZ; i++) {
            r.cont_sum1[i] = other.count * cont_sum1[i];
            r.cont_sum2[i] = other.count * cont_sum2[i];
        }

        for (size_t i = 0; i < CONT_SZ2; i++) {
            r.cont_sum1[CONT_SZ + i] = count * other.cont_sum1[i];
            r.cont_sum2[CONT_SZ + i] = count * other.cont_sum2[i];
        }

        // CONT_i * CONT_j for different i, j such that i < j
        const auto *cont_in2 = cont_degree2.data();
        auto *cont_out2 = r.cont_degree2.data();
        for (size_t i = 0; i < CONT_SZ; i++) {
            for (size_t j = i + 1; j < CONT_SZ; j++) {
                *cont_out2++ = other.count * (*cont_in2++);
            }
            for (size_t j = 0; j < CONT_SZ2; j++) {
                *cont_out2++ = cont_sum1[i] * other.cont_sum1[j];
            }
        }
        for (size_t i = 0; i < RingCofactor<IDX2, CONT_SZ2, CAT_SZ2>::CONT_DEG2_SZ; i++) {
            *cont_out2++ = count * other.cont_degree2[i];
        }
        
        // CAT, CAT_i * CAT_i
        for (size_t i = 0; i < CAT_SZ; i++) {
            r.cat_sum1[i] = other.count * cat_sum1[i];
            r.cat_sum2[i] = other.count * cat_sum2[i];
        }

        for (size_t i = 0; i < CAT_SZ2; i++) {
            r.cat_sum1[CAT_SZ + i] = count * other.cat_sum1[i];
            r.cat_sum2[CAT_SZ + i] = count * other.cat_sum2[i];
        }

        // CAT_i * CAT_j for different i, j such that i < j
        const auto *cat_in2 = cat_degree2.data();
        auto *cat_out2 = r.cat_degree2.data();
        for (size_t i = 0; i < CAT_SZ; i++) {
            for (size_t j = i + 1; j < CAT_SZ; j++) {
                *cat_out2++ = other.count * (*cat_in2++);
            }
            for (size_t j = 0; j < CAT_SZ2; j++) {
                *cat_out2++ = cat_sum1[i] * other.cat_sum1[j];
            }
        }
        for (size_t i = 0; i < RingCofactor<IDX2, CONT_SZ2, CAT_SZ2>::CAT_DEG2_SZ; i++) {
            *cat_out2++ = count * other.cat_degree2[i];
        }

        // CONV_i * CAT_j 
        auto *cont_cat_out2 = r.cont_cat_degree2.data();
        const auto* cont_cat_in1 = cont_cat_degree2.data();
        for (size_t i = 0; i < CONT_SZ; i++) {
            for (size_t j = 0; j < CAT_SZ; j++) {
                *cont_cat_out2++ = other.count * (*cont_cat_in1++);
            }
            for (size_t j = 0; j < CAT_SZ2; j++) {
                *cont_cat_out2++ = cont_sum1[i] * other.cat_sum1[j];
            }
        }
        const auto* cont_cat_in2 = other.cont_cat_degree2.data();
        for (size_t i = 0; i < CONT_SZ2; i++) {
            for (size_t j = 0; j < CAT_SZ; j++) {
                *cont_cat_out2++ = other.cont_sum1[i] * cat_sum1[j];
            }
            for (size_t j = 0; j < CAT_SZ2; j++) {
                *cont_cat_out2++ = count * (*cont_cat_in2++);
            }
        }

        return r;
    }

    RingCofactor operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    RingCofactor multiply(long alpha) const {
        RingCofactor<IDX, CONT_SZ, CAT_SZ> r;
        r.count = alpha * count;
        for (size_t i = 0; i < CONT_SZ; i++) {
            r.cont_sum1[i] = alpha * cont_sum1[i];
            r.cont_sum2[i] = alpha * cont_sum2[i];
        }
        for (size_t i = 0; i < CONT_DEG2_SZ; i++) {
            r.cont_degree2[i] = alpha * cont_degree2[i];
        }
        for (size_t i = 0; i < CAT_SZ; i++) {
            r.cat_sum1[i] = alpha * cat_sum1[i];
            r.cat_sum2[i] = alpha * cat_sum2[i];
        }
        for (size_t i = 0; i < CAT_DEG2_SZ; i++) {
            r.cat_degree2[i] = alpha * cat_degree2[i];
        }
        for (size_t i = 0; i < CONT_CAT_DEG2_SZ; i++) {
            r.cont_cat_degree2[i] = alpha * cont_cat_degree2[i];
        }
        return r;
    }

    void clear() {
        count = 0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, IDX);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cont_sum1.data(), CONT_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cont_sum2.data(), CONT_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cont_degree2.data(), CONT_DEG2_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cat_sum1.data(), CAT_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cat_sum2.data(), CAT_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cat_degree2.data(), CAT_DEG2_SZ);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cont_cat_degree2.data(), CONT_CAT_DEG2_SZ);
    }
};

template <size_t IDX, size_t CONT_SZ, size_t CAT_SZ>
RingCofactor<IDX, CONT_SZ, CAT_SZ> operator*(long int alpha, const RingCofactor<IDX, CONT_SZ, CAT_SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <size_t IDX, size_t CONT_SZ, size_t CAT_SZ>
using RingCofactorMixed = RingCofactor<IDX, CONT_SZ, CAT_SZ>;

template <typename T>
inline double convert(T x) { return x; }

template <size_t IDX, typename... Args>
RingCofactorMixed<IDX, sizeof...(Args), 0> Uliftcont(Args&&... args) {
    return RingCofactorMixed<IDX, sizeof...(Args), 0>(convert(args)...);
}

template <size_t IDX, typename... Args>
RingCofactorMixed<IDX, 0, sizeof...(Args)> Uliftcat(Args&&... args) {
    return RingCofactorMixed<IDX, 0, sizeof...(Args)>(true, args...);
}

#endif /* RING_COFACTOR_DEGREE1_CATEGORICAL_HPP */