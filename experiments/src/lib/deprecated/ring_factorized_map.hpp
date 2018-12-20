#ifndef RINGFACTORIZED_HPP
#define RINGFACTORIZED_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <size_t IDX, typename... Keys>
struct RingFactorizedRelation {
    static const RingFactorizedRelation zero;

    std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>> store;

    explicit RingFactorizedRelation() { }

    explicit RingFactorizedRelation(const Keys&... keys) {
        store[std::make_tuple(keys...)] = 1L;
    }

    inline bool operator==(const RingFactorizedRelation& other) const {
        return store == other.store;
    }

    inline bool isZero() const {
        return store.empty();
    }

    RingFactorizedRelation& operator+=(const RingFactorizedRelation& other) {
        for (auto it : other.store)
            store[it.first] += it.second;
        return *this;
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), RingFactorizedRelation<IDX, Keys...>>::type
      operator*(const RingFactorizedRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return RingFactorizedRelation<IDX, Keys...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), RingFactorizedRelation<IDX2, Keys2...>>::type
      operator*(const RingFactorizedRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return RingFactorizedRelation<IDX2, Keys2...>::zero;
        return other.multiply(*this);        
    }

    template <size_t IDX2, typename... Keys2>
    RingFactorizedRelation<IDX, Keys...> multiply(const RingFactorizedRelation<IDX2, Keys2...>& other) const {
        RingFactorizedRelation<IDX, Keys...> r;
        size_t count2 = other.store.size();
        for (auto it1 : store) 
            r.store[it1.first] = it1.second * count2;
        return r;
    }

    RingFactorizedRelation operator*(long int alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha); 
    }

    RingFactorizedRelation multiply(long int alpha) const {
        RingFactorizedRelation<IDX, Keys...> r;
        for (auto it : store)
            r.store[it.first] = it.second * alpha;
        return r; 
    }

    void clear() { store.clear(); }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, store.size());
        for (auto it : store) {
            ar << ELEM_SEPARATOR;
            ar << it.first;
            ar << ELEM_SEPARATOR;
            DBT_SERIALIZATION_NVP(ar, it.second);
        }          
    }
};

template <size_t IDX, typename... Keys>
const RingFactorizedRelation<IDX, Keys...> RingFactorizedRelation<IDX, Keys...>::zero = RingFactorizedRelation<IDX, Keys...>();

template <size_t IDX, typename... Keys>
RingFactorizedRelation<IDX, Keys...> operator*(long int alpha, const RingFactorizedRelation<IDX, Keys...>& r) {
    if (alpha == 1L) return r;
    return r.multiply(alpha);
}

template <size_t IDX, typename... Args>
RingFactorizedRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
    return RingFactorizedRelation<IDX, Args...>(args...);
}

#endif /* RINGFACTORIZED_HPP */