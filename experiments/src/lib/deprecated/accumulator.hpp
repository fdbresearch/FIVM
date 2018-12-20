#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include<tuple>

template <typename... T>
struct Accumulator {
    std::tuple<T...> values;
    Accumulator(T&&... t) : values(std::forward<T>(t)...) { }
    Accumulator(std::tuple<T...>&& t) : values(std::forward<decltype(t)>(t)) { }
};

template <typename T>
struct FactoryAccumulator {

    static Accumulator<T> create(T&& t) {
        return Accumulator<T>(std::forward<T>(t));
    }

    static Accumulator<const T&> create(const T& t) {
        return Accumulator<const T&>(t);
    }
};

// Define a type which holds an unsigned integer value 
template <size_t> struct Int { };

template <typename Accumulator, typename Target, typename Key, size_t N>
inline void accumulatorApply(const Accumulator& acc, Target& target, Key&& key, long value, Int<N>) {
    for (auto &it : std::get<N-1>(acc).store) {
        accumulatorApply(acc, target, 
            tuple_cat(std::get<0>(it), std::forward<decltype(key)>(key)), 
                std::get<1>(it) * value, Int<N-1>());
    }
}

template <typename Accumulator, typename Target, typename Key>
inline void accumulatorApply(const Accumulator& acc, Target& target, Key&& key, long value, Int<1>) {
    for (auto &it : std::get<0>(acc).store) {
        auto t = std::tuple_cat(std::get<0>(it), key);
        target[t] += std::get<1>(it) * value;
        if (target[t] == 0L) target.erase(t);
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

#endif /* ACCUMULATOR_HPP */