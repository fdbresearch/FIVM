//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
#ifndef DBTOASTER_MACRO_HPP
#define DBTOASTER_MACRO_HPP

#define STRING(s) #s

//-----------------------------------------------------------------------------
// Microsoft Visual Studio

#if defined(_MSC_VER)

typedef unsigned int uint32_t;

#define INLINE        inline
#define FORCE_INLINE  __forceinline
#define NEVER_INLINE  __declspec(noinline)

//-----------------------------------------------------------------------------
// Other compilers

#else //  defined(_MSC_VER)

#include <stdint.h>

// Force inlining currently disabled as it introduces bugs in GCC 6.3.0 and 5.4.0
#define FORCE_INLINE inline //__attribute__((always_inline))
#define NEVER_INLINE __attribute__((noinline))

#endif  //  !defined(_MSC_VER)

#endif /* DBTOASTER_MACRO_HPP */