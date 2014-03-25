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
    const static unsigned int seed;

private:
    const static int init;  // used to init the rand seed, its value makes no sense
};

#define RANDOM_H
#endif