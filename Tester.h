/**
*   usage : 1.  test all units in CPPutilibs.
*
*   note :  1.
*/

#ifndef TESTER_H
#define TESTER_H


#include <iostream>
#include <sstream>

#include "type/Double.h"
#include "type/Convert.h"
#include "type/Integer.h"
#include "type/VectorMD.h"

#include "random/Random.h"
#include "random/RangeRand.h"
#include "random/RandSelect.h"

#include "algorithm/Graph.h"
#include "algorithm/BidirectionIndex.h"

#include "log/Timer.h"
#include "log/Log.h"
#include "log/Trace.h"


void testDouble();
void testConvert();
void testInteger();
void testVectorMD();

void testRandom();
void testRangeRand();
void testRandSelect();

void testGraph();
void testBidirectionIndex();

void testTimer();
void testLog();
void testTrace();
void f( int i );    // for testTrace()


#endif