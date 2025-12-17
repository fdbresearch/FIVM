#ifndef RINGFACTORIZED_HPP
#define RINGFACTORIZED_HPP

#include <array>
#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename... Keys>
using SingletonArray = std::array<std::tuple<std::tuple<Keys...>, int64_t>, 1>;

template <typename... Keys>
using Map = std::unordered_map<std::tuple<Keys...>, int64_t, hash_tuple::hash<std::tuple<Keys...>>>;

template <size_t, typename>
struct Accumulator;

template <size_t, typename>
struct FactoryAccumulator;

template <size_t Idx, typename... Keys>
struct SingletonRelation {
    SingletonArray<Keys...> store;
    static int64_t count;

    explicit SingletonRelation(const Keys&... keys, int64_t value)
        : store { std::make_pair(std::make_tuple(keys...), value) } { }

    inline bool isZero() const { return false; }
};

template <size_t Idx, typename... Keys>
int64_t SingletonRelation<Idx, Keys...>::count = 1;

template <size_t Idx, typename... Keys>
struct RelationMap {
    Map<Keys...> store;
    int64_t count;

    explicit RelationMap() : count(0) { }

    RelationMap(Map<Keys...>&& s, int64_t c) : store(std::forward<decltype(s)>(s)), count(c) { }

    inline bool isZero() const { return count == 0; }

    RelationMap& operator+=(const RelationMap& other) {
        count += other.count;
        for (auto &it : other.store) {
            store[it.first] += it.second;
            if (store[it.first] == 0L) store.erase(it.first);
        }
        return *this;
    }

    template <typename... Args>
    RelationMap& operator+=(const Accumulator<Idx, Args...>& acc) {
        if (!acc.isZero()) acc.apply(*this);
        return *this;
    }

    RelationMap& operator=(const RelationMap& other) {
        store.clear();
        count = other.count;
        for (auto &it : other.store)
            store[it.first] = it.second;
        return *this;
    }

    template <typename... Args>
    RelationMap& operator=(const Accumulator<Idx, Args...>& acc) {
        store.clear();
        count = 0;
        if (!acc.isZero()) acc.apply(*this);
        return *this;
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), Accumulator<Idx, const RelationMap<Idx, Keys...>&>>::type
    operator*(const RelationMap<Idx2, Keys2...>& other) {
        return FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(*this) *
                    FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&>>::type
    operator*(const RelationMap<Idx2, Keys2...>& other) {
        return FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(other) *
                    FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(*this);
    }

    Accumulator<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha) const {
        return FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(*this, alpha);
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
Accumulator<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha, const RelationMap<Idx, Keys...>& r) {
    return FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(r, alpha);
}

template <size_t Idx, typename... Keys>
Accumulator<Idx, const RelationMap<Idx, Keys...>&> operator*(long alpha, RelationMap<Idx, Keys...>&& r) {
    return FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(std::forward<decltype(r)>(r), alpha);
}

template <size_t Idx, typename T>
struct Accumulator {
    T value;
    int64_t scale;

    Accumulator(T&& t, int64_t s) : value(std::forward<T>(t)), scale(s) { }

    template <typename Target>
    inline void apply(Target& target) const {
        if (scale == 0) return;
        for (auto &it : value.store) {
            target.count += std::get<1>(it) * scale;
            target.store[std::get<0>(it)] += std::get<1>(it) * scale;
        }
    }

    inline bool isZero() const { return scale == 0; }
};

template <size_t Idx, typename T>
struct FactoryAccumulator {

    static Accumulator<Idx, T> create(T&& t, int64_t scale = 1) {
        return Accumulator<Idx, T>(std::forward<T>(t), t.count > 0 ? scale : 0L);
    }

    static Accumulator<Idx, const T&> create(const T& t, int64_t scale = 1) {
        return Accumulator<Idx, const T&>(t, t.count > 0 ? scale : 0);
    }
};

// ACCUMULATOR v LONG
template <size_t Idx, typename T>
Accumulator<Idx, T> operator*(int64_t a, Accumulator<Idx, T>&& b) {
    if (a == 1) return std::move(b);
    return Accumulator<Idx, T>(std::forward<decltype(b.value)>(b.value), b.scale * a);
}

template <size_t Idx, typename T>
Accumulator<Idx, T> operator*(Accumulator<Idx, T>&& a, int64_t b) {
    if (b == 1) return std::move(a);
    return Accumulator<Idx, T>(std::forward<decltype(a.value)>(a.value), a.scale * b);
}

// ACCUMULATOR v RELATION MAP
template <size_t Idx, typename T, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, T>>::type
operator*(Accumulator<Idx, T>&& a, const RelationMap<Idx2, Keys2...>& b) {
    return std::forward<decltype(a)>(a) *
                FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b);
}

template <size_t Idx, typename T, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, T>>::type
operator*(const RelationMap<Idx2, Keys2...>& b, Accumulator<Idx, T>&& a) {
    return std::forward<decltype(a)>(a) *
                FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b);
}

template <size_t Idx, typename T, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx2 < Idx), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&>>::type
operator*(Accumulator<Idx, T>&& a, const RelationMap<Idx2, Keys2...>& b) {
    return FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b) *
                std::forward<decltype(a)>(a);
}

template <size_t Idx, typename T, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx2 < Idx), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&>>::type
operator*(const RelationMap<Idx2, Keys2...>& b, Accumulator<Idx, T>&& a) {
    return FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b) *
                std::forward<decltype(a)>(a);
}

// ACCUMULATOR v ACCUMULATOR
template <size_t Idx, typename T, size_t Idx2, typename T2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, T>>::type
operator*(Accumulator<Idx, T>&& a, Accumulator<Idx2, T2>&& b) {
    return Accumulator<Idx, T>(std::forward<decltype(a.value)>(a.value), a.scale * b.scale);
}

template <size_t Idx, typename T, size_t Idx2, typename T2>
typename std::enable_if<(Idx > Idx2), Accumulator<Idx2, T2>>::type
operator*(Accumulator<Idx, T>&& a, Accumulator<Idx2, T2>&& b) {
    return Accumulator<Idx2, T2>(std::forward<decltype(b.value)>(b.value), b.scale * a.scale);
}

// LIFTING FUNCTION
template <size_t Idx, typename... Args>
Accumulator<Idx, SingletonRelation<Idx, Args...>> Ulift(Args&... args) {
    return FactoryAccumulator<Idx, SingletonRelation<Idx, Args...>>::create(SingletonRelation<Idx, Args...>(args..., 1L));
}

template <size_t Idx, typename... Keys>
using RingFactorizedRelation = RelationMap<Idx, Keys...>;

#endif /* RINGRELATIONAL_HPP */