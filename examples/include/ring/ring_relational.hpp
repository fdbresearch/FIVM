#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <vector>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename... Keys>
using Vector = std::vector<std::tuple<std::tuple<Keys...>, long>>;

template <typename... Keys>
using Map = std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>>;

template <size_t Idx, typename... Keys>
struct RingRelation;

template <size_t Idx, typename... Keys>
struct VectorRelation {
    Vector<Keys...> store;

    explicit VectorRelation() { }

    explicit VectorRelation(size_t n) {
        store.reserve(n);
    }

    explicit VectorRelation(const std::tuple<Keys...>& key)
        : store { std::make_tuple(key, 1L) } { }

    inline bool isZero() const { return store.empty(); }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), VectorRelation<Idx, Keys..., Keys2...>>::type
    operator*(const RingRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx, Keys..., Keys2...>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), VectorRelation<Idx, Keys..., Keys2...>>::type
    operator*(const VectorRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx, Keys..., Keys2...>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), VectorRelation<Idx2, Keys2..., Keys...>>::type
    operator*(const RingRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx2, Keys2..., Keys...>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), VectorRelation<Idx2, Keys2..., Keys...>>::type
    operator*(const VectorRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx2, Keys2..., Keys...>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename... Keys2>
    VectorRelation<Idx, Keys..., Keys2...> multiply(const VectorRelation<Idx2, Keys2...>& other) const {
        VectorRelation<Idx, Keys..., Keys2...> r(store.size() * other.store.size());
        for (auto &t1 : store)
            for (auto &t2 : other.store)
                r.store.push_back(
                    std::make_tuple(
                        std::tuple_cat(std::get<0>(t1), std::get<0>(t2)),
                        std::get<1>(t1) * std::get<1>(t2)));
        return r;
    }

    template <size_t Idx2, typename... Keys2>
    VectorRelation<Idx, Keys..., Keys2...> multiply(const RingRelation<Idx2, Keys2...>& other) const {
        VectorRelation<Idx, Keys..., Keys2...> r(store.size() * other.store.size());
        for (auto &t2 : other.store)
            for (auto &t1 : store)
                r.store.push_back(
                    std::make_tuple(
                        std::tuple_cat(std::get<0>(t1), t2.first),
                        std::get<1>(t1) * t2.second));
        return r;
    }

    VectorRelation operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    VectorRelation multiply(long alpha) const {
        VectorRelation<Idx, Keys...> r(store.size());
        for (auto &t : store)
            r.store.push_back(std::make_tuple(std::get<0>(t), std::get<1>(t) * alpha));
        return r;
    }
};

template <size_t Idx, typename... Keys>
VectorRelation<Idx, Keys...> operator*(long alpha, VectorRelation<Idx, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);
}

template <size_t Idx, typename... Keys>
struct RingRelation {
    Map<Keys...> store;

    explicit RingRelation() { }

    template <typename K, typename... Ks>
    explicit RingRelation(const K& k, const Ks&... ks)
        : store { { std::make_tuple(k, ks...), 1L } } { }

    explicit RingRelation(const std::tuple<Keys...>& keys)
        : store { { keys, 1L } } { }

    inline bool isZero() const { return store.empty(); }

    RingRelation& operator+=(const RingRelation& other) {
        if (other.isZero()) return *this;
        for (auto &it : other.store) {
            store[it.first] += it.second;
            if (store[it.first] == 0L) store.erase(it.first);
        }
        return *this;
    }

    RingRelation& operator+=(const VectorRelation<Idx, const Keys...>& other) {
        if (other.isZero()) return *this;
        for (auto &t : other.store) {
            store[std::get<0>(t)] += std::get<1>(t);
            if (store[std::get<0>(t)] == 0L) store.erase(std::get<0>(t));
        }
        return *this;
    }

    RingRelation& operator+=(const VectorRelation<Idx, Keys...>& other) {
        if (other.isZero()) return *this;
        for (auto &t : other.store) {
            store[std::get<0>(t)] += std::get<1>(t);
            if (store[std::get<0>(t)] == 0L) store.erase(std::get<0>(t));
        }
        return *this;
    }

    template <size_t Idx2>
    RingRelation& operator=(const RingRelation<Idx2, Keys...> & other) {
        store = other.store;
        return *this;
    }

    RingRelation& operator=(const VectorRelation<Idx, Keys...>& other) {
        store.clear();
        for (auto &t : other.store)
            store[std::get<0>(t)] += std::get<1>(t);
        return *this;
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), VectorRelation<Idx, Keys..., Keys2...>>::type
    operator*(const RingRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx, Keys..., Keys2...>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), VectorRelation<Idx, Keys..., Keys2...>>::type
    operator*(const VectorRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx, Keys..., Keys2...>();
        return this->multiply(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), VectorRelation<Idx2, Keys2..., Keys...>>::type
    operator*(const RingRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx2, Keys2..., Keys...>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), VectorRelation<Idx2, Keys2..., Keys...>>::type
    operator*(const VectorRelation<Idx2, Keys2...>& other) const {
        if (isZero() || other.isZero()) return VectorRelation<Idx2, Keys2..., Keys...>();
        return other.multiply(*this);
    }

    template <size_t Idx2, typename... Keys2>
    VectorRelation<Idx, Keys..., Keys2...> multiply(const RingRelation<Idx2, Keys2...>& other) const {
        VectorRelation<Idx, Keys..., Keys2...> r;
        for (auto &it1 : store)
            for (auto &it2 : other.store)
                r.store.push_back(std::make_tuple(
                    std::tuple_cat(it1.first, it2.first),
                    it1.second * it2.second));
        return r;
    }

    template <size_t Idx2, typename... Keys2>
    VectorRelation<Idx, Keys..., Keys2...> multiply(const VectorRelation<Idx2, Keys2...>& other) const {
        VectorRelation<Idx, Keys..., Keys2...> r;
        for (auto &it1 : store)
            for (auto &t2 : other.store)
                r.store.push_back(std::make_tuple(
                    std::tuple_cat(it1.first, std::get<0>(t2)),
                    it1.second * std::get<1>(t2)));
        return r;
    }

    RingRelation operator*(long alpha) const {
        if (alpha == 1L) return *this;
        return multiply(alpha);
    }

    RingRelation multiply(long alpha) const {
        RingRelation<Idx, Keys...> r;
        if (alpha == 0L) return r;

        for (auto &it : store)
            r.store[it.first] = it.second * alpha;
        return r;
    }

    void clear() { store.clear(); }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, store.size());
        for (auto &it : store) {
            ar << ELEM_SEPARATOR;
            ar << it.first << " -> " << it.second;
        }
    }
};

template <size_t Idx, typename... Keys>
RingRelation<Idx, Keys...> operator*(long alpha, const RingRelation<Idx, Keys...>& r) {
    if (alpha == 1L) return r;
    return r.multiply(alpha);
}

template <size_t Idx, typename... Keys>
RingRelation<Idx, Keys...> operator*(long alpha, RingRelation<Idx, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);
}

template <size_t Idx, typename... Args>
VectorRelation<Idx, typename std::remove_cv<Args>::type...> Ulift(Args&... args) {
    return VectorRelation<Idx, typename std::remove_cv<Args>::type...>(std::make_tuple(args...));
}

#endif /* RINGRELATIONAL_HPP */