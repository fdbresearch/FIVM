#ifndef DBTOASTER_TYPES_HPP
#define DBTOASTER_TYPES_HPP

// #include <stdexcept>
#include "string.hpp"

// #define DOUBLE_ZERO_APPROXIMATED
// #define DOUBLE_ZERO_THRESHOLD 1e-8

namespace dbtoaster
{
    typedef long date;

    typedef long DATE_TYPE;

    typedef double DOUBLE_TYPE;

    typedef size_t HASH_RES_t;

    //typedef RefCountedString STRING_TYPE;

    typedef PooledRefCountedString STRING_TYPE;

    //typedef VariableLengthString STRING_TYPE;

    //typedef FixedLengthString<10> STRING_TYPE;

    //typedef std::string STRING_TYPE;

    template<typename V>
    struct ZeroValue 
    {
        static const V zero;
        FORCE_INLINE static const V get() { return V::zero; }
        FORCE_INLINE static bool isZero(const V& a) { return (a == V::zero); }
    };
    template<typename V>
    const V ZeroValue<V>::zero = V::zero;

    template<>
    struct ZeroValue<int> 
    {
        static const int zero;
        FORCE_INLINE static const int get() { return 0; }
        FORCE_INLINE static bool isZero(const int& a) { return (a == 0); }
    };
    const int ZeroValue<int>::zero = 0;

    template<>
    struct ZeroValue<long> 
    {
        static const long zero;
        FORCE_INLINE static const long get() { return 0L; }
        FORCE_INLINE static bool isZero(const long& a) { return (a == 0L); }
    };
    const long ZeroValue<long>::zero = 0L;

    template<>
    struct ZeroValue<float> 
    {
        static const float zero;
        FORCE_INLINE static const float get() { return 0.0f; }
        FORCE_INLINE static bool isZero(const float& a) { return (a == 0.0f); }
    };
    const float ZeroValue<float>::zero = 0.0f;

    template<>
    struct ZeroValue<double> 
    {
        static const double zero;
        FORCE_INLINE static double get() { return 0.0; }
    #ifdef DOUBLE_ZERO_APPROXIMATED
        FORCE_INLINE static bool isZero(const double& a) { return (a >= -DOUBLE_ZERO_THRESHOLD && a <= DOUBLE_ZERO_THRESHOLD); }
    #else
        FORCE_INLINE static bool isZero(const double& a) { return (a == 0.0); }
    #endif
    };
    const double ZeroValue<double>::zero = 0.0;

}

#endif /* DBTOASTER_TYPES_HPP */
