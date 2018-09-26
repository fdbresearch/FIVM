#ifndef DBTOASTER_DEGREE_HPP
#define DBTOASTER_DEGREE_HPP

#include <vector>
#include "types.hpp"
#include "serialization.hpp"

namespace dbtoaster 
{
    template<int SIZE>
    struct Degree 
    {
        int deg[SIZE];

        Degree() { }

        Degree(std::vector<std::string>& fields)
        {
            if (fields.size() < SIZE) return;
            for (int i = 0; i < SIZE; i++) 
                deg[i] = std::stoi(fields[i]);
        }

        void writeTo(std::ostream& o) 
        {
            for (int i = 0; i < SIZE; i++) 
                write(o, deg[i]);
        }

        void readFrom(std::istream& i)
        {
            for (int i = 0; i < SIZE; i++) 
                read(i, deg[i]);
        }
    };
}

#endif /* DBTOASTER_DEGREE_HPP */