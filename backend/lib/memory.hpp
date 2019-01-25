//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_MEMORY_HPP
#define DBTOASTER_MEMORY_HPP

#include <gperftools/heap-profiler.h>

static void startHeapProfiler(const char* prefix) {
    HeapProfilerStart(prefix);
}

static void stopHeapProfiler() {
    HeapProfilerStop();
}

static void dumpHeapProfile(const char* prefix) {
    HeapProfilerDump(prefix);
}

/*
// #include <malloc.h>

static void display_mallinfo() {
    // IMPORTANT: malloc_stats is used for experiments on Ubuntu but doesn't work on Mac
    malloc_stats();

    // Here is an alternative to malloc_stats, which incidentally gives some strange results
    struct mallinfo mi;

    mi = mallinfo();

    cout << "Total allocated space (uordblks): " << mi.uordblks << "     "
         << "Total free space (fordblks): " << mi.fordblks << "     "
         << "Total non-mmapped bytes (arena): " << mi.arena << "     "
         << "Bytes in mapped regions (hblkhd): " << mi.hblkhd << "     "
         << "Max. total allocated space (usmblks): " << mi.usmblks;

    // printf("Total non-mmapped bytes (arena):       %d\n", mi.arena);
    // printf("# of free chunks (ordblks):            %d\n", mi.ordblks);
    // printf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
    // printf("# of mapped regions (hblks):           %d\n", mi.hblks);
    // printf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
    // printf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
    // printf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
    // printf("Total allocated space (uordblks):      %d\n", mi.uordblks);
    // printf("Total free space (fordblks):           %d\n", mi.fordblks);
    // printf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
}
*/

#endif /* DBTOASTER_MEMORY_HPP */