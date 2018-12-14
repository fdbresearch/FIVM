#ifndef DBTOASTER_TYPES_HPP
#define DBTOASTER_TYPES_HPP

#include "string.hpp"

// #define DOUBLE_ZERO_APPROXIMATED
// #define DOUBLE_ZERO_THRESHOLD 1e-8

namespace dbtoaster
{
    typedef long date;

    typedef long DATE_TYPE;

    typedef double DOUBLE_TYPE;

    typedef size_t HASH_RES_t;

    typedef PooledRefCountedString STRING_TYPE;

    //typedef RefCountedString STRING_TYPE;

    //typedef VariableLengthString STRING_TYPE;

    //typedef FixedLengthString<10> STRING_TYPE;

    //typedef std::string STRING_TYPE;

    template<typename T>
    struct Value {
        static const T zero;
        inline static bool isZero(const T& a) { return a.isZero(); }
    };

    template<typename T>
    const T Value<T>::zero = T();

    template<>
    struct Value<int> {
        static constexpr int zero = 0;
        inline static bool isZero(const int& a) { return (a == 0); }
    };

    template<>
    struct Value<char> {
        static constexpr char zero = 0;
        inline static bool isZero(const char& a) { return (a == 0); }
    };

    template<>
    struct Value<short> {
        static constexpr short zero = 0;
        inline static bool isZero(const short& a) { return (a == 0); }
    };

    template<>
    struct Value<long> {
        static constexpr long zero = 0L;
        inline static bool isZero(const long& a) { return (a == 0L); }
    };

    template<>
    struct Value<float> {
        static constexpr float zero = 0.0f;
        inline static bool isZero(const float& a) { return (a == 0.0f); }
    };

    template<>
    struct Value<double> {
        static constexpr double zero = 0.0;
#ifdef DOUBLE_ZERO_APPROXIMATED
        inline static bool isZero(const double& a) { return (a >= -DOUBLE_ZERO_THRESHOLD && a <= DOUBLE_ZERO_THRESHOLD); }
#else
        inline static bool isZero(const double& a) { return (a == 0.0); }
#endif        
    };
}

#endif /* DBTOASTER_TYPES_HPP */
