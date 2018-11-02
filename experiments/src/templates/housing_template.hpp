#ifndef DBTOASTER_HOUSING_TEMPLATE_HPP
#define DBTOASTER_HOUSING_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"
#include "degree.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;

const string dataPath = "datasets/housing-4-normalised";
const string dataset = "";

namespace dbtoaster 
{
    struct data_t;    

    #if defined(RELATION_HOUSE_STATIC)
        std::vector<HOUSE_entry> house;
        std::vector<HOUSE_entry>::iterator houseIt;
    #elif defined(RELATION_HOUSE_DYNAMIC)
        std::vector<DELTA_HOUSE_entry> house;
        std::vector<DELTA_HOUSE_entry>::iterator houseIt;
    #endif

    #if defined(RELATION_SHOP_STATIC)
        std::vector<SHOP_entry> shop;
        std::vector<SHOP_entry>::iterator shopIt;
    #elif defined(RELATION_SHOP_DYNAMIC)
        std::vector<DELTA_SHOP_entry> shop;
        std::vector<DELTA_SHOP_entry>::iterator shopIt;
    #endif

    #if defined(RELATION_INSTITUTION_STATIC)
        std::vector<INSTITUTION_entry> institution;
        std::vector<INSTITUTION_entry>::iterator institutionIt;    
    #elif defined(RELATION_INSTITUTION_DYNAMIC)
        std::vector<DELTA_INSTITUTION_entry> institution;
        std::vector<DELTA_INSTITUTION_entry>::iterator institutionIt;    
    #endif

    #if defined(RELATION_RESTAURANT_STATIC)
        std::vector<RESTAURANT_entry> restaurant;
        std::vector<RESTAURANT_entry>::iterator restaurantIt;
    #elif defined(RELATION_RESTAURANT_DYNAMIC)
        std::vector<DELTA_RESTAURANT_entry> restaurant;
        std::vector<DELTA_RESTAURANT_entry>::iterator restaurantIt;
    #endif

    #if defined(RELATION_DEMOGRAPHICS_STATIC)
        std::vector<DEMOGRAPHICS_entry> demographics;
        std::vector<DEMOGRAPHICS_entry>::iterator demographicsIt;
    #elif defined(RELATION_DEMOGRAPHICS_DYNAMIC)
        std::vector<DELTA_DEMOGRAPHICS_entry> demographics;
        std::vector<DELTA_DEMOGRAPHICS_entry>::iterator demographicsIt;
    #endif

    #if defined(RELATION_TRANSPORT_STATIC)
        std::vector<TRANSPORT_entry> transport;
        std::vector<TRANSPORT_entry>::iterator transportIt;
    #elif defined(RELATION_TRANSPORT_DYNAMIC)
        std::vector<DELTA_TRANSPORT_entry> transport;
        std::vector<DELTA_TRANSPORT_entry>::iterator transportIt;
    #endif

    #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
        std::vector<DEGREE1_entry> degree1;
    #endif

    #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
        std::vector<DEGREE2_entry> degree2;
    #endif

    #if defined(RELATION_DEGREE3_STATIC) || defined(RELATION_DEGREE3_DYNAMIC)
        std::vector<DEGREE3_entry> degree3;
    #endif

    #if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
        std::vector<DEGREE4_entry> degree4;
    #endif

    #if defined(RELATION_DEGREE5_STATIC) || defined(RELATION_DEGREE5_DYNAMIC)
        std::vector<DEGREE5_entry> degree5;
    #endif

    #if defined(RELATION_DEGREE10_STATIC) || defined(RELATION_DEGREE10_DYNAMIC)
        std::vector<DEGREE10_entry> degree10;
    #endif   

