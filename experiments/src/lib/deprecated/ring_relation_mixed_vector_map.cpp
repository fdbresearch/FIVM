#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

template <size_t IDX, typename... Keys>
struct RingRelation;

template <size_t IDX, typename... Keys>
struct VectorRelation {  
    static const VectorRelation zero;

    std::vector<std::tuple<std::tuple<Keys...>, long>> store;

    explicit VectorRelation() { }

    explicit VectorRelation(const Keys&... keys)
        : store { std::make_tuple(std::make_tuple(keys...), 1L) } { }

    inline bool operator==(const VectorRelation& other) const {
        return store == other.store;
    }

    inline bool operator==(const RingRelation<IDX, Keys...>& other) const {
        return store.size() == other.store.size();
    }

    inline bool isZero() const {
        return store.empty();
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), VectorRelation<IDX, Keys..., Keys2...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX, Keys..., Keys2...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), VectorRelation<IDX2, Keys2..., Keys...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX2, Keys2..., Keys...>::zero;
        return other.multiply(*this);        
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), VectorRelation<IDX, Keys..., Keys2...>>::type
      operator*(const VectorRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX, Keys..., Keys2...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), VectorRelation<IDX2, Keys2..., Keys...>>::type
      operator*(const VectorRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX2, Keys2..., Keys...>::zero;
        return other.multiply(*this);        
    }   

    template <size_t IDX2, typename... Keys2>
    VectorRelation<IDX, Keys..., Keys2...> multiply(const VectorRelation<IDX2, Keys2...>& other) const {
        VectorRelation<IDX, Keys..., Keys2...> r;
        for (auto &t1 : store)
            for (auto &t2 : other.store)
                r.store.push_back(
                    std::make_tuple(
                        std::tuple_cat(std::get<0>(t1), std::get<0>(t2)),
                        std::get<1>(t1) * std::get<1>(t2)
                    ));
        return r;
    }

    template <size_t IDX2, typename... Keys2>
    VectorRelation<IDX, Keys..., Keys2...> multiply(const RingRelation<IDX2, Keys2...>& other) const {
        VectorRelation<IDX, Keys..., Keys2...> r;
        for (auto &t2 : other.store)
            for (auto &t1 : store)
                r.store.push_back(
                    std::make_tuple(
                        std::tuple_cat(std::get<0>(t1), t2.first),
                        std::get<1>(t1) * t2.second
                    ));
        return r;
    }

    VectorRelation multiply(long int alpha) const {
        VectorRelation<IDX, Keys...> r;
        for (auto &t : store)
            r.store.push_back(std::make_tuple(std::get<0>(t), std::get<1>(t) * alpha));
        return r; 
    }

    // template <
    //     size_t IDX2, typename... Keys2,
    //     size_t IDX3, typename... Keys3,
    //     size_t IDX4, typename... Keys4,
    //     size_t IDX5, typename... Keys5,
    //     size_t IDX6, typename... Keys6,
    //     size_t IDX7, typename... Keys7
    // >
    // VectorRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6..., Keys7...>
    //     multiply7(
    //         const RingRelation<IDX2, Keys2...>& other2,
    //         const RingRelation<IDX3, Keys3...>& other3,
    //         const RingRelation<IDX4, Keys4...>& other4,
    //         const RingRelation<IDX5, Keys5...>& other5,
    //         const RingRelation<IDX6, Keys6...>& other6,
    //         const RingRelation<IDX7, Keys7...>& other7) const {
    //     VectorRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6..., Keys7...> r;
    //     for (auto &it1 : store) 
    //         for (auto &it2 : other2.store)
    //             for (auto &it3 : other3.store)
    //                 for (auto &it4 : other4.store)
    //                     for (auto &it5 : other5.store)
    //                         for (auto &it6 : other6.store)
    //                             for (auto &it7 : other7.store)
    //                                 r.store.push_back(std::make_tuple(
    //                                     std::tuple_cat(
    //                                         std::get<0>(it1), 
    //                                         it2.first,
    //                                         it3.first,
    //                                         it4.first,
    //                                         it5.first,
    //                                         it6.first,
    //                                         it7.first
    //                                     ), 
    //                                     std::get<1>(it1) * 
    //                                     it2.second *
    //                                     it3.second *
    //                                     it4.second *
    //                                     it5.second *
    //                                     it6.second *
    //                                     it7.second
    //                                 ));
    //     return r;
    // }

};

template <size_t IDX, typename... Keys>
const VectorRelation<IDX, Keys...> VectorRelation<IDX, Keys...>::zero = VectorRelation<IDX, Keys...>();

template <size_t IDX, typename... Keys>
VectorRelation<IDX, Keys...> operator*(long int alpha, VectorRelation<IDX, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);    
}


template <size_t IDX, typename... Keys>
struct RingRelation {

    static const RingRelation zero;

    std::unordered_map<std::tuple<Keys...>, long, hash_tuple::hash<std::tuple<Keys...>>> store;

    explicit RingRelation() { }

    inline bool operator==(const RingRelation& other) const {
        return store == other.store;
    }

    inline bool isZero() const {
        return store.empty();
    }

    RingRelation& operator+=(const RingRelation& other) {
        for (auto &it : other.store)
            store[it.first] += it.second;
        return *this;
    }

    RingRelation& operator+=(const VectorRelation<IDX, Keys...>& other) {
        for (auto &t : other.store)
            store[std::get<0>(t)] += std::get<1>(t);
        return *this;
    }

