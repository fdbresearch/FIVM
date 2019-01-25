//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_TYPES_HPP
#define DBTOASTER_TYPES_HPP

#include "string.hpp"

// #define DOUBLE_ZERO_APPROXIMATED
// #define DOUBLE_ZERO_THRESHOLD 1e-8

namespace dbtoaster {

    typedef long date;

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
    template<typename T> const T Value<T>::zero = T();

    template<>
    struct Value<signed char> {
        static const signed char zero;
        inline static bool isZero(const signed char& a) { return (a == 0); }
    };
    const signed char Value<signed char>::zero = 0;

    template<>
    struct Value<short> {
        static const short zero;
        inline static bool isZero(const short& a) { return (a == 0); }
    };
    const short Value<short>::zero = 0;

    template<>
    struct Value<int> {
        static const int zero;
        inline static bool isZero(const int& a) { return (a == 0); }
    };
    const int Value<int>::zero = 0;

    template<>
    struct Value<long> {
        static const long zero;
        inline static bool isZero(const long& a) { return (a == 0L); }
    };
    const long Value<long>::zero = 0L;

    template<>
    struct Value<char> {
        static const char zero;
        inline static bool isZero(const char& a) { return (a == 0); }
    };
    const char Value<char>::zero = 0;

    template<>
    struct Value<float> {
        static const float zero;
        inline static bool isZero(const float& a) { return (a == 0.0f); }
    };
    const float Value<float>::zero = 0.0f;

    template<>
    struct Value<double> {
        static const double zero;
#ifdef DOUBLE_ZERO_APPROXIMATED
        inline static bool isZero(const double& a) { return (a >= -DOUBLE_ZERO_THRESHOLD && a <= DOUBLE_ZERO_THRESHOLD); }
#else
        inline static bool isZero(const double& a) { return (a == 0.0); }
#endif
    };
    const double Value<double>::zero = 0.0;
}

#endif /* DBTOASTER_TYPES_HPP */
