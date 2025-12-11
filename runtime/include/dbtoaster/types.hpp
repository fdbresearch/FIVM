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

#include <cstdint> 

#include "string.hpp"

// #define DOUBLE_ZERO_APPROXIMATED
// #define DOUBLE_ZERO_THRESHOLD 1e-8

namespace dbtoaster {

    typedef int32_t DATE_TYPE;

    typedef size_t HASH_TYPE;

    typedef std::string STRING_TYPE;

    // typedef PooledRefCountedString STRING_TYPE;

    //typedef RefCountedString STRING_TYPE;

    //typedef VariableLengthString STRING_TYPE;

    //typedef FixedLengthString<10> STRING_TYPE;

    template<typename T>
    struct Value {
        static const T zero;
        inline static bool isZero(const T& a) { return a.isZero(); }
    };
    template<typename T> const T Value<T>::zero = T();

    template<>
    struct Value<int8_t> {
        static const int8_t zero;
        inline static bool isZero(const int8_t& a) { return (a == 0); }
    };
    const int8_t Value<int8_t>::zero = 0;

    template<>
    struct Value<int16_t> {
        static const int16_t zero;
        inline static bool isZero(const int16_t& a) { return (a == 0); }
    };
    const int16_t Value<int16_t>::zero = 0;

    template<>
    struct Value<int32_t> {
        static const int32_t zero;
        inline static bool isZero(const int32_t& a) { return (a == 0); }
    };
    const int32_t Value<int32_t>::zero = 0;

    template<>
    struct Value<int64_t> {
        static const int64_t zero;
        inline static bool isZero(const int64_t& a) { return (a == 0L); }
    };
    const int64_t Value<int64_t>::zero = 0L;

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

    template<>
    struct Value<char> {
        static const char zero;
        inline static bool isZero(const char& a) { return (a == 0); }
    };
    const char Value<char>::zero = 0;

}

#endif /* DBTOASTER_TYPES_HPP */
