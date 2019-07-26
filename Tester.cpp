#include "Tester.h"

#include <future>


namespace ap { // atomic print.

static std::mutex printMutex;

template<typename Info>
void print(const char *msg, Info info) {
    printMutex.lock();  std::cerr << msg << " " << info << std::endl; printMutex.unlock();
}

template<typename Duration>
void printSleepPrint(const char *msg, Duration duration) {
    print(msg, "begin");
    if (duration > 0us) {
        std::this_thread::sleep_for(duration);
    } else {
        std::this_thread::yield();
    }
    print(msg, "end");
}

}


using namespace std;
using namespace szx;
using namespace ap;


int main() {
    //testDouble();
    //testTimer();
    //testRandom();
    //testRangeRand();
    //testSampling();
    //testGraph();
    //testLog();
    //testBidirectionIndex();
    //testConvert();
    //testTrace();
    //testVectorMD();
    //testDirectory();
    //testCounter();
    //testSetOperation();
    //testFile();
    //testTimer();
    //testFileLock();
    //testTermination();
    testMemory();
    //testArr();
    //testString();
    //testPriorityQueue();
    //testMath();
    //testOscillator();
    //testZeroBasedConsecutiveIdMap();
    //testLoopQueue();
    //testSemaphore();
    //testThreadPool();
    //testInterval();
    //testInteger();
    //testKnapsack();
    //testCombinationMap();

    return 0;
}

void testDouble() {
    Double d(10.000001);
    Double d1(10);
    double d2(10);
    double dd(9.999999);

    cout << d1 + d2 << endl;
    cout << d2 + d1 << endl;
    cout << d + d1 << endl;
    cout << (d1 == d) << ',' << (d == d1) << endl;
    cout << (d1 == d2) << ',' << (d2 == d1) << endl;
    cout << (d == d2) << ',' << (d2 == d) << endl;
    cout << (dd == d2) << ',' << (d2 == dd) << endl;
    cout << &d << endl;
}

void testRandom() {
    Random rd(Random::generateSeed());

    for (int i = 0; i < 20; ++i) {
        cout << rd() << " " << rd.pick(100) << " " << rd.pick(100, 200) << " " << rd.isPicked(2, 5) << endl;
    }
}

void testSampling() {
    // Sampling1.
    int result = 0;

    Random rd;
    Sampling1 rs(rd, Sampling1::StartCount::NoPresetElement);
    for (int i = 0; i < 10; ++i) {
        if (rs.isPicked()) {
            result = i;
        }
    }
    cout << result << endl;

    rs.reset(Sampling1::StartCount::WithPresetElement);
    for (int i = 10; i >= 0; --i) {
        if (rs.isMinimal(result, i)) {
            result = i;
            cout << result << endl;
        } else {
            cout << "skip " << i << endl;
        }
    }
    cout << result << endl;

    // Sampling.
    enum { Population = 20, TargetNum = 10 };

    unsigned seed = random_device()();
    Random rgen0(seed), rgen1(seed);

    int count[Population + 1] = { 0 };
    for (int i = 0; i < 1000000; ++i) {
        Sampling rs0(rgen0, TargetNum);
        Sampling_Static<TargetNum> rs1(rgen1);
        int r[TargetNum + 1] = { 0 };
        for (int j = 1; j <= Population; ++j) {
            int index = rs0.isPicked();
            r[index] = j;

            if (index != (rs1.replaceIndex() + 1)) {
                cout << "error" << endl;
            }
        }
        for (int j = 1; j <= TargetNum; ++j) { ++count[r[j]]; }
    }
    for (int i = 1; i <= Population; ++i) {
        cout << count[i] << endl;
    }
}

