#ifndef RETAILER_TEMPLATE_HPP
#define RETAILER_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"
#include "degree.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;
using namespace dbtoaster;

const string dataPath = "data/retailer";
const string dataset = "";

#if defined(RELATION_INVENTORY_STATIC) || (defined(RELATION_INVENTORY_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<INVENTORY_entry> inventory;
    std::vector<INVENTORY_entry>::iterator inventoryIt;
#elif defined(RELATION_INVENTORY_DYNAMIC)
    std::vector<DELTA_INVENTORY_entry> inventory;
    std::vector<DELTA_INVENTORY_entry>::iterator inventoryIt;
#endif

#if defined(RELATION_LOCATION_STATIC) || (defined(RELATION_LOCATION_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<LOCATION_entry> location;
    std::vector<LOCATION_entry>::iterator locationIt;
#elif defined(RELATION_LOCATION_DYNAMIC)
    std::vector<DELTA_LOCATION_entry> location;
    std::vector<DELTA_LOCATION_entry>::iterator locationIt;
#endif

#if defined(RELATION_CENSUS_STATIC) || (defined(RELATION_CENSUS_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<CENSUS_entry> census;
    std::vector<CENSUS_entry>::iterator censusIt;
#elif defined(RELATION_CENSUS_DYNAMIC)
    std::vector<DELTA_CENSUS_entry> census;
    std::vector<DELTA_CENSUS_entry>::iterator censusIt;
#endif

#if defined(RELATION_ITEM_STATIC) || (defined(RELATION_ITEM_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<ITEM_entry> item;
    std::vector<ITEM_entry>::iterator itemIt;
#elif defined(RELATION_ITEM_DYNAMIC)
    std::vector<DELTA_ITEM_entry> item;
    std::vector<DELTA_ITEM_entry>::iterator itemIt;
#endif

#if defined(RELATION_WEATHER_STATIC) || (defined(RELATION_WEATHER_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<WEATHER_entry> weather;
    std::vector<WEATHER_entry>::iterator weatherIt;
#elif defined(RELATION_WEATHER_DYNAMIC)
    std::vector<DELTA_WEATHER_entry> weather;
    std::vector<DELTA_WEATHER_entry>::iterator weatherIt;
#endif

#if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
    std::vector<DEGREE1_entry> degree1;
#endif

#if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
    std::vector<DEGREE4_entry> degree4;
#endif

#if defined(RELATION_DEGREE6_STATIC) || defined(RELATION_DEGREE6_DYNAMIC)
    std::vector<DEGREE6_entry> degree6;
#endif

#if defined(RELATION_DEGREE13_STATIC) || defined(RELATION_DEGREE13_DYNAMIC)
    std::vector<DEGREE13_entry> degree13;
#endif

#if defined(RELATION_DEGREE15_STATIC) || defined(RELATION_DEGREE15_DYNAMIC)
    std::vector<DEGREE15_entry> degree15;
#endif

void load_relations() {
    Stopwatch sw;

    #if defined(RELATION_INVENTORY_STATIC) || defined(RELATION_INVENTORY_DYNAMIC)
        sw.restart();
        inventory.clear();
        readFromFile(inventory, dataPath + "/" + dataset + "/Inventory.tbl", '|');
        sw.stop();
        std::cout << "Loaded Inventory (" << inventory.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_LOCATION_STATIC) || defined(RELATION_LOCATION_DYNAMIC)
        sw.restart();
        location.clear();
        readFromFile(location, dataPath + "/" + dataset + "/Location.tbl", '|');
        sw.stop();
        std::cout << "Loaded Location (" << location.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_CENSUS_STATIC) || defined(RELATION_CENSUS_DYNAMIC)
        sw.restart();
        census.clear();
        readFromFile(census, dataPath + "/" + dataset + "/Census.tbl", '|');
        sw.stop();
        std::cout << "Loaded Census (" << census.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_ITEM_STATIC) || defined(RELATION_ITEM_DYNAMIC)
        sw.restart();
        item.clear();
        readFromFile(item, dataPath + "/" + dataset + "/Item.tbl", '|');
        sw.stop();
        std::cout << "Loaded Item (" << item.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_WEATHER_STATIC) || defined(RELATION_WEATHER_DYNAMIC)
        sw.restart();
        weather.clear();
        readFromFile(weather, dataPath + "/" + dataset + "/Weather.tbl", '|');
        sw.stop();
        std::cout << "Loaded Weather (" << weather.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
        degree1.clear();
        readFromFile(degree1, dataPath + "/" + dataset + "/../degree/Degree1.tbl", '|');
    #endif

    #if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
        degree4.clear();
        readFromFile(degree4, dataPath + "/" + dataset + "/../degree/Degree4.tbl", '|');
    #endif

    #if defined(RELATION_DEGREE6_STATIC) || defined(RELATION_DEGREE6_DYNAMIC)
        degree6.clear();
        readFromFile(degree6, dataPath + "/" + dataset + "/../degree/Degree6.tbl", '|');
    #endif

    #if defined(RELATION_DEGREE13_STATIC) || defined(RELATION_DEGREE13_DYNAMIC)
        degree13.clear();
        readFromFile(degree13, dataPath + "/" + dataset + "/../degree/Degree13.tbl", '|');
    #endif

    #if defined(RELATION_DEGREE15_STATIC) || defined(RELATION_DEGREE15_DYNAMIC)
        degree15.clear();
        readFromFile(degree15, dataPath + "/" + dataset + "/../degree/Degree15.tbl", '|');
    #endif
}

