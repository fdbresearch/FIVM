#ifndef RINGCOFACTOR_DUMMY_HPP
#define RINGCOFACTOR_DUMMY_HPP

#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;
typedef DOUBLE_TYPE RingCofactor;

RingCofactor Ulift(DOUBLE_TYPE a, int idx) {
    return a;
}

RingCofactor Ulift2(DOUBLE_TYPE a1, DOUBLE_TYPE a2, int idx) {
    return a1 * a2;
}

RingCofactor Ulift3(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, int idx) {
    return a1 * a2 * a3;
}

RingCofactor Ulift4(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, int idx) {
    return a1 * a2 * a3 * a4;
}

RingCofactor Ulift5(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, int idx) {
    return a1 * a2 * a3 * a4 * a5;
}

RingCofactor Ulift6(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, DOUBLE_TYPE a6, int idx) {
    return a1 * a2 * a3 * a4 * a5 * a6;
}

RingCofactor Ulift10(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, int idx) {
    return a1 * a2 * a3 * a4 * a5 * a6 * a7 * a8 * a9 * a10;
}

RingCofactor Ulift13(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10, 
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, int idx) {
    return a1 * a2 * a3 * a4 * a5 * a6 * a7 * a8 * a9 * a10 * a11 * a12 * a13;
}

RingCofactor Ulift15(DOUBLE_TYPE a1, DOUBLE_TYPE a2, DOUBLE_TYPE a3, DOUBLE_TYPE a4, DOUBLE_TYPE a5, 
                     DOUBLE_TYPE a6, DOUBLE_TYPE a7, DOUBLE_TYPE a8, DOUBLE_TYPE a9, DOUBLE_TYPE a10,
                     DOUBLE_TYPE a11, DOUBLE_TYPE a12, DOUBLE_TYPE a13, DOUBLE_TYPE a14, DOUBLE_TYPE a15, int idx) {
 return a1 * a2 * a3 * a4 * a5 * a6 * a7 * a8 * a9 * a10 * a11 * a12 * a13 * a14 * a15;
}
#endif /* RINGCOFACTOR_NAIVE_HPP */