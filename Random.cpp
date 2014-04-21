#include "Random.h"

using namespace std;

// record the random seed in case of future usage
unsigned int Random::seed = setSeed();

unsigned int Random::setSeed( unsigned int s )
{
    seed = s;
    srand( seed );
    return seed;
}