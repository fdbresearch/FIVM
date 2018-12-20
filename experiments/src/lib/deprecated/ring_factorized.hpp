#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"
#include "accumulator.hpp"

using namespace dbtoaster;

template <typename... Keys>
using Vector = std::vector<std::tuple<std::tuple<Keys...>, long>>; 

template <typename... Keys>
using Map = std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>>; 

template <typename T>
using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

// Define a type which holds an unsigned integer value 
template <size_t> struct Int { };

template <typename Accumulator, typename Target, size_t N>
inline void accumulatorApply(const Accumulator& acc, Target& target, long value, Int<N>) {
    accumulatorApply(acc, target, value * std::get<N-1>(acc).store.size(), Int<N-1>());
}

template <typename Accumulator, typename Target>
inline void accumulatorApply(const Accumulator& acc, Target& target, long value, Int<1>) {
    for (auto &it : std::get<0>(acc).store) {
        target[std::get<0>(it)] += std::get<1>(it) * value;
    }
}

template <typename Accumulator, size_t N>
inline bool accumulatorIsZero(const Accumulator& acc, Int<N>) {
    return std::get<N-1>(acc).isZero() || accumulatorIsZero(acc, Int<N-1>());
}

template <typename Accumulator>
inline bool accumulatorIsZero(const Accumulator& acc, Int<1>) {
    return std::get<0>(acc).isZero();
}

template <size_t Id, typename Store>
struct Relation {
    Store store;

    static constexpr size_t getId() { return Id; }

    explicit Relation() { }
    Relation(Store&& s) : store(std::forward<Store>(s)) { }

    inline bool isZero() const {
        return store.empty();
    }

    inline bool operator==(const Relation& other) const {
        return store == other.store;
    }    
};

template <size_t Id, typename... Keys>
struct RelationVector : public Relation<Id, Vector<Keys...>> {

    RelationVector(const Keys&... keys, long value)
        : Relation<Id, Vector<Keys...>>(Vector<Keys...> { std::make_pair(std::make_tuple(keys...), value) }) { }
};

template <size_t Id, typename... Keys>
struct RelationMap : public Relation<Id, Map<Keys...>> {

    static const RelationMap zero;

    explicit RelationMap() : Relation<Id, Map<Keys...>>() { }

    RelationMap& operator+=(const RelationMap& other) {
        for (auto &it : other.store)
            this->store[it.first] += it.second;
        return *this;
    }

    template <typename... Args2>
    RelationMap& operator+=(const Accumulator<Args2...>& acc) {
        if (!accumulatorIsZero(acc.values, Int<sizeof...(Args2)>()))
            accumulatorApply(acc.values, this->store, 1L, Int<sizeof...(Args2)>());
        return *this;
    }

    RelationMap& operator=(const RelationMap& other) {
        this->store.clear();
        for (auto &it : other.store)
            this->store[it.first] += it.second;
        return *this;
    }

    template <typename... Args2>
    RelationMap& operator=(const Accumulator<Args2...>& acc) {
        this->store.clear();
        if (!accumulatorIsZero(acc.values, Int<sizeof...(Args2)>()))
            accumulatorApply(acc.values, this->store, 1L, Int<sizeof...(Args2)>());
        return *this;
    }

    template <size_t Id2, typename... Keys2>
    typename std::enable_if<(Id < Id2), Accumulator<const RelationMap<Id, Keys...>&, const RelationMap<Id2, Keys2...>&>>::type
    operator*(const RelationMap<Id2, Keys2...>& other) {
        return FactoryAccumulator<RelationMap<Id, Keys...>>::create(*this) * 
                    FactoryAccumulator<RelationMap<Id2, Keys2...>>::create(other);
    }

