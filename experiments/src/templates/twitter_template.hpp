#ifndef DBTOASTER_TWITTER_TEMPLATE_HPP
#define DBTOASTER_TWITTER_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"
#include "degree.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;

const string dataPath = "datasets/twitter";
const string dataset = "";
const string filename = "higgs-social_network.tbl";

namespace dbtoaster 
{
    class data_t;

    // ASSUMPTION: All twitterX datasets have equal size


    #if defined(RELATION_TWITTER1_STATIC)
        std::vector<TWITTER1_entry> twitter1;
        std::vector<TWITTER1_entry>::iterator twitterIt1;
    #elif defined(RELATION_TWITTER1_DYNAMIC)
        std::vector<DELTA_TWITTER1_entry> twitter1;
        std::vector<DELTA_TWITTER1_entry>::iterator twitterIt1;
    #endif

    #if defined(RELATION_TWITTER2_STATIC)
        std::vector<TWITTER2_entry> twitter2;
        std::vector<TWITTER2_entry>::iterator twitterIt2;    
    #elif defined(RELATION_TWITTER2_DYNAMIC)
        std::vector<DELTA_TWITTER2_entry> twitter2;
        std::vector<DELTA_TWITTER2_entry>::iterator twitterIt2;    
    #endif

    #if defined(RELATION_TWITTER3_STATIC)
        std::vector<TWITTER3_entry> twitter3;
        std::vector<TWITTER3_entry>::iterator twitterIt3;
    #elif defined(RELATION_TWITTER3_DYNAMIC)
        std::vector<DELTA_TWITTER3_entry> twitter3;
        std::vector<DELTA_TWITTER3_entry>::iterator twitterIt3;
    #endif

    #if defined(RELATION_TWITTER4_STATIC)
        std::vector<TWITTER4_entry> twitter4;
        std::vector<TWITTER4_entry>::iterator twitterIt4;
    #elif defined(RELATION_TWITTER4_DYNAMIC)
        std::vector<DELTA_TWITTER4_entry> twitter4;
        std::vector<DELTA_TWITTER4_entry>::iterator twitterIt4;
    #endif

    #if defined(RELATION_TWITTER5_STATIC)
        std::vector<TWITTER5_entry> twitter5;
        std::vector<TWITTER5_entry>::iterator twitterIt5;
    #elif defined(RELATION_TWITTER5_DYNAMIC)
        std::vector<DELTA_TWITTER5_entry> twitter5;
        std::vector<DELTA_TWITTER5_entry>::iterator twitterIt5;
    #endif

    #if defined(RELATION_TWITTER6_STATIC)
        std::vector<TWITTER6_entry> twitter6;
        std::vector<TWITTER6_entry>::iterator twitterIt6;
    #elif defined(RELATION_TWITTER6_DYNAMIC)
        std::vector<DELTA_TWITTER6_entry> twitter6;
        std::vector<DELTA_TWITTER6_entry>::iterator twitterIt6;
    #endif

    #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
        std::vector<DEGREE1_entry> degree1;
    #endif

    #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
        std::vector<DEGREE2_entry> degree2;
    #endif

