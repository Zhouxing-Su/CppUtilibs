#include "RangeRand.h"

using namespace std;


RangeRand::RangeRand( int lb, int ub ) : lowerBound( lb ), upperBound( ub ), range( (ub>lb)?(ub - lb + 1):1 )
{
}


RangeRand::~RangeRand()
{
}


int RangeRand::operator() ()
{
    return ((rand() % range) + lowerBound);
}