void destroy_relations() {

    #if defined(RELATION_INVENTORY_STATIC) || defined(RELATION_INVENTORY_DYNAMIC)
        inventory.clear();
    #endif

    #if defined(RELATION_LOCATION_STATIC) || defined(RELATION_LOCATION_DYNAMIC)
        location.clear();
    #endif

    #if defined(RELATION_CENSUS_STATIC) || defined(RELATION_CENSUS_DYNAMIC)
        census.clear();
    #endif

    #if defined(RELATION_ITEM_STATIC) || defined(RELATION_ITEM_DYNAMIC)
        item.clear();
    #endif

    #if defined(RELATION_WEATHER_STATIC) || defined(RELATION_WEATHER_DYNAMIC)
        weather.clear();
    #endif

    #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
        degree1.clear();
    #endif

    #if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
        degree4.clear();
    #endif

    #if defined(RELATION_DEGREE6_STATIC) || defined(RELATION_DEGREE6_DYNAMIC)
        degree6.clear();
    #endif

    #if defined(RELATION_DEGREE13_STATIC) || defined(RELATION_DEGREE13_DYNAMIC)
        degree13.clear();
    #endif

    #if defined(RELATION_DEGREE15_STATIC) || defined(RELATION_DEGREE15_DYNAMIC)
        degree15.clear();
    #endif
}

#ifdef LOG_MEMORY_INFO
    #define PRINT_MEMORY_INFO dumpHeapProfile("RETAILER");
#else
    #define PRINT_MEMORY_INFO
#endif

#ifdef LOG_OUTPUT
    #define PRINT_LOG \
    { \
        if (data.tN >= nextLogPoint) { \
            timeval current_time; \
            gettimeofday(&current_time, NULL); \
            long endTime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000; \
            std::cout << "Processing " << data.tN << " tuples at " << endTime << "    "; \
            PRINT_MEMORY_INFO \
            std::cout << std::endl; \
            nextLogPoint = data.tN + LOG_OUTPUT; \
        } \
    }
#else
    #define PRINT_LOG
#endif

void process_tables(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_tables";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif

    #ifdef RELATION_INVENTORY_STATIC
        for (size_t i = 0; i < inventory.size(); i++) {
            data.on_insert_INVENTORY(inventory[i]);
        }
    #endif

    #ifdef RELATION_LOCATION_STATIC
        for (size_t i = 0; i < location.size(); i++) {
            data.on_insert_LOCATION(location[i]);
        }
    #endif

    #ifdef RELATION_CENSUS_STATIC
        for (size_t i = 0; i < census.size(); i++) {
            data.on_insert_CENSUS(census[i]);
        }
    #endif

    #ifdef RELATION_ITEM_STATIC
        for (size_t i = 0; i < item.size(); i++) {
            data.on_insert_ITEM(item[i]);
        }
    #endif

    #ifdef RELATION_WEATHER_STATIC
        for (size_t i = 0; i < weather.size(); i++) {
            data.on_insert_WEATHER(weather[i]);
        }
    #endif

    #ifdef RELATION_DEGREE1_STATIC
        for (size_t i = 0; i < degree1.size(); i++) {
            data.on_insert_DEGREE1(degree1[i]);
        }
    #endif

    #ifdef RELATION_DEGREE4_STATIC
        for (size_t i = 0; i < degree4.size(); i++) {
            data.on_insert_DEGREE4(degree4[i]);
        }
    #endif

    #ifdef RELATION_DEGREE6_STATIC
        for (size_t i = 0; i < degree6.size(); i++) {
            data.on_insert_DEGREE6(degree6[i]);
        }
    #endif

    #ifdef RELATION_DEGREE13_STATIC
        for (size_t i = 0; i < degree13.size(); i++) {
            data.on_insert_DEGREE13(degree13[i]);
        }
    #endif

    #ifdef RELATION_DEGREE15_STATIC
        for (size_t i = 0; i < degree15.size(); i++) {
            data.on_insert_DEGREE15(degree15[i]);
        }
    #endif

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_tables: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif
}

