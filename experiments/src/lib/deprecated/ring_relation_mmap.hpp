#ifndef RINGRELATIONAL_HPP
#define RINGRELATIONAL_HPP

#include <tuple>
#include <type_traits>
#include "types.hpp"
#include "hash.hpp"
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
RingRelation<IDX, Keys...> operator*(long int alpha, const RingRelation<IDX, Keys...>& r) {
    if (alpha == 1L) return r;
    return r.multiply(alpha);
}

template <size_t IDX, typename... Args>
RingRelation<IDX, Args...> Ulift(size_t idx, Args&... args) {
    return RingRelation<IDX, Args...>(args...);
}

#endif /* RINGRELATIONAL_HPP */