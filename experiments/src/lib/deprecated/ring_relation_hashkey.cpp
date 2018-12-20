#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename T>
struct hasher {
    size_t operator()(T const& t) const {
        return std::hash<T>()(t);
    }
};

template <typename T>
struct hasher<std::tuple<size_t, T>> {
    inline size_t operator()(std::tuple<size_t, T> const& tt) const {
        return std::get<0>(tt);
    }
};

template <size_t IDX, typename... Keys>
struct RingRelation {
    static const RingRelation zero;
    static const hash_tuple::hash<std::tuple<Keys...>> tuple_hasher;

    std::unordered_map<std::tuple<size_t, std::tuple<Keys...>>, long, hasher<std::tuple<size_t, std::tuple<Keys...>>>> store;

    explicit RingRelation() { }

    explicit RingRelation(const Keys&... keys) 
        : store { { std::make_tuple(tuple_hasher(std::make_tuple(keys...)), std::make_tuple(keys...)), 1L } } { }

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
            for (auto it2 : other.store) {
                size_t h1 = std::get<0>(it1.first);
                size_t h2 = std::get<0>(it2.first);
                h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
                r.store[std::make_tuple(h1, std::tuple_cat(std::get<1>(it1.first), std::get<1>(it2.first)))] = it1.second * it2.second;
            }
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
            ar << std::get<1>(it.first);
            ar << ELEM_SEPARATOR;
            DBT_SERIALIZATION_NVP(ar, it.second);
        }          
    }
};

template <size_t IDX, typename... Keys>
const RingRelation<IDX, Keys...> RingRelation<IDX, Keys...>::zero = RingRelation<IDX, Keys...>();

template <size_t IDX, typename... Keys>
const hash_tuple::hash<std::tuple<Keys...>> RingRelation<IDX, Keys...>::tuple_hasher = hash_tuple::hash<std::tuple<Keys...>>();

template <size_t IDX, typename... Keys>
RingRelation<IDX, Keys...> operator*(long int alpha, const RingRelation<IDX, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);
}

template <size_t IDX, typename... Args>
RingRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
    return RingRelation<IDX, Args...>(args...);
}

// template <size_t IDX, typename... Keys>
// struct RingRelation {
//     static const RingRelation zero;

//     std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>> store;

//     explicit RingRelation() { }

//     explicit RingRelation(const Keys&... keys) : store { {std::make_tuple(keys...), 1L} } { }

//     inline bool operator==(const RingRelation& other) const {
//         return store == other.store;
//     }

//     inline bool isZero() const {
//         return store.empty();
//     }

//     RingRelation& operator+=(const RingRelation& other) {
//         for (auto it : other.store)
//             store[it.first] += it.second;
//         return *this;
//     }

//     template <size_t IDX2, typename... Keys2>
//     typename std::enable_if<(IDX < IDX2), RingRelation<IDX, Keys..., Keys2...>>::type
//       operator*(const RingRelation<IDX2, Keys2...>& other) const {
//         if (isZero() || other.isZero())
//             return RingRelation<IDX, Keys..., Keys2...>::zero;
//         return this->multiply(other);
//     }

//     template <size_t IDX2, typename... Keys2>
//     typename std::enable_if<(IDX > IDX2), RingRelation<IDX2, Keys2..., Keys...>>::type
//       operator*(const RingRelation<IDX2, Keys2...>& other) const {
//         if (isZero() || other.isZero())
//             return RingRelation<IDX2, Keys2..., Keys...>::zero;
//         return other.multiply(*this);        
//     }

//     template <size_t IDX2, typename... Keys2>
//     RingRelation<IDX, Keys..., Keys2...> multiply(const RingRelation<IDX2, Keys2...>& other) const {
//         RingRelation<IDX, Keys..., Keys2...> r;
//         for (auto it1 : store) 
//             for (auto it2 : other.store)
//                 r.store[std::tuple_cat(it1.first, it2.first)] = it1.second * it2.second;
//         return r;
//     }

//     RingRelation operator*(long int alpha) const {
//         if (alpha == 1L) return *this;        
//         return multiply(alpha); 
//     }

//     RingRelation multiply(long int alpha) const {
//         RingRelation<IDX, Keys...> r;
//         for (auto it : store)
//             r.store[it.first] = it.second * alpha;
//         return r; 
//     }

//     void clear() { store.clear(); }

//     template<class Archive>
//     void serialize(Archive& ar, const unsigned int version) const {
//         ar << ELEM_SEPARATOR;
//         DBT_SERIALIZATION_NVP(ar, store.size());
//         for (auto it : store) {
//             ar << ELEM_SEPARATOR;
//             ar << it.first;
//             ar << ELEM_SEPARATOR;
//             DBT_SERIALIZATION_NVP(ar, it.second);
//         }          
//     }
// };

// template <size_t IDX, typename... Keys>
// const RingRelation<IDX, Keys...> RingRelation<IDX, Keys...>::zero = RingRelation<IDX, Keys...>();

// template <size_t IDX, typename... Keys>
// RingRelation<IDX, Keys...> operator*(long int alpha, const RingRelation<IDX, Keys...>&& r) {
//     if (alpha == 1L) return std::move(r);
//     return r.multiply(alpha);
// }

// template <size_t IDX, typename... Args>
// RingRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
//     return RingRelation<IDX, Args...>(args...);
// }

#endif /* RINGRELATIONAL_HPP */