/**
*   usage : other class using std::rand() should inherit this class to ensure that
*           the global random seed is only initialized once.
*
*   problem :
*           1. the seed can not be set by user.
*/

#ifndef RANDOM_H

#include <ctime>
#include <cstdlib>

class Random
{
public:
    static unsigned int setSeed( unsigned int seed = static_cast<unsigned int>(time( NULL ) + clock()) );
    static unsigned int getSeed()
    {
        return seed;
    }

private:
    static unsigned int seed;
};

#define RANDOM_H
#endif