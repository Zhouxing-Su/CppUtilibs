/**
*   usage:  create function object which can generate random integers between [lowerBound,upperBound].

(example code)
====================================================================
#include <iostream>
#include "RangeRand.h"

using namespace std;

int main( int argc, char **argv )
{
RangeRand r( 1, 10 );

for (int i = 0; i < 20; i++) {
cout << r() << endl;
}
return 0;
}
====================================================================
*/

#ifndef RANGERAND_H


#include <cstdlib>
#include <ctime>

class RangeRand
{
public:
    // type definitions ===================

    // static constant ===================

    // member constant ===================
    const int lowerBound;
    const int upperBound;
    const int range;

    // static methods ===================

    // member methods ===================
    RangeRand( int lowerBound = 0, int upperBound = RAND_MAX );
    ~RangeRand();

    int operator() ();

private:
    static int seed;    // this is used for calling srand() only once and should not be used elsewhere !
};


#define RANGERAND_H
#endif