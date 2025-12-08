//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_SERIALIZATION_HPP
#define DBTOASTER_SERIALIZATION_HPP

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <array>
#include "types.hpp"

#define DBT_SERIALIZATION_NVP_OF_ARRAY(ar, name, length)  dbtoaster::serialize_nvp_array(ar, STRING(name), name, length)
#define DBT_SERIALIZATION_NVP_OF_PTR(ar, name)  dbtoaster::serialize_nvp(ar, STRING(name), *name)
#define DBT_SERIALIZATION_NVP(ar, name)  dbtoaster::serialize_nvp(ar, STRING(name), name)
#define ELEM_SEPARATOR "\n\t\t\t"

namespace dbtoaster {

    // Define a type which holds an unsigned integer value
    template<std::size_t> struct int_{};

    template <typename Archive, class Tuple, size_t Pos>
    Archive& print_tuple(Archive& ar, const Tuple& t, int_<Pos> ) {
      ar << std::setprecision(15) << std::get< std::tuple_size<Tuple>::value - Pos>(t) << ", ";
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

    template <typename T, typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const T & t) {
        t.serialize(ar, version);
        ar << "\n";
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const int8_t & t) {
        ar << (short) t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const int & t) {
        ar << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const size_t & t) {
        ar << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const long & t) {
        ar << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const long long & t) {
        ar << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const STRING_TYPE & t) {
        ar << t.c_str();
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const float & t) {
        ar << std::setprecision(7) << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const double & t) {
        ar << std::setprecision(15) << t;
        return ar;
    }

    template <typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const char & t) {
        ar << t;
        return ar;
    }

    template <typename T, size_t N, typename Archive>
    inline Archive & serialize(Archive & ar, const unsigned int version, const std::array<T, N>& a) {
        ar << ELEM_SEPARATOR << "[";
        for (size_t i = 0; i < N; i++) {
            serialize(ar, 0, a[i]);
            ar << ", ";
        }
        ar << "]";
        return ar;
    }

    template <typename Archive, typename K, typename V, typename H>
    inline Archive & serialize(Archive & ar, const unsigned int version, const std::unordered_map<K,V,H>& m) {
        ar << ELEM_SEPARATOR << "[";
        for (auto &it : m) {
            ar << it.first << " -> ";
            serialize(ar, 0, it.second);
            ar << ", ";
        }
        ar << "]";
        return ar;
    }

    template <typename T, typename Archive>
    inline Archive & serialize_nvp(Archive & ar, const char * name, const T & t) {
        ar << "<"  << name << ">";
        serialize(ar, 0, t);
        ar << "</" << name << ">";
        return ar;
    }

    template <typename T, typename Archive>
    inline Archive & serialize_nvp_array(Archive & ar, const char * name, const T *t, size_t length) {
        ar << "<"  << name << ">[ ";
        for (size_t i = 0; i < length; i++) {
            serialize(ar, 0, t[i]);
            ar << " ";
        }
        ar << " ]</" << name << ">";
        return ar;
    }

    template <typename T, typename Archive>
    inline Archive & serialize_nvp_tabbed(Archive & ar, const char * name, const T & t, const char* tab) {
        ar << tab << "<"  << name << ">";
        serialize(ar, 0, t);
        ar << tab << "</" << name << ">";
        return ar;
    }

    template <typename T>
    inline void write(std::ostream& o, const T& t) {
        o.write((char*) &t, sizeof(T));
    }

    template<>
    inline void write(std::ostream& o, const STRING_TYPE& t) {
        size_t length = t.size();
        o.write((char*) &length, sizeof(size_t));
        o.write((char*) t.c_str(), sizeof(char) * length);
    }

    template <typename T>
    inline void read(std::istream& i, T& t) {
        i.read((char*) &t, sizeof(T));
    }

    template <>
    inline void read(std::istream& i, STRING_TYPE& t) {
        size_t length;
        i.read((char*) &length, sizeof(size_t));
        char* str = new char[length + 1];
        i.read((char*) str, sizeof(char) * length);
        str[length] = 0;
        t = STRING_TYPE(str, length);
        delete[] str;
    }

}

#endif /* DBTOASTER_SERIALIZATION_HPP */
