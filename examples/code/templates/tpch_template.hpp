#ifndef TPCH_TEMPLATE_HPP
#define TPCH_TEMPLATE_HPP

#include <vector>
#include "stopwatch.hpp"
#include "csvreader.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;
using namespace dbtoaster;

const string dataPath = "data/tpch0.5";
const string dataset = "";

#if defined(RELATION_LINEITEM_STATIC) || (defined(RELATION_LINEITEM_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<LINEITEM_entry> lineitem;
    std::vector<LINEITEM_entry>::iterator lineitemIt;
#elif defined(RELATION_LINEITEM_DYNAMIC)
    std::vector<DELTA_LINEITEM_entry> lineitem;
    std::vector<DELTA_LINEITEM_entry>::iterator lineitemIt;
#endif

#if defined(RELATION_ORDERS_STATIC) || (defined(RELATION_ORDERS_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<ORDERS_entry> orders;
    std::vector<ORDERS_entry>::iterator ordersIt;
#elif defined(RELATION_ORDERS_DYNAMIC)
    std::vector<DELTA_ORDERS_entry> orders;
    std::vector<DELTA_ORDERS_entry>::iterator ordersIt;
#endif

#if defined(RELATION_CUSTOMER_STATIC) || (defined(RELATION_CUSTOMER_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<CUSTOMER_entry> customer;
    std::vector<CUSTOMER_entry>::iterator customerIt;
#elif defined(RELATION_CUSTOMER_DYNAMIC)
    std::vector<DELTA_CUSTOMER_entry> customer;
    std::vector<DELTA_CUSTOMER_entry>::iterator customerIt;
#endif

#if defined(RELATION_PART_STATIC) || (defined(RELATION_PART_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<PART_entry> part;
    std::vector<PART_entry>::iterator partIt;
#elif defined(RELATION_PART_DYNAMIC)
    std::vector<DELTA_PART_entry> part;
    std::vector<DELTA_PART_entry>::iterator partIt;
#endif

#if defined(RELATION_PARTSUPP_STATIC) || (defined(RELATION_PARTSUPP_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<PARTSUPP_entry> partsupp;
    std::vector<PARTSUPP_entry>::iterator partsuppIt;
#elif defined(RELATION_PARTSUPP_DYNAMIC)
    std::vector<DELTA_PARTSUPP_entry> partsupp;
    std::vector<DELTA_PARTSUPP_entry>::iterator partsuppIt;
#endif

#if defined(RELATION_SUPPLIER_STATIC) || (defined(RELATION_SUPPLIER_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<SUPPLIER_entry> supplier;
    std::vector<SUPPLIER_entry>::iterator supplierIt;
#elif defined(RELATION_SUPPLIER_DYNAMIC)
    std::vector<DELTA_SUPPLIER_entry> supplier;
    std::vector<DELTA_SUPPLIER_entry>::iterator supplierIt;
#endif

#if defined(RELATION_NATION_STATIC) || (defined(RELATION_NATION_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<NATION_entry> nation;
#elif defined(RELATION_NATION_DYNAMIC)
    std::vector<DELTA_NATION_entry> nation;
#endif

#if defined(RELATION_REGION_STATIC) || (defined(RELATION_REGION_DYNAMIC) && !defined(BATCH_SIZE))
    std::vector<REGION_entry> region;
#elif defined(RELATION_REGION_DYNAMIC)
    std::vector<DELTA_REGION_entry> region;
#endif

void load_relations() {
    Stopwatch sw;

    #if defined(RELATION_LINEITEM_STATIC) || defined(RELATION_LINEITEM_DYNAMIC)
        sw.restart();
        lineitem.clear();
        readFromFile(lineitem, dataPath + "/" + dataset + "/lineitem.csv", '|');
        sw.stop();
        std::cout << "Loaded Lineitem (" << lineitem.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_ORDERS_STATIC) || defined(RELATION_ORDERS_DYNAMIC)
        sw.restart();
        orders.clear();
        readFromFile(orders, dataPath + "/" + dataset + "/orders.csv", '|');
        sw.stop();
        std::cout << "Loaded Orders (" << orders.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_CUSTOMER_STATIC) || defined(RELATION_CUSTOMER_DYNAMIC)
        sw.restart();
        customer.clear();
        readFromFile(customer, dataPath + "/" + dataset + "/customer.csv", '|');
        sw.stop();
        std::cout << "Loaded Customer (" << customer.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_PART_STATIC) || defined(RELATION_PART_DYNAMIC)
        sw.restart();
        part.clear();
        readFromFile(part, dataPath + "/" + dataset + "/part.csv", '|');
        sw.stop();
        std::cout << "Loaded Part (" << part.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_PARTSUPP_STATIC) || defined(RELATION_PARTSUPP_DYNAMIC)
        sw.restart();
        partsupp.clear();
        readFromFile(partsupp, dataPath + "/" + dataset + "/partsupp.csv", '|');
        sw.stop();
        std::cout << "Loaded Partsupp (" << partsupp.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_SUPPLIER_STATIC) || defined(RELATION_SUPPLIER_DYNAMIC)
        sw.restart();
        supplier.clear();
        readFromFile(supplier, dataPath + "/" + dataset + "/supplier.csv", '|');
        sw.stop();
        std::cout << "Loaded Supplier (" << supplier.size() << ")... " << sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;
    #endif

    #if defined(RELATION_NATION_STATIC) || defined(RELATION_NATION_DYNAMIC)
        nation.clear();
        readFromFile(nation, dataPath + "/" + dataset + "/nation.csv", '|');
    #endif

    #if defined(RELATION_REGION_STATIC) || defined(RELATION_REGION_DYNAMIC)
        region.clear();
        readFromFile(region, dataPath + "/" + dataset + "/region.csv", '|');
    #endif
}

