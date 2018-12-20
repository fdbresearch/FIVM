#include<iostream>
#include<tuple>
#include<utility>
#include<vector>

using namespace std;

// Define a type which holds an unsigned integer value 
template<std::size_t> struct int_{};

template <typename Archive, class Tuple, size_t Pos>
Archive& print_tuple(Archive& ar, const Tuple& t, int_<Pos> ) {
  ar << std::get< std::tuple_size<Tuple>::value - Pos>(t) << ", ";
  return print_tuple(ar, t, int_<Pos - 1>());
}

template <typename Archive, class Tuple>
Archive& print_tuple(Archive& ar, const Tuple& t, int_<1> ) {
  return ar << std::get<std::tuple_size<Tuple>::value - 1>(t);
}

template <typename Archive, class... Args>
Archive& operator<<(Archive& ar, const std::tuple<Args...>& t) {
  ar << '('; 
  print_tuple(ar, t, int_<sizeof...(Args)>()); 
  return ar << ')';
}

template <typename... ARGS>
struct Complex {
    std::vector<std::tuple<std::tuple<ARGS...>, long>> store;
};

Complex<bool> fun() { 
    Complex<bool> r;
    r.store.push_back(std::make_tuple(true, 100L));
    return r; 
}

template<class Tuple, class Key, size_t N>
struct TupleBuilder {
    inline static void build(const Tuple& t, Key&& key, long value) {
        for (auto &it : std::get<N-1>(t).store) {
            TupleBuilder<Tuple, decltype(tuple_cat(std::get<0>(it), key)), N-1>::build(t, 
                tuple_cat(std::get<0>(it), key), std::get<1>(it) * value);
        }
    }
};

template<class Tuple, class Key>
struct TupleBuilder<Tuple, Key, 1> {
    inline static void build(const Tuple& t, Key&& key, long value) {
        for (auto &it : std::get<0>(t).store)
            cout << "KEY: "
                 << std::tuple_cat(std::get<0>(it), key) 
                 << "    VALUE: " 
                 << (std::get<1>(it) * value) 
                 << endl;
    }
};

template<typename... Args>
void test(std::tuple<Args...> t) {
    TupleBuilder<decltype(t), std::tuple<>, sizeof...(Args)>::build(t, std::tuple<>(), 1L);
}


template<class Tuple, class Key, size_t N>
struct AggregatorBuilder {
    inline static void build(const Tuple& t, Key&& key, long value) {
        for (auto &it : std::get<N-1>(t).store) {
            AggregatorBuilder<Tuple, decltype(tuple_cat(std::get<0>(it), key)), N-1>::build(t, 
                tuple_cat(std::get<0>(it), key), std::get<1>(it) * value);
        }
    }
};

template<class Tuple, class Key>
struct AggregatorBuilder<Tuple, Key, 1> {
    inline static void build(const Tuple& t, Key&& key, long value) {
        for (auto &it : std::get<0>(t).store)
            cout << "KEY: "
                 << std::tuple_cat(std::get<0>(it), key) 
                 << "    VALUE: " 
                 << (std::get<1>(it) * value) 
                 << endl;
    }
};

template<typename... Args>
void test2(std::tuple<Args...> t) {
    AggregatorBuilder<decltype(t), std::tuple<>, sizeof...(Args)>::build(t, std::tuple<>(), 1L);
}


template <typename... T>
struct Accumulator {
    
    std::tuple<T...> values;

    Accumulator(T&&... t) : values(std::forward<T>(t)...) { }
    Accumulator(std::tuple<T...>&& t) : values(std::forward<std::tuple<T...>>(t)) { }

    template <typename... U>
    inline Accumulator<T..., U...> operator*(Accumulator<U...>&& u) {
        return Accumulator<T..., U...>(std::tuple_cat(values, std::forward<std::tuple<U...>>(u.values)));
    }

    template <typename... U>
    inline Accumulator<T..., U...> operator*(const Accumulator<U...>& u) {
        return Accumulator<T..., U...>(std::tuple_cat(values, u.values));
    }
};

template <size_t ID, typename... T>
struct OrderedAccumulator {
    
    std::tuple<T...> values; 

    OrderedAccumulator(T&&... t) : values(std::forward<T>(t)...) { }
    OrderedAccumulator(std::tuple<T...>&& t) : values(std::forward<std::tuple<T...>>(t)) { }

    template <size_t ID2, typename... U>
    typename std::enable_if<(ID <= ID2), OrderedAccumulator<ID, T..., U...>>::type
    operator*(OrderedAccumulator<ID2, U...>&& u) {
        return OrderedAccumulator<ID, T..., U...>(std::tuple_cat(values, std::forward<std::tuple<U...>>(u.values)));
    }

    template <size_t ID2, typename... U>
    typename std::enable_if<(ID2 < ID), OrderedAccumulator<ID2, U..., T...>>::type
    operator*(OrderedAccumulator<ID2, U...>&& u) {
        return OrderedAccumulator<ID2, U..., T...>(std::tuple_cat(std::forward<std::tuple<U...>>(u.values), values));
    }    

    template <size_t ID2, typename... U>
    typename std::enable_if<(ID < ID2), OrderedAccumulator<ID, T..., U...>>::type
    operator*(const OrderedAccumulator<ID2, U...>& u) {
        return OrderedAccumulator<ID, T..., U...>(std::tuple_cat(values, u.values));
    }

    template <size_t ID2, typename... U>
    typename std::enable_if<(ID2 < ID), OrderedAccumulator<ID2, U..., T...>>::type
    operator*(const OrderedAccumulator<ID2, U...>& u) {
        return OrderedAccumulator<ID2, U..., T...>(std::tuple_cat(u.values, values));
    }
};

struct FactoryAccumulator { 

    // template <typename T>
    // static Accumulator<T> create(T&& t) {
    //     return Accumulator<T>(std::forward<T>(t));
    // }

    // template <typename T>
    // static Accumulator<const T&> create(const T& t) {
    //     return Accumulator<const T&>(t);
    // }    

    template <size_t ID = 0, typename T>
    static OrderedAccumulator<ID, T> create(T&& t) {
        return OrderedAccumulator<ID, T>(std::forward<T>(t));
    }

    template <size_t ID = 0, typename T>
    static OrderedAccumulator<ID, const T&> create(const T& t) {
        return OrderedAccumulator<ID, const T&>(t);
    }
};

int main() {
    auto x_ = Complex<int, long>();
    x_.store.push_back(std::make_tuple(std::make_tuple(23, 455293L), -1L));
    x_.store.push_back(std::make_tuple(std::make_tuple(6, 111293L), 2L));

    auto y_ = Complex<double, char, long>(); 
    y_.store.push_back(std::make_tuple(std::make_tuple(5.394, 'd', 3232L), 1L));
    y_.store.push_back(std::make_tuple(std::make_tuple(-3.4, 'M', 0L), -4L));

    auto x = FactoryAccumulator::create(x_);
    auto y = FactoryAccumulator::create<7>(y_);
    auto z = FactoryAccumulator::create<1>(fun());

    auto p1 = x * y;
    auto p2 = x * p1;
    auto p3 = p2 * FactoryAccumulator::create<1>(fun());

    auto p4 = p1 * FactoryAccumulator::create<2>(fun());
    auto p5 = p4 * p3;
    auto p6 = p4 * (x * y); 

    auto p7 = (x * y) * FactoryAccumulator::create<3>(fun());
    auto p8 = (x * p5) * p4;
    auto p9 = (p3 * p4) * (x * y);

    test2(p9.values);

    return 0;
}