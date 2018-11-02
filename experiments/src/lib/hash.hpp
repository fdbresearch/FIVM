#ifndef DBTOASTER_HASH_HPP
#define DBTOASTER_HASH_HPP

#include <tuple>
#include "macro.hpp"

namespace dbtoaster 
{
    uint32_t MurmurHash2(const void* key, int len, uint32_t seed);

    std::hash<double> double_hasher;

    template <class T>
    FORCE_INLINE void hash_combine(size_t& seed, const T& v)
    {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <>
    FORCE_INLINE void hash_combine(size_t& seed, const long& v)
    {
        seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <>
    FORCE_INLINE void hash_combine(size_t& seed, const double& v)
    {
        double v_local = v;     // passing &v introduces bugs (!?)
        seed ^= MurmurHash2(&v_local, sizeof(double), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        // seed ^= double_hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<size_t SZ>
    FORCE_INLINE void hash_combine(size_t& seed, const FixedLengthString<SZ>& v)
    {        
        seed ^= MurmurHash2(v.data_, sizeof(char) * (v.size_ - 1), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<>
    FORCE_INLINE void hash_combine(size_t& seed, const VariableLengthString& v)
    {
        seed ^= MurmurHash2(v.data_, sizeof(char) * (v.size_ - 1), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<>
    FORCE_INLINE void hash_combine(size_t& seed, const RefCountedString& v)
    {
        seed ^= MurmurHash2(v.data_, sizeof(char) * (v.size_ - 1), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<>
    FORCE_INLINE void hash_combine(size_t& seed, const PooledRefCountedString& v)
    {
        seed ^= MurmurHash2(v.data_, sizeof(char) * (v.size_ - 1), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    FORCE_INLINE uint32_t MurmurHash2(const void* key, int len, uint32_t seed)
    {
        // 'm' and 'r' are mixing constants generated offline.
        // They're not really 'magic', they just happen to work well.
        const uint32_t m = 0x5bd1e995;
        const int r = 24;

        // Initialize the hash to a 'random' value
        uint32_t h = seed ^ len;

        // Mix 4 bytes at a time into the hash
        const unsigned char* data = (const unsigned char*)key;

        while (len >= 4)
        {
            uint32_t k = *(uint32_t*)data;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        // Handle the last few bytes of the input array
        switch (len)
        {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
            h *= m;
        };

        // Do a few final mixes of the hash to ensure the last few
        // bytes are well-incorporated.
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }
}

namespace hash_tuple {

    template <typename T>
    struct hash {
        size_t operator()(T const& t) const {
            return std::hash<T>()(t);
        }
    };

    template <class T>
    inline void hash_combine(std::size_t& seed, T const& v) {
        seed ^= hash_tuple::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    // Recursive template code derived from Matthieu M.
    template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
    struct HashValueImpl {
      static void apply(size_t& seed, Tuple const& tuple) {
        HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
      }
    };

    template <class Tuple>
    struct HashValueImpl<Tuple, 0> {
      static void apply(size_t& seed, Tuple const& tuple) {
        hash_combine(seed, std::get<0>(tuple));
      }
    };

    template <typename ... TT>
    struct hash<std::tuple<TT...>> {
        size_t operator()(std::tuple<TT...> const& tt) const {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }
    };
}

#endif /* DBTOASTER_HASH_HPP */