void process_streams(dbtoaster::data_t& data) {
    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory before process_streams: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif

    size_t i = 0, count = 0;

    #ifdef LOG_OUTPUT
        long nextLogPoint = 0;
        std::cout << std::endl;
        PRINT_LOG
    #endif

    #ifdef BATCH_SIZE

        size_t batchSize = BATCH_SIZE;

        #ifdef RELATION_CENSUS_DYNAMIC
            censusIt = census.begin();
        #endif

        #ifdef RELATION_LOCATION_DYNAMIC
            locationIt = location.begin();
        #endif

        #ifdef RELATION_ITEM_DYNAMIC
            itemIt = item.begin();
        #endif

        #ifdef RELATION_WEATHER_DYNAMIC
            weatherIt = weather.begin();
        #endif

        #ifdef RELATION_INVENTORY_DYNAMIC
            inventoryIt = inventory.begin();
        #endif

        #ifdef RELATION_CENSUS_DYNAMIC
            for (; i < census.size() / batchSize; i++) {
                data.on_batch_update_CENSUS(censusIt, censusIt + batchSize);
                censusIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_LOCATION_DYNAMIC
                    data.on_batch_update_LOCATION(locationIt, locationIt + batchSize);
                    locationIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_ITEM_DYNAMIC
                    data.on_batch_update_ITEM(itemIt, itemIt + batchSize);
                    itemIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_batch_update_WEATHER(weatherIt, weatherIt + batchSize);
                    weatherIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_batch_update_INVENTORY(inventoryIt, inventoryIt + batchSize);
                    inventoryIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (censusIt != census.end()) {
                data.on_batch_update_CENSUS(censusIt, census.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_LOCATION_DYNAMIC
            for (; i < location.size() / batchSize; i++) {
                data.on_batch_update_LOCATION(locationIt, locationIt + batchSize);
                locationIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_ITEM_DYNAMIC
                    data.on_batch_update_ITEM(itemIt, itemIt + batchSize);
                    itemIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_batch_update_WEATHER(weatherIt, weatherIt + batchSize);
                    weatherIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_batch_update_INVENTORY(inventoryIt, inventoryIt + batchSize);
                    inventoryIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (locationIt != location.end()) {
                data.on_batch_update_LOCATION(locationIt, location.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_ITEM_DYNAMIC
            for (; i < item.size() / batchSize; i++) {
                data.on_batch_update_ITEM(itemIt, itemIt + batchSize);
                itemIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_batch_update_WEATHER(weatherIt, weatherIt + batchSize);
                    weatherIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_batch_update_INVENTORY(inventoryIt, inventoryIt + batchSize);
                    inventoryIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (itemIt != item.end()) {
                data.on_batch_update_ITEM(itemIt, item.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_WEATHER_DYNAMIC
            for (; i < weather.size() / batchSize; i++) {
                data.on_batch_update_WEATHER(weatherIt, weatherIt + batchSize);
                weatherIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_batch_update_INVENTORY(inventoryIt, inventoryIt + batchSize);
                    inventoryIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (weatherIt != weather.end()) {
                data.on_batch_update_WEATHER(weatherIt, weather.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_INVENTORY_DYNAMIC
            for (; i < inventory.size() / batchSize; i++) {
                data.on_batch_update_INVENTORY(inventoryIt, inventoryIt + batchSize);
                inventoryIt += batchSize;
                PRINT_LOG
            }
            if (inventoryIt != inventory.end()) {
                data.on_batch_update_INVENTORY(inventoryIt, inventory.end());
                PRINT_LOG
            }
        #endif

    #else

        #ifdef RELATION_CENSUS_DYNAMIC
            count = census.size();

            for (; i < count; i++) {
                data.on_insert_CENSUS(census[i]);
                PRINT_LOG

                #ifdef RELATION_LOCATION_DYNAMIC
                    data.on_insert_LOCATION(location[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_ITEM_DYNAMIC
                    data.on_insert_ITEM(item[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_insert_WEATHER(weather[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_insert_INVENTORY(inventory[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_LOCATION_DYNAMIC
            count = location.size();

            for (; i < count; i++) {
                data.on_insert_LOCATION(location[i]);
                PRINT_LOG

                #ifdef RELATION_ITEM_DYNAMIC
                    data.on_insert_ITEM(item[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_insert_WEATHER(weather[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_insert_INVENTORY(inventory[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_ITEM_DYNAMIC
            count = item.size();

            for (; i < count; i++) {
                data.on_insert_ITEM(item[i]);
                PRINT_LOG

                #ifdef RELATION_WEATHER_DYNAMIC
                    data.on_insert_WEATHER(weather[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_insert_INVENTORY(inventory[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_WEATHER_DYNAMIC
            count = weather.size();

            for (; i < count; i++) {
                data.on_insert_WEATHER(weather[i]);
                PRINT_LOG

                #ifdef RELATION_INVENTORY_DYNAMIC
                    data.on_insert_INVENTORY(inventory[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_INVENTORY_DYNAMIC
            count = inventory.size();

            for (; i < count; i++) {
                data.on_insert_INVENTORY(inventory[i]);
                PRINT_LOG
            }
        #endif
    #endif

    #ifdef LOG_OUTPUT
        nextLogPoint = data.tN;
        PRINT_LOG
    #endif

    #ifdef LOG_MEMORY_INFO
        std::cout << "Memory after process_streams: ";
        PRINT_MEMORY_INFO
        std::cout << std::endl;
    #endif
}

void print_result(dbtoaster::data_t& data) {
    data.serialize(std::cout, 0);
}

#endif /* RETAILER_TEMPLATE_HPP */