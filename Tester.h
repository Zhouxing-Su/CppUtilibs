/**
*   usage : 1.  test all units in CPPutilibs.
*
*   note :  1.
*/

#ifndef TESTER_H
#define TESTER_H


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

#include "io/File.h"

#include "type/Double.h"
#include "type/Convert.h"
#include "type/Integer.h"
#include "type/VectorMD.h"
#include "type/Arr.h"
#include "type/PriorityQueue.h"
#include "type/ConsecutiveNonNegativeIdMap.h"
#include "type/LoopQueue.h"

#include "random/Random.h"
#include "random/RandSelect.h"
#include "random/RandSample.h"

#include "algorithm/Graph.h"
#include "algorithm/BidirectionIndex.h"
#include "algorithm/Counter.h"
#include "algorithm/SetOperation.h"
#include "algorithm/String.h"

#include "log/Timer.h"
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

void testFile();

void testDouble();
void testConvert();
void testInteger();
void testVectorMD();
void testArr();
void testPriorityQueue();
void testConsecutiveNonNegativeIdMap();
void testLoopQueue();

void testRandom();
void testRandSelect();
void testRandSample();

void testGraph();
void testBidirectionIndex();
void testCounter();
void testSetOperation();
void testString();

void testTimer();
void testLog();
void testTrace();

void testMath();
void testOscillator();
void testInterval();


#endif