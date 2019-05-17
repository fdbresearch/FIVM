#ifndef DEGREE_HPP
#define DEGREE_HPP

#include <vector>
#include <string>
#include <ostream>

template<int SIZE>
struct Degree {
    int deg[SIZE];

    Degree() { }

    Degree(std::vector<std::string>& fields) {
        if (fields.size() < SIZE) return;
        for (int i = 0; i < SIZE; i++)
            deg[i] = std::stoi(fields[i]);
    }

    void writeTo(std::ostream& o) {
        for (int i = 0; i < SIZE; i++)
            write(o, deg[i]);
    }

    void readFrom(std::istream& i) {
        for (int i = 0; i < SIZE; i++)
            read(i, deg[i]);
    }
};
#endif /* DEGREE_HPP */