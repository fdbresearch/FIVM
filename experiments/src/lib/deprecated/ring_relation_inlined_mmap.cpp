#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <unordered_map>
#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

#include "mmap.hpp"

template <size_t IDX, typename... Keys>
struct RingRelation {
    static const RingRelation zero;

    typedef struct Record_ {
        std::tuple<Keys...> keys;
        long __av;
        Record_* nxt;
        Record_* prv;
        explicit Record_() : nxt(nullptr), prv(nullptr) { }
        explicit Record_(const std::tuple<Keys...>&& k) : keys(k), __av(1L), nxt(nullptr), prv(nullptr) { }
    } Record;

    typedef struct {
        inline static size_t hash(const Record& e) {
            return hash_tuple::hash<std::tuple<Keys...>>()(e.keys);
        }
        
        inline static bool equals(const Record& x, const Record& y) {
          return x.keys == y.keys;
        }
    } Record_Index;


    MultiHashMap<Record, long, PrimaryHashIndex<Record, Record_Index>> store;


    explicit RingRelation() { }

    explicit RingRelation(const Keys&... keys) {
        store.insert(Record(std::make_tuple(keys...)));
    }

    inline bool operator==(const RingRelation& other) const {
        return store == other.store;
    }

    inline bool isZero() const {
        return store.count() == 0;
    }

    RingRelation& operator+=(const RingRelation& other) {
        Record* r = other.store.head;
        while (r) {
            store.addOrDelOnZero(*r, r->__av);
            r = r->nxt;
        }
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
        typename RingRelation<IDX, Keys..., Keys2...>::Record record;
        RingRelation<IDX, Keys..., Keys2...> relation;
        
        Record* r1 = store.head;
        while (r1) {
            typename RingRelation<IDX2, Keys2...>::Record* r2 = other.store.head;
            while (r2) {
                record.keys = std::tuple_cat(r1->keys, r2->keys);
                record.__av = r1->__av * r2->__av;
                relation.store.addOrDelOnZero(record, record.__av);
                r2 = r2->nxt;
            }
            r1 = r1->nxt;
        }
        return relation;
    }

    template <
        size_t IDX2, typename... Keys2,
        size_t IDX3, typename... Keys3,
        size_t IDX4, typename... Keys4,
        size_t IDX5, typename... Keys5,
        size_t IDX6, typename... Keys6
    >    
    RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> 
        multiply(
            const RingRelation<IDX2, Keys2...>& other2,
            const RingRelation<IDX3, Keys3...>& other3,
            const RingRelation<IDX4, Keys4...>& other4,
            const RingRelation<IDX5, Keys5...>& other5,
            const RingRelation<IDX6, Keys6...>& other6) const {

        typename RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...>::Record record;
        RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> relation;

        Record* r1 = store.head;
        while (r1) {
            typename RingRelation<IDX2, Keys2...>::Record* r2 = other2.store.head;
            while (r2) {
                typename RingRelation<IDX3, Keys3...>::Record* r3 = other3.store.head;
                while (r3) {
                    typename RingRelation<IDX4, Keys4...>::Record* r4 = other4.store.head;
                    while (r4) {
                        typename RingRelation<IDX5, Keys5...>::Record* r5 = other5.store.head;
                        while (r5) {
                            typename RingRelation<IDX6, Keys6...>::Record* r6 = other6.store.head;
                            while (r6) {
                                record.keys = std::tuple_cat(r1->keys, r2->keys, r3->keys, r4->keys, r5->keys, r6->keys);
                                record.__av = r1->__av * r2->__av * r3->__av * r4->__av * r5->__av * r6->__av;
                                relation.store.addOrDelOnZero(record, record.__av);
                                
                                r6 = r6->nxt;
                            }                            
                            r5 = r5->nxt;
                        }                        
                        r4 = r4->nxt;
                    }                    
                    r3 = r3->nxt;
                }                
                r2 = r2->nxt;
            }
            r1 = r1->nxt;
        }
        return relation;
    }

    RingRelation operator*(long int alpha) const {
        if (alpha == 1L) return *this;        
        return multiply(alpha); 
    }

    RingRelation multiply(long int alpha) const {
        RingRelation relation;
        Record* r = store.head;
        while (r) {            
            relation.store.add(*r, r->__av * alpha);
            r = r->nxt;
        }
        return relation; 
    }

    void clear() { store.clear(); }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, store.count());
        Record* r = store.head;
        while (r) {
            ar << ELEM_SEPARATOR;
            ar << r->keys;
            ar << ELEM_SEPARATOR;
            ar << r->__av;
            r = r->nxt;
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

//     template <
//         size_t IDX2, typename... Keys2,
//         size_t IDX3, typename... Keys3,
//         size_t IDX4, typename... Keys4,
//         size_t IDX5, typename... Keys5,
//         size_t IDX6, typename... Keys6
//     >    
//     RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> 
//         multiply(
//             const RingRelation<IDX2, Keys2...>& other2,
//             const RingRelation<IDX3, Keys3...>& other3,
//             const RingRelation<IDX4, Keys4...>& other4,
//             const RingRelation<IDX5, Keys5...>& other5,
//             const RingRelation<IDX6, Keys6...>& other6) const {
//         RingRelation<IDX, Keys..., Keys2..., Keys3..., Keys4..., Keys5..., Keys6...> r;
//         for (auto it1 : store) 
//             for (auto it2 : other2.store)
//                 for (auto it3 : other3.store)
//                     for (auto it4 : other4.store)
//                         for (auto it5 : other5.store)
//                             for (auto it6 : other6.store)
//                                 r.store[std::tuple_cat(
//                                     it1.first, 
//                                     it2.first, 
//                                     it3.first,
//                                     it4.first,
//                                     it5.first,
//                                     it6.first)] = it1.second * it2.second * it3.second * it4.second * it5.second * it6.second;
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