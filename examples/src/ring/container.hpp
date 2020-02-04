#ifndef CONTAINER_HPP
#define CONTAINER_HPP

// Define a type which holds an unsigned integer value
template <size_t> struct Int { };

template <size_t Idx, typename... Values>
struct Container {
    std::tuple<Values...> values;
    long scale;

    Container(Values&&... vs, long s) : values(std::forward<Values>(vs)...), scale(s) { }

    Container(std::tuple<Values...>&& t, long s) : values(std::forward<decltype(t)>(t)), scale(s) { }

    inline bool isZero() const { return scale == 0L; }
};

template <size_t Idx>
struct Container<Idx> {
    inline bool isZero() const { return true; }
};

template <size_t Idx, typename Value>
struct FactoryContainer {
    static Container<Idx, Value> create(Value&& v, long scale = 1L) {
        return Container<Idx, Value>(std::forward<Value>(v), v.isZero() ? 0L : scale);
    }

    static Container<Idx, const Value&> create(const Value& v, long scale = 1L) {
        return Container<Idx, const Value&>(v, v.isZero() ? 0L : scale);
    }
};

// CONTAINER and LONG
template <size_t Idx, typename... Values>
Container<Idx, Values...> operator*(long a, Container<Idx, Values...>&& b) {
    if (a == 1L) return std::move(b);
    return Container<Idx, Values...>(std::forward<decltype(b.values)>(b.values), b.scale * a);
}

template <size_t Idx, typename... Values>
Container<Idx, Values...> operator*(Container<Idx, Values...>&& a, long b) {
    if (b == 1L) return std::move(a);
    return Container<Idx, Values...>(std::forward<decltype(a.values)>(a.values), a.scale * b);
}

// CONTAINER and VALUE
template <size_t Idx, typename... Values, typename V>
typename std::enable_if<(Idx < V::INDEX), Container<Idx, Values..., const V&>>::type
operator*(Container<Idx, Values...>&& a, const V& b) {
    return std::forward<decltype(a)>(a) * FactoryContainer<V::INDEX, V>::create(b);
}

template <size_t Idx, typename... Values, typename V>
typename std::enable_if<(Idx < V::INDEX), Container<Idx, Values..., const V&>>::type
operator*(const V& b, Container<Idx, Values...>&& a) {
    return std::forward<decltype(a)>(a) * FactoryContainer<V::INDEX, V>::create(b);
}

template <size_t Idx, typename... Values, typename V>
typename std::enable_if<(V::INDEX < Idx), Container<V::INDEX, const V&, Values...>>::type
operator*(Container<Idx, Values...>&& a, const V& b) {
    return FactoryContainer<V::INDEX, V>::create(b) * std::forward<decltype(a)>(a);
}

template <size_t Idx, typename... Values, typename V>
typename std::enable_if<(V::INDEX < Idx), Container<V::INDEX, const V&, Values...>>::type
operator*(const V& b, Container<Idx, Values...>&& a) {
    return FactoryContainer<V::INDEX, V>::create(b) * std::forward<decltype(a)>(a);
}

// CONTAINER and CONTAINER
template <size_t Idx, typename... Values, size_t Idx2, typename... Values2>
typename std::enable_if<(Idx < Idx2), Container<Idx, Values..., Values2...>>::type
operator*(Container<Idx, Values...>&& a, Container<Idx2, Values2...>&& b) {
    return Container<Idx, Values..., Values2...>(
            std::tuple_cat(std::forward<decltype(a.values)>(a.values),
                           std::forward<decltype(b.values)>(b.values)),
            a.scale * b.scale);
}

template <size_t Idx, typename... Values, size_t Idx2, typename... Values2>
typename std::enable_if<(Idx > Idx2), Container<Idx2, Values2..., Values...>>::type
operator*(Container<Idx, Values...>&& a, Container<Idx2, Values2...>&& b) {
    return Container<Idx2, Values2..., Values...>(
            std::tuple_cat(std::forward<decltype(b.values)>(b.values),
                           std::forward<decltype(a.values)>(a.values)),
            a.scale * b.scale);
}

#endif /* CONTAINER_HPP */