#ifndef RINGDATACUBE_HPP
#define RINGDATACUBE_HPP

#include <tuple>
#include "types.hpp"
#include "serialization.hpp"
#include "container.hpp"
#include "ring_relational.hpp"

template <size_t Idx, typename... Args>
struct DataCubeVector { };

template <size_t Idx>
struct DataCubeVector<Idx> {
    long count;
    double sum1;      // empty keys

    explicit DataCubeVector() : count(0L), sum1(0.0) { }

    explicit DataCubeVector(double value) 
        : count(1L), sum1(value) { }

    inline bool isZero() const { return count == 0; }

    DataCubeVector& operator+=(const DataCubeVector& other) {
        if (other.isZero()) return *this;
        count += other.count;
        sum1 += other.sum1;
        return *this;
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx>();
        return this->multiply(other);
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2>();
        return other.multiply(*this);
    }

    template <size_t Idx2>
    DataCubeVector<Idx> multiply(const DataCubeVector<Idx2>& other) const {
        DataCubeVector<Idx> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        return r;
    }

    template <size_t Idx2, typename K2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K2>>::type
    operator*(const DataCubeVector<Idx2, K2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K2>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2>>::type
    operator*(const DataCubeVector<Idx2, K2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2>
    DataCubeVector<Idx, K2> multiply(const DataCubeVector<Idx2, K2>& other) const {
        DataCubeVector<Idx, K2> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        return r;
    }

    template <size_t Idx2, typename K2, typename K3>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx,K2, K3>>::type
    operator*(const DataCubeVector<Idx2, K2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K2, K3>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2, typename K3>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K3>>::type
    operator*(const DataCubeVector<Idx2, K2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2, K3>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2, typename K3>
    DataCubeVector<Idx, K2, K3> multiply(const DataCubeVector<Idx2, K2, K3>& other) const {
        DataCubeVector<Idx, K2, K3> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;     
        std::get<0>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<1>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        return r;
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K2, K3, K4>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2, K3, K4>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    DataCubeVector<Idx, K2, K3, K4> multiply(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        DataCubeVector<Idx, K2, K3, K4> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<1>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<3>(other.relations).multiply(sum1);
        std::get<4>(r.relations) = std::get<4>(other.relations).multiply(sum1);
        std::get<5>(r.relations) = std::get<5>(other.relations).multiply(sum1);
        std::get<6>(r.relations) = std::get<6>(other.relations).multiply(sum1);
        return r;
    }

    template <size_t Idx2, typename K1, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K1, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3, K4>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K1, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K3, K4, K1>>::type
    operator*(const DataCubeVector<Idx2, K1, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K1, K2, K3, K4>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K1, typename K2, typename K3, typename K4>
    DataCubeVector<Idx, K1, K2, K3, K4> multiply(const DataCubeVector<Idx2, K1, K2, K3, K4>& other) const {
        DataCubeVector<Idx, K1, K2, K3, K4> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<1>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<3>(other.relations).multiply(sum1);
        std::get<4>(r.relations) = std::get<4>(other.relations).multiply(sum1);
        std::get<5>(r.relations) = std::get<5>(other.relations).multiply(sum1);
        std::get<6>(r.relations) = std::get<6>(other.relations).multiply(sum1);
        std::get<7>(r.relations) = std::get<7>(other.relations).multiply(sum1);
        std::get<8>(r.relations) = std::get<8>(other.relations).multiply(sum1);
        std::get<9>(r.relations) = std::get<9>(other.relations).multiply(sum1);
        std::get<10>(r.relations) = std::get<10>(other.relations).multiply(sum1);
        std::get<11>(r.relations) = std::get<11>(other.relations).multiply(sum1);
        std::get<12>(r.relations) = std::get<12>(other.relations).multiply(sum1);
        std::get<13>(r.relations) = std::get<13>(other.relations).multiply(sum1);
        std::get<14>(r.relations) = std::get<14>(other.relations).multiply(sum1);        
        return r;
    }

    DataCubeVector operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    DataCubeVector multiply(long alpha) const {
        DataCubeVector<Idx> r;
        if (alpha == 0L) return r;
        r.count = count * alpha;
        r.sum1 = sum1 * alpha;
        return r;
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum1);
    }
};

template <size_t Idx, typename K1>
struct DataCubeVector<Idx, K1> {
    long count;
    double sum1;      // empty keys
    std::tuple<RingRelation<Idx, K1>> relations;

    explicit DataCubeVector() : count(0L), sum1(0.0) { }

    explicit DataCubeVector(long key) 
        : count(1L), sum1(1.0), relations { RingRelation<Idx, K1>(key) } { }

    inline bool isZero() const { return count == 0; }

    DataCubeVector& operator+=(const DataCubeVector& other) {
        if (other.isZero()) return *this;
        count += other.count;
        sum1 += other.sum1;
        std::get<0>(relations) += std::get<0>(other.relations);
        return *this;
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1>();
        return this->multiply(other);
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K1>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K1>();
        return other.multiply(*this);
    }

    template <size_t Idx2>
    DataCubeVector<Idx, K1> multiply(const DataCubeVector<Idx2>& other) const {
        DataCubeVector<Idx, K1> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        return r;
    }

    template <size_t Idx2, typename K2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2>>::type
    operator*(const DataCubeVector<Idx2, K2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K1>>::type
    operator*(const DataCubeVector<Idx2, K2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2, K1>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2>
    DataCubeVector<Idx, K1, K2> multiply(const DataCubeVector<Idx2, K2>& other) const {
        DataCubeVector<Idx, K1, K2> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        return r;
    }

    template <size_t Idx2, typename K2, typename K3>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2, K2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2, typename K3>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K3, K1>>::type
    operator*(const DataCubeVector<Idx2, K2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2, K3, K1>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2, typename K3>
    DataCubeVector<Idx, K1, K2, K3> multiply(const DataCubeVector<Idx2, K2, K3>& other) const {
        DataCubeVector<Idx, K1, K2, K3> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;     
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);        
        std::get<1>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        std::get<4>(r.relations) = std::get<0>(relations).multiply(std::get<1>(other.relations));
        std::get<5>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        std::get<6>(r.relations) = std::get<0>(relations).multiply(std::get<2>(other.relations));      
        return r;
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3, K4>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K2, K3, K4, K1>>::type
    operator*(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K2, K3, K4, K1>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K2, typename K3, typename K4>
    DataCubeVector<Idx, K1, K2, K3, K4> multiply(const DataCubeVector<Idx2, K2, K3, K4>& other) const {
        DataCubeVector<Idx, K1, K2, K3, K4> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<2>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        std::get<4>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        std::get<5>(r.relations) = std::get<0>(relations).multiply(std::get<1>(other.relations));
        std::get<6>(r.relations) = std::get<0>(relations).multiply(std::get<2>(other.relations));
        std::get<7>(r.relations) = std::get<3>(other.relations).multiply(sum1);
        std::get<8>(r.relations) = std::get<4>(other.relations).multiply(sum1);
        std::get<9>(r.relations) = std::get<5>(other.relations).multiply(sum1);
        std::get<10>(r.relations) = std::get<0>(relations).multiply(std::get<3>(other.relations));
        std::get<11>(r.relations) = std::get<0>(relations).multiply(std::get<4>(other.relations));
        std::get<12>(r.relations) = std::get<0>(relations).multiply(std::get<5>(other.relations));
        std::get<13>(r.relations) = std::get<6>(other.relations).multiply(sum1);
        std::get<14>(r.relations) = std::get<0>(relations).multiply(std::get<6>(other.relations));
        return r;
    }

    DataCubeVector operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    DataCubeVector multiply(long alpha) const {
        DataCubeVector<Idx, K1> r;
        if (alpha == 0L) return r;
        r.count = count * alpha;
        r.sum1 = sum1 * alpha;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(alpha);
        return r;
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum1);
        ar << ELEM_SEPARATOR;
        std::get<0>(relations).serialize(ar, version);
    }
};

template <size_t Idx, typename K1, typename K2>
struct DataCubeVector<Idx, K1, K2> {
    long count;
    double sum1;
    std::tuple<RingRelation<Idx, K1>, RingRelation<Idx, K2>, RingRelation<Idx, K1, K2>> relations;
    
    explicit DataCubeVector() : count(0L), sum1(0.0) { }
    
    inline bool isZero() const { return count == 0L; }

    DataCubeVector& operator+=(const DataCubeVector& other) {
        if (other.isZero()) return *this;
        count += other.count;
        sum1 += other.sum1;
        std::get<0>(relations) += std::get<0>(other.relations);
        std::get<1>(relations) += std::get<1>(other.relations);
        std::get<2>(relations) += std::get<2>(other.relations);
        return *this;
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2>();
        return this->multiply(other);
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K1, K2>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K1, K2>();
        return other.multiply(*this);
    }

    template <size_t Idx2>
    DataCubeVector<Idx, K1, K2> multiply(const DataCubeVector<Idx2>& other) const {
        DataCubeVector<Idx, K1, K2> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        return r;
    }

    template <size_t Idx2, typename K3>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K3>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K3, K1, K2>>::type
    operator*(const DataCubeVector<Idx2, K3>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K3, K1, K2>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K3>
    DataCubeVector<Idx, K1, K2, K3> multiply(const DataCubeVector<Idx2, K3>& other) const {
        DataCubeVector<Idx, K1, K2, K3> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        std::get<4>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        std::get<5>(r.relations) = std::get<1>(relations).multiply(std::get<0>(other.relations));
        std::get<6>(r.relations) = std::get<2>(relations).multiply(std::get<0>(other.relations));
        return r;
    }

    template <size_t Idx2, typename K3, typename K4>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3, K4>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K3, typename K4>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K3, K4, K1, K2>>::type
    operator*(const DataCubeVector<Idx2, K3, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K3, K4, K1, K2>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K3, typename K4>
    DataCubeVector<Idx, K1, K2, K3, K4> multiply(const DataCubeVector<Idx2, K3, K4>& other) const {
        DataCubeVector<Idx, K1, K2, K3, K4> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<3>(r.relations) = std::get<1>(other.relations).multiply(sum1);
        std::get<4>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        std::get<5>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        std::get<6>(r.relations) = std::get<0>(relations).multiply(std::get<1>(other.relations));
        std::get<7>(r.relations) = std::get<1>(relations).multiply(std::get<0>(other.relations));
        std::get<8>(r.relations) = std::get<1>(relations).multiply(std::get<1>(other.relations));
        std::get<9>(r.relations) = std::get<2>(other.relations).multiply(sum1);
        std::get<10>(r.relations) = std::get<2>(relations).multiply(std::get<0>(other.relations));
        std::get<11>(r.relations) = std::get<2>(relations).multiply(std::get<1>(other.relations));
        std::get<12>(r.relations) = std::get<0>(relations).multiply(std::get<2>(other.relations));
        std::get<13>(r.relations) = std::get<1>(relations).multiply(std::get<2>(other.relations));
        std::get<14>(r.relations) = std::get<2>(relations).multiply(std::get<2>(other.relations));
        return r;
    }

    DataCubeVector operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    DataCubeVector multiply(long alpha) const {
        DataCubeVector<Idx, K1, K2> r;
        if (alpha == 0L) return r;
        r.count = count * alpha;
        r.sum1 = sum1 * alpha;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(alpha);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(alpha);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(alpha);
        return r;
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum1);
        ar << ELEM_SEPARATOR;
        std::get<0>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<1>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<2>(relations).serialize(ar, version);
    }

};

template <size_t Idx, typename K1, typename K2, typename K3>
struct DataCubeVector<Idx, K1, K2, K3> {
    long count;
    double sum1;
    std::tuple<
        RingRelation<Idx, K1>, RingRelation<Idx, K2>, RingRelation<Idx, K3>, 
        RingRelation<Idx, K1, K2>, RingRelation<Idx, K1, K3>, RingRelation<Idx, K2, K3>, 
        RingRelation<Idx, K1, K2, K3>
    > relations;
    
    explicit DataCubeVector() : count(0L), sum1(0.0) { }
    
    inline bool isZero() const { return count == 0L; }

    DataCubeVector& operator+=(const DataCubeVector& other) {
        if (other.isZero()) return *this;
        count += other.count;
        sum1 += other.sum1;
        std::get<0>(relations) += std::get<0>(other.relations);
        std::get<1>(relations) += std::get<1>(other.relations);
        std::get<2>(relations) += std::get<2>(other.relations);
        std::get<3>(relations) += std::get<3>(other.relations);
        std::get<4>(relations) += std::get<4>(other.relations);
        std::get<5>(relations) += std::get<5>(other.relations);
        std::get<6>(relations) += std::get<6>(other.relations);
        return *this;
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3>();
        return this->multiply(other);
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K1, K2, K3>();
        return other.multiply(*this);
    }

    template <size_t Idx2>
    DataCubeVector<Idx, K1, K2, K3> multiply(const DataCubeVector<Idx2>& other) const {
        DataCubeVector<Idx, K1, K2, K3> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        std::get<3>(r.relations) = std::get<3>(relations).multiply(other.sum1);
        std::get<4>(r.relations) = std::get<4>(relations).multiply(other.sum1);
        std::get<5>(r.relations) = std::get<5>(relations).multiply(other.sum1);
        std::get<6>(r.relations) = std::get<6>(relations).multiply(other.sum1);
        return r;
    }

    template <size_t Idx2, typename K4>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3, K4>>::type
    operator*(const DataCubeVector<Idx2, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3, K4>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename K4>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K4, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2, K4>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K4, K1, K2, K3>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename K4>
    DataCubeVector<Idx, K1, K2, K3, K4> multiply(const DataCubeVector<Idx2, K4>& other) const {
        DataCubeVector<Idx, K1, K2, K3, K4> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        std::get<3>(r.relations) = std::get<0>(other.relations).multiply(sum1);
        std::get<4>(r.relations) = std::get<3>(relations).multiply(other.sum1);
        std::get<5>(r.relations) = std::get<4>(relations).multiply(other.sum1);
        std::get<6>(r.relations) = std::get<0>(relations).multiply(std::get<0>(other.relations));
        std::get<7>(r.relations) = std::get<5>(relations).multiply(other.sum1);
        std::get<8>(r.relations) = std::get<1>(relations).multiply(std::get<0>(other.relations));
        std::get<9>(r.relations) = std::get<2>(relations).multiply(std::get<0>(other.relations));
        std::get<10>(r.relations) = std::get<6>(relations).multiply(other.sum1);
        std::get<11>(r.relations) = std::get<3>(relations).multiply(std::get<0>(other.relations));
        std::get<12>(r.relations) = std::get<4>(relations).multiply(std::get<0>(other.relations));
        std::get<13>(r.relations) = std::get<5>(relations).multiply(std::get<0>(other.relations));
        std::get<14>(r.relations) = std::get<6>(relations).multiply(std::get<0>(other.relations));
        return r;
    }

    DataCubeVector operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    DataCubeVector multiply(long alpha) const {
        DataCubeVector<Idx, K1, K2, K3> r;
        if (alpha == 0L) return r;
        r.count = count * alpha;
        r.sum1 = sum1 * alpha;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(alpha);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(alpha);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(alpha);
        std::get<3>(r.relations) = std::get<3>(relations).multiply(alpha);
        std::get<4>(r.relations) = std::get<4>(relations).multiply(alpha);
        std::get<5>(r.relations) = std::get<5>(relations).multiply(alpha);
        std::get<6>(r.relations) = std::get<6>(relations).multiply(alpha);
        return r;
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum1);
        ar << ELEM_SEPARATOR;
        std::get<0>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<1>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<2>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<3>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<4>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<5>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<6>(relations).serialize(ar, version);
    }

};

template <size_t Idx, typename K1, typename K2, typename K3, typename K4>
struct DataCubeVector<Idx, K1, K2, K3, K4> {
    long count;
    double sum1;
    std::tuple<
        RingRelation<Idx, K1>, RingRelation<Idx, K2>, RingRelation<Idx, K3>, RingRelation<Idx, K4>, 
        RingRelation<Idx, K1, K2>, RingRelation<Idx, K1, K3>, RingRelation<Idx, K1, K4>, 
        RingRelation<Idx, K2, K3>, RingRelation<Idx, K2, K4>, RingRelation<Idx, K3, K4>, 
        RingRelation<Idx, K1, K2, K3>, RingRelation<Idx, K1, K2, K4>, RingRelation<Idx, K1, K3, K4>, RingRelation<Idx, K2, K3, K4>,
        RingRelation<Idx, K1, K2, K3, K4>
    > relations;
    
    explicit DataCubeVector() : count(0L), sum1(0.0) { }
    
    inline bool isZero() const { return count == 0L; }

    DataCubeVector& operator+=(const DataCubeVector& other) {
        if (other.isZero()) return *this;
        count += other.count;
        sum1 += other.sum1;
        std::get<0>(relations) += std::get<0>(other.relations);
        std::get<1>(relations) += std::get<1>(other.relations);
        std::get<2>(relations) += std::get<2>(other.relations);
        std::get<3>(relations) += std::get<3>(other.relations);
        std::get<4>(relations) += std::get<4>(other.relations);
        std::get<5>(relations) += std::get<5>(other.relations);
        std::get<6>(relations) += std::get<6>(other.relations);
        std::get<7>(relations) += std::get<7>(other.relations);
        std::get<8>(relations) += std::get<8>(other.relations);
        std::get<9>(relations) += std::get<9>(other.relations);
        std::get<10>(relations) += std::get<10>(other.relations);
        std::get<11>(relations) += std::get<11>(other.relations);
        std::get<12>(relations) += std::get<12>(other.relations);
        std::get<13>(relations) += std::get<13>(other.relations);
        std::get<14>(relations) += std::get<14>(other.relations);
        return *this;
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx < Idx2), DataCubeVector<Idx, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx, K1, K2, K3>();
        return this->multiply(other);
    }

    template <size_t Idx2>
    typename std::enable_if<(Idx > Idx2), DataCubeVector<Idx2, K1, K2, K3>>::type
    operator*(const DataCubeVector<Idx2>& other) const {
        if (isZero() || other.isZero()) return DataCubeVector<Idx2, K1, K2, K3>();
        return other.multiply(*this);
    }

    template <size_t Idx2>
    DataCubeVector<Idx, K1, K2, K3> multiply(const DataCubeVector<Idx2>& other) const {
        DataCubeVector<Idx, K1, K2, K3> r;
        r.count = count * other.count;
        r.sum1 = sum1 * other.sum1;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(other.sum1);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(other.sum1);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(other.sum1);
        std::get<3>(r.relations) = std::get<3>(relations).multiply(other.sum1);
        std::get<4>(r.relations) = std::get<4>(relations).multiply(other.sum1);
        std::get<5>(r.relations) = std::get<5>(relations).multiply(other.sum1);
        std::get<6>(r.relations) = std::get<6>(relations).multiply(other.sum1);
        std::get<7>(r.relations) = std::get<7>(relations).multiply(other.sum1);
        std::get<8>(r.relations) = std::get<8>(relations).multiply(other.sum1);
        std::get<9>(r.relations) = std::get<9>(relations).multiply(other.sum1);
        std::get<10>(r.relations) = std::get<10>(relations).multiply(other.sum1);
        std::get<11>(r.relations) = std::get<11>(relations).multiply(other.sum1);
        std::get<12>(r.relations) = std::get<12>(relations).multiply(other.sum1);
        std::get<13>(r.relations) = std::get<13>(relations).multiply(other.sum1);
        std::get<14>(r.relations) = std::get<14>(relations).multiply(other.sum1);
        return r;
    }

    DataCubeVector operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    DataCubeVector multiply(long alpha) const {
        DataCubeVector<Idx, K1, K2, K3, K4> r;
        if (alpha == 0L) return r;
        r.count = count * alpha;
        r.sum1 = sum1 * alpha;
        std::get<0>(r.relations) = std::get<0>(relations).multiply(alpha);
        std::get<1>(r.relations) = std::get<1>(relations).multiply(alpha);
        std::get<2>(r.relations) = std::get<2>(relations).multiply(alpha);
        std::get<3>(r.relations) = std::get<3>(relations).multiply(alpha);
        std::get<4>(r.relations) = std::get<4>(relations).multiply(alpha);
        std::get<5>(r.relations) = std::get<5>(relations).multiply(alpha);
        std::get<6>(r.relations) = std::get<6>(relations).multiply(alpha);
        std::get<7>(r.relations) = std::get<7>(relations).multiply(alpha);
        std::get<8>(r.relations) = std::get<8>(relations).multiply(alpha);
        std::get<9>(r.relations) = std::get<9>(relations).multiply(alpha);
        std::get<10>(r.relations) = std::get<10>(relations).multiply(alpha);
        std::get<11>(r.relations) = std::get<11>(relations).multiply(alpha);
        std::get<12>(r.relations) = std::get<12>(relations).multiply(alpha);
        std::get<13>(r.relations) = std::get<13>(relations).multiply(alpha);
        std::get<14>(r.relations) = std::get<14>(relations).multiply(alpha);
        return r;
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum1);
        ar << ELEM_SEPARATOR;
        std::get<0>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<1>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<2>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<3>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<4>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<5>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<6>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<7>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<8>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<9>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<10>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<11>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<12>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<13>(relations).serialize(ar, version);
        ar << ELEM_SEPARATOR;
        std::get<14>(relations).serialize(ar, version);
    }

};


template <size_t Idx, typename... Keys>
DataCubeVector<Idx, Keys...> operator*(long alpha, const DataCubeVector<Idx, Keys...>& r) {
    if (alpha == 1L) return r;
    return r.multiply(alpha);
}

template <size_t Idx, typename... Keys>
DataCubeVector<Idx, Keys...> operator*(long alpha, DataCubeVector<Idx, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);
}

// LIFTING FUNCTION
template <size_t Idx, typename T>
DataCubeVector<Idx, typename std::remove_cv<T>::type> Uliftgroupby(T& arg) {
    return DataCubeVector<Idx, typename std::remove_cv<T>::type>(arg);
}

template <size_t Idx, typename T>
DataCubeVector<Idx> Uliftsum(T& arg) {
    return DataCubeVector<Idx>(arg);
}

template <size_t Idx, typename... Keys>
using DataCube = DataCubeVector<Idx, Keys...>;

#endif /* RINGDATACUBE_HPP */