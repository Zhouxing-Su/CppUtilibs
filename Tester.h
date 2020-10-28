////////////////////////////////
/// usage : 1.	test all units in CPPutilibs.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_TESTER_H
#define SZX_CPPUTILIBS_TESTER_H


#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "system/Directory.h"
#include "system/Termination.h"
#include "system/Memory.h"
#include "system/Semaphore.h"
#include "system/ThreadPool.h"
#include "system/ColorStr.h"

#include "io/File.h"

#include "type/Double.h"
#include "type/Convert.h"
#include "type/Integer.h"
#include "type/VectorMD.h"
#include "type/Arr.h"
#include "type/PriorityQueue.h"
#include "type/ZeroBasedConsecutiveIdMap.h"
#include "type/LoopQueue.h"
#include "type/CombinationMap.h"

#include "random/Random.h"
#include "random/Sampling.h"

#include "algorithm/Graph.h"
#include "algorithm/ConsecutiveIdSet.h"
#include "algorithm/Counter.h"
#include "algorithm/Container.h"
#include "algorithm/String.h"
#include "algorithm/Knapsack.h"

#include "log/Timer.h"
#include "log/DateTime.h"
#include "log/Log.h"
#include "log/Trace.h"

#include "math/Math.h"
#include "math/Oscillator.h"
#include "math/Interval.h"


void testDirectory();
void testTermination();
void testMemory();
void testSemaphore();
void testThreadPool();
void testColorStr();

void testFile();

void testDouble();
void testConvert();
void testInteger();
void testVectorMD();
void testArr();
void testPriorityQueue();
void testZeroBasedConsecutiveIdMap();
void testLoopQueue();
void testCombinationMap();

void testRandom();
void testSampling();

void testGraph();
void testConsecutiveIdSet();
void testCounter();
void testContainer();
void testString();
void testKnapsack();

void testTimer();
void testDateTime();
void testLog();
void testTrace();

void testMath();
void testOscillator();
void testInterval();


#endif // SZX_CPPUTILIBS_TESTER_H