    void load_relations()
    {
        Stopwatch sw;

        #if defined(RELATION_TWITTER1_STATIC) || defined(RELATION_TWITTER1_DYNAMIC)
            sw.restart();
            twitter1.clear();
            readFromFile(twitter1, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter1 (" << twitter1.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TWITTER2_STATIC) || defined(RELATION_TWITTER2_DYNAMIC)
            sw.restart();
            twitter2.clear();
            readFromFile(twitter2, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter2 (" << twitter2.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TWITTER3_STATIC) || defined(RELATION_TWITTER3_DYNAMIC)
            sw.restart();
            twitter3.clear();
            readFromFile(twitter3, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter3 (" << twitter3.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TWITTER4_STATIC) || defined(RELATION_TWITTER4_DYNAMIC)
            sw.restart();
            twitter4.clear();
            readFromFile(twitter4, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter4 (" << twitter4.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TWITTER5_STATIC) || defined(RELATION_TWITTER5_DYNAMIC)
            sw.restart();
            twitter5.clear();
            readFromFile(twitter5, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter5 (" << twitter5.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_TWITTER6_STATIC) || defined(RELATION_TWITTER6_DYNAMIC)
            sw.restart();
            twitter6.clear();
            readFromFile(twitter6, dataPath + "/" + dataset + "/" + filename, '|');
            sw.stop();
            std::cout << "Loaded Twitter6 (" << twitter6.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
        #endif

        #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
            degree1.clear();
            readFromFile(degree1, dataPath + "/" + dataset + "/../degree/Degree1.tbl", '|');            
        #endif

        #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
            degree2.clear();
            readFromFile(degree2, dataPath + "/" + dataset + "/../degree/Degree2.tbl", '|');            
        #endif
    }

    void destroy_relations()
    {
        #if defined(RELATION_TWITTER1_STATIC) || defined(RELATION_TWITTER1_DYNAMIC)
            twitter1.clear();
        #endif

        #if defined(RELATION_TWITTER2_STATIC) || defined(RELATION_TWITTER2_DYNAMIC)
            twitter2.clear();
        #endif

        #if defined(RELATION_TWITTER3_STATIC) || defined(RELATION_TWITTER3_DYNAMIC)
            twitter3.clear();
        #endif

        #if defined(RELATION_TWITTER4_STATIC) || defined(RELATION_TWITTER4_DYNAMIC)
            twitter4.clear();
        #endif

        #if defined(RELATION_TWITTER5_STATIC) || defined(RELATION_TWITTER5_DYNAMIC)
            twitter5.clear();
        #endif

        #if defined(RELATION_TWITTER6_STATIC) || defined(RELATION_TWITTER6_DYNAMIC)
            twitter6.clear();
        #endif

        #if defined(RELATION_DEGREE1_STATIC) || defined(RELATION_DEGREE1_DYNAMIC)
            degree1.clear();
        #endif

        #if defined(RELATION_DEGREE2_STATIC) || defined(RELATION_DEGREE2_DYNAMIC)
            degree2.clear();
        #endif
    }

    #ifdef LOG_MEMORY_INFO
        #define PRINT_MEMORY_INFO dumpHeapProfile("TWITTER");
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

        #ifdef RELATION_TWITTER1_STATIC
            for (size_t i = 0; i < twitter1.size(); i++) 
            {   
                data.on_insert_TWITTER1(twitter1[i]);
            }
        #endif

        #ifdef RELATION_TWITTER2_STATIC
            for (size_t i = 0; i < twitter2.size(); i++) 
            {   
                data.on_insert_TWITTER2(twitter2[i]);
            }
        #endif

        #ifdef RELATION_TWITTER3_STATIC
            for (size_t i = 0; i < twitter3.size(); i++) 
            {   
                data.on_insert_TWITTER3(twitter3[i]);
            }
        #endif

        #ifdef RELATION_TWITTER4_STATIC
            for (size_t i = 0; i < twitter4.size(); i++) 
            {   
                data.on_insert_TWITTER4(twitter4[i]);
            }
        #endif

        #ifdef RELATION_TWITTER5_STATIC
            for (size_t i = 0; i < twitter5.size(); i++) 
            {   
                data.on_insert_TWITTER5(twitter5[i]);
            }
        #endif

        #ifdef RELATION_TWITTER6_STATIC
            for (size_t i = 0; i < twitter6.size(); i++) 
            {   
                data.on_insert_TWITTER6(twitter6[i]);
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

            #ifdef RELATION_TWITTER1_DYNAMIC
                twitterIt1 = twitter1.begin();
            #endif 
            
            #ifdef RELATION_TWITTER2_DYNAMIC
                twitterIt2 = twitter2.begin();
            #endif 
            
            #ifdef RELATION_TWITTER3_DYNAMIC
                twitterIt3 = twitter3.begin();
            #endif 

            #ifdef RELATION_TWITTER4_DYNAMIC
                twitterIt4 = twitter4.begin();
            #endif 

            #ifdef RELATION_TWITTER5_DYNAMIC
                twitterIt5 = twitter5.begin();
            #endif 

            #ifdef RELATION_TWITTER6_DYNAMIC
                twitterIt3 = twitter6.begin();
            #endif

            for (; i < twitter1.size() / batchSize; i++) {

                #ifdef RELATION_TWITTER1_DYNAMIC
                    data.on_batch_update_TWITTER1(twitterIt1, twitterIt1 + batchSize);
                    twitterIt1 += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER2_DYNAMIC
                    data.on_batch_update_TWITTER2(twitterIt2, twitterIt2 + batchSize);
                    twitterIt2 += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER3_DYNAMIC
                    data.on_batch_update_TWITTER3(twitterIt3, twitterIt3 + batchSize);
                    twitterIt3 += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER4_DYNAMIC
                    data.on_batch_update_TWITTER4(twitterIt4, twitterIt4 + batchSize);
                    twitterIt4 += batchSize;
                    PRINT_LOG
                #endif
                #ifdef RELATION_TWITTER5_DYNAMIC
                    data.on_batch_update_TWITTER5(twitterIt5, twitterIt5 + batchSize);
                    twitterIt5 += batchSize;
                    PRINT_LOG
                #endif
                #ifdef RELATION_TWITTER6_DYNAMIC
                    data.on_batch_update_TWITTER6(twitterIt5, twitterIt6 + batchSize);
                    twitterIt6 += batchSize;
                    PRINT_LOG
                #endif
            }

            #ifdef RELATION_TWITTER1_DYNAMIC
                if (twitterIt1 != twitter1.end()) {
                    data.on_batch_update_TWITTER1(twitterIt1, twitter1.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_TWITTER2_DYNAMIC
                if (twitterIt2 != twitter2.end()) {
                    data.on_batch_update_TWITTER2(twitterIt2, twitter2.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_TWITTER3_DYNAMIC
                if (twitterIt3 != twitter3.end()) {
                    data.on_batch_update_TWITTER3(twitterIt3, twitter3.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_TWITTER4_DYNAMIC
                if (twitterIt4 != twitter4.end()) {
                    data.on_batch_update_TWITTER4(twitterIt4, twitter4.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_TWITTER5_DYNAMIC
                if (twitterIt5 != twitter5.end()) {
                    data.on_batch_update_TWITTER5(twitterIt5, twitter5.end());
                    PRINT_LOG
                }
            #endif

            #ifdef RELATION_TWITTER6_DYNAMIC
                if (twitterIt6 != twitter6.end()) {
                    data.on_batch_update_TWITTER6(twitterIt6, twitter6.end());
                    PRINT_LOG
                }
            #endif

        #else

            count = twitter1.size();

            for (; i < count; i++) {

                #ifdef RELATION_TWITTER1_DYNAMIC
                    data.on_insert_TWITTER1(twitter1[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER2_DYNAMIC
                    data.on_insert_TWITTER2(twitter2[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER3_DYNAMIC
                    data.on_insert_TWITTER3(twitter3[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_TWITTER4_DYNAMIC
                    data.on_insert_TWITTER4(twitter4[i]);
                    PRINT_LOG
                #endif
                #ifdef RELATION_TWITTER5_DYNAMIC
                    data.on_insert_TWITTER5(twitter5[i]);
                    PRINT_LOG
                #endif
                #ifdef RELATION_TWITTER6_DYNAMIC
                    data.on_insert_TWITTER6(twitter6[i]);
                    PRINT_LOG
                #endif                
            }
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

#endif /* DBTOASTER_TWITTER_TEMPLATE_HPP */
