#include "Random.h"

using namespace std;

// record the random seed in case of future usage
const unsigned int Random::seed = static_cast<unsigned int>(time( NULL ));

// use this initialization expression as static initial blocks in java
const int Random::init = (srand( seed ), 0);