    template <size_t Id2, typename... Keys2>
    typename std::enable_if<(Id > Id2), Accumulator<const RelationMap<Id2, Keys2...>&, const RelationMap<Id, Keys...>&>>::type
    operator*(const RelationMap<Id2, Keys2...>& other) {
        return FactoryAccumulator<RelationMap<Id2, Keys2...>>::create(other) * 
                    FactoryAccumulator<RelationMap<Id, Keys...>>::create(*this);
    }

    template<class Stream>
    void serialize(Stream& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, this->store.size());
        for (auto &it : this->store) {
            ar << ELEM_SEPARATOR;
            ar << it.first;
            ar << ELEM_SEPARATOR;
            DBT_SERIALIZATION_NVP(ar, it.second);
        }
    }
};

template <size_t Id, typename... Keys>
const RelationMap<Id, Keys...> RelationMap<Id, Keys...>::zero = RelationMap<Id, Keys...>();

template <typename... Args>
Accumulator<Args...> operator*(long a, Accumulator<Args...>&& b) {
    return std::forward<decltype(b)>(b);        // ignore a
}

template <typename... Args>
Accumulator<Args...> operator*(Accumulator<Args...>&& a, long b) {
    return std::forward<decltype(a)>(a);        // ignore b
}

template <typename Value, typename... Values, size_t Id2, typename... Keys2>
typename std::enable_if<(base_type<Value>::getId() < Id2), Accumulator<Value, Values..., const RelationMap<Id2, Keys2...>&>>::type
operator*(Accumulator<Value, Values...>&& a, const RelationMap<Id2, Keys2...>& b) {
    return std::forward<decltype(a)>(a) * FactoryAccumulator<RelationMap<Id2, Keys2...>>::create(b);
}

template <typename Value, typename... Values, size_t Id2, typename... Keys2>
typename std::enable_if<(Id2 < base_type<Value>::getId()), Accumulator<const RelationMap<Id2, Keys2...>&, Value, Values...>>::type
operator*(Accumulator<Value, Values...>&& a, const RelationMap<Id2, Keys2...>& b) {
    return FactoryAccumulator<RelationMap<Id2, Keys2...>>::create(b) * std::forward<decltype(a)>(a);
}

template <typename Value, typename... Values, typename Value2, typename... Values2>
typename std::enable_if<(base_type<Value>::getId() < base_type<Value2>::getId()),
                        Accumulator<Value, Values..., Value2, Values2...>>::type
operator*(Accumulator<Value, Values...>&& a, Accumulator<Value2, Values2...>&& b) {
    return Accumulator<Value, Values..., Value2, Values2...>(
            std::tuple_cat( std::forward<decltype(a.values)>(a.values),
                            std::forward<decltype(b.values)>(b.values) ));
}

template <typename Value, typename... Values, typename Value2, typename... Values2>
typename std::enable_if<(base_type<Value>::getId() > base_type<Value2>::getId()),
                        Accumulator<Value2, Values2..., Value, Values...>>::type
operator*(Accumulator<Value, Values...>&& a, Accumulator<Value2, Values2...>&& b) {
    return Accumulator<Value2, Values2..., Value, Values...>(
            std::tuple_cat( std::forward<decltype(b.values)>(b.values),
                            std::forward<decltype(a.values)>(a.values) ));
}

template <typename... Args, size_t Id, typename... Keys>
bool operator==(Accumulator<Args...>&& a, const RelationMap<Id, Keys...>& b) {
    return false;
}

template <typename... Args, size_t Id, typename... Keys>
bool operator==(const Accumulator<Args...>& a, const RelationMap<Id, Keys...>& b) {
    return false;
}

template <size_t Id, typename... Args>
Accumulator<RelationVector<Id, Args...>> Ulift(size_t, Args&... args) {
    return FactoryAccumulator<RelationVector<Id, Args...>>::create(RelationVector<Id, Args...>(args..., 1L));
}

template <size_t Id, typename... Keys>
using RingFactorizedRelation = RelationMap<Id, Keys...>;

#endif /* RINGRELATIONAL_HPP */