#include "Tester.h"


using namespace std;
using namespace szx;


int main() {
    //testDouble();
    //testTimer();
    //testRandom();
    //testRangeRand();
    //testRandSelect();
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
    //testMemory();
    //testArr();
    //testString();
    testInteger();
    //testPriorityQueue();
    //testDijkstraPathGenerator();

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
    vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8 };

    for (int i = 0; i < 20; ++i) {
        cout << Random::rand() << endl;
    }

    Random::setSeq(v);
    for (int i = 0; i < 20; ++i) {
        cout << Random::rand() << endl;
    }

    Random::setSeed();
    for (int i = 0; i < 20; ++i) {
        cout << Random::rand() << endl;
    }
}

void testRangeRand() {
    RangeRand r(1, 10);

    for (int i = 0; i < 20; ++i) {
        cout << r() << endl;
    }
}

void testRandSelect() {
    int result = 0;

    RandSelect rs;
    for (int i = 0; i < 10; ++i) {
        if (rs.isSelected()) {
            result = i;
        }
    }
    cout << result << endl;

    rs.reset(2);    // select 1 out of 11 elements (0-9 and the last selected number)
    for (int i = 0; i < 10; ++i) {
        if (rs.isSelected()) {
            result = i;
        }
        cout << result << endl;
    }

    int o[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<int> r = RandSelect::randSelect(10, 4);
    for (int i = 0; i < 4; ++i) {
        cout << o[r[i]] << ' ';
    }
    cout << endl;
}

void testGraph() {
    // ====== usage for an GeometricalGraph ======
    const int pointNum = 20;
    const int coordRange = 10;
    RangeRand plrr(-coordRange, coordRange);
    RangeRand vrr(0, pointNum - 1);

    // generate a pointList
    GeometricalGraph::PointList pointList;
    for (int i = 0; i < pointNum; ++i) {
        pointList.push_back(GeometricalGraph::Point(plrr(), plrr()));
    }

    GeometricalGraph gg(pointList);



    // ====== usage for an UndDirectedGraph ======
    UndirectedGraph<double> ug(gg);
    ug.getShortestPath();    // can be leave out
    ug.getDistSeqTable();
    ug.printShortestDist(cout);
    ug.printDistSeqTable(cout);

    int vertex = ug.nthClosestVertex(vrr(), vrr());
    vertex = ug.findVertexWithinRadius(vrr(), coordRange);
    int vnum = ug.findVertexNumWithinRadius(vrr(), coordRange);
    TopologicalGraph<double>::Distance dd = ug.distance(vrr(), vrr());



    // ====== usage for an DirectedGraph ======
    const unsigned nodeNum = 10;
    const unsigned arcNum = 20;
    const int minVertexIndex = 1;
    RangeRand alrr(minVertexIndex, nodeNum + minVertexIndex - 1);
    RangeRand drr(1, 255);

    // generate an arcList for an directed graph with unsigned distance randomly
    TopologicalGraph<>::ArcList arcList;
    for (int i = 0; i < arcNum; ++i) {
        int startVertex = alrr();
        int endVertex;
        do {
            endVertex = alrr();
        } while (endVertex == startVertex);
        arcList.push_back(TopologicalGraph<>::Arc(startVertex, endVertex, drr()));
    }

    DirectedGraph<> dg(arcList, nodeNum, minVertexIndex);
    dg.getShortestPath();    // can be leave out
    dg.getDistSeqTable();
    dg.printShortestDist(cout);
    dg.printDistSeqTable(cout);

    vertex = dg.nthClosestVertex(alrr(), alrr());
    vertex = dg.findVertexWithinRadius(alrr(), drr());
    vnum = dg.findVertexNumWithinRadius(alrr(), drr());
    TopologicalGraph<>::Distance ud = dg.distance(alrr(), alrr());
}

void testLog() {
    Log<>::write("test log true\n", true);
    Log<>::write("test log false\n", false);
    Debug<>::write("test debug true\n", true);
    Debug<>::write("test debug false\n", false);
}

void testBidirectionIndex() {
    BidirectionIndex bi(10);
    bi.insert(5);
    bi.insert(9);
    bi.insert(2);
    bi.insert(8);
    bi.insert(0);
    bi.insert(9);
    //bi.insert( 10 );      // fail and crack
    bi.eraseElement(2);
    //bi.eraseElement( 4 ); // fail and crack
    bi.eraseIndex(1);
    //bi.eraseIndex( 8 );   // fail and crack
    int i = bi.indexOf(5);
    cout << i << endl;
    //cout << bi.indexOf( 3 ) << endl;      // fail and crack
    //cout << bi.elementAt( 8 ) << endl;    // fail and crack
    cout << bi.elementAt(i) << endl;

    BidirectionIndex_Safe bis(10, 1);
    bis.insert(5);
    bis.insert(9);
    bis.insert(2);
    bis.insert(8);
    bis.insert(0);        // fail but won't crack
    bis.insert(10);
    bis.eraseElement(2);
    bis.eraseElement(4);  // fail but won't crack
    bis.eraseIndex(1);
    bis.eraseIndex(8);    // fail but won't crack
    int j = bis.indexOf(5);
    cout << j << endl;
    cout << bis.indexOf(3) << endl;   // fail but won't crack
    cout << bis.elementAt(8) << endl; // fail but won't crack
    cout << bis.elementAt(j) << endl;
}

void testTrace() {
    Trace::setHandler();
    f_testTrace(6);
}

void f_testTrace(int i) {
    Trace t("f_testTrace()");
    if (i != 22) {
        Trace t("if");
        f_testTrace(i + 1);
    }

    for (int k = 0; k < 222; ++k) {
        ostringstream oss;
        oss << k;
        Trace t("for, k=" + oss.str());
        if (k == 46) {
            Trace::dumpCallStack(cout);
            *(int*)0 = 1;   // crash
        }
    }
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
    PriorityQueueBase<int> *pqh = new PriorityQueueUsingHeap<int>();
    PriorityQueueBase<int> *pqs = new PriorityQueueUsingSet<int>();

    pqh->add(1);
    pqh->add(3);
    pqh->add(0);
    pqh->remove(1);
    pqh->reorder();
    pqh->push(2);
    pqh->push(4);
    cout << pqh->top() << endl;
    while (!pqh->empty()) {
        pqs->push(pqh->top());
        pqh->pop();
    }
    pqs->remove(4);
    cout << pqs->top() << endl;
    pqs->clear();
    cout << pqs->empty() << endl;
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
    cout << File::getLastNoneEmptyLine("../TestCases/FileTest1.txt") << endl;
    cout << File::getLastNoneEmptyLine("../TestCases/FileTest2.txt") << endl;
    cout << File::getLastNoneEmptyLine("../TestCases/FileTest3.txt") << endl;
}

void testSetOperation() {
    char a = 'a';
    set<char> cs = { 'b', 'n', 'm' };
    string s1 = "asd";
    char s2[] = "zxc";

    char target = 'a';
    cout << contain<char>(a, target) << endl;
    cout << contain(cs, target) << endl;
    cout << contain(s1, target) << endl;
    cout << contain(s2, target) << endl;
    cout << contain("test", target) << endl;

    target = 'b';
    cout << contain<char>(a, target) << endl;
    cout << contain(cs, target) << endl;
    cout << contain(s1, target) << endl;
    cout << contain(s2, target) << endl;
    cout << contain("test", target) << endl;

    target = 'c';
    cout << contain<char>(a, target) << endl;
    cout << contain(cs, target) << endl;
    cout << contain(s1, target) << endl;
    cout << contain(s2, target) << endl;
    cout << contain("test", target) << endl;
}

void testString() {
    cout << String::shortestEditDistance(
        "AGTCTGACGC",
        "AGTAAGTAGGC") << endl;
}

void testDijkstraPathGenerator() {
    vector<vector<DijkstraPathGenerator<>::Edge>> adjacencyList = {
        {{1, 2}, {2, 2}},
        {{0, 3}, {3, 1}},
        {{0, 1}},
        {{1, 3}, {2, 4}}
    };
    DijkstraPathGenerator<> dpg(adjacencyList, 0);
    cout << dpg.next([](int n) { return n > 2; }) << endl;
    for (int n = dpg.getLastNode(); n != DijkstraPathGenerator<>::InvalidId; n = dpg.getPrevNode(n)) {
        cout << n << "<-";
    }
    cout << endl;
}

void testTimer() {
    Timer t(chrono::seconds(10));
    long long i = 0;

    Timer::Microsecond start = Timer::getCPUtime();
    for (; !t.isTimeOut(); ++i) {
        for (int j = 0; j < 1000000; ++j) {
            i = j;
        }
        cout << t.restTime().count() << endl;
    }
    Timer::Microsecond end = Timer::getCPUtime();

    cout << end - start << endl;
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
    cout << Memory::getAvailablePhysicalMemory() << endl
        << Memory::getTotalPhysicalMemory() << endl
        << Memory::getMemoryStatus().dwMemoryLoad << endl;
}
