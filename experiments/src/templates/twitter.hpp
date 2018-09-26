#ifndef DBTOASTER_TWITTER_HPP
#define DBTOASTER_TWITTER_HPP

#include <vector>
#include "types.hpp"
#include "serialization.hpp"

namespace dbtoaster 
{

    struct Twitter
    {
        long a;
        long b;
        long __av;

        Twitter() { }
        
        Twitter(std::vector<std::string>& fields)
        {
            if (fields.size() < 2) return;
            a = std::stol(fields[0]);
            b = std::stol(fields[1]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, a);
            write(o, b);
        }

        void readFrom(std::istream& i)
        {
            read(i, a);
            read(i, b);
        }
    };

    struct TwitterDouble
    {
        DOUBLE_TYPE a;
        DOUBLE_TYPE b;
        long __av;

        TwitterDouble() { }
        
        TwitterDouble(std::vector<std::string>& fields)
        {
            if (fields.size() < 2) return;
            a = std::stod(fields[0]);
            b = std::stod(fields[1]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, a);
            write(o, b);
        }

        void readFrom(std::istream& i)
        {
            read(i, a);
            read(i, b);
        }
    };
}

#endif /* DBTOASTER_TWITTER_HPP */