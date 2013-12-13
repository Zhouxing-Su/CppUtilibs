#include "RangeRand.h"

using namespace std;

int RangeRand::seed = (srand( static_cast<unsigned>(time( NULL )) ), 0);

RangeRand::RangeRand( int lb, int ub ) : lowerBound( lb ), upperBound( ub ), range( ub - lb + 1 )
{
}


RangeRand::~RangeRand()
{
}


int RangeRand::operator() ()
{
    return ((rand() % range) + lowerBound);
}