void destroy_relations() {
    #if defined(RELATION_LINEITEM_STATIC) || defined(RELATION_LINEITEM_DYNAMIC)
        lineitem.clear();
    #endif

    #if defined(RELATION_ORDERS_STATIC) || defined(RELATION_ORDERS_DYNAMIC)
        orders.clear();
    #endif

    #if defined(RELATION_CUSTOMER_STATIC) || defined(RELATION_CUSTOMER_DYNAMIC)
        customer.clear();
    #endif

    #if defined(RELATION_PART_STATIC) || defined(RELATION_PART_DYNAMIC)
        part.clear();
    #endif

    #if defined(RELATION_PARTSUPP_STATIC) || defined(RELATION_PARTSUPP_DYNAMIC)
        partsupp.clear();
    #endif

    #if defined(RELATION_SUPPLIER_STATIC) || defined(RELATION_SUPPLIER_DYNAMIC)
        supplier.clear();
    #endif

    #if defined(RELATION_NATION_STATIC) || defined(RELATION_NATION_DYNAMIC)
        nation.clear();
    #endif

    #if defined(RELATION_REGION_STATIC) || defined(RELATION_REGION_DYNAMIC)
        region.clear();
    #endif
}

#ifdef LOG_MEMORY_INFO
    #define PRINT_MEMORY_INFO dumpHeapProfile("TPCH");
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

    #ifdef RELATION_LINEITEM_STATIC
        for (size_t i = 0; i < lineitem.size(); i++) {
            data.on_insert_LINEITEM(lineitem[i]);
        }
    #endif

    #ifdef RELATION_ORDERS_STATIC
        for (size_t i = 0; i < orders.size(); i++) {
            data.on_insert_ORDERS(orders[i]);
        }
    #endif

    #ifdef RELATION_CUSTOMER_STATIC
        for (size_t i = 0; i < customer.size(); i++) {
            data.on_insert_CUSTOMER(customer[i]);
        }
    #endif

    #ifdef RELATION_PART_STATIC
        for (size_t i = 0; i < part.size(); i++) {
            data.on_insert_PART(part[i]);
        }
    #endif

    #ifdef RELATION_PARTSUPP_STATIC
        for (size_t i = 0; i < partsupp.size(); i++) {
            data.on_insert_PARTSUPP(partsupp[i]);
        }
    #endif

    #ifdef RELATION_SUPPLIER_STATIC
        for (size_t i = 0; i < supplier.size(); i++) {
            data.on_insert_SUPPLIER(supplier[i]);
        }
    #endif

    #ifdef RELATION_NATION_STATIC
        for (size_t i = 0; i < nation.size(); i++) {
            data.on_insert_NATION(nation[i]);
        }
    #endif

    #ifdef RELATION_REGION_STATIC
        for (size_t i = 0; i < region.size(); i++) {
            data.on_insert_REGION(region[i]);
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
        cout << endl;
        PRINT_LOG
    #endif

    #ifdef BATCH_SIZE

        size_t batchSize = BATCH_SIZE;

        #ifdef RELATION_SUPPLIER_DYNAMIC
            supplierIt = supplier.begin();
        #endif
        #ifdef RELATION_CUSTOMER_DYNAMIC
            customerIt = customer.begin();
        #endif
        #ifdef RELATION_PART_DYNAMIC
            partIt = part.begin();
        #endif
        #ifdef RELATION_PARTSUPP_DYNAMIC
            partsuppIt = partsupp.begin();
        #endif
        #ifdef RELATION_ORDERS_DYNAMIC
            ordersIt = orders.begin();
        #endif
        #ifdef RELATION_LINEITEM_DYNAMIC
            lineitemIt = lineitem.begin();
        #endif

        #ifdef RELATION_SUPPLIER_DYNAMIC
            for (; i < supplier.size() / batchSize; i++) {
                data.on_batch_update_SUPPLIER(supplierIt, supplierIt + batchSize);
                supplierIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_CUSTOMER_DYNAMIC
                    data.on_batch_update_CUSTOMER(customerIt, customerIt + batchSize);
                    customerIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_PART_DYNAMIC
                    data.on_batch_update_PART(partIt, partIt + batchSize);
                    partIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_batch_update_PARTSUPP(partsuppIt, partsuppIt + batchSize);
                    partsuppIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_batch_update_ORDERS(ordersIt, ordersIt + batchSize);
                    ordersIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                    lineitemIt += batchSize;
                    PRINT_LOG
                #endif

            }
            if (supplierIt != supplier.end()) {
                data.on_batch_update_SUPPLIER(supplierIt, supplier.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_CUSTOMER_DYNAMIC
            for (; i < customer.size() / batchSize; i++) {
                data.on_batch_update_CUSTOMER(customerIt, customerIt + batchSize);
                customerIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_PART_DYNAMIC
                    data.on_batch_update_PART(partIt, partIt + batchSize);
                    partIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_batch_update_PARTSUPP(partsuppIt, partsuppIt + batchSize);
                    partsuppIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_batch_update_ORDERS(ordersIt, ordersIt + batchSize);
                    ordersIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                    lineitemIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (customerIt != customer.end()) {
                data.on_batch_update_CUSTOMER(customerIt, customer.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_PART_DYNAMIC
            for (; i < part.size() / batchSize; i++) {
                data.on_batch_update_PART(partIt, partIt + batchSize);
                partIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_batch_update_PARTSUPP(partsuppIt, partsuppIt + batchSize);
                    partsuppIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_batch_update_ORDERS(ordersIt, ordersIt + batchSize);
                    ordersIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                    lineitemIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (partIt != part.end()) {
                data.on_batch_update_PART(partIt, part.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_PARTSUPP_DYNAMIC
            for (; i < partsupp.size() / batchSize; i++)
            {
                data.on_batch_update_PARTSUPP(partsuppIt, partsuppIt + batchSize);
                partsuppIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_batch_update_ORDERS(ordersIt, ordersIt + batchSize);
                    ordersIt += batchSize;
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                    lineitemIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (partsuppIt != partsupp.end()) {
                data.on_batch_update_PARTSUPP(partsuppIt, partsupp.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_ORDERS_DYNAMIC
            for (; i < orders.size() / batchSize; i++) {
                data.on_batch_update_ORDERS(ordersIt, ordersIt + batchSize);
                ordersIt += batchSize;
                PRINT_LOG

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                    lineitemIt += batchSize;
                    PRINT_LOG
                #endif
            }
            if (ordersIt != orders.end()) {
                data.on_batch_update_ORDERS(ordersIt, orders.end());
                PRINT_LOG
            }
        #endif

        #ifdef RELATION_LINEITEM_DYNAMIC
            for (; i < lineitem.size() / batchSize; i++) {
                data.on_batch_update_LINEITEM(lineitemIt, lineitemIt + batchSize);
                lineitemIt += batchSize;
                PRINT_LOG
            }
            if (lineitemIt != lineitem.end()) {
                data.on_batch_update_LINEITEM(lineitemIt, lineitem.end());
                PRINT_LOG
            }
        #endif

    #else

        #ifdef RELATION_SUPPLIER_DYNAMIC
            count = supplier.size();

            for (; i < count; i++) {
                data.on_insert_SUPPLIER(supplier[i]);
                PRINT_LOG

                #ifdef RELATION_CUSTOMER_DYNAMIC
                    data.on_insert_CUSTOMER(customer[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_PART_DYNAMIC
                    data.on_insert_PART(part[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_insert_PARTSUPP(partsupp[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_insert_ORDERS(orders[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_insert_LINEITEM(lineitem[i]);
                    PRINT_LOG
                #endif

            }
        #endif

        #ifdef RELATION_CUSTOMER_DYNAMIC
            count = customer.size();

            for (; i < count; i++) {
                data.on_insert_CUSTOMER(customer[i]);
                PRINT_LOG

                #ifdef RELATION_PART_DYNAMIC
                    data.on_insert_PART(part[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_insert_PARTSUPP(partsupp[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_insert_ORDERS(orders[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_insert_LINEITEM(lineitem[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_PART_DYNAMIC
            count = part.size();

            for (; i < count; i++) {
                data.on_insert_PART(part[i]);
                PRINT_LOG

                #ifdef RELATION_PARTSUPP_DYNAMIC
                    data.on_insert_PARTSUPP(partsupp[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_insert_ORDERS(orders[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_insert_LINEITEM(lineitem[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_PARTSUPP_DYNAMIC
            count = partsupp.size();

            for (; i < count; i++) {
                data.on_insert_PARTSUPP(partsupp[i]);
                PRINT_LOG

                #ifdef RELATION_ORDERS_DYNAMIC
                    data.on_insert_ORDERS(orders[i]);
                    PRINT_LOG
                #endif

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_insert_LINEITEM(lineitem[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_ORDERS_DYNAMIC
            count = orders.size();

            for (; i < count; i++) {
                data.on_insert_ORDERS(orders[i]);
                PRINT_LOG

                #ifdef RELATION_LINEITEM_DYNAMIC
                    data.on_insert_LINEITEM(lineitem[i]);
                    PRINT_LOG
                #endif
            }
        #endif

        #ifdef RELATION_LINEITEM_DYNAMIC
            count = lineitem.size();

            for (; i < count; i++) {
                data.on_insert_LINEITEM(lineitem[i]);
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

#endif /* TPCH_TEMPLATE_HPP */