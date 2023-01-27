#ifndef RING_GENERAL_COFACTOR_HPP
#define RING_GENERAL_COFACTOR_HPP

#include <array>
#include "types.hpp"
#include "dictionary.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <size_t IDX, size_t NUM_CONT, size_t NUM_CAT>
struct RingCofactor {
    static constexpr size_t SZ_SCALAR_ARRAY = NUM_CONT * (NUM_CONT + 3) / 2;
    static constexpr size_t SZ_SCALAR_ARRAY_CONT_CONT = NUM_CONT * (NUM_CONT + 1) / 2;
    static constexpr size_t SZ_RELATION_ARRAY = NUM_CAT * (2 * NUM_CONT + NUM_CAT + 1) / 2;
    static constexpr size_t SZ_RELATION_ARRAY_CAT_CAT = NUM_CAT * (NUM_CAT - 1) / 2;

    int count;
    std::array<double, SZ_SCALAR_ARRAY> scalar_array{};
    std::array<Dictionary, SZ_RELATION_ARRAY> relation_array{};

    explicit RingCofactor() : count(0) { } 

    template <typename... Args>
    explicit RingCofactor(Args&&... args): count(1) {
        static_assert(NUM_CONT + NUM_CAT == sizeof...(args), "Wrong number of arguments");

        double arr[sizeof...(args)] = { args... };

        const double *const cont_values = arr;
        double *raw_scalar_array = scalar_array.data();
        for (size_t i = 0; i < NUM_CONT; i++) {
            *raw_scalar_array++ = cont_values[i];
        }
        for (size_t i = 0; i < NUM_CONT; i++) {
            for (size_t j = i; j < NUM_CONT; j++) {
                *raw_scalar_array++ = cont_values[i] * cont_values[j];
            }
        }

        const double *const cat_values = arr + NUM_CONT;
        Dictionary *raw_relation_array = relation_array.data();
        // CAT
        for (size_t i = 0; i < NUM_CAT; i++) {
            Tuple t;
            t.key = static_cast<uint64_t>(cat_values[i]);
            t.value = 1.0;
            *raw_relation_array++ = Dictionary(t);
        }
        // CONT * CAT
        for (size_t i = 0; i < NUM_CONT; i++) {
            for (size_t j = 0; j < NUM_CAT; j++) {
                Tuple t;
                t.key = static_cast<uint64_t>(cat_values[i]);
                t.value = cont_values[j];
                *raw_relation_array++ = Dictionary(t);
            }
        }
        // CAT * CAT
        for (size_t i = 0; i < NUM_CAT; i++) {
            for (size_t j = i + 1; j < NUM_CAT; j++) {
                Tuple t;
                t.slots[0] = static_cast<uint32_t>(cat_values[i]);
                t.slots[1] = static_cast<uint32_t>(cat_values[j]);
                t.value = 1.0;
                *raw_relation_array++ = Dictionary(t);
            }
        }        
    }

    inline bool isZero() const { return count == 0; }

    RingCofactor& operator+=(const RingCofactor& other) {
        if (other.isZero()) return *this;
        count += other.count;
        for (size_t i = 0; i < SZ_SCALAR_ARRAY; i++) {
            scalar_array[i] += other.scalar_array[i];
        }
        for (size_t i = 0; i < SZ_RELATION_ARRAY; i++) {
            relation_array[i] += other.relation_array[i];
        }
        return *this;
    }

    template <size_t IDX2, size_t NUM_CONT2, size_t NUM_CAT2>
    typename std::enable_if<(IDX < IDX2), RingCofactor<IDX, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2>>::type
    operator*(const RingCofactor<IDX2, NUM_CONT2, NUM_CAT2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2>();
        return this->multiply(other);
    }

    template <size_t IDX2, size_t NUM_CONT2, size_t NUM_CAT2>
    typename std::enable_if<(IDX > IDX2), RingCofactor<IDX2, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2>>::type
    operator*(const RingCofactor<IDX2, NUM_CONT2, NUM_CAT2>& other) const {
        if (isZero() || other.isZero()) return RingCofactor<IDX2, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2>();
        return other.multiply(*this);
    }

