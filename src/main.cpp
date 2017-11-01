#include <sched.h>
#include "macro.hpp"
#include "types.hpp"
#include "functions.hpp"
#include "stopwatch.hpp"
#include "serialization.hpp"

#ifdef LOG_MEMORY_INFO
#include "memory.hpp"
#endif

using namespace std;
using namespace dbtoaster;

void RunQuery(bool printResult) 
{
    std::cout << "-------------" << std::endl;

    load_relations();

    Stopwatch sw, local_sw;
    for (int run = 0; run < NUMBER_OF_RUNS; run++) 
    {
    //    std::cout << "Press ENTER...";
    //    char ch;
    //    std::cin >> ch;

#ifdef LOG_MEMORY_INFO
        startHeapProfiler("MAIN");
#endif

        data_t data;

        std::cout << "-------------" << std::endl;

        local_sw.restart();
        std::cout << "1. Processing tables... " << std::flush;
        process_tables(data);
        local_sw.stop();
        std::cout << local_sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        sw.restart();

        local_sw.restart();
        std::cout << "2. OnSystemReady... " << std::flush;
        data.on_system_ready_event();
        local_sw.stop();
        std::cout << local_sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        local_sw.restart();
        std::cout << "3. Processing streams... " << std::flush;;
        process_streams(data);
        local_sw.stop();
        std::cout << local_sw.elapsedTimeInMilliSeconds() << " ms" << std::endl;

        sw.stop();

        if (printResult) print_result(data);
        
        std::cout << "    Run: " << run 
                  << "    Processed: " << data.tN 
                  << "    Skipped: " << data.tS 
                  << "    Execution time: " << sw.elapsedTimeInMilliSeconds() << " ms"
#ifdef BATCH_SIZE
                  << "    Batch size: " << BATCH_SIZE
#endif
                  << std::endl
                  << "-------------"
                  << std::endl;
    }

#ifdef LOG_MEMORY_INFO
        stopHeapProfiler();
#endif

    destroy_relations();
}

int main(int argc, char** argv)
{
    bool printResult = true;
    for (int i = 0; i < argc; i++) {
        printResult = printResult && (strcmp(argv[i], "--no-output") != 0);
    }

#ifndef __APPLE__
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
#endif

    RunQuery(printResult);
    return 0;
}
    