    void load_relations()
    {
        Stopwatch sw;

        #if defined(RELATION_HOUSE_STATIC) || defined(RELATION_HOUSE_DYNAMIC)
            sw.restart();
            house.clear();
            readFromFile(house, dataPath + "/" + dataset + "/House.tbl", '|');
            sw.stop();
            std::cout << "Loaded House (" << house.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_SHOP_STATIC) || defined(RELATION_SHOP_DYNAMIC)
            sw.restart();
            shop.clear();
            readFromFile(shop, dataPath + "/" + dataset + "/Shop.tbl", '|');
            sw.stop();
            std::cout << "Loaded Shop (" << shop.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_INSTITUTION_STATIC) || defined(RELATION_INSTITUTION_DYNAMIC)
            sw.restart();
            institution.clear();
            readFromFile(institution, dataPath + "/" + dataset + "/Institution.tbl", '|');
            sw.stop();
            std::cout << "Loaded Institution (" << institution.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_RESTAURANT_STATIC) || defined(RELATION_RESTAURANT_DYNAMIC)
            sw.restart();
            restaurant.clear();
            readFromFile(restaurant, dataPath + "/" + dataset + "/Restaurant.tbl", '|');
            sw.stop();
            std::cout << "Loaded Restaurant (" << restaurant.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_DEMOGRAPHICS_STATIC) || defined(RELATION_DEMOGRAPHICS_DYNAMIC)
            sw.restart();
            demographics.clear();
            readFromFile(demographics, dataPath + "/" + dataset + "/Demographics.tbl", '|');
            sw.stop();
            std::cout << "Loaded Demographics (" << demographics.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TRANSPORT_STATIC) || defined(RELATION_TRANSPORT_DYNAMIC)
            sw.restart();
            transport.clear();
            readFromFile(transport, dataPath + "/" + dataset + "/Transport.tbl", '|');
            sw.stop();
            std::cout << "Loaded Transport (" << transport.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
            degree1.clear();
            readFromFile(degree1, dataPath + "/" + dataset + "/../degree/Degree1.tbl", '|');
        #endif

        #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
            degree2.clear();
            readFromFile(degree2, dataPath + "/" + dataset + "/../degree/Degree2.tbl", '|');
        #endif

        #if defined(RELATION_DEGREE3_STATIC) || defined(RELATION_DEGREE3_DYNAMIC)
            degree3.clear();
            readFromFile(degree3, dataPath + "/" + dataset + "/../degree/Degree3.tbl", '|');
        #endif

        #if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
            degree4.clear();
            readFromFile(degree4, dataPath + "/" + dataset + "/../degree/Degree4.tbl", '|');
        #endif

        #if defined(RELATION_DEGREE5_STATIC) || defined(RELATION_DEGREE5_DYNAMIC)
            degree5.clear();
            readFromFile(degree5, dataPath + "/" + dataset + "/../degree/Degree5.tbl", '|');
        #endif

        #if defined(RELATION_DEGREE10_STATIC) || defined(RELATION_DEGREE10_DYNAMIC)
            degree10.clear();
            readFromFile(degree10, dataPath + "/" + dataset + "/../degree/Degree10.tbl", '|');
        #endif
    }

    void destroy_relations()
    {
        #if defined(RELATION_HOUSE_STATIC) || defined(RELATION_HOUSE_DYNAMIC)
            house.clear();
        #endif

        #if defined(RELATION_SHOP_STATIC) || defined(RELATION_SHOP_DYNAMIC)
            shop.clear();
        #endif

        #if defined(RELATION_INSTITUTION_STATIC) || defined(RELATION_INSTITUTION_DYNAMIC)
            institution.clear();
        #endif

        #if defined(RELATION_RESTAURANT_STATIC) || defined(RELATION_RESTAURANT_DYNAMIC)
            restaurant.clear();
        #endif

        #if defined(RELATION_DEMOGRAPHICS_STATIC) || defined(RELATION_DEMOGRAPHICS_DYNAMIC)
            demographics.clear();
        #endif

        #if defined(RELATION_TRANSPORT_STATIC) || defined(RELATION_TRANSPORT_DYNAMIC)
            transport.clear();
        #endif    

        #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
            degree1.clear();
        #endif

        #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
            degree2.clear();
        #endif

        #if defined(RELATION_DEGREE3_STATIC) || defined(RELATION_DEGREE3_DYNAMIC)
            degree3.clear();
        #endif

        #if defined(RELATION_DEGREE4_STATIC) || defined(RELATION_DEGREE4_DYNAMIC)
            degree4.clear();
        #endif

        #if defined(RELATION_DEGREE5_STATIC) || defined(RELATION_DEGREE5_DYNAMIC)
            degree5.clear();
        #endif

        #if defined(RELATION_DEGREE10_STATIC) || defined(RELATION_DEGREE10_DYNAMIC)
            degree10.clear();
        #endif
    }

    #ifdef LOG_MEMORY_INFO
        #define PRINT_MEMORY_INFO dumpHeapProfile("HOUSING");
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

    void process_tables(dbtoaster::data_t& data)
    {
        #ifdef LOG_MEMORY_INFO
            std::cout << "Memory before process_tables";
            PRINT_MEMORY_INFO
            std::cout << std::endl;
        #endif

        #ifdef RELATION_HOUSE_STATIC
            for (size_t i = 0; i < house.size(); i++) 
            {   
                data.on_insert_HOUSE(house[i]);
            }
        #endif

        #ifdef RELATION_SHOP_STATIC
            for (size_t i = 0; i < shop.size(); i++) 
            {   
                data.on_insert_SHOP(shop[i]);
            }
        #endif

        #ifdef RELATION_INSTITUTION_STATIC
            for (size_t i = 0; i < institution.size(); i++) 
            {   
                data.on_insert_INSTITUTION(institution[i]);
            }
        #endif

        #ifdef RELATION_RESTAURANT_STATIC
            for (size_t i = 0; i < restaurant.size(); i++) 
            {   
                data.on_insert_RESTAURANT(restaurant[i]);
            }
        #endif

        #ifdef RELATION_DEMOGRAPHICS_STATIC
            for (size_t i = 0; i < demographics.size(); i++) 
            {   
                data.on_insert_DEMOGRAPHICS(demographics[i]);
            }
        #endif

        #ifdef RELATION_TRANSPORT_STATIC
            for (size_t i = 0; i < transport.size(); i++) 
            {   
                data.on_insert_TRANSPORT(transport[i]);
            }
        #endif

        #ifdef RELATION_DEGREE1_STATIC
            for (size_t i = 0; i < degree1.size(); i++) 
            {
                data.on_insert_DEGREE1(degree1[i]);
            }
        #endif

        #ifdef RELATION_DEGREE2_STATIC
            for (size_t i = 0; i < degree2.size(); i++) 
            {
                data.on_insert_DEGREE2(degree2[i]);
            }
        #endif

        #ifdef RELATION_DEGREE3_STATIC
            for (size_t i = 0; i < degree3.size(); i++) 
            {
                data.on_insert_DEGREE3(degree3[i]);
            }
        #endif

        #ifdef RELATION_DEGREE4_STATIC
            for (size_t i = 0; i < degree4.size(); i++) 
            {
                data.on_insert_DEGREE4(degree4[i]);
            }
        #endif

        #ifdef RELATION_DEGREE5_STATIC
            for (size_t i = 0; i < degree5.size(); i++) 
            {
                data.on_insert_DEGREE5(degree5[i]);
            }
        #endif

        #ifdef RELATION_DEGREE10_STATIC
            for (size_t i = 0; i < degree10.size(); i++) 
            {
                data.on_insert_DEGREE10(degree10[i]);
            }
        #endif

        #ifdef LOG_MEMORY_INFO
            std::cout << "Memory after process_tables: ";    
            PRINT_MEMORY_INFO
            std::cout << std::endl;
        #endif
    }

    void process_streams(dbtoaster::data_t& data)
    {
        #ifdef LOG_MEMORY_INFO
            std::cout << "Memory before process_streams: ";    
            PRINT_MEMORY_INFO
            std::cout << std::endl;
        #endif

        size_t i = 0, count = 0;

        #ifdef LOG_OUTPUT
            long nextLogPoint = 0;
            cout << endl;
            PRINT_LOG
        #endif

        #ifdef BATCH_SIZE 

            size_t batchSize = BATCH_SIZE;

            demographicsIt = demographics.begin();
            transportIt = transport.begin();
            institutionIt = institution.begin();
            restaurantIt = restaurant.begin();
            houseIt = house.begin();
            shopIt = shop.begin();
            

            #ifdef RELATION_DEMOGRAPHICS_DYNAMIC
                for (; i < demographics.size() / batchSize; i++)
                {
                    data.on_batch_update_DEMOGRAPHICS(demographicsIt, demographicsIt + batchSize);
                    demographicsIt += batchSize;
                    PRINT_LOG

                    #ifdef RELATION_TRANSPORT_DYNAMIC
                        data.on_batch_update_TRANSPORT(transportIt, transportIt + batchSize);
                        transportIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_INSTITUTION_DYNAMIC
                        data.on_batch_update_INSTITUTION(institutionIt, institutionIt + batchSize);
                        institutionIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_batch_update_RESTAURANT(restaurantIt, restaurantIt + batchSize);
                        restaurantIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_batch_update_HOUSE(houseIt, houseIt + batchSize);
                        houseIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                        shopIt += batchSize;
                        PRINT_LOG
                    #endif

                }
                if (demographicsIt != demographics.end()) {
                    data.on_batch_update_DEMOGRAPHICS(demographicsIt, demographics.end());
                    PRINT_LOG
                }                
            #endif

            #ifdef RELATION_TRANSPORT_DYNAMIC
                for (; i < transport.size() / batchSize; i++)
                {
                    data.on_batch_update_TRANSPORT(transportIt, transportIt + batchSize);
                    transportIt += batchSize;
                    PRINT_LOG

                    #ifdef RELATION_INSTITUTION_DYNAMIC
                        data.on_batch_update_INSTITUTION(institutionIt, institutionIt + batchSize);
                        institutionIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_batch_update_RESTAURANT(restaurantIt, restaurantIt + batchSize);
                        restaurantIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_batch_update_HOUSE(houseIt, houseIt + batchSize);
                        houseIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                        shopIt += batchSize;
                        PRINT_LOG
                    #endif
                }
                if (transportIt != transport.end()) {
                    data.on_batch_update_TRANSPORT(transportIt, transport.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_INSTITUTION_DYNAMIC
                for (; i < institution.size() / batchSize; i++)
                {
                    data.on_batch_update_INSTITUTION(institutionIt, institutionIt + batchSize);
                    institutionIt += batchSize;
                    PRINT_LOG

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_batch_update_RESTAURANT(restaurantIt, restaurantIt + batchSize);
                        restaurantIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_batch_update_HOUSE(houseIt, houseIt + batchSize);
                        houseIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                        shopIt += batchSize;
                        PRINT_LOG
                    #endif
                }
                if (institutionIt != institution.end()) {
                    data.on_batch_update_INSTITUTION(institutionIt, institution.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_RESTAURANT_DYNAMIC
                for (; i < restaurant.size() / batchSize; i++)
                {
                    data.on_batch_update_RESTAURANT(restaurantIt, restaurantIt + batchSize);
                    restaurantIt += batchSize;
                    PRINT_LOG

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_batch_update_HOUSE(houseIt, houseIt + batchSize);
                        houseIt += batchSize;
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                        shopIt += batchSize;
                        PRINT_LOG
                    #endif
                }
                if (restaurantIt != restaurant.end()) {
                    data.on_batch_update_RESTAURANT(restaurantIt, restaurant.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_HOUSE_DYNAMIC
                for (; i < house.size() / batchSize; i++)
                {
                    data.on_batch_update_HOUSE(houseIt, houseIt + batchSize);
                    houseIt += batchSize;
                    PRINT_LOG

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                        shopIt += batchSize;
                        PRINT_LOG
                    #endif
                }
                if (houseIt != house.end()) {
                    data.on_batch_update_HOUSE(houseIt, house.end());
                    PRINT_LOG
                }                
            #endif

            #ifdef RELATION_SHOP_DYNAMIC
                for (; i < shop.size() / batchSize; i++)
                {
                    data.on_batch_update_SHOP(shopIt, shopIt + batchSize);
                    shopIt += batchSize;
                    PRINT_LOG
                }
                if (shopIt != shop.end()) {
                    data.on_batch_update_SHOP(shopIt, shop.end());
                    PRINT_LOG
                }
            #endif

        #else

            #ifdef RELATION_DEMOGRAPHICS_DYNAMIC
                count = demographics.size();

                for (; i < count; i++) 
                {
                    data.on_insert_DEMOGRAPHICS(demographics[i]);
                    PRINT_LOG

                    #ifdef RELATION_TRANSPORT_DYNAMIC
                        data.on_insert_TRANSPORT(transport[i]);
                        PRINT_LOG
                    #endif 

                    #ifdef RELATION_INSTITUTION_DYNAMIC
                        data.on_insert_INSTITUTION(institution[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_insert_RESTAURANT(restaurant[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_insert_HOUSE(house[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_insert_SHOP(shop[i]);
                        PRINT_LOG
                    #endif

                }
            #endif 

            #ifdef RELATION_TRANSPORT_DYNAMIC
                count = transport.size();

                for (; i < count; i++) 
                {
                    data.on_insert_TRANSPORT(transport[i]);
                    PRINT_LOG

                    #ifdef RELATION_INSTITUTION_DYNAMIC
                        data.on_insert_INSTITUTION(institution[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_insert_RESTAURANT(restaurant[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_insert_HOUSE(house[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_insert_SHOP(shop[i]);
                        PRINT_LOG
                    #endif
                }
            #endif 

            #ifdef RELATION_INSTITUTION_DYNAMIC
                count = institution.size();

                for (; i < count; i++) 
                {
                    data.on_insert_INSTITUTION(institution[i]);
                    PRINT_LOG

                    #ifdef RELATION_RESTAURANT_DYNAMIC
                        data.on_insert_RESTAURANT(restaurant[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_insert_HOUSE(house[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_insert_SHOP(shop[i]);
                        PRINT_LOG
                    #endif
                }
            #endif 

            #ifdef RELATION_RESTAURANT_DYNAMIC
                count = restaurant.size();

                for (; i < count; i++)
                {
                    data.on_insert_RESTAURANT(restaurant[i]);
                    PRINT_LOG

                    #ifdef RELATION_HOUSE_DYNAMIC
                        data.on_insert_HOUSE(house[i]);
                        PRINT_LOG
                    #endif

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_insert_SHOP(shop[i]);
                        PRINT_LOG
                    #endif
                }
            #endif

            #ifdef RELATION_HOUSE_DYNAMIC
                count = house.size();

                for (; i < count; i++) 
                {
                    data.on_insert_HOUSE(house[i]);
                    PRINT_LOG

                    #ifdef RELATION_SHOP_DYNAMIC
                        data.on_insert_SHOP(shop[i]);
                        PRINT_LOG
                    #endif
                }
            #endif

            #ifdef RELATION_SHOP_DYNAMIC
                count = shop.size();

                for (; i < count; i++) 
                {
                    data.on_insert_SHOP(shop[i]);
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

    void print_result(dbtoaster::data_t& data)
    {
        data.serialize(std::cout, 0);
    }
}

#endif /* DBTOASTER_HOUSING_TEMPLATE_HPP */