    template <size_t IDX2, size_t NUM_CONT2, size_t NUM_CAT2>
    RingCofactor<IDX, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2> 
    multiply(const RingCofactor<IDX2, NUM_CONT2, NUM_CAT2>& other) const {
        static_assert(IDX + NUM_CONT + NUM_CAT == IDX2, "Cofactor sizes do not match");

        RingCofactor<IDX, NUM_CONT + NUM_CONT2, NUM_CAT + NUM_CAT2> r;
        r.count = count * other.count;

        // PART 1 - MULTIPLY SCALAR ARRAYS

        double *out_scalar_array_iter = r.scalar_array.data();

        // CONT degree 1
        const double *const a_sum1_scalar_array = scalar_array.data();
        for (size_t i = 0; i < NUM_CONT; i++) {
            *out_scalar_array_iter++ = other.count * a_sum1_scalar_array[i];
        }
        const double *const b_sum1_scalar_array = other.scalar_array.data();
        for (size_t i = 0; i < NUM_CONT2; i++) {
            *out_scalar_array_iter++ = count * b_sum1_scalar_array[i];
        }

        // CONT degree 2
        const double *const a_sum2_scalar_array = a_sum1_scalar_array + NUM_CONT;
        const double *a_sum2_scalar_array_iter = a_sum2_scalar_array; 
        for (size_t i = 0; i < NUM_CONT; i++) {
            for (size_t j = i; j < NUM_CONT; j++) {
                *out_scalar_array_iter++ = other.count * (*a_sum2_scalar_array_iter++);
            }
            for (size_t j = 0; j < NUM_CONT2; j++) {
                *out_scalar_array_iter++ = a_sum1_scalar_array[i] * b_sum1_scalar_array[j];
            }
        }
        const double *const b_sum2_scalar_array = b_sum1_scalar_array + NUM_CONT2;
        for (size_t i = 0; i < RingCofactor<IDX2, NUM_CONT2, NUM_CAT2>::SZ_SCALAR_ARRAY_CONT_CONT; i++) {
            *out_scalar_array_iter++ = count * b_sum2_scalar_array[i];
        }

        //
        // PART 2 - MULTIPLY RELATION ARRAYS
        Dictionary *out_relation_iter = r.relation_array.data();

        // CAT degree 1
        const Dictionary *const a_sum1_relation_array = relation_array.data();
        for (size_t i = 0; i < NUM_CAT; i++) {
            *out_relation_iter++ = a_sum1_relation_array[i].multiply(other.count);
        }
        const Dictionary *const b_sum1_relation_array = other.relation_array.data();
        for (size_t i = 0; i < NUM_CAT2; i++) {
            *out_relation_iter++ = b_sum1_relation_array[i].multiply(count);
        }

        // CAT degree 2
        const Dictionary* const a_sum2_relation_array = a_sum1_relation_array + NUM_CAT;
        const Dictionary* const b_sum2_relation_array = b_sum1_relation_array + NUM_CAT2;

        const Dictionary* a_sum2_relation_array_iter = a_sum2_relation_array;
        for (size_t i = 0; i < NUM_CONT; i++) {

            // (cont_A * cat_A) * count_B
            for (size_t j = 0; j < NUM_CAT; j++) {
                *out_relation_iter++ = (*a_sum2_relation_array_iter++).multiply(other.count);
            }

            // cont_A * cat_B
            for (size_t j = 0; j < NUM_CAT2; j++) {
                *out_relation_iter++ = b_sum1_relation_array[j].multiply(a_sum1_scalar_array[i]);
            }
        }

        const Dictionary* b_sum2_relation_array_iter = b_sum2_relation_array;
        for (size_t i = 0; i < NUM_CONT2; i++) {

            // cont_B * cat_A
            for (size_t j = 0; j < NUM_CAT; j++) {
                *out_relation_iter++ = a_sum1_relation_array[j].multiply(b_sum1_scalar_array[i]);
            }

            // (cont_B * cat_B) * count_A
            for (size_t j = 0; j < NUM_CAT2; j++) {
                *out_relation_iter++ = (*b_sum2_relation_array_iter++).multiply(count);
            }
        }

        for (size_t i = 0; i < NUM_CAT; i++) {

            // (cat_A * cat_A) * count_B
            for (size_t j = i + 1; j < NUM_CAT; j++) {
                *out_relation_iter++ = (*a_sum2_relation_array_iter++).multiply(other.count);
            }

            // cat_A * cat_B
            for (size_t j = 0; j < NUM_CAT2; j++) {
                *out_relation_iter++ = a_sum1_relation_array[i].multiply(b_sum1_relation_array[j]);
            }
        }

        // (cat_B * cat_B) * count_A
        for (size_t i = 0; i < RingCofactor<IDX2, NUM_CONT2, NUM_CAT2>::SZ_RELATION_ARRAY_CAT_CAT; i++) {
            *out_relation_iter++ = (*b_sum2_relation_array_iter++).multiply(count);
        }

        return r;
    }

    RingCofactor operator*(long alpha) const {
        if (alpha == 0L) return RingCofactor<IDX, NUM_CONT, NUM_CAT>();
        return multiply(alpha);
    }

    RingCofactor multiply(long alpha) const {
        RingCofactor<IDX, NUM_CONT, NUM_CAT> r;
        r.count = alpha * count;
        for (size_t i = 0; i < SZ_SCALAR_ARRAY; i++) {
            r.scalar_array[i] = scalar_array[i] * alpha;
        }
        for (size_t i = 0; i < SZ_RELATION_ARRAY; i++) {
            r.relation_array[i] = relation_array[i].multiply(alpha);
        }
        return r;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, IDX);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, scalar_array.data(), SZ_SCALAR_ARRAY);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, relation_array.data(), SZ_RELATION_ARRAY);
    }
};

template <size_t IDX, size_t CONT_SZ, size_t CAT_SZ>
RingCofactor<IDX, CONT_SZ, CAT_SZ> operator*(long int alpha, const RingCofactor<IDX, CONT_SZ, CAT_SZ>& c) {
    if (alpha == 1L) return c;
    return c.multiply(alpha);
}

template <size_t IDX, size_t CONT_SZ, size_t CAT_SZ>
using RingCofactorGeneral = RingCofactor<IDX, CONT_SZ, CAT_SZ>;

template <typename T>
inline double convert(T x) { return x; }

template <size_t IDX, typename... Args>
RingCofactorGeneral<IDX, sizeof...(Args), 0> Uliftcont(Args&&... args) {
    return RingCofactorGeneral<IDX, sizeof...(Args), 0>(convert(args)...);
}

template <size_t IDX, typename... Args>
RingCofactorGeneral<IDX, 0, sizeof...(Args)> Uliftcat(Args&&... args) {
    return RingCofactorGeneral<IDX, 0, sizeof...(Args)>(convert(args)...);
}

#endif /* RING_GENERAL_COFACTOR_HPP */
