#ifndef DBTOASTER_USRETAILER_HPP
#define DBTOASTER_USRETAILER_HPP

#include <vector>
#include "types.hpp"
#include "serialization.hpp"

namespace dbtoaster 
{

    struct Inventory
    {
        long locn;
        long dateid;
        DOUBLE_TYPE ksn;
        DOUBLE_TYPE inventoryunits;
        long __av;

        Inventory() { }
        
        Inventory(std::vector<std::string>& fields)
        {
            if (fields.size() < 4) return;
            locn = std::stol(fields[0]);
            dateid = std::stol(fields[1]);
            ksn = std::stod(fields[2]);
            inventoryunits = std::stod(fields[3]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, locn);
            write(o, dateid);
            write(o, ksn);
            write(o, inventoryunits);
        }

        void readFrom(std::istream& i)
        {
            read(i, locn);
            read(i, dateid);
            read(i, ksn);
            read(i, inventoryunits);
        }
    };

    struct Location
    {
        long locn;
        long zip;
        DOUBLE_TYPE rgn_cd;
        DOUBLE_TYPE clim_zn_nbr;
        DOUBLE_TYPE tot_area_sq_ft;
        DOUBLE_TYPE sell_area_sq_ft;
        DOUBLE_TYPE avghhi;
        DOUBLE_TYPE supertargetdistance;
        DOUBLE_TYPE supertargetdrivetime;
        DOUBLE_TYPE targetdistance;
        DOUBLE_TYPE targetdrivetime;
        DOUBLE_TYPE walmartdistance;
        DOUBLE_TYPE walmartdrivetime;
        DOUBLE_TYPE walmartsupercenterdistance;
        DOUBLE_TYPE walmartsupercenterdrivetime;
        long __av;

        Location() { }
        
        Location(std::vector<std::string>& fields)
        {
            if (fields.size() < 15) return;

            locn = std::stol(fields[0]);
            zip = std::stol(fields[1]);
            rgn_cd = std::stod(fields[2]);
            clim_zn_nbr = std::stod(fields[3]);
            tot_area_sq_ft = std::stod(fields[4]);
            sell_area_sq_ft = std::stod(fields[5]);
            avghhi = std::stod(fields[6]);
            supertargetdistance = std::stod(fields[7]);
            supertargetdrivetime = std::stod(fields[8]);
            targetdistance = std::stod(fields[9]);
            targetdrivetime = std::stod(fields[10]);
            walmartdistance = std::stod(fields[11]);
            walmartdrivetime = std::stod(fields[12]);
            walmartsupercenterdistance = std::stod(fields[13]);
            walmartsupercenterdrivetime = std::stod(fields[14]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, locn);
            write(o, zip);
            write(o, rgn_cd);
            write(o, clim_zn_nbr);
            write(o, tot_area_sq_ft);
            write(o, sell_area_sq_ft);
            write(o, avghhi);
            write(o, supertargetdistance);
            write(o, supertargetdrivetime);
            write(o, targetdistance);
            write(o, targetdrivetime);
            write(o, walmartdistance);
            write(o, walmartdrivetime);
            write(o, walmartsupercenterdistance);
            write(o, walmartsupercenterdrivetime);
            write(o, __av);            
        }

        void readFrom(std::istream& i)
        {
            read(i, locn);
            read(i, zip);
            read(i, rgn_cd);
            read(i, clim_zn_nbr);
            read(i, tot_area_sq_ft);
            read(i, sell_area_sq_ft);
            read(i, avghhi);
            read(i, supertargetdistance);
            read(i, supertargetdrivetime);
            read(i, targetdistance);
            read(i, targetdrivetime);
            read(i, walmartdistance);
            read(i, walmartdrivetime);
            read(i, walmartsupercenterdistance);
            read(i, walmartsupercenterdrivetime);
        }        
    };

