#ifndef RINGRELATIONAL_OPT_HPP
#define RINGRELATIONAL_OPT_HPP

#include <array>
#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"
#include "container.hpp"

using namespace dbtoaster;

template <typename... Keys>
using SingletonArray = std::array<std::tuple<std::tuple<Keys...>, long>, 1>;

template <typename... Keys>
using Map = std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>>;

template <size_t Idx, typename... Keys>
struct SingletonRelation {
    SingletonArray<Keys...> store;
    static long count;

    explicit SingletonRelation(const Keys&... keys, long value)
        : store { std::make_pair(std::make_tuple(keys...), value) } { }

    inline bool isZero() const { return std::get<1>(store[0]) == 0L; }
};

template <size_t Idx, typename... Keys>
long SingletonRelation<Idx, Keys...>::count = 1L;


template <size_t Idx, typename... Keys>
struct RelationMap {
    static constexpr size_t INDEX = Idx;

    Map<Keys...> store;

    explicit RelationMap() { }

    RelationMap(Map<Keys...>&& s) : store(std::forward<decltype(s)>(s)) { }

    inline bool isZero() const { return store.empty(); }

    RelationMap& operator+=(const RelationMap& other) {
        for (auto &it : other.store) {
            this->store[it.first] += it.second;
            if (this->store[it.first] == 0L) this->store.erase(it.first);
        }
        return *this;
    }

    template <typename... Args>
    RelationMap& operator+=(const Container<Idx, Args...>& c) {
        if (!c.isZero()) apply(c);
        return *this;
    }

    RelationMap& operator=(const RelationMap& other) {
        this->store.clear();
        for (auto &it : other.store)
            this->store[it.first] = it.second;
        return *this;
    }

    template <typename... Args>
    RelationMap& operator=(const Container<Idx, Args...>& c) {
        this->store.clear();
        if (!c.isZero()) apply(c);
        return *this;
    }

    template <typename... Args>
    inline void apply(const Container<Idx, Args...>& c) {
        // apply(c, std::tuple<>(), c.scale, Int<sizeof...(Args)>());
        apply2(c, c.scale, Int<sizeof...(Args)>());
    }

    template <size_t N, typename... Args, typename... TupleKeys>
    inline void apply2(const Container<Idx, Args...>& c, long value, Int<N>, TupleKeys&&... keys) {
        for (auto &it : std::get<N-1>(c.values).store) {
            apply2(c, std::get<1>(it) * value, Int<N-1>(), std::get<0>(it), std::forward<decltype(keys)>(keys)...);
        }
    }

    template <typename... Args, typename... TupleKeys>
    inline void apply2(const Container<Idx, Args...>& c, long value, Int<1>, TupleKeys&&... keys) {
        for (auto &it : std::get<0>(c.values).store) {
            auto t = std::tuple_cat(std::get<0>(it), std::forward<decltype(keys)>(keys)...);
            store[t] += std::get<1>(it) * value;
            if (store[t] == 0L) this->store.erase(t);
        }
    }

    template <typename Key, size_t N, typename... Args>
    inline void apply(const Container<Idx, Args...>& c, Key&& key, long value, Int<N>) {
        for (auto &it : std::get<N-1>(c.values).store) {
            apply(c, tuple_cat(std::get<0>(it), std::forward<decltype(key)>(key)), std::get<1>(it) * value, Int<N-1>());
        }
    }

    template <typename Key, typename... Args>
    inline void apply(const Container<Idx, Args...>& c, Key&& key, long value, Int<1>) {
        for (auto &it : std::get<0>(c.values).store) {
            auto t = std::tuple_cat(std::get<0>(it), key);
            store[t] += std::get<1>(it) * value;
            if (store[t] == 0L) this->store.erase(t);
        }
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), Container<Idx, const RelationMap<Idx, Keys...>&, const RelationMap<Idx2, Keys2...>&>>::type
    operator*(const RelationMap<Idx2, Keys2...>& other) {
        return FactoryContainer<Idx, RelationMap<Idx, Keys...>>::create(*this) *
                    FactoryContainer<Idx2, RelationMap<Idx2, Keys2...>>::create(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), Container<Idx2, const RelationMap<Idx2, Keys2...>&, const RelationMap<Idx, Keys...>&>>::type
    operator*(const RelationMap<Idx2, Keys2...>& other) {
        return FactoryContainer<Idx2, RelationMap<Idx2, Keys2...>>::create(other) *
                    FactoryContainer<Idx, RelationMap<Idx, Keys...>>::create(*this);
    }

    Container<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha) const {
        return FactoryContainer<Idx, RelationMap<Idx, Keys...>>::create(*this, alpha);
    }

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
Container<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha, const RelationMap<Idx, Keys...>& r) {
    return FactoryContainer<Idx, RelationMap<Idx, Keys...>>::create(r, alpha);
}

template <size_t Idx, typename... Keys>
Container<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha, RelationMap<Idx, Keys...>&& r) {
    return FactoryContainer<Idx, RelationMap<Idx, Keys...>>::create(std::forward<decltype(r)>(r), alpha);
}

// LIFTING FUNCTION
template <size_t Idx, typename... Args>
Container<Idx, SingletonRelation<Idx, Args...>> Ulift(Args&... args) {
    return FactoryContainer<Idx, SingletonRelation<Idx, Args...>>::create(SingletonRelation<Idx, Args...>(args..., 1L));
}

template <size_t Idx, typename... Keys>
using RingRelation = RelationMap<Idx, Keys...>;

#endif /* RINGRELATIONAL_OPT_HPP */