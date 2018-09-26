#ifndef DBTOASTER_HOUSING_HPP
#define DBTOASTER_HOUSING_HPP

#include <vector>
#include "types.hpp"
#include "serialization.hpp"

namespace dbtoaster 
{

    struct House
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE h2;
        DOUBLE_TYPE h3;
        DOUBLE_TYPE h4;
        DOUBLE_TYPE h5;
        DOUBLE_TYPE h6;
        DOUBLE_TYPE h7;
        DOUBLE_TYPE h8;
        DOUBLE_TYPE h9;
        DOUBLE_TYPE h10;
        DOUBLE_TYPE h11;
        long __av;

        House() { }
        
        House(std::vector<std::string>& fields)
        {
            if (fields.size() < 11) return;
            postcode = std::stod(fields[0]);
            h2 = std::stod(fields[1]);
            h3 = std::stod(fields[2]);
            h4 = std::stod(fields[3]);
            h5 = std::stod(fields[4]);
            h6 = std::stod(fields[5]);
            h7 = std::stod(fields[6]);
            h8 = std::stod(fields[7]);
            h9 = std::stod(fields[8]);
            h10 = std::stod(fields[9]);
            h11 = std::stod(fields[10]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, h2);
            write(o, h3);
            write(o, h4);
            write(o, h5);
            write(o, h6);
            write(o, h7);
            write(o, h8);
            write(o, h9);
            write(o, h10);
            write(o, h11);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, h2);
            read(i, h3);
            read(i, h4);
            read(i, h5);
            read(i, h6);
            read(i, h7);
            read(i, h8);
            read(i, h9);
            read(i, h10);
            read(i, h11);
        }
    };

    struct Shop
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE s2;
        DOUBLE_TYPE s3;
        DOUBLE_TYPE s4;
        DOUBLE_TYPE s5;
        DOUBLE_TYPE s6;
        long __av;

        Shop() { }
        
        Shop(std::vector<std::string>& fields)
        {
            if (fields.size() < 6) return;
            postcode = std::stod(fields[0]);
            s2 = std::stod(fields[1]);
            s3 = std::stod(fields[2]);
            s4 = std::stod(fields[3]);
            s5 = std::stod(fields[4]);
            s6 = std::stod(fields[5]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, s2);
            write(o, s3);
            write(o, s4);
            write(o, s5);
            write(o, s6);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, s2);
            read(i, s3);
            read(i, s4);
            read(i, s5);
            read(i, s6);
        }
    };

    struct Institution
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE i2;
        DOUBLE_TYPE i3;
        long __av;

        Institution() { }
        
        Institution(std::vector<std::string>& fields)
        {
            if (fields.size() < 3) return;
            postcode = std::stod(fields[0]);
            i2 = std::stod(fields[1]);
            i3 = std::stod(fields[2]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, i2);
            write(o, i3);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, i2);
            read(i, i3);
        }
    };


    struct Restaurant
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE r2;
        DOUBLE_TYPE r3;
        long __av;

        Restaurant() { }
        
        Restaurant(std::vector<std::string>& fields)
        {
            if (fields.size() < 3) return;
            postcode = std::stod(fields[0]);
            r2 = std::stod(fields[1]);
            r3 = std::stod(fields[2]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, r2);
            write(o, r3);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, r2);
            read(i, r3);
        }
    };

    struct Demographics
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE d2;
        DOUBLE_TYPE d3;
        DOUBLE_TYPE d4;
        DOUBLE_TYPE d5;
        long __av;

        Demographics() { }
        
        Demographics(std::vector<std::string>& fields)
        {
            if (fields.size() < 5) return;
            postcode = std::stod(fields[0]);
            d2 = std::stod(fields[1]);
            d3 = std::stod(fields[2]);
            d4 = std::stod(fields[3]);
            d5 = std::stod(fields[4]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, d2);
            write(o, d3);
            write(o, d4);
            write(o, d5);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, d2);
            read(i, d3);
            read(i, d4);
            read(i, d5);
        }
    };

    struct Transport
    {
        DOUBLE_TYPE postcode;
        DOUBLE_TYPE t2;
        DOUBLE_TYPE t3;
        DOUBLE_TYPE t4;
        long __av;

        Transport() { }
        
        Transport(std::vector<std::string>& fields)
        {
            if (fields.size() < 4) return;
            postcode = std::stod(fields[0]);
            t2 = std::stod(fields[1]);
            t3 = std::stod(fields[2]);
            t4 = std::stod(fields[3]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, postcode);
            write(o, t2);
            write(o, t3);
            write(o, t4);
        }

        void readFrom(std::istream& i)
        {
            read(i, postcode);
            read(i, t2);
            read(i, t3);
            read(i, t4);
        }
    };    
}

#endif /* DBTOASTER_HOUSING_HPP */