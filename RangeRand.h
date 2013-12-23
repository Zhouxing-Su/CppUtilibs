/**
*   usage:  create function object which can generate random integers between [lowerBound,upperBound].
*/

#ifndef RANGERAND_H
#define RANGERAND_H


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


#endif