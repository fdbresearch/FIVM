#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <size_t IDX, typename... Keys>
struct RingRelation {
    static const RingRelation zero;

    std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>> store;

    explicit RingRelation() { }

    explicit RingRelation(const Keys&... keys) {
        store[std::make_tuple(keys...)] = 1L;
    }

    inline bool operator==(const RingRelation& other) const {
        return store == other.store;
    }

    inline bool isZero() const {
        return store.empty();
    }

    RingRelation& operator+=(const RingRelation& other) {
        for (auto it : other.store)
            store[it.first] += it.second;
        return *this;
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), RingRelation<IDX, Keys..., Keys2...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return RingRelation<IDX, Keys..., Keys2...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), RingRelation<IDX2, Keys2..., Keys...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return RingRelation<IDX2, Keys2..., Keys...>::zero;
        return other.multiply(*this);        
    }

    template <size_t IDX2, typename... Keys2>
    RingRelation<IDX, Keys..., Keys2...> multiply(const RingRelation<IDX2, Keys2...>& other) const {
        RingRelation<IDX, Keys..., Keys2...> r;
        for (auto it1 : store) 
            for (auto it2 : other.store)
                r.store[std::tuple_cat(it1.first, it2.first)] = it1.second * it2.second;
        return r;
    }

    RingRelation operator*(long int alpha) const {
        if (alpha == 1L) return *this;        
        return multiply(alpha); 
    }

    RingRelation multiply(long int alpha) const {
        RingRelation<IDX, Keys...> r;
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
const RingRelation<IDX, Keys...> RingRelation<IDX, Keys...>::zero = RingRelation<IDX, Keys...>();

template <size_t IDX, typename... Keys>
RingRelation<IDX, Keys...> operator*(long int alpha, const RingRelation<IDX, Keys...>& r) {
    if (alpha == 1L) return r;
    return r.multiply(alpha);
}

template <size_t IDX, typename... Args>
RingRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
    return RingRelation<IDX, Args...>(args...);
}

#endif /* RINGRELATIONAL_HPP */