void testGraph() {
    using ID = int;
    using Coord = double;
    using Weight = int;
    using Capacity = int;

    using PlanarGraph = GeometricalGraph<ID, Coord>;
    using Network = TopologicalGraph<ID, Weight, Capacity>;
    using UNetwork = UndirectedGraph<ID, Weight, Capacity>;
    using DNetwork = DirectedGraph<ID, Weight, Capacity>;

    Random rd;

    /// usage for an GeometricalGraph.
    const ID pointNum = 20;
    const Weight coordRange = 10;
    auto coordRand = [&]() { return rd.pick(-coordRange, coordRange + 1); };
    auto pointRand = [&]() { return rd.pick(pointNum); };

    // generate a pointList.
    PlanarGraph::PointList pointList;
    for (ID i = 0; i < pointNum; ++i) {
        pointList.push_back(GeometricalGraph<>::Point(coordRand(), coordRand()));
    }

    PlanarGraph gg(pointList);


    /// usage for an TopologicalGraph.
    Network::AdjMat<Weight> adjMat(Network::toAdjMat(gg));

    cout << adjMat.at(pointRand(), pointRand()) << endl;


    /// usage for an DirectedGraph.
    const ID edgeNum = 150;
    const ID src = 0;
    const ID dst = 4;
    const Weight maxWeight = static_cast<Weight>(edgeNum * coordRange * 2 * sqrt(2));

    auto capRand = [&]() { return rd.pick(15); };

    // generate an arcList for an directed graph with unsigned distance randomly
    vector<Network::Edge> edges;
    for (ID i = 0; i < pointNum; ++i) {
        for (ID j = 0; j < pointNum; ++j) {
            if (!rd.isPicked(edgeNum, pointNum * pointNum)) { continue; }
            edges.push_back(Network::Edge{ i, j, adjMat.at(i, j), capRand() });
        }
    }

    Network::AdjList adjList(UNetwork::toAdjList<vector>(edges, pointNum));

    DNetwork::ShortestPath::BellmanFord bf(adjList, src);
    bf.findSingleSourcePaths();
    if (bf.pathFound(dst)) {
        Network::Path path(bf.getPath(dst, false, false));
        for (auto iter = path.begin(); iter != path.end(); ++iter) {
            cout << *iter << " ";
        }
        cout << ": " << bf.getDist(dst) << endl;
    }

    DNetwork::ShortestPath::Dijkstra dj(adjList, src, maxWeight);
    int count = 10;
    dj.next([&](ID) { return (--count < 0); });
    cout << dj.getDist(dj.getLastNode()) << endl;

    DNetwork::ShortestPath::Floyd fd(adjMat);
    fd.findAllPairsPaths(false);

    DNetwork::ShortestPath::AStar as(adjList, src, [&](ID node) {
        return fd.adjMat.at(node, dst);
    }, maxWeight);
    as.findPointToPointPath(dst);
    if (as.pathFound(dst)) {
        cout << as.getDist(dst) << " " << fd.adjMat.at(src, dst) << endl;
    }

    Capacity restDemand = 30;
    DNetwork::MinCostFlow::SuccessiveShortestPath ssp(adjList, src, dst);
    cout << ssp.find(restDemand) << " : " << restDemand << endl;
    Network::AdjList flow(ssp.retrieveFlow());
    vector<Network::Traffic> traffics;
    ssp.retrievePaths(traffics, flow);
    for (auto iter = traffics.front().nodes.begin(); iter != traffics.front().nodes.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << ": " << traffics.front().bandwidth << endl;
}

void testLog() {
    enum LogType {
        SzxTestFunctionA = Log::Debug,
        SzxTestFunctionB = Log::Info,
        AsdTestClassC = Log::On,
        AsdTestClassD = Log::Off,
    };

    Log(LogType::SzxTestFunctionA) << "some debug information left by szx." << endl;
    Log(LogType::SzxTestFunctionB, cout) << "some trivial information from szx." << endl;

    Log(LogType::AsdTestClassC) << "some useful information given by asd." << endl;
    Log(LogType::AsdTestClassD) << "some useless information from asd." << endl;
}

void testConsecutiveIdSet() {
    ConsecutiveIdSet bi(10);
    bi.insert(5);
    bi.insert(9);
    bi.insert(2);
    bi.insert(8);
    bi.insert(0);
    bi.insert(9);
    //bi.insert(10);      // fail and crack
    bi.eraseItem(2);
    //bi.eraseItem(4); // fail and crack
    bi.eraseIndex(1);
    //bi.eraseIndex(8);   // fail and crack
    int i = bi.indexOf(5);
    cout << i << endl;
    //cout << bi.indexOf(3) << endl;      // fail and crack
    //cout << bi.itemAt(8) << endl;    // fail and crack
    cout << bi.itemAt(i) << endl;

    ConsecutiveIdSet_Safe bis(10, 1);
    bis.insert(5);
    bis.insert(9);
    bis.insert(2);
    bis.insert(8);
    bis.insert(0);        // fail but won't crack
    bis.insert(10);
    bis.eraseItem(2);
    bis.eraseItem(4);  // fail but won't crack
    bis.eraseIndex(1);
    bis.eraseIndex(8);    // fail but won't crack
    int j = bis.indexOf(5);
    cout << j << endl;
    cout << bis.indexOf(3) << endl;   // fail but won't crack
    cout << bis.itemAt(8) << endl; // fail but won't crack
    cout << bis.itemAt(j) << endl;
}

static void testTrace_f(int i) {
    Trace t0("testTrace_f()");
    if (i != 22) {
        Trace t1("if");
        testTrace_f(i + 1);
    }

    for (int k = 0; k < 222; ++k) {
        ostringstream oss;
        oss << k;
        Trace t1("for, k=" + oss.str());
        if (k == 46) {
            Trace::dumpCallStack(cout);
            *(int*)0 = 1;   // crash
        }
    }
}

void testTrace() {
    Trace::setHandler();
    testTrace_f(6);
}

void testConvert() {
    cout << (std::numeric_limits<string>::max)() << endl;

    string s("54321");
    int i = toType<int>(s);
    cout << "i=" << i << endl;

    i += 12345;
    s = toString(i);
    cout << "s=" << s << endl;
}

void testInteger() {
    Integer integer;
    int i = toType<int>(toString(integer));
    string s("123");

    integer = integer + s;
}

void testVectorMD() {
    // traditional multi-dimensional vector
    vector< vector< vector<int> > > vvv(
        4, vector< vector<int> >(
            3, vector<int>(
                2, 1)));

    // new multi-dimensional vector
    // with a traditional way of initialization
    VectorMD<int, 3> v3d(
        4, VectorMD<int, 2>(
            3, VectorMD<int>(
                2, 1)));

    // new multi-dimensional vector
    // with a new way of initialization
    vector<int> s = { 3, 4, 5, 6 };
    VectorMD<int, 4> v4d(s, 2);

    // new multi-dimensional vector
    // with a new way of simpler format of initialization
    VectorMD<int, 2> v2d({ 7, 8 }, 3);

    // VectorMD inherits all operations from std::vector
    v4d[1][2][3][4] = 0;
    *(v3d[0][1].begin()) = 1;
    v2d.at(5).push_back(4);
    v4d.pop_back();
}

void testArr() {
    Arr2D<int> a(4, 3);
    for (int i = 0; i < a.size1(); ++i) {
        for (int j = 0; j < a.size2(); ++j) {
            a.at(i, j) = i + j;
        }
    }
    for (auto i = a.begin(); i != a.end(); ++i) {
        cout << *i << endl;
    }
}

void testPriorityQueue() {
    impl::PriorityQueue::BucketL1Impl<double> pqb(10);
    PriorityQueue<int> pqs;

    pqb.push(1.0, 1);
    pqb.push(3.0, 3);
    pqb.push(0.0, 0);
    pqb.push(2.0, 2);
    pqb.push(4.0, 4);
    cout << pqb.top() << endl;
    while (!pqb.empty()) {
        pqs.push(pqb.topPriority(), pqb.topPriority());
        pqb.pop();
    }
    pqs.remove(4);
    cout << pqs.top() << endl;
    pqs.clear();
    cout << pqs.empty() << endl;
}

void testZeroBasedConsecutiveIdMap() {
    ZeroBasedConsecutiveIdMap<int> idMap(20);

    vector<int> aid = { 3543, 6434, 897454, 75646, 246584, 1654 };
    vector<int> cid;

    for (auto iter = aid.begin(); iter != aid.end(); ++iter) {
        cid.push_back(idMap.toConsecutiveId(*iter));
        cout << *iter << " -> " << cid.back() << endl;
    }
    for (auto iter = cid.begin(); iter != cid.end(); ++iter) {
        cout << *iter << " -> " << idMap.toArbitraryId(*iter) << endl;
    }
}

void testLoopQueue() {
    LoopQueue<int> lq(5);

    lq.pushBack(1);
    lq.pushBack(2);

    for (int i = 3; i < 8; ++i) {
        lq.pushBack(i);
        lq.popFront();
        lq.pushBack(++i);
    }

    while (!lq.empty()) {
        cout << lq.front() << endl;
        lq.popFront();
    }
}

void testCombinationMap() {
    int nodeNum = 200;
    int tourNum = 100000;
    int minNodeNumInTour = 3;
    int writeCount = 4 * tourNum;
    int readCount = 12 * tourNum;
    string cacheFilePath = "TspCache.csv";

    struct Tour {
        int dist;
        vector<int> nodes;
    };

    using TspCache = CombinationMap<Tour, int>;

    mt19937 rgen;
    chrono::steady_clock::time_point begin, end;

    cerr << "init test data: ";
    begin = chrono::steady_clock::now();
    TspCache::ItemList nodes(nodeNum);
    for (int i = 0; i < nodeNum; ++i) { nodes[i] = i; }
    vector<Tour> tours(tourNum);
    for (int i = 0; i < tourNum; ++i) {
        if (rgen() % 1000 != 0) { shuffle(nodes.begin(), nodes.end(), rgen); } // small number of duplicated paths with different distances.
        int tourLen = (rgen() % (nodeNum - minNodeNumInTour)) + minNodeNumInTour;
        tours[i].nodes.resize(tourLen);
        for (int n = 0; n < tourLen; ++n) { tours[i].nodes[n] = nodes[n]; }
        tours[i].dist = static_cast<int>(rgen() % 10000) - 100; // allow negative distance.
    }
    TspCache::ItemSet containNode(nodeNum);
    TspCache tspCache(nodeNum, tourNum);
    end = chrono::steady_clock::now();
    cerr << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

    cerr << endl;

    cerr << "zero overhead set test (x" << writeCount << "): ";
    rgen.seed();
    begin = chrono::steady_clock::now();
    int overwriteCount = 0;
    for (int i = 0; i < writeCount; ++i) {
        Tour &tour(tours[rgen() % tours.size()]); // pick a random tour.
        tspCache.toItemSet(tour.nodes, containNode);
        ++overwriteCount;
    }
    end = chrono::steady_clock::now();
    cerr << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

    cerr << "              set test (x" << writeCount << "): ";
    rgen.seed();
    begin = chrono::steady_clock::now();
    overwriteCount = 0;
    for (int i = 0; i < writeCount; ++i) {
        Tour &tour(tours[rgen() % tours.size()]); // pick a random tour.
        tspCache.toItemSet(tour.nodes, containNode);
        overwriteCount += tspCache.set(containNode, tour, // add it to cache.
            [](const Tour &oldTour, const Tour &newTour) { return newTour.dist < oldTour.dist; });
    }
    end = chrono::steady_clock::now();
    cerr << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cerr << "overwrite=" << overwriteCount << endl;

    cerr << endl;

    cerr << "zero overhead get test (x" << readCount << "): ";
    rgen.seed();
    begin = chrono::steady_clock::now();
    int missCount = 0;
    for (int i = 0; i < readCount; ++i) {
        Tour &tour(tours[rgen() % tours.size()]); // pick a random tour.
        tspCache.toItemSet(tour.nodes, containNode);
        for (int perturb = rgen() % 8 - 1; perturb > 0; --perturb) {
            containNode[rgen() % nodeNum] = (rgen() & 1); // modify the node set randomly.
        }
        missCount += tour.nodes.empty();
    }
    end = chrono::steady_clock::now();
    cerr << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;

    cerr << "              get test (x" << readCount << "): ";
    rgen.seed();
    begin = chrono::steady_clock::now();
    missCount = 0;
    for (int i = 0; i < readCount; ++i) {
        Tour &tour(tours[rgen() % tours.size()]); // pick a random tour.
        tspCache.toItemSet(tour.nodes, containNode);
        for (int perturb = rgen() % 8 - 1; perturb > 0; --perturb) {
            containNode[rgen() % nodeNum] = (rgen() & 1); // modify the node set randomly.
        }
        const Tour &sln(tspCache.get(containNode)); // read it from cache.
        missCount += sln.nodes.empty();
    }
    end = chrono::steady_clock::now();
    cerr << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cerr << "miss=" << missCount << endl;
}

void testDirectory() {
    Directory::makeSureDirExist("a/b/c/d/e");
    Directory::makeSureDirExist("a/b/c/f/g/");
}

void testCounter() {
    Counter<int> c;

    vector<int> v(5);
    generate(v.begin(), v.end(), c);

    for_each(v.begin(), v.end(), [](int val) { cout << val << endl; });
}

void testFile() {
    cout << File::readAll("../TestCases/FileTest1.txt") << endl;
    cout << File::readAll("../TestCases/FileTest2.txt") << endl;
    cout << File::readAll("../TestCases/FileTest3.txt") << endl;
}

void testContainer() {
    set<char> cs = { 'b', 'n', 'm' };

    char target = 'a';
    cout << contains(cs, target) << endl;
    target = 'b';
    cout << contains(cs, target) << endl;
    target = 'c';
    cout << contains(cs, target) << endl;
}

void testString() {
    cout << String::shortestEditDistance(
        "AGTCTGACGC",
        "AGTAAGTAGGC") << endl;
}

void testKnapsack() {
    int runCount = 10;

    //int maxItemWidth = 2500;
    //int maxItemHeight = 1500;
    //int minItemLength = 100;
    //int capacity = 6000 * 3210;
    //int maxItemNum = 300;
    //int minItemNum = 50;
    //int candidateNum = 8;
    int maxItemWidth = 1250;
    int maxItemHeight = 750;
    int minItemLength = 50;
    int capacity = 3000 * 1605;
    int maxItemNum = 150;
    int minItemNum = 25;
    int candidateNum = 4;

    Random r;
    vector<Knapsack::Item<int>> items;
    items.reserve(maxItemNum);
    auto resetItemList = [&]() {
        items.clear();
        for (int i = r.pick(minItemNum, maxItemNum); i >= 0; --i) {
            items.push_back(r.pick(minItemLength, maxItemWidth) * r.pick(minItemLength, maxItemHeight));
        }
    };
    auto printResult = [](ostream &os, Knapsack::List<Knapsack::IdSet> slns, int weight, int value) {
        os << "weight=" << weight << " value=" << value << endl;
        for (auto s = slns.begin(); s != slns.end(); ++s) {
            int i = 0;
            for (auto item = s->begin(); item != s->end(); ++item, ++i) {
                if (*item) { os << i << " "; }
            }
            os << endl;
        }
        os << endl;
    };

    // speed test.
    {
        Timer timer(0ms);
        r.setSeed();
        for (int k = 0; k < runCount; ++k) {
            resetItemList();

            int value = 2;
            Knapsack::List<Knapsack::IdSet> slns;
            int weight = Knapsack::dynamicProgramming(items, capacity, value, slns, candidateNum);
            cout << k << " (" << items.size() << "): " << timer.elapsedSeconds() << endl;
        }
        cout << "dp: " << timer.elapsedSeconds() << endl;
    }
    cout << endl;
    {
        Timer timer(0ms);
        r.setSeed();
        for (int k = 0; k < runCount; ++k) {
            resetItemList();

            int value = 2;
            Knapsack::List<Knapsack::IdSet> slns;
            int weight = Knapsack::dynamicProgrammingForSparseWeightDistribution(items, capacity, value, slns, candidateNum);
            cout << k << " (" << items.size() << "): " << timer.elapsedSeconds() << endl;
        }
        cout << "dp+: " << timer.elapsedSeconds() << endl;
    }

    // correctness check.
    {
        ofstream ofs("dp0.txt");
        r.setSeed();
        for (int k = 0; k < runCount; ++k) {
            resetItemList();

            int value = 2;
            Knapsack::List<Knapsack::IdSet> slns;
            int weight = Knapsack::dynamicProgramming(items, capacity, value, slns, candidateNum);
            printResult(ofs, slns, weight, value);
        }
    }

    {
        ofstream ofs("dp1.txt");
        r.setSeed();
        for (int k = 0; k < runCount; ++k) {
            resetItemList();

            int value = 2;
            Knapsack::List<Knapsack::IdSet> slns;
            int weight = Knapsack::dynamicProgrammingForSparseWeightDistribution(items, capacity, value, slns, candidateNum);
            printResult(ofs, slns, weight, value);
        }
    }

}

#if SZX_CPPUTILIBS_TIMER_CPP_STYLE
ostream& operator<<(ostream &os, Timer::Millisecond ms) { return os << ms.count(); }
#endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE

void testTimer() {
    Timer t(chrono::seconds(10));
    long long i = 0;

    Timer::TimePoint start = Timer::Clock::now();
    for (; !t.isTimeOut(); ++i) {
        for (int j = 0; j < 1000000; ++j) {
            i = j;
        }
        cout << t.restMilliseconds() << endl;
    }
    Timer::TimePoint end = Timer::Clock::now();

    cout << Timer::durationInMillisecond(start, end) << endl;
    cout << i << endl;
}

void testTermination() {
    Termination::waitForCodeAsync("szx");

    do {
        cout << "0";
        this_thread::sleep_for(chrono::milliseconds(100));
    } while (false);
}

void testMemory() {
    cout << Memory::getPhysicalMemoryByteSize() << endl
        << Memory::getFreePhysicalMemoryByteSize() << endl
        << Memory::getPhysicalMemoryKbSize() << endl
        << Memory::memoryUsage().physicalMemory << endl
        << Memory::peakMemoryUsage().virtualMemory << endl;

    cout << Memory::setMaxMemorySize(1, Memory::Scale::MB) << endl;

    char *c = new char[1024];
    delete c;
    cout << "allocation successful." << endl;

    try {
        c = new char[2 * 1024 * 1024];
        delete c;
    } catch (...) {
        cout << "quota exceeded." << endl;
    }
}

void testSemaphore() {
    Semaphore semaphore(2);

    for (int i = 0; i < 8; ++i) {
        thread([&semaphore, i]() {
            semaphore.wait();
            print("t", i);
        }).detach();
    }

    this_thread::sleep_for(1s);

    for (int i = 0; i < 8; ++i) {
        thread([&semaphore, i]() {
            this_thread::sleep_for(chrono::milliseconds(i * 500));
            semaphore.notify();
        }).detach();
    }

    this_thread::sleep_for(5s);
}

static void testThreadPool_f1() {
    printSleepPrint("f1", 2s);
}
static void testThreadPool_f2(int &i) {
    printSleepPrint("f2", 1000ms);
    ++i;
}

void testThreadPool() {
    struct {
        void operator()(int k) {
            printSleepPrint("f3", 2s);
            i = k;
        }
        int i;
    } f3 = { 0 };
    struct {
        void operator()() {
            printSleepPrint("f4", 1s);
            ++i;
        }
        int i;
    } f4 = { 0 };
    struct F5 {
        void operator()() const {
            printSleepPrint("f5", 0h);
            ++i;
        }
        mutable int i;
    } f5o = { 0 };
    const F5 &f5(f5o);

    int f2i = 0;

    packaged_task<int(void)> timer([]() {
        int sec = 0;
        for (; sec < 7; ++sec) {
            print("--------", sec);
            this_thread::sleep_for(1s);
        }
        return sec;
    });
    future<int> duration(timer.get_future());

    this_thread::sleep_for(500ms);

    print("f2.i=", f2i);
    print("f3.i=", f3.i);
    print("f4.i=", f4.i);
    print("f5.i=", f5.i);

    ThreadPool<impl::ThreadPool::QueueImpl> tp(3);
    tp.push(timer);

    {
        ThreadPool<impl::ThreadPool::SingleSlotImpl> tp0(4);
        tp0.push([]() { printSleepPrint("f0", 0.05min); });
        tp0.push(testThreadPool_f1);
        tp0.push(std::bind(testThreadPool_f2, std::ref(f2i)));
        print("tp0(4)", "all jobs are pushed");
    } // automatic pending.

    tp.push(std::bind(std::ref(f3), 1));
    tp.push(f4);
    tp.push(f5);
    print("tp(3)", "all jobs are pushed");

    //tp.pend(); // comment this line to test force stopping.
    tp.stop(); // f5 will be abandoned without pend().

    print("f2.i=", f2i);
    print("f3.i=", f3.i);
    print("f4.i=", f4.i);
    print("f5.i=", f5.i);

    print("duration=", duration.get());
}


static void testMathLog2() {
    std::mt19937 rgen(Random::generateSeed());
    int len = 100000000;
    std::vector<unsigned> r(len);
    std::vector<unsigned> v(len);
    for (auto i = 0; i < len; ++i) { v[i] = rgen(); }
    Timer::TimePoint start, end;
    int testItem = rgen() % len;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = v[i];
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " emptyLoop " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = static_cast<int>(std::log2(v[i]));
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " std::log2 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v1(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v1 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v2(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v2 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v3(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v3 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v4(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v4 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v5(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v5 " << Timer::durationInMillisecond(start, end).count() << std::endl;

    start = Timer::Clock::now();
    for (auto i = 0; i < len; ++i) {
        r[i] = impl::Math::log2v6(v[i]);
    }
    end = Timer::Clock::now();
    cout << r[testItem] << " log2v6 " << Timer::durationInMillisecond(start, end).count() << std::endl;
}

static void testMathUpdateMinMax() {
    int i = 0;
    if (Math::updateMin(i, 0)) {
        cout << "i has been updated to" << i << endl;
    }
    if (Math::updateMin(i, 10)) {
        cout << "i has been updated to" << i << endl;
    }
    if (Math::updateMin(i, -2)) {
        cout << "i has been updated to" << i << endl;
    }
    if (Math::updateMax(i, 3)) {
        cout << "i has been updated to" << i << endl;
    }
    if (Math::updateMax(i, -5)) {
        cout << "i has been updated to" << i << endl;
    }
    if (Math::updateMax(i, 3)) {
        cout << "i has been updated to" << i << endl;
    }
}

static void testMathNormalize() {
    vector<int> v = { 58, 9999, 6587, 354, 1984, 3989 };

    Math::normalize(v, 100000);
    for (auto iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    Math::normalize(v, 1000);
    for (auto iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;
}

void testMath() {
    cout << Math::round(1.7) << " " << Math::round(2.1) << endl;

    cout << Math::power2(0) << " " << Math::power2(1) << " " << Math::power2(4) << " " << Math::power2(-2) << endl;

    testMathLog2();

    testMathUpdateMinMax();

    cout << Math::average(6, 2, 4, 3) << endl;

    testMathNormalize();
}

void testOscillator() {
    Oscillator<int> oscillator(-4, 9, 3, Oscillator<int>::Direction::Down, 1);

    for (; oscillator != oscillator.end(); ++oscillator) {
        cout << oscillator++ << endl;
        cout << oscillator << endl;
    }
}

void testInterval() {
    Interval<int> i0(1, 4);
    Interval<int> i1(5, 8);
    Interval<int> i2(-1, 6);
    Interval<int> i3(2, 7);

    cout << i0.before(i1) << endl;
    cout << i2.cover(i0) << endl;
    cout << i0.cover(2) << endl;
    cout << i0.isValid() << endl;
    cout << i0.beginBefore(i1) << endl;
    cout << i0.beginBefore(3) << endl;
    cout << i0.endBefore(i1) << endl;
    cout << i0.endBefore(6) << endl;
    cout << Interval<int>::isOverlapped(i0, i3) << endl;
    cout << Interval<int>::overlap(i0, i3).begin << endl;
    cout << Interval<int>::overlap(i1, i3).end << endl;
}
