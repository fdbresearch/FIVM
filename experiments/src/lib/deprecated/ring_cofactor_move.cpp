#include <iostream>
#include "ring_cofactor_move.hpp"

using namespace std;

int main(int argc, char** argv) 
{
    RingCofactor r1(1);
    // r1.count = 42;

    RingCofactor r2 = Ulift(4.504, 0);

    RingCofactor r3;

    r3 += r1 * r2 * r2;

    r3 += r3;

    RingCofactor r4;

    r4 += 4 * r4 * 2 * r3;

    // RingCofactor&& r4 = r1 * r2 * r2;    

    cout << r3.count << endl;

    // auto r3 = r1 * r2;

    return 0;
}