    struct Census
    {
        long zip;
        long population;
        long white;
        long asian;
        long pacific;
        long blackafrican;
        DOUBLE_TYPE medianage;
        long occupiedhouseunits;
        long houseunits;
        long families;
        long households;
        long husbwife;
        long males;
        long females;
        long householdschildren;
        long hispanic;
        long __av;

        Census() { }
        
        Census(std::vector<std::string>& fields)
        {
            if (fields.size() < 16) return;

            zip = std::stol(fields[0]);
            population = std::stol(fields[1]);
            white = std::stol(fields[2]);
            asian = std::stol(fields[3]);
            pacific = std::stol(fields[4]);
            blackafrican = std::stol(fields[5]);
            medianage = std::stod(fields[6]);
            occupiedhouseunits = std::stol(fields[7]);
            houseunits = std::stol(fields[8]);
            families = std::stol(fields[9]);
            households = std::stol(fields[10]);
            husbwife = std::stol(fields[11]);
            males = std::stol(fields[12]);
            females = std::stol(fields[13]);
            householdschildren = std::stol(fields[14]);
            hispanic = std::stol(fields[15]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, zip);
            write(o, population);
            write(o, white);
            write(o, asian);
            write(o, pacific);
            write(o, blackafrican);
            write(o, medianage);
            write(o, occupiedhouseunits);
            write(o, houseunits);
            write(o, families);
            write(o, households);
            write(o, husbwife);
            write(o, males);
            write(o, females);
            write(o, householdschildren);
            write(o, hispanic);
        }

        void readFrom(std::istream& i)
        {
            read(i, zip);
            read(i, population);
            read(i, white);
            read(i, asian);
            read(i, pacific);
            read(i, blackafrican);
            read(i, medianage);
            read(i, occupiedhouseunits);
            read(i, houseunits);
            read(i, families);
            read(i, households);
            read(i, husbwife);
            read(i, males);
            read(i, females);
            read(i, householdschildren);
            read(i, hispanic);
        }
    };

    struct Item
    {
        DOUBLE_TYPE ksn;
        long subcategory;
        long category;
        long categorycluster;
        DOUBLE_TYPE prize;
        long __av;

        Item() { }
        
        Item(std::vector<std::string>& fields)
        {
            if (fields.size() < 5) return;

            ksn = std::stod(fields[0]);
            subcategory = std::stol(fields[1]);
            category = std::stol(fields[2]);
            categorycluster = std::stol(fields[3]);
            prize = std::stod(fields[4]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, ksn);
            write(o, subcategory);
            write(o, category);
            write(o, categorycluster);
            write(o, prize);
        }

        void readFrom(std::istream& i)
        {
            read(i, ksn);
            read(i, subcategory);
            read(i, category);
            read(i, categorycluster);
            read(i, prize);
        }
    };

    struct Weather
    {
        long locn;
        long dateid;
        long rain;
        long snow;
        long maxtemp;
        long mintemp;
        DOUBLE_TYPE meanwind;
        long thunder;
        long __av;

        Weather() { }
        
        Weather(std::vector<std::string>& fields)
        {
            if (fields.size() < 8) return;

            locn = std::stoi(fields[0]);
            dateid = std::stoi(fields[1]);
            rain = std::stoi(fields[2]);
            snow = std::stoi(fields[3]);
            maxtemp = std::stoi(fields[4]);
            mintemp = std::stoi(fields[5]);
            meanwind = std::stod(fields[6]);
            thunder = std::stoi(fields[7]);
            __av = 1L;
        }

        void writeTo(std::ostream& o) 
        {
            write(o, locn);
            write(o, dateid);
            write(o, rain);
            write(o, snow);
            write(o, maxtemp);
            write(o, mintemp);
            write(o, meanwind);
            write(o, thunder);
        }

        void readFrom(std::istream& i)
        {
            read(i, locn);
            read(i, dateid);
            read(i, rain);
            read(i, snow);
            read(i, maxtemp);
            read(i, mintemp);
            read(i, meanwind);
            read(i, thunder);
        }
    };
}

#endif /* DBTOASTER_USRETAILER_HPP */