    RingRelation& operator=(const VectorRelation<IDX, Keys...>& other) {
        store.clear();
        for (auto &t : other.store)
            store[std::get<0>(t)] = std::get<1>(t);
        return *this;
    }    
    
    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), VectorRelation<IDX, Keys..., Keys2...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX, Keys..., Keys2...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), VectorRelation<IDX2, Keys2..., Keys...>>::type
      operator*(const RingRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX2, Keys2..., Keys...>::zero;
        return other.multiply(*this);        
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX < IDX2), VectorRelation<IDX, Keys..., Keys2...>>::type
      operator*(const VectorRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX, Keys..., Keys2...>::zero;
        return this->multiply(other);
    }

    template <size_t IDX2, typename... Keys2>
    typename std::enable_if<(IDX > IDX2), VectorRelation<IDX2, Keys2..., Keys...>>::type
      operator*(const VectorRelation<IDX2, Keys2...>& other) const {
        if (isZero() || other.isZero())
            return VectorRelation<IDX2, Keys2..., Keys...>::zero;
        return other.multiply(*this);        
    }

    template <size_t IDX2, typename... Keys2>
    VectorRelation<IDX, Keys..., Keys2...> multiply(const RingRelation<IDX2, Keys2...>& other) const {        
        VectorRelation<IDX, Keys..., Keys2...> r;
        for (auto &it1 : store) 
            for (auto &it2 : other.store)
                r.store.push_back(std::make_tuple(
                    std::tuple_cat(it1.first, it2.first), 
                    it1.second * it2.second));
        return r;
    }

    template <size_t IDX2, typename... Keys2>
    VectorRelation<IDX, Keys..., Keys2...> multiply(const VectorRelation<IDX2, Keys2...>& other) const {       
        VectorRelation<IDX, Keys..., Keys2...> r;
        for (auto &it1 : store)
            for (auto &t2 : other.store)
                r.store.push_back(std::make_tuple(
                    std::tuple_cat(it1.first, std::get<0>(t2)),
                    it1.second * std::get<1>(t2)));
        return r;
    }

    RingRelation operator*(long int alpha) const {
        if (alpha == 1L) return *this;        
        return multiply(alpha); 
    }

    RingRelation multiply(long int alpha) const {
        RingRelation<IDX, Keys...> r;
        for (auto &it : store)
            r.store[it.first] = it.second * alpha;
        return r; 
    }

    void clear() { store.clear(); }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, store.size());
        for (auto &it : store) {
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
RingRelation<IDX, Keys...> operator*(long int alpha, const RingRelation<IDX, Keys...>&& r) {
    if (alpha == 1L) return std::move(r);
    return r.multiply(alpha);
}

template <size_t IDX, typename... Args>
VectorRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
    return VectorRelation<IDX, Args...>(args...);
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

//     // template <
//     //     size_t IDX2, typename... Keys2,
//     //     size_t IDX3, typename... Keys3,
//     //     size_t IDX4, typename... Keys4,
//     //     size_t IDX5, typename... Keys5,
//     //     size_t IDX6, typename... Keys6
//     // >    
//     // std::vector<std::tuple<std::tuple<Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...>, long>> 
//     //     multiply(
//     //         const RingRelation<IDX2, Keys2...>& other2,
//     //         const RingRelation<IDX3, Keys3...>& other3,
//     //         const RingRelation<IDX4, Keys4...>& other4,
//     //         const RingRelation<IDX5, Keys5...>& other5,
//     //         const RingRelation<IDX6, Keys6...>& other6) const {
//     //     std::vector<std::tuple<std::tuple<Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...>, long>> v;
//     //     for (auto it1 : store) 
//     //         for (auto it2 : other2.store)
//     //             for (auto it3 : other3.store)
//     //                 for (auto it4 : other4.store)
//     //                     for (auto it5 : other5.store)
//     //                         for (auto it6 : other6.store)
//     //                             v.push_back(std::make_tuple(
//     //                                 std::tuple_cat(
//     //                                 it1.first, 
//     //                                 it2.first, 
//     //                                 it3.first,
//     //                                 it4.first,
//     //                                 it5.first,
//     //                                 it6.first), it1.second * it2.second * it3.second * it4.second * it5.second * it6.second));
//     //     return v;
//     // }

//     // template <
//     //     size_t IDX2, typename... Keys2,
//     //     size_t IDX3, typename... Keys3,
//     //     size_t IDX4, typename... Keys4,
//     //     size_t IDX5, typename... Keys5,
//     //     size_t IDX6, typename... Keys6
//     // >    
//     // RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> 
//     //     multiply(
//     //         const RingRelation<IDX2, Keys2...>& other2,
//     //         const RingRelation<IDX3, Keys3...>& other3,
//     //         const RingRelation<IDX4, Keys4...>& other4,
//     //         const RingRelation<IDX5, Keys5...>& other5,
//     //         const RingRelation<IDX6, Keys6...>& other6) const {
//     //     RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> r;
//     //     for (auto it1 : store) 
//     //         for (auto it2 : other2.store)
//     //             for (auto it3 : other3.store)
//     //                 for (auto it4 : other4.store)
//     //                     for (auto it5 : other5.store)
//     //                         for (auto it6 : other6.store)
//     //                             r.store[std::tuple_cat(
//     //                                 it1.first, 
//     //                                 it2.first, 
//     //                                 it3.first,
//     //                                 it4.first,
//     //                                 it5.first,
//     //                                 it6.first)] = it1.second * it2.second * it3.second * it4.second * it5.second * it6.second;
//     //     return r;
//     // }

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