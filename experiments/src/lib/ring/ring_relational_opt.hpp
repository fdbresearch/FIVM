#ifndef RINGRELATIONAL_OPT_HPP
#define RINGRELATIONAL_OPT_HPP

#include <vector>
#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <typename... Keys>
using Vector = std::vector<std::tuple<std::tuple<Keys...>, long>>; 

template <typename... Keys>
using Map = std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>>; 

template <size_t, typename...>
struct Accumulator;

template <size_t, typename>
struct FactoryAccumulator;

template <size_t Idx, typename... Keys>
struct RelationVector {
    
    Vector<Keys...> store;

    explicit RelationVector(const Keys&... keys, long value)
        : store(Vector<Keys...> { std::make_pair(std::make_tuple(keys...), value) }) { }

    inline bool isZero() const { return store.empty(); }        
};

template <size_t Idx, typename... Keys>
struct RelationMap {

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
    RelationMap& operator+=(const Accumulator<Idx, Args...>& acc) {
        if (!acc.isZero()) acc.apply(this->store);
        return *this;
    }

    RelationMap& operator=(const RelationMap& other) {
        this->store.clear();
        for (auto &it : other.store)
            this->store[it.first] = it.second;
        return *this;
    }

    template <typename... Args>
    RelationMap& operator=(const Accumulator<Idx, Args...>& acc) {
        this->store.clear();
        if (!acc.isZero()) acc.apply(this->store);
        return *this;
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx < Idx2), Accumulator<Idx, const RelationMap<Idx, Keys...>&, const RelationMap<Idx2, Keys2...>&>>::type
    operator*(const RelationMap<Idx2, Keys2...>& other) {
        return FactoryAccumulator<Idx, RelationMap<Idx, Keys...>>::create(*this) * 
                    FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(other);
    }

    template <size_t Idx2, typename... Keys2>
    typename std::enable_if<(Idx > Idx2), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&, const RelationMap<Idx, Keys...>&>>::type
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

// Define a type which holds an unsigned integer value 
template <size_t> struct Int { };

template <size_t Idx, typename... T>
struct Accumulator {
    long scale;
    std::tuple<T...> values;

    Accumulator(T&&... t, long s) : scale(s), values(std::forward<T>(t)...) { }
    Accumulator(std::tuple<T...>&& t, long s) : scale(s), values(std::forward<decltype(t)>(t)) { }

    template <typename Target>
    inline void apply(Target& target) const {
        apply(target, std::tuple<>(), scale, Int<sizeof...(T)>());
    }

    inline bool isZero() const { return scale == 0L || isZero(Int<sizeof...(T)>()); }

private:
    template <typename Target, typename Key, size_t N>
    inline void apply(Target& target, Key&& key, long value, Int<N>) const {
        for (auto &it : std::get<N-1>(values).store) {
            apply(target, tuple_cat(std::get<0>(it), std::forward<decltype(key)>(key)), std::get<1>(it) * value, Int<N-1>());
        }
    }

    template <typename Target, typename Key>
    inline void apply(Target& target, Key&& key, long value, Int<1>) const {
        for (auto &it : std::get<0>(values).store) {
            auto t = std::tuple_cat(std::get<0>(it), key);
            target[t] += std::get<1>(it) * value;
            if (target[t] == 0L) target.erase(t);
        }
    }

    template <size_t N>
    inline bool isZero(Int<N>) const {
        return std::get<N-1>(values).isZero() || isZero(Int<N-1>());
    }

    inline bool isZero(Int<1>) const {
        return std::get<0>(values).isZero();
    }
};

template <size_t Idx, typename T>
struct FactoryAccumulator {

    static Accumulator<Idx, T> create(T&& t, long scale = 1L) {
        return Accumulator<Idx, T>(std::forward<T>(t), scale);
    }

    static Accumulator<Idx, const T&> create(const T& t, long scale = 1L) {
        return Accumulator<Idx, const T&>(t, scale);
    }
};

// ACCUMULATOR v LONG
template <size_t Idx, typename... Keys>
Accumulator<Idx, Keys...> operator*(long a, Accumulator<Idx, Keys...>&& b) {
    if (a == 1L) return std::move(b);
    return Accumulator<Idx, Keys...>(std::forward<decltype(b.values)>(b.values), b.scale * a);
}

template <size_t Idx, typename... Keys>
Accumulator<Idx, Keys...> operator*(Accumulator<Idx, Keys...>&& a, long b) {
    if (b == 1L) return std::move(a);
    return Accumulator<Idx, Keys...>(std::forward<decltype(a.values)>(a.values), a.scale * b);
}

// ACCUMULATOR v RELATION MAP
template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, Keys..., const RelationMap<Idx2, Keys2...>&>>::type
operator*(Accumulator<Idx, Keys...>&& a, const RelationMap<Idx2, Keys2...>& b) {
    return std::forward<decltype(a)>(a) * 
                FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b);
}

template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, Keys..., const RelationMap<Idx2, Keys2...>&>>::type
operator*(const RelationMap<Idx2, Keys2...>& b, Accumulator<Idx, Keys...>&& a) {
    return std::forward<decltype(a)>(a) * 
                FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b);
}

template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx2 < Idx), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&, Keys...>>::type
operator*(Accumulator<Idx, Keys...>&& a, const RelationMap<Idx2, Keys2...>& b) {
    return FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b) *
                std::forward<decltype(a)>(a);
}

template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx2 < Idx), Accumulator<Idx2, const RelationMap<Idx2, Keys2...>&, Keys...>>::type
operator*(const RelationMap<Idx2, Keys2...>& b, Accumulator<Idx, Keys...>&& a) {
    return FactoryAccumulator<Idx2, RelationMap<Idx2, Keys2...>>::create(b) *
                std::forward<decltype(a)>(a);
}

// ACCUMULATOR v ACCUMULATOR
template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx < Idx2), Accumulator<Idx, Keys..., Keys2...>>::type
operator*(Accumulator<Idx, Keys...>&& a, Accumulator<Idx2, Keys2...>&& b) {
    return Accumulator<Idx, Keys..., Keys2...>(
            std::tuple_cat( std::forward<decltype(a.values)>(a.values),
                            std::forward<decltype(b.values)>(b.values) ), 
            a.scale * b.scale);
}

template <size_t Idx, typename... Keys, size_t Idx2, typename... Keys2>
typename std::enable_if<(Idx > Idx2), Accumulator<Idx2, Keys2..., Keys...>>::type
operator*(Accumulator<Idx, Keys...>&& a, Accumulator<Idx2, Keys2...>&& b) {
    return Accumulator<Idx2, Keys2..., Keys...>(
            std::tuple_cat( std::forward<decltype(b.values)>(b.values),
                            std::forward<decltype(a.values)>(a.values) ),
            a.scale * b.scale);
}

// LIFTING FUNCTION
template <size_t Idx, typename... Args>
Accumulator<Idx, RelationVector<Idx, Args...>> Ulift(Args&... args) {
    return FactoryAccumulator<Idx, RelationVector<Idx, Args...>>::create(RelationVector<Idx, Args...>(args..., 1L));
}

template <size_t Idx, typename... Keys>
using RingRelation = RelationMap<Idx, Keys...>;

#endif /* RINGRELATIONAL_OPT_HPP */