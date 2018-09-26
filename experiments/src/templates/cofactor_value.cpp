#ifndef DBTOASTER_AGG_VALUE_HPP
#define DBTOASTER_AGG_VALUE_HPP

#include "macro.hpp"
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template<int SUM_SIZE, int COFACTOR_SIZE = (SUM_SIZE * (SUM_SIZE + 1)) / 2>
struct AGG_value {
    static AGG_value<SUM_SIZE, COFACTOR_SIZE> zero;
    int count;
    DOUBLE_TYPE sums[SUM_SIZE];
    DOUBLE_TYPE cofactors[COFACTOR_SIZE];

    explicit AGG_value() {
        count = 0;
        memset(sums, 0, SUM_SIZE * sizeof(DOUBLE_TYPE));
        memset(cofactors, 0, COFACTOR_SIZE * sizeof(DOUBLE_TYPE));
    }

    ~AGG_value() {
        count = 0;
        memset(sums, 0, SUM_SIZE * sizeof(DOUBLE_TYPE));
        memset(cofactors, 0, COFACTOR_SIZE * sizeof(DOUBLE_TYPE));
    }

    FORCE_INLINE AGG_value& operator +=(const AGG_value<SUM_SIZE, COFACTOR_SIZE> &obj) {
        count += obj.count;
        for (size_t i = 0; i < SUM_SIZE; i++) sums[i] += obj.sums[i];
        for (size_t i = 0; i < COFACTOR_SIZE; i++) cofactors[i] += obj.cofactors[i];
        return *this;
    }

    FORCE_INLINE bool operator ==(const AGG_value &obj) const {
        return (count == obj.count && sums == obj.sums && cofactors == obj.cofactors);
    }

    FORCE_INLINE void clear() { 
        count = 0;
        memset(sums, 0, SUM_SIZE * sizeof(DOUBLE_TYPE));
        memset(cofactors, 0, COFACTOR_SIZE * sizeof(DOUBLE_TYPE));      
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sums, SUM_SIZE);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cofactors, COFACTOR_SIZE);
    }
};

template<int SUM_SIZE, int COFACTOR_SIZE>
AGG_value<SUM_SIZE, COFACTOR_SIZE> AGG_value<SUM_SIZE, COFACTOR_SIZE>::zero = AGG_value<SUM_SIZE, COFACTOR_SIZE>();

template<>
struct AGG_value<1,1> {
    static AGG_value<1,1> zero;    
    int count;
    DOUBLE_TYPE sums[1];
    DOUBLE_TYPE cofactors[1];

    explicit AGG_value() {
        count = 0;
        sums[0] = 0.0;
        cofactors[0] = 0.0;
    }

    ~AGG_value() {
        count = 0;
        sums[0] = 0.0;
        cofactors[0] = 0.0;
    }

    FORCE_INLINE AGG_value& operator +=(const AGG_value<1,1> &obj) {
        count += obj.count;
        sums[0] += obj.sums[0];
        cofactors[0] += obj.cofactors[0];
        return *this;
    }

    FORCE_INLINE bool operator ==(const AGG_value &obj) const {
        return (count == obj.count && sums == obj.sums && cofactors == obj.cofactors);
    }

    FORCE_INLINE void clear() { 
        count = 0;
        sums[0] = 0.0;
        cofactors[0] = 0.0;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, sums, 1);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP_OF_ARRAY(ar, cofactors, 1);
    }
};

AGG_value<1,1> AGG_value<1,1>::zero = AGG_value<1,1>();

#endif /* DBTOASTER_AGG_VALUE_HPP */