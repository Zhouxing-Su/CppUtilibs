////////////////////////////////
/// usage : 1.	template for Geometrical Graph and Topological Graph.
///             some basic related operation on graph is implemented.
/// 
/// note  : 1.  
////////////////////////////////

#ifndef SZX_CPPUTILIBS_GRAPH_H
#define SZX_CPPUTILIBS_GRAPH_H


#include <array>
#include <vector>

#include <cmath>

#include "../type/PriorityQueue.h"
#include "../system/Exception.h"


// [off] use compare base priorty queue instead of bucket/radix based one.
//       it will disable lazy remove, which keep the relaxed node in the queue. it requires the priority queue
//       implementation tracks the original priority and the number of items in queue is irrelevant to its performance.
//       bucket queue may be a good reason to enable this switch.
#define SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE  0

// [?] use adjacency matrix to store weight and capacity.
#define SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_ADJ_MATRIX  0
// [on] update the topology even if the demand has been met (to retrieve flow or check consistency of data structure).
#define SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT  1
// [off] using dijkstra (with negative weight reduction after augmentation) instead of bellman-ford.
#define SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA  0 // not finished!!!!
// [on] record pointer instead of index of AdjNode in the adjacency list.
//      enable it only when you are sure that the container iterator will never be invalidated.
#define SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_TRACK_ADJ_NODE_POINTER  1


namespace szx {

template<typename ID = int>
class Graph {
public:
    static constexpr ID InvalidId = -1;
    static constexpr ID InvalidIndex = -1;
};


template<typename ID = int, typename Coord = double>
class GeometricalGraph : public Graph<ID> {
public:
    class Point {
    public:
        Point() {}
        Point(Coord cx, Coord cy) : x(cx), y(cy) {}

        Coord x;
        Coord y;
    };
    using PointList = std::vector<Point>;

    class Rectangle {
    public:
        Rectangle(Coord l, Coord r, Coord t, Coord b) : left(l), right(r), top(t), bottom(b) {}

        Coord left, right, top, bottom;
    };


    GeometricalGraph(const PointList &pl) : pointList(pl), minCoverRectSolved(false),
        minCoverRect(pl[0].x, pl[0].y, pl[0].x, pl[0].y) {}


    ID getVertexNum() const { return pointList.size(); }

    const PointList& getPointList() const { return pointList; }
    const Point& getPoint(ID i) const { return pointList[i]; }

    template<typename Weight = Coord>
    Weight distance(ID src, ID dst) const {
        Coord dx = getPoint(src).x - getPoint(dst).x;
        Coord dy = getPoint(src).y - getPoint(dst).y;
        return static_cast<Weight>(std::sqrt(dx * dx + dy * dy));
    }

    // regularize both the x and y of points to [0.0, 1.0].
    void regularize() {
        shift(Point(0, 0));

        // EXTEND[szx][5]: regularize geometrical graph.
    }

    // shift all points to where the left bottom of the minimal covering rectangle
    // of all points is located at the given coordinate.
    void shift(Point newLeftBottom) {
        getMinCoverRect();

        Coord shiftLeft = minCoverRect.left - newLeftBottom.x;
        Coord shiftDown = minCoverRect.bottom - newLeftBottom.y;

        // move each point.
        for (PointList::iterator iter = pointList.begin(); iter != pointList.end(); iter++) {
            iter->x -= shiftLeft;
            iter->y -= shiftDown;
        }

        // update minimal covering rectangle.
        minCoverRect.left -= shiftLeft;
        minCoverRect.right -= shiftLeft;
        minCoverRect.top -= shiftDown;
        minCoverRect.bottom -= shiftDown;
    }

    // stretch or shrink coordinate of all points.
    void stretch(double xAmplification, double yAmplification) {
        for (PointList::iterator iter = pointList.begin(); iter != pointList.end(); iter++) {
            iter->x *= xAmplification;
            iter->y *= yAmplification;
        }
    }
    void stretch(double amplification) { stretch(amplification, amplification); }

    // get the minimal covering rectangle of all points.
    const Rectangle& getMinCoverRect() {
        if (!minCoverRectSolved) {
            for (PointList::const_iterator iter = pointList.begin(); iter != pointList.end(); iter++) {
                if (iter->x < minCoverRect.left) {
                    minCoverRect.left = iter->x;
                } else if (iter->x > minCoverRect.right) {
                    minCoverRect.right = iter->x;
                }
                if (iter->y < minCoverRect.bottom) {
                    minCoverRect.bottom = iter->y;
                } else if (iter->y > minCoverRect.top) {
                    minCoverRect.top = iter->y;
                }
            }

            minCoverRectSolved = true;
        }

        return minCoverRect;
    }

protected:
    PointList pointList;

    bool minCoverRectSolved;
    Rectangle minCoverRect;
};


template<typename ID = int, typename Weight = int, typename Capacity = int>
class TopologicalGraph : public Graph<ID> {
public:
    static constexpr Weight NoLinkWeight = (std::numeric_limits<Weight>::max)() / 2; // avoid max+max overflow.;
    static constexpr Capacity InfiniteCapacity = (std::numeric_limits<Capacity>::max)();


    struct Edge {
        ID src;
        ID dst;
        Weight weight;
        Capacity capacity;
    };

    struct AdjInfo {
        Weight weight;
        Capacity capacity;
    };

    struct AdjNode {
        AdjNode(ID dstNodeId, Weight edgeWeight = 0, Capacity edgeCapacity = InfiniteCapacity)
            : dst(dstNodeId), weight(edgeWeight), capacity(edgeCapacity) {}

        ID dst;
        Weight weight;
        Capacity capacity;
    };
    using AdjNodePtr = AdjNode*;

    template<typename T = AdjNode>
    using AdjVec = std::vector<T>;

    // EXTEND[szx][3]: make it a template and use AdjVec<ID> as its argument.
    using AdjList = std::vector<AdjVec<>>;

    template<typename T = AdjInfo>
    using AdjMat = Arr2D<T>;

    using Path = std::vector<ID>;

    struct Traffic {
        Path nodes;
        Capacity bandwidth;
    };


    template<typename Coord = Weight>
    static AdjMat<Weight> toAdjMat(const GeometricalGraph<ID, Coord> &geoGraph) {
        AdjMat<Weight> adjMat(geoGraph.getVertexNum(), geoGraph.getVertexNum());

        for (ID i = 0; i < geoGraph.getVertexNum(); ++i) {
            for (ID j = 0; j < i; ++j) {
                adjMat.at(i, j) = adjMat.at(j, i) = static_cast<Weight>(geoGraph.distance(i, j));
            }
            adjMat.at(i, i) = 0;
        }

        return adjMat;
    }
};

template<typename ID = int, typename Weight = int, typename Capacity = int>
class UndirectedGraph : public TopologicalGraph<ID, Weight, Capacity> {
public:
    template<template<typename...> class List = std::vector>
    static AdjList toAdjList(const List<Edge> &edges, ID nodeNum) {
        AdjList adjList(nodeNum);

        for (auto iter = edges.begin(); iter != edges.end(); ++iter) {
            adjList[iter->src].push_back(AdjNode(iter->dst, iter->weight, iter->capacity));
            adjList[iter->dst].push_back(AdjNode(iter->src, iter->weight, iter->capacity));
        }

        return adjList;
    }
};

template<typename ID = int, typename Weight = int, typename Capacity = int>
class DirectedGraph : public TopologicalGraph<ID, Weight, Capacity> {
public:
    class ShortestPath {
    public:
        struct Cmp {
            explicit Cmp() : cost(nullptr) {}
            explicit Cmp(std::vector<Weight> *c) : cost(c) {}
            // less weight means higher priority. and it is optimized for the dst gets greater ID.
            bool operator()(ID l, ID r) const { return (((*cost)[l] != (*cost)[r]) ? ((*cost)[l] < (*cost)[r]) : (l > r)); }
            std::vector<Weight> *cost;
        };


        class Base {
        public:
            virtual ~Base() {}

            // invalidate the results (when the topology is changed) for recomputing.
            virtual void clear() { throw NotImplementedException(); }


            virtual Path getReversePath(ID /*dst*/, bool /*withoutSrc*/ = true, bool /*withoutDst*/ = true) const { throw NotImplementedException(); }
            virtual Path getPath(ID /*dst*/, bool /*withoutSrc*/ = true, bool /*withoutDst*/ = true) const { throw NotImplementedException(); }
        };

        class PointToPoint : public virtual Base {
            // pass in the other node (the first one is given in the constructor).
            // return true if the path between the given nodes is found, false otherwise.
            virtual bool findPointToPointPath(ID /*dst*/) { throw NotImplementedException(); }

            virtual bool pathFound(ID /*dst*/) const { throw NotImplementedException(); }
        };

        class SingleSource : public virtual Base {
            // find the paths from the node given in the constructor to all other nodes.
            virtual void findSingleSourcePaths() { throw NotImplementedException(); }
        };

        class AllPairs : public virtual Base {
            // find the paths between each pair of nodes in the graph.
            virtual void findAllPairsPaths(bool /*recordPath*/ = true) { throw NotImplementedException(); }
        };


        class BellmanFord : public PointToPoint, SingleSource {
        public:
            // OPTIMIZE[szx][2]: use less maxCount to achieve early exit?
            static constexpr ID DefaultInitMaxEnqueueCount = 12; // make sure (DefaultInitMaxEnqueueCount < adjList.size()).


            BellmanFord(const AdjList& adjacencyList, ID srcNodeId) : adjList(adjacencyList),
                src(srcNodeId), nodesToRelax(adjacencyList.size()), inQueue(adjList.size()),
                prevNode(adjacencyList.size()), minCost(adjacencyList.size()) {
                clear();
            }

            virtual void clear() override {
                std::fill(prevNode.begin(), prevNode.end(), InvalidId);

                std::fill(minCost.begin(), minCost.end(), NoLinkWeight);
                minCost[src] = 0;

                nodesToRelax.clear();
                nodesToRelax.pushBack(src);

                std::fill(inQueue.begin(), inQueue.end(), false);
                inQueue[src] = true;
            }


            // find shortest paths to all other nodes from source.
            virtual void findSingleSourcePaths() override {
                while (!nodesToRelax.empty()) {
                    // OPTIMIZE[szx][8]: Large Label Last optimization.
                    ID node = nodesToRelax.front();

                    inQueue[node] = false;
                    nodesToRelax.popFront();

                    const AdjVec<>& adjVec(adjList[node]);
                    for (auto adjNode = adjVec.begin(); adjNode != adjVec.end(); ++adjNode) {
                        if (adjNode->capacity <= 0) { continue; }
                        Weight newDist = minCost[node] + adjNode->weight;
                        if (newDist < minCost[adjNode->dst]) {
                            minCost[adjNode->dst] = newDist;
                            prevNode[adjNode->dst] = node;

                            if (!inQueue[adjNode->dst]) {
                                inQueue[adjNode->dst] = true;
                                // Small Label First optimization.
                                (minCost[adjNode->dst] < minCost[node])
                                    ? nodesToRelax.pushFront(adjNode->dst)
                                    : nodesToRelax.pushBack(adjNode->dst);
                            }
                        }
                    }
                }
            }
            // return one of the node in the cycle.
            ID findSingleSourcePaths_bfsNegtiveCycleDetect() {
                ID maxEnqueueCount = DefaultInitMaxEnqueueCount;
                std::vector<ID> count(adjList.size(), 0);

                ++count[src];
                while (!nodesToRelax.empty()) {
                    // OPTIMIZE[szx][8]: Large Label Last optimization.
                    ID node = nodesToRelax.front();

                    inQueue[node] = false;
                    nodesToRelax.popFront();

                    const AdjVec<>& adjVec(adjList[node]);
                    for (auto adjNode = adjVec.begin(); adjNode != adjVec.end(); ++adjNode) {
                        if (adjNode->capacity <= 0) { continue; }
                        Weight newDist = minCost[node] + adjNode->weight;
                        if (newDist < minCost[adjNode->dst]) {
                            minCost[adjNode->dst] = newDist;
                            prevNode[adjNode->dst] = node;

                            if (++count[adjNode->dst] > maxEnqueueCount) { // potential negative weight cycle detected.
                                std::vector<bool> visited(adjList.size(), false);
                                for (ID m = adjList.size() - 1; m >= 0; --m) {
                                    std::vector<bool> visitedThisRound(adjList.size(), false);
                                    for (ID n = m; n != InvalidId; n = prevNode[n]) {
                                        if (visitedThisRound[n]) { return n; }
                                        if (visited[n]) { break; }
                                        visitedThisRound[n] = true;
                                        visited[n] = true;
                                    }
                                } // false positive.
                                ++maxEnqueueCount; // EXTEND[szx][9]: check (maxEnqueueCount < adjList.size())?
                            }

                            if (!inQueue[adjNode->dst]) {
                                inQueue[adjNode->dst] = true;
                                nodesToRelax.pushBack(adjNode->dst);
                            }
                        }
                    }
                }

                return InvalidId;
            }
            ID findSingleSourcePaths_dfsNegtiveCycleDetect() {
                // EXTEND[szx][9]: finish dfs version of bellman-ford.
                return InvalidId;
            }

            virtual bool findPointToPointPath(ID dst) override {
                findSingleSourcePaths();
                return pathFound(dst);
            }

            virtual bool pathFound(ID dst) const override { return (prevNode[dst] != InvalidId); }

            virtual Path getReversePath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path;
                path.reserve(adjList.size());
                ID endNode = withoutSrc ? src : InvalidId;
                if (withoutDst) { dst = prevNode[dst]; }
                for (; dst != endNode; dst = prevNode[dst]) { path.push_back(dst); }
                return path;
            }
            virtual Path getPath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path(getReversePath(dst, withoutSrc, withoutDst));
                std::reverse(path.begin(), path.end());
                return path;
            }

            Path getReverseCycle(ID dst, bool dupDst = true) const {
                Path cycle;
                cycle.reserve(adjList.size());
                cycle.push_back(dst);
                for (ID n = prevNode[dst]; n != dst; n = prevNode[n]) { cycle.push_back(n); }
                if (dupDst) { cycle.push_back(dst); }
                return cycle;
            }
            Path getCycle(ID dst, bool dupDst = true) const {
                Path cycle(getReverseCycle(dst, dupDst));
                std::reverse(cycle.begin(), cycle.end());
                return cycle;
            }

            const std::vector<Weight>& getDistList() const { return minCost; }
            Weight getDist(ID dst) const { return minCost[dst]; }

        protected:
            /// input.
            const AdjList& adjList;
            ID src;
            /// auxiliary.
            LoopQueue<ID> nodesToRelax;
            std::vector<bool> inQueue;
            /// output.
            std::vector<ID> prevNode;
            std::vector<Weight> minCost;

        private:
            BellmanFord(const BellmanFord &) = delete;
            BellmanFord& operator=(const BellmanFord &) = delete;
        };


        #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_ADJ_MATRIX
        class BellmanFord_mat : public PointToPoint, SingleSource {
        public:
            BellmanFord_mat(const std::vector<std::vector<ID> >& radjacencyList, const AdjMat<std::array<AdjInfo, 2> >& adjInfo, ID srcNodeId) : radjList(radjacencyList),
                src(srcNodeId), nodesToRelax(radjacencyList.size()), inQueue(radjList.size()),
                prevNode(radjacencyList.size()), minCost(radjacencyList.size()), adjNodeInfo(adjInfo) {
                clear();
            }

            enum LinkType { Real, Fake };

            void clear() override {
                std::fill(prevNode.begin(), prevNode.end(), InvalidId);

                std::fill(minCost.begin(), minCost.end(), NoLinkWeight);
                minCost[src] = 0;

                nodesToRelax.clear();
                nodesToRelax.pushBack(src);

                std::fill(inQueue.begin(), inQueue.end(), false);
                inQueue[src] = true;
            }


            // find shortest paths to all other nodes from source.
            void findSingleSourcePaths() override {
                while (!nodesToRelax.empty()) {
                    // OPTIMIZE[szx][8]: Large Label Last optimization.
                    ID node = nodesToRelax.front();

                    inQueue[node] = false;
                    nodesToRelax.popFront();


                    for (auto adjNode = radjList[node].begin(); adjNode != radjList[node].end(); ++adjNode) {
                        if ((adjNodeInfo.at(*adjNode, node)[LinkType::Real].capacity <= 0)
                            && (adjNodeInfo.at(*adjNode, node)[LinkType::Fake].capacity <= 0)) {
                            continue;
                        }
                        Weight newDist;
                        if (adjNodeInfo.at(*adjNode, node)[LinkType::Fake].capacity > 0) {
                            newDist = minCost[node] + adjNodeInfo.at(*adjNode, node)[LinkType::Fake].weight;
                        } else if (adjNodeInfo.at(*adjNode, node)[LinkType::Real].capacity > 0) {
                            newDist = minCost[node] + adjNodeInfo.at(*adjNode, node)[LinkType::Real].weight;
                        }
                        if (newDist < minCost[*adjNode]) {
                            minCost[*adjNode] = newDist;
                            prevNode[*adjNode] = node;

                            if (!inQueue[*adjNode]) {
                                inQueue[*adjNode] = true;
                                // Small Label First optimization.
                                (minCost[*adjNode] < minCost[node])
                                    ? nodesToRelax.pushFront(*adjNode)
                                    : nodesToRelax.pushBack(*adjNode);
                            }
                        }
                    }
                }

                // EXTEND[szx][8]: negative weight cycle detection.
            }

            bool findPointToPointPath(ID dst) override {
                findSingleSourcePaths();
                return (prevNode[dst] != InvalidId);
            }

            Path getReversePath(ID dst, bool withoutSrc = true, bool withoutDst = true) const  override {
                Path path;
                path.reserve(radjList.size());
                ID endNode = withoutSrc ? src : InvalidId;
                if (withoutDst) { dst = prevNode[dst]; }
                for (; dst != endNode; dst = prevNode[dst]) { path.push_back(dst); }
                return path;
            }
            Path getPath(ID dst, bool withoutSrc = true, bool withoutDst = true) const  override {
                Path path(getReversePath(dst, withoutSrc, withoutDst));
                std::reverse(path.begin(), path.end());
                return path;
            }

            const std::vector<Weight>& getDistList() const { return minCost; }
            Weight getDist(ID dst) const { return minCost[dst]; }

        protected:
            /// input.
            const std::vector<std::vector<ID> >& radjList;
            const AdjMat<std::array<AdjInfo, 2> >& adjNodeInfo;
            ID src;
            /// auxiliary.
            LoopQueue<ID> nodesToRelax;
            std::vector<bool> inQueue;
            /// output.
            std::vector<ID> prevNode;
            std::vector<Weight> minCost;

        private:
            BellmanFord_mat(const BellmanFord_mat &) = delete;
            BellmanFord_mat& operator=(const BellmanFord_mat &) = delete;
        };
        #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_ADJ_MATRIX


        class Floyd : public AllPairs {
        public:
            Floyd(const Arr2D<Weight> &squareWeightMatrix) : adjMat(squareWeightMatrix) {}


            // find shortest paths between each pair of nodes.
            // src -> nextMedian[src, dst] ->->->->-> dst
            virtual void findAllPairsPaths(bool recordPath = true) override {
                ID nodeNum = adjMat.size1();

                if (recordPath) {
                    nextMedian.init(nodeNum, nodeNum);
                    auto iter = nextMedian.begin();
                    for (ID src = 0; src < nodeNum; ++src) {
                        for (ID dst = 0; dst < nodeNum; ++dst) { *(iter++) = dst; }
                    }
                }

                for (ID mid = 0; mid < nodeNum; ++mid) {
                    for (ID src = 0; src < nodeNum; ++src) {
                        for (ID dst = 0; dst < nodeNum; ++dst) {
                            Weight w = adjMat.at(src, mid) + adjMat.at(mid, dst);
                            if (w < adjMat.at(src, dst)) {
                                adjMat.at(src, dst) = w;
                                if (recordPath) { nextMedian.at(src, dst) = nextMedian.at(src, mid); }
                            }
                        }
                    }
                }
            }

            Path getPath(ID src, ID dst) const {
                Path path;
                path.reserve(nextMedian.size1());
                while (src != dst) { path.push_back(src = nextMedian.at(src, dst)); }
                return path;
            }

            ID getPathLength(ID src, ID dst) const {
                ID len = 0;
                while (src != dst) { src = nextMedian.at(src, dst); ++len; }
                return len;
            }


            /// input & output.
            Arr2D<Weight> adjMat;
            /// output.
            Arr2D<ID> nextMedian;
        };


        class Dijkstra : public PointToPoint, SingleSource {
        public:
            #if SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
            using PriorityQueueImp = impl::PriorityQueue::SetImpl<ID, Cmp>;
            #else
            using PriorityQueueImp = impl::PriorityQueue::BucketL1Impl<ID>;
            #endif // SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE


            Dijkstra(const AdjList &adjacencyList, ID source, Weight maxWeight = NoLinkWeight)
                : adjList(adjacencyList), src(source), minCost(adjacencyList.size()),
                prevNode(adjacencyList.size()), nodeVisited(adjacencyList.size()),
                #if SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                unvisitedNodes(Cmp(&minCost)) {
                #else
                nodeQueued(adjacencyList.size()), unvisitedNodes(maxWeight) {
                #endif // SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                clear();
            }

            virtual void clear() override {
                std::fill(minCost.begin(), minCost.end(), NoLinkWeight);
                minCost[src] = 0;
                std::fill(prevNode.begin(), prevNode.end(), InvalidId);
                unvisitedNodes.clear();
                std::fill(nodeVisited.begin(), nodeVisited.end(), false);

                #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                std::fill(nodeQueued.begin(), nodeQueued.end(), false);
                nodeNumInQueue = 0;
                #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                nodeVisited[src] = true;

                #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                nodeQueued[src] = true;
                #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                for (auto adjNode = adjList[src].begin(); adjNode != adjList[src].end(); ++adjNode) {
                    if (adjNode->capacity <= 0) { continue; }

                    Weight newDist = adjNode->weight;
                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    nodeQueued[adjNode->dst] = true;
                    ++nodeNumInQueue;
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    minCost[adjNode->dst] = newDist;
                    prevNode[adjNode->dst] = src;

                    unvisitedNodes.push(adjNode->dst, newDist);
                }
            }

            void reset(ID source) {
                src = source;
                clear();
            }
            void reset(const std::vector<bool> &isNodeVisited) {
                clear();
                nodeVisited = isNodeVisited;
            }
            void reset(const std::vector<bool> &isNodeVisited, ID source) {
                src = source;
                reset(isNodeVisited);
            }


            template<typename Pred>
            ID next(const Pred &isTarget) {
                while (!unvisitedNodes.empty()) {
                    ID nearestNode = unvisitedNodes.top();

                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    if (nodeVisited[nearestNode]) { unvisitedNodes.pop(); continue; }
                    if (nodeNumInQueue <= 0) { return InvalidId; }
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    if (isTarget(nearestNode)) { return nearestNode; }

                    nodeVisited[nearestNode] = true;
                    unvisitedNodes.pop();

                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    --nodeNumInQueue;
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    for (auto adjNode = adjList[nearestNode].begin(); adjNode != adjList[nearestNode].end(); ++adjNode) {
                        if (nodeVisited[adjNode->dst]) { continue; } // assume if a node is visited, `newDist < minCost[adjNode->dst]` will always be false.
                        if (adjNode->capacity <= 0) { continue; }

                        Weight newDist = minCost[nearestNode] + adjNode->weight;
                        if (newDist < minCost[adjNode->dst]) {
                            #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                            if (!nodeQueued[adjNode->dst]) {
                                nodeQueued[adjNode->dst] = true;
                                ++nodeNumInQueue;
                            }
                            #else
                            unvisitedNodes.remove(adjNode->dst);
                            #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                            minCost[adjNode->dst] = newDist;
                            prevNode[adjNode->dst] = nearestNode;

                            unvisitedNodes.push(adjNode->dst, newDist);
                        }
                    }
                }

                return InvalidId;
            }
            ID nextClosest() {
                // OPTIMIZE[szx][8]: record prevClosest node to save the trailing next() call.
                if (unvisitedNodes.empty()) { return InvalidId; }
                ID nearestNode = unvisitedNodes.top();
                next([nearestNode](ID node) { return (node != nearestNode); });
                return nearestNode;
            }
            virtual bool findPointToPointPath(ID dst) override { return (next([dst](ID node) { return (node == dst); }) != InvalidId); }
            virtual void findSingleSourcePaths() override { next([](ID) { return false; }); }

            virtual bool pathFound(ID dst) const override { return (prevNode[dst] != InvalidId); }

            virtual Path getReversePath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path;
                path.reserve(adjList.size());
                ID endNode = withoutSrc ? src : InvalidId;
                if (withoutDst) { dst = prevNode[dst]; }
                for (; dst != endNode; dst = prevNode[dst]) { path.push_back(dst); }
                return path;
            }
            virtual Path getPath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path(getReversePath(dst, withoutSrc, withoutDst));
                std::reverse(path.begin(), path.end());
                return path;
            }

            const std::vector<Weight>& getDistList() const { return minCost; }
            Weight getDist(ID dst) const { return minCost[dst]; }

            ID getPrevNode(ID node) const { return prevNode[node]; }
            const std::vector<ID>& getPrevNodeList() const { return prevNode; }

            const AdjList& getAdjacencyList() const { return adjList; }
            ID getSource() const { return src; }
            const std::vector<bool>& getNodeVisited() const { return nodeVisited; }
            const PriorityQueueImp& getUnvisitedNodes() const { return unvisitedNodes; }
            ID getLastNode() const { return (const_cast<Dijkstra*>(this))->unvisitedNodes.top(); }

        protected:
            /// input.
            const AdjList &adjList;
            ID src;
            /// output.
            std::vector<Weight> minCost; // `minCost[node]` is the min cost from source to `node`.
            std::vector<ID> prevNode; // `prevNode[node]` is where the `node` came from in the shortest path.
            std::vector<bool> nodeVisited;

            #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
            std::vector<bool> nodeQueued; // `nodeQueued[node]` means if the node has been enqueued.
            ID nodeNumInQueue; // number of nodes that is in queue currently.
            #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

            PriorityQueueImp unvisitedNodes;

        private:
            Dijkstra& operator=(const Dijkstra &) = delete;
        };


        class AStar : public PointToPoint {
        public:
            #if SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
            using PriorityQueueImp = impl::PriorityQueue::SetImpl<ID, Cmp>;
            #else
            using PriorityQueueImp = impl::PriorityQueue::BucketL1Impl<ID>;
            #endif // SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

            using HeuristicFunction = std::function<Weight(ID)>;


            AStar(const AdjList &adjacencyList, ID source, HeuristicFunction approxCostToDst, Weight maxWeight = NoLinkWeight)
                : adjList(adjacencyList), src(source), minCost(adjacencyList.size()), prevNode(adjacencyList.size()),
                nodeVisited(adjacencyList.size()), approxPathCosts(adjacencyList.size()), approxCost(approxCostToDst),
                #if SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                unvisitedNodes(Cmp(&approxPathCosts)) {
                #else
                nodeQueued(adjacencyList.size()), unvisitedNodes(maxWeight) {
                #endif // SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                clear();
            }

            virtual void clear() override {
                std::fill(minCost.begin(), minCost.end(), NoLinkWeight);
                minCost[src] = 0;
                std::fill(prevNode.begin(), prevNode.end(), InvalidId);
                std::fill(approxPathCosts.begin(), approxPathCosts.end(), NoLinkWeight);
                unvisitedNodes.clear();
                unvisitedNodes.push(src, 0); // it won't bother that it is not the approximate cost to dst.
                std::fill(nodeVisited.begin(), nodeVisited.end(), false);

                #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                std::fill(nodeQueued.begin(), nodeQueued.end(), false);
                nodeNumInQueue = 1;
                #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
            }


            virtual bool findPointToPointPath(ID dst) override {
                nodeVisited[src] = true;
                unvisitedNodes.pop();

                #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                nodeQueued[src] = true;
                --nodeNumInQueue;
                #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                for (auto adjNode = adjList[src].begin(); adjNode != adjList[src].end(); ++adjNode) {
                    if (adjNode->capacity <= 0) { continue; }

                    Weight newDist = adjNode->weight;
                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    nodeQueued[adjNode->dst] = true;
                    ++nodeNumInQueue;
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    minCost[adjNode->dst] = newDist;
                    // OPTIMIZE[szx][9]: apply tie breaking optimization.
                    // http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#breaking-ties
                    newDist += approxCost(adjNode->dst);
                    approxPathCosts[adjNode->dst] = newDist;
                    prevNode[adjNode->dst] = src;

                    unvisitedNodes.push(adjNode->dst, newDist);
                }

                while (!unvisitedNodes.empty()) {
                    ID nearestNode = unvisitedNodes.top();

                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    if (nodeVisited[nearestNode]) { unvisitedNodes.pop(); continue; }
                    if (nodeNumInQueue <= 0) { return InvalidId; }
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    if (nearestNode == dst) { return true; }

                    nodeVisited[nearestNode] = true;
                    unvisitedNodes.pop();

                    #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                    --nodeNumInQueue;
                    #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                    for (auto adjNode = adjList[nearestNode].begin(); adjNode != adjList[nearestNode].end(); ++adjNode) {
                        if (nodeVisited[adjNode->dst]) { continue; } // assume if a node is visited, `newDist < minCost[adjNode->dst]` will always be false.
                        if (adjNode->capacity <= 0) { continue; }

                        Weight newDist = minCost[nearestNode] + adjNode->weight;
                        if (newDist < minCost[adjNode->dst]) { // EXTEND[szx][8]: check if node is visited when applying inadmissible heuristics.
                            #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
                            if (!nodeQueued[adjNode->dst]) {
                                nodeQueued[adjNode->dst] = true;
                                ++nodeNumInQueue;
                            }
                            #else
                            unvisitedNodes.remove(adjNode->dst);
                            #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

                            minCost[adjNode->dst] = newDist;
                            // OPTIMIZE[szx][9]: apply tie breaking optimization.
                            // http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html#breaking-ties
                            newDist += approxCost(adjNode->dst);
                            approxPathCosts[adjNode->dst] = newDist;
                            prevNode[adjNode->dst] = nearestNode;

                            unvisitedNodes.push(adjNode->dst, newDist);
                        }
                    }
                }

                return false;
            }

            virtual bool pathFound(ID dst) const override { return (prevNode[dst] != InvalidId); }

            virtual Path getReversePath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path;
                path.reserve(adjList.size());
                ID endNode = withoutSrc ? src : InvalidId;
                if (withoutDst) { dst = prevNode[dst]; }
                for (; dst != endNode; dst = prevNode[dst]) { path.push_back(dst); }
                return path;
            }
            virtual Path getPath(ID dst, bool withoutSrc = true, bool withoutDst = true) const override {
                Path path(getReversePath(dst, withoutSrc, withoutDst));
                std::reverse(path.begin(), path.end());
                return path;
            }

            Weight getDist(ID dst) const { return minCost[dst]; }

        protected:
            /// input.
            const AdjList &adjList;
            ID src;
            /// output.
            std::vector<Weight> minCost; // `minCost[node]` is the min cost from source to `node`.
            std::vector<ID> prevNode; // `prevNode[node]` is where the `node` came from in the shortest path.
            std::vector<bool> nodeVisited;

            std::vector<Weight> approxPathCosts; // `approxPathCosts[node]` is the cost from `src` to `dst` via `node`.
                                                 // `Weight approxCost(ID node)` is the heuristic function (the lower bound of cost from `node` to `dst`).
                                                 // you may need to calibrate the approximate cost manually when the topology is changed significantly.
            HeuristicFunction approxCost;

            #if !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE
            std::vector<bool> nodeQueued; // `nodeQueued[node]` means if the node has been enqueued.
            ID nodeNumInQueue; // number of nodes that is in queue currently.
            #endif // !SZX_CPPUTILIBS_SHORTEST_PATH_CMP_BASED_QUEUE

            PriorityQueueImp unvisitedNodes;

        private:
            AStar& operator=(const AStar &) = delete;
        };
    };


    class MinCostFlow {
    public:
        // (real) link A->B: a link that exists in the original topology goes from A to B.
        // fake link A->B: an artificial link that is not in the original topology goes from A to B.
        // symmetric links: real link A->B and fake link B->A is a pair of symmetric links.
        // 
        // the bandwidth sum of a pair of symmetric links is equal to the orginal bandwidth of the real link.
        // the costs on a link is the opposite number of its symmetric link.
        class SuccessiveShortestPath {
        public:
            using LinkPos = AdjNodePtr; // EXTEND[szx][9]: use iterator?
            using LinkPosPair = std::array<LinkPos, 2>; // EXTEND[szx][9]: use struct, std::array or std::vector?


            enum LinkType { Real, Fake };


            // radjacencyList is a reversed adjacency list that records in-degrees of each node.
            SuccessiveShortestPath() {}
            SuccessiveShortestPath(const AdjList &radjacencyList, ID producerId, ID consumerId)
                : nodeNum(radjacencyList.size()), producer(producerId), consumer(consumerId),
                radjList(radjacencyList), adjNodePos(nodeNum, nodeNum) {}

            void backup(SuccessiveShortestPath &ssp) const {
                ssp.nodeNum = nodeNum;
                ssp.producer = producer;
                ssp.consumer = consumer;
                ssp.radjList = radjList;

                ssp.totalCost = totalCost;
                ssp.totalFlow = totalFlow;
            }

            void recover(const SuccessiveShortestPath &ssp) {
                init(ssp.radjList, ssp.producer, ssp.consumer);

                totalCost = ssp.totalCost;
                totalFlow = ssp.totalFlow;
            }

            void init() {
                reserve();

                adjNodePos.reset(Arr2D<LinkPosPair>::ResetOption::AllBits0);
                for (ID dst = 0; dst < nodeNum; ++dst) {
                    ID adjNodeNum = radjList[dst].size();
                    for (ID i = 0; i < adjNodeNum; ++i) {
                        AdjNode &adjNode(radjList[dst][i]);
                        LinkPosPair &pos(adjNodePos.at(adjNode.dst, dst));
                        if (adjNode.weight > 0) {
                            pos[LinkType::Real] = &adjNode;
                        } else if (adjNode.weight < 0) {
                            pos[LinkType::Fake] = &adjNode;
                        } else {
                            if ((dst == producer) || (adjNode.dst == consumer)) {
                                pos[LinkType::Fake] = &adjNode;
                            } else {
                                pos[LinkType::Real] = &adjNode;
                            }
                        }
                    }
                }

                totalCost = 0;
                totalFlow = 0;
            }
            void init(const AdjList &radjacencyList, ID producerId, ID consumerId) {
                nodeNum = radjacencyList.size();
                producer = producerId;
                consumer = consumerId;
                radjList = radjacencyList;
                adjNodePos.init(nodeNum, nodeNum);

                init();
            }


            // the demand is the requested amount of flow and will be set to the amount that can not be met.
            // return the cost for the flow.
            Weight find(Capacity &demand) {
                demand -= totalFlow;
                return find(producer, consumer, demand, true);
            }

            // get the flow on the link from src to dst. only valid after find() calls.
            Capacity getFlow(ID src, ID dst) const {
                AdjNodePtr p = adjNodePos.at(dst, src)[LinkType::Fake];
                return ((p == nullptr) ? 0 : p->capacity);
            }

            AdjList retrieveFlow() const {
                AdjList flow(nodeNum);
                for (auto src = 0; src < nodeNum; ++src) {
                    if ((src == producer) || (src == consumer)) { continue; }
                    Capacity bandwidth = getFlow(producer, src);
                    if (bandwidth > 0) { flow[producer].push_back(AdjNode(src, 0, bandwidth)); }
                    bandwidth = getFlow(src, consumer);
                    if (bandwidth > 0) { flow[src].push_back(AdjNode(consumer, 0, bandwidth)); }
                    for (auto dst = 0; dst < nodeNum; ++dst) {
                        if ((dst == producer) || (dst == consumer)) { continue; }
                        bandwidth = getFlow(src, dst);
                        if (bandwidth <= 0) { continue; }
                        //flow[src].push_back(AdjNode(dst, 0, bandwidth));
                        flow[src].push_back(AdjNode(dst, getAdjNode_fast(src, dst, LinkType::Real).weight, bandwidth));
                    }
                }

                return flow;
            }

            // retrieve real traffics from flow network. only valid after find() calls.
            // it better to reserve proper capacity for `traffics` to avoid reallocation.
            void retrievePaths(std::vector<Traffic>& traffics, AdjList &flow) const {
                struct FlowTrace {
                    typename AdjVec<>::iterator curAdjNode;
                    typename AdjVec<>::iterator endAdjNode;
                    Capacity bandwidth;
                };

                if (flow.empty()) { return; }

                std::vector<FlowTrace> flowStack;
                std::vector<bool> visited(nodeNum, false);

                flowStack.reserve(nodeNum);
                flowStack.push_back({ flow[producer].begin(), flow[producer].end(), InfiniteCapacity });
                //visited[producer] = true; // it is trivial since there is no link comeing back.

                for (Traffic traffic; ;) {
                    FlowTrace &trace(flowStack.back());

                    if ((trace.curAdjNode == trace.endAdjNode) || (trace.bandwidth <= 0)) {
                        flowStack.pop_back();
                        if (flowStack.empty()) { break; }
                        ++flowStack.back().curAdjNode;

                        if (traffic.nodes.empty()) { continue; }
                        visited[traffic.nodes.back()] = false;
                        traffic.nodes.pop_back();
                        continue;
                    }

                    ID nextNode = trace.curAdjNode->dst;
                    if (visited[nextNode]) { continue; }

                    if (nextNode == consumer) {
                        traffic.bandwidth = (std::min)(trace.bandwidth, trace.curAdjNode->capacity);
                        traffics.push_back(traffic);
                        for (auto iter = flowStack.begin(); iter != flowStack.end(); ++iter) {
                            iter->bandwidth -= traffic.bandwidth;
                            iter->curAdjNode->capacity -= traffic.bandwidth;
                        }

                        ++trace.curAdjNode;
                        continue;
                    }

                    flowStack.push_back({ flow[nextNode].begin(), flow[nextNode].end(),
                        (std::min)(trace.bandwidth, trace.curAdjNode->capacity) });
                    visited[nextNode] = true;
                    traffic.nodes.push_back(nextNode);
                }
            }
            void retrievePaths(std::vector<Traffic>& traffics) const {
                AdjList flow(retrieveFlow());
                retrievePaths(traffics, flow);
            }

            void setWeight(ID src, ID dst, Weight weight) {
                // TODO[szx][7]: update totalCost on the link.
                AdjNodePtr p = adjNodePos.at(src, dst)[LinkType::Real];
                if (p != nullptr) { p->weight = weight; }
                p = adjNodePos.at(dst, src)[LinkType::Fake];
                if (p != nullptr) { p->weight = -weight; }
            }

            // increase the bandwidth of the link from producer to its adjacent node `src`.
            void setGreaterCapacity(ID src, Capacity bandwidth, bool updateFlow) {
                getAdjNode_fast(producer, src, LinkType::Real).capacity = (bandwidth - getFlow(producer, src));
                if (updateFlow) { find(producer, consumer, (bandwidth = 0), false); }
            }

            // decrease the bandwidth of the link from producer to its adjacent node `src`.
            void setLessCapacity(ID src, Capacity bandwidth) {
                reduceFLow(src, getFlow(producer, src) - bandwidth);
                getAdjNode_fast(producer, src, LinkType::Real).capacity = (bandwidth - getFlow(producer, src));
            }

        protected:
            // solve single-source single-sink min-cost flow problem.
            // assume the supply is the capacity of links from `producer` and the demand is the capacity of links to `consumer`.
            // assume the `producer` and `consumer` are artificial nodes to leave out in the path.
            // assume there is no in-degree for `producer` and no out-degree for `consumer`.
            // when it is impossible to meet the demand, return the min-cost max flow solution.
            // the demand will be set to the left demand that cannot be satisfied.
            // if it is an incremental evaluation and the bandwidth of links have been raised, set (exitOnceMeetDemand = false) to apply cycle canceling.
            // return the cost for the flow.
            Weight find(ID source, ID target, Capacity &demand, bool exitOnceMeetDemand) {
                if (exitOnceMeetDemand) {
                    if (demand <= 0) { return totalCost; }
                } else { // there will be no capacity on links to target, start cycle detection from source.
                         // TODO[szx][9]: make cycle detection more generic (start searching from source or target may not reach most nodes).
                    std::swap(source, target);
                }

                // OPTIMIZE[szx][0]: store dst only and put other fields into AdjMat.

                #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                typename ShortestPath::Dijkstra shortestPath(radjList, target);
                #else
                typename ShortestPath::BellmanFord shortestPath(radjList, target);
                #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                for (Traffic traffic; ; shortestPath.clear()) {
                    /// find and retrieve traffic.
                    if (exitOnceMeetDemand) {
                        if (!shortestPath.findPointToPointPath(source)) {
                            return totalCost; // unable to send more flows to meet the demand.
                        }
                        traffic.bandwidth = demand;
                        traffic.nodes = shortestPath.getReversePath(source, false, false);
                    } else {
                        ID cycleNode = shortestPath.findSingleSourcePaths_bfsNegtiveCycleDetect();
                        if (cycleNode != InvalidId) { // negtive weight cycle found.
                            traffic.bandwidth = InfiniteCapacity;
                            traffic.nodes = shortestPath.getReverseCycle(cycleNode, true);
                        } else if (demand <= 0 || !shortestPath.pathFound(source)) {
                            return totalCost; // unable to send more flows to meet the demand.
                        } else { // augment path found.
                            traffic.bandwidth = demand;
                            traffic.nodes = shortestPath.getReversePath(source, false, false);
                        }
                    }

                    /// determine bandwidth.
                    // (useRealLink[i] == true) indicate that the i_th link in the path goes through 
                    // the real link instead of the symmetric fake link.
                    std::vector<bool> useRealLink(traffic.nodes.size() - 1);
                    auto realLink = useRealLink.begin();
                    ID src = traffic.nodes.front();
                    for (auto node = traffic.nodes.begin() + 1; node != traffic.nodes.end(); src = *node, ++node, ++realLink) {
                        ID dst = *node;
                        // for a multigraph, if the shortest path goes from A to B, then the arc between them with min cost will be selected.
                        // the cost of fake links are always negative, so if a link from A to B is in the shortest path,
                        // it must go through the fake link when it is available or there will be shorter path.
                        LinkPosPair &linkPos(adjNodePos.at(src, dst));
                        *realLink = ((linkPos[LinkType::Fake] == nullptr) || linkPos[LinkType::Fake]->capacity <= 0);
                        Capacity bandwidth = linkPos[*realLink ? LinkType::Real : LinkType::Fake]->capacity;
                        Math::updateMin(traffic.bandwidth, bandwidth);
                    }

                    /// update cost.
                    if (exitOnceMeetDemand) {
                        totalCost += (traffic.bandwidth * shortestPath.getDist(source));
                    } else {
                        Weight cycleCost = 0;
                        realLink = useRealLink.begin();
                        src = traffic.nodes.front();
                        for (auto dst = traffic.nodes.begin() + 1; dst != traffic.nodes.end(); src = *dst, ++dst, ++realLink) {
                            cycleCost += getAdjNode_fast(src, *dst, (*realLink ? LinkType::Real : LinkType::Fake)).weight;
                        }
                        totalCost += (traffic.bandwidth * cycleCost);
                    }

                    /// termination condition.
                    if (exitOnceMeetDemand) {
                        demand -= traffic.bandwidth;
                        totalFlow += traffic.bandwidth;
                        #if !SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                        if (demand <= 0) { break; }
                        #endif // !SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                    }

                    /// update network.
                    // EXTEND[szx][6]: update super link cost?
                    #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                    // reduce costs to eliminate negative cost links.
                    shortestPath.findSingleSourcePaths();
                    for (dst = 0; dst < nodeNum; ++dst) {
                        AdjVec<> &adjVec(radjList[dst]);
                        for (auto adjNode = adjVec.begin(); adjNode != adjVec.end(); ++adjNode) {
                            // TODO[szx][0]: skip all appended fake links.
                            adjNode->weight += (shortestPath.getDist(dst) - shortestPath.getDist(adjNode->dst));
                        }
                    }
                    #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA

                    // update bandwidth of links in traffic.
                    // skip reverse links generation/updating on super links due to no path will turn back to producer
                    // or come out from consumer to form a cycle, which means the symmetric fake links will never be used.
                    src = traffic.nodes.front();
                    realLink = useRealLink.begin();
                    for (auto node = traffic.nodes.begin() + 1; node != traffic.nodes.end(); src = *node, ++node, ++realLink) {
                        ID dst = *node;

                        if (*realLink) { // choose real link.
                                         // reduce bandwidth of the chosen link.
                            AdjNode &link(getAdjNode_fast(src, dst, LinkType::Real));
                            link.capacity -= traffic.bandwidth;

                            // raise bandwidth of the symmetric fake link.
                            #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                            getAdjNode_safe(dst, src, LinkType::Fake, 0).capacity += traffic.bandwidth;
                            #else
                            getAdjNode_safe(dst, src, LinkType::Fake, -link.weight).capacity += traffic.bandwidth;
                            #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                        } else { // choose fake link.
                                 // reduce bandwidth of the chosen link.
                            getAdjNode_fast(src, dst, LinkType::Fake).capacity -= traffic.bandwidth;

                            // raise bandwidth of the symmetric real link.
                            getAdjNode_fast(dst, src, LinkType::Real).capacity += traffic.bandwidth;
                        }
                    }

                    #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                    if (exitOnceMeetDemand && (demand <= 0)) { break; }
                    #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                }

                return totalCost;
            }

            // reduce the flow sent directly from producer to its adjacent node `src`.
            void reduceFLow(ID src, Capacity quantity) {
                if (quantity <= 0) { return; }
                AdjNode &adjNode(getAdjNode_fast(producer, src, LinkType::Real));
                totalCost -= (quantity * adjNode.weight);
                totalFlow -= quantity;
                getAdjNode_fast(src, producer, LinkType::Fake).capacity -= quantity;
                Capacity bandwidth = adjNode.capacity + quantity;
                adjNode.capacity = 0;

                // EXTEND[szx][0]: what if there is not enough flow? add a dummy link from producer to consumer temporarily?
                // send reverse flow from consumer to the removal node.
                find(producer, src, quantity, true);

                adjNode.capacity = bandwidth;
            }

            // get the reference of the adjNode without existence check.
            const AdjNode& getAdjNode_fast(ID src, ID dst, LinkType type) const {
                return *adjNodePos.at(src, dst)[type];
            }
            AdjNode& getAdjNode_fast(ID src, ID dst, LinkType type) {
                return const_cast<AdjNode&>(static_cast<const SuccessiveShortestPath*>(this)->getAdjNode_fast(src, dst, type));
            }
            // get one if it exists, create one with 0 capacity otherwise.
            AdjNode& getAdjNode_safe(ID src, ID dst, LinkType type, Weight weight) {
                AdjNodePtr &p = adjNodePos.at(src, dst)[type];
                if (p == nullptr) {
                    radjList[dst].push_back(AdjNode(src, weight, 0));
                    p = &(radjList[dst].back());
                }
                return *p;
            }

            // reserve enough capacity to make sure the adjacency vectors will never be reallocated.
            void reserve() {
                for (auto adjVec = radjList.begin(); adjVec != radjList.end(); ++adjVec) {
                    // TODO[szx][1]: actually it should be (in-degree + out-degree).
                    //               it only works on symmentric graphs (all links are bidirectional).
                    adjVec->reserve(radjList.size() * 2);
                }
            }


            /// input.
            ID nodeNum;
            ID producer;
            ID consumer;
            // reversed adjacency list of the residual network.
            // make sure the adjacency vectors will never be reallocated.
            AdjList radjList;
            /// output.
            Weight totalCost;
            Capacity totalFlow;
            /// auxiliary.
            // adjNodePos[src, dst][type] is the index of the `type` link from src to dst in radjList[dst].
            Arr2D<LinkPosPair> adjNodePos;
        };


        #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_ADJ_MATRIX
        class SuccessiveShortestPath_mat {
        public:
            using LinkPos = AdjNodePtr; // EXTEND[szx][9]: use iterator?
            using LinkPosPair = std::array<LinkPos, 2>; // EXTEND[szx][9]: use struct, std::array or std::vector?
            using AdjInfoPair = std::array<AdjInfo, 2>;

            enum LinkType { Real, Fake };


            // radjacencyList is a reversed adjacency list that records in-degrees of each node.
            SuccessiveShortestPath_mat() {}
            SuccessiveShortestPath_mat(const AdjList &radjacencyList, ID producerId, ID consumerId)
                : nodeNum(radjacencyList.size()), producer(producerId), consumer(consumerId), radjList(radjacencyList),
                adjNodePos(nodeNum, nodeNum), radjNodeId(nodeNum), adjNodeInfo(nodeNum, nodeNum) {}

            void backup(SuccessiveShortestPath &ssp) const {
                ssp.nodeNum = nodeNum;
                ssp.producer = producer;
                ssp.consumer = consumer;
                ssp.radjList = radjList;

                ssp.totalCost = totalCost;
                ssp.totalFlow = totalFlow;
            }

            void recover(const SuccessiveShortestPath &ssp) {
                init(ssp.radjList, ssp.producer, ssp.consumer);

                totalCost = ssp.totalCost;
                totalFlow = ssp.totalFlow;
            }

            void init() {
                reserve();

                //adjNodePos.reset(Arr2D<LinkPosPair>::ResetOption::AllBits0);
                adjNodeInfo.reset(Arr2D<AdjInfoPair>::ResetOption::AllBits0);
                for (ID dst = 0; dst < nodeNum; ++dst) {
                    ID adjNodeNum = radjList[dst].size();
                    for (ID i = 0; i < adjNodeNum; ++i) {
                        if (radjList[dst][i].weight > 0) {
                            adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Real].weight = radjList[dst][i].weight;
                            adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Real].capacity = radjList[dst][i].capacity;
                        } else if (radjList[dst][i].weight < 0) {
                            adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Fake].weight = radjList[dst][i].weight;
                            adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Fake].capacity = radjList[dst][i].capacity;
                        } else {
                            if ((dst == producer) || radjList[dst][i].dst == consumer) {
                                adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Fake].weight = radjList[dst][i].weight;
                                adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Fake].capacity = radjList[dst][i].capacity;
                            } else {
                                adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Real].weight = radjList[dst][i].weight;
                                adjNodeInfo.at(radjList[dst][i].dst, dst)[LinkType::Real].capacity = radjList[dst][i].capacity;
                            }

                        }
                        radjNodeId[dst].push_back(radjList[dst][i].dst);
                    }
                }

                totalCost = 0;
                totalFlow = 0;
            }
            void init(const AdjList &radjacencyList, ID producerId, ID consumerId) {
                nodeNum = radjacencyList.size();
                producer = producerId;
                consumer = consumerId;
                radjList = radjacencyList;
                adjNodePos.init(nodeNum, nodeNum);
                adjNodeInfo.init(nodeNum, nodeNum);

                init();
            }


            Weight find(Capacity &demand) {
                demand -= totalFlow;
                return find(producer, consumer, demand);
            }

            // get the flow on the link from src to dst. only valid after find() calls.
            Capacity getFlow(ID src, ID dst) const {
                return ((adjNodeInfo.at(dst, src)[LinkType::Fake].capacity <= 0) ? 0 : adjNodeInfo.at(dst, src)[LinkType::Fake].capacity);
            }

            AdjList retrieveFlow() const {
                AdjList flow(nodeNum);
                for (auto src = 0; src < nodeNum; ++src) {
                    if ((src == producer) || (src == consumer)) { continue; }
                    Capacity bandwidth = getFlow(producer, src);
                    if (bandwidth > 0) { flow[producer].push_back(AdjNode(src, 0, bandwidth)); }
                    bandwidth = getFlow(src, consumer);
                    if (bandwidth > 0) { flow[src].push_back(AdjNode(consumer, 0, bandwidth)); }
                    for (auto dst = 0; dst < nodeNum; ++dst) {
                        if ((dst == producer) || (dst == consumer)) { continue; }
                        bandwidth = getFlow(src, dst);
                        if (bandwidth <= 0) { continue; }
                        flow[src].push_back(AdjNode(dst, adjNodeInfo.at(src, dst)[LinkType::Real].weight, bandwidth));
                    }
                }

                return flow;
            }

            // retrieve real traffics from flow network. only valid after find() calls.
            // it better to reserve proper capacity for `traffics` to avoid reallocation.
            void retrievePaths(std::vector<Traffic>& traffics, AdjList &flow) const {
                struct FlowTrace {
                    typename AdjVec<>::iterator curAdjNode;
                    typename AdjVec<>::iterator endAdjNode;
                    Capacity bandwidth;
                };

                if (flow.empty()) { return; }

                std::vector<FlowTrace> flowStack;
                std::vector<bool> visited(nodeNum, false);

                flowStack.reserve(nodeNum);
                flowStack.push_back({ flow[producer].begin(), flow[producer].end(), InfiniteCapacity });
                //visited[producer] = true; // it is trivial since there is no link comeing back.

                for (Traffic traffic; ;) {
                    FlowTrace &trace(flowStack.back());

                    if ((trace.curAdjNode == trace.endAdjNode) || (trace.bandwidth <= 0)) {
                        flowStack.pop_back();
                        if (flowStack.empty()) { break; }
                        ++flowStack.back().curAdjNode;

                        if (traffic.nodes.empty()) { continue; }
                        visited[traffic.nodes.back()] = false;
                        traffic.nodes.pop_back();
                        continue;
                    }

                    ID nextNode = trace.curAdjNode->dst;
                    if (visited[nextNode]) { continue; }

                    if (nextNode == consumer) {
                        traffic.bandwidth = std::min(trace.bandwidth, trace.curAdjNode->capacity);
                        traffics.push_back(traffic);
                        for (auto iter = flowStack.begin(); iter != flowStack.end(); ++iter) {
                            iter->bandwidth -= traffic.bandwidth;
                            iter->curAdjNode->capacity -= traffic.bandwidth;
                        }

                        ++trace.curAdjNode;
                        continue;
                    }

                    flowStack.push_back({ flow[nextNode].begin(), flow[nextNode].end(),
                        std::min(trace.bandwidth, trace.curAdjNode->capacity) });
                    visited[nextNode] = true;
                    traffic.nodes.push_back(nextNode);
                }
            }
            void retrievePaths(std::vector<Traffic>& traffics) const {
                AdjList flow(retrieveFlow());
                retrievePaths(traffics, flow);
            }

            void setWeight(ID src, ID dst, Weight weight) {
                // TODO[szx][7]: update totalCost on the link.
                if (adjNodeInfo.at(src, dst)[LinkType::Real].capacity > 0) {
                    adjNodeInfo.at(src, dst)[LinkType::Real].weight = weight;
                }
                if (adjNodeInfo.at(dst, src)[LinkType::Fake].capacity > 0) {
                    adjNodeInfo.at(dst, src)[LinkType::Fake].weight = -weight;
                }
            }

            // increase the bandwidth of the link from producer to its adjacent node `src`.
            void increaseCapacity(ID src, Capacity quantity) {
                adjNodeInfo.at(producer, src)[LinkType::Real].capacity += quantity;
            }
            void setGreaterCapacity(ID src, Capacity bandwidth) {
                adjNodeInfo.at(producer, src)[LinkType::Real].capacity = bandwidth;
            }

            // decrease the bandwidth of the link from producer to its adjacent node `src`.
            void decreaseCapacity(ID src, Capacity quantity) {
                Capacity &capacity(adjNodeInfo.at(producer, src)[LinkType::Real].capacity);
                reduceFLow(src, quantity - (capacity - getFlow(producer, src)));
                capacity -= quantity;
            }
            void setLessCapacity(ID src, Capacity bandwidth) {
                reduceFLow(src, getFlow(producer, src) - bandwidth);
                adjNodeInfo.at(producer, src)[LinkType::Real].capacity = bandwidth;
            }

            // reduce the flow sent directly from producer to its adjacent node `src`.
            void reduceFLow(ID src, Capacity quantity) {
                if (quantity <= 0) { return; }
                totalCost -= (quantity *adjNodeInfo.at(producer, src)[LinkType::Real].weight);
                totalFlow -= quantity;
                adjNodeInfo.at(src, producer)[LinkType::Fake].capacity -= quantity
                    Capacity bandwidth = adjNode.capacity + quantity;
                adjNodeInfo.at(producer, src)[LinkType::Fake].capacity = 0;

                // send reverse flow from consumer to the removal node.
                find(producer, src, quantity);

                adjNode.capacity = bandwidth;
            }
            void reduceFLow(ID src) {
                reduceFLow(src, getFlow(producer, src));
            }

        protected:
            // solve single-source single-sink min-cost flow problem.
            // assume the supply is the capacity of links from `producer` and the demand is the capacity of links to `consumer`.
            // assume the `producer` and `consumer` are artificial nodes to leave out in the path.
            // assume there is no in-degree for `producer` and no out-degree for `consumer`.
            // ???assume the cost of out-degrees of `producer` and in-degrees of `consumer` are 0.
            // when it is impossible to meet the demand, return the min-cost max flow solution.
            // the demand will be set to the left demand that cannot be satisfied.
            Weight find(ID source, ID target, Capacity &demand) {
                if (demand <= 0) { return totalCost; }

                // OPTIMIZE[szx][0]: store dst only and put other fields into AdjMat.

                #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                typename ShortestPath::Dijkstra shortestPath(radjList, target);
                #else
                typename ShortestPath::BellmanFord_mat shortestPath(radjNodeId, adjNodeInfo, consumer);
                #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                for (Traffic traffic; ; shortestPath.clear()) {
                    /// find traffic.
                    if (!shortestPath.findPointToPointPath(source)) {
                        return totalCost; // unable to send more flows to meet the demand.
                    }

                    /// retrieve traffic.
                    traffic.nodes = shortestPath.getReversePath(source, false, false);

                    std::vector<bool> useRealLink(traffic.nodes.size() - 1);
                    auto realLink = useRealLink.begin();

                    // assume no augment path will come back to producer or come out from consumer through fake links.
                    ID src = traffic.nodes.front();
                    traffic.bandwidth = demand;
                    for (auto node = traffic.nodes.begin() + 1; node != traffic.nodes.end(); src = *node, ++node, ++realLink) {
                        ID dst = *node;
                        // for a multigraph, if the shortest path goes from A to B, then the arc between them with min cost will be selected.
                        // the cost of fake links are always negative, so if a link from A to B is in the shortest path,
                        // it must go through the fake link when it is available or there will be shorter path.
                        *realLink = (adjNodeInfo.at(src, dst)[LinkType::Fake].capacity <= 0);
                        Capacity bandwidth = *realLink ? adjNodeInfo.at(src, dst)[LinkType::Real].capacity : adjNodeInfo.at(src, dst)[LinkType::Fake].capacity;
                        Math::updateMin(traffic.bandwidth, bandwidth);
                    }

                    totalCost += (traffic.bandwidth * shortestPath.getDist(source));

                    /// termination condition.
                    demand -= traffic.bandwidth;
                    totalFlow += traffic.bandwidth;
                    #if !SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                    if (demand <= 0) { break; }
                    #endif // !SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT

                    /// update network.
                    // EXTEND[szx][6]: update super link cost?
                    #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                    // reduce costs to eliminate negative cost links.
                    shortestPath.findSingleSourcePaths();
                    for (dst = 0; dst < nodeNum; ++dst) {
                        AdjVec<> &adjVec(radjList[dst]);
                        for (auto adjNode = adjVec.begin(); adjNode != adjVec.end(); ++adjNode) {
                            // TODO[szx][0]: skip all appended fake links.
                            adjNode->weight += (shortestPath.getDist(dst) - shortestPath.getDist(adjNode->dst));
                        }
                    }
                    #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA

                    // update bandwidth of links in traffic.
                    // skip reverse links generation/updating on super links due to no path will turn back to producer
                    // or come out from consumer to form a cycle, which means the symmetric fake links will never be used.
                    src = traffic.nodes.front();
                    realLink = useRealLink.begin();
                    for (auto node = traffic.nodes.begin() + 1; node != traffic.nodes.end(); src = *node, ++node, ++realLink) {
                        ID dst = *node;

                        if (*realLink) { // choose real link.
                                         // reduce bandwidth of the chosen link.
                            adjNodeInfo.at(src, dst)[LinkType::Real].capacity -= traffic.bandwidth;

                            // raise bandwidth of the symmetric fake link.
                            #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                            getAdjNode_safe(dst, src, LinkType::Fake, 0).capacity += traffic.bandwidth;
                            #else
                            adjNodeInfo.at(dst, src)[LinkType::Fake].weight = -adjNodeInfo.at(src, dst)[LinkType::Real].weight;
                            adjNodeInfo.at(dst, src)[LinkType::Fake].capacity += traffic.bandwidth;
                            #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_BY_DIJKSTRA
                        } else { // choose fake link.
                                 // reduce bandwidth of the chosen link.
                            adjNodeInfo.at(src, dst)[LinkType::Fake].capacity -= traffic.bandwidth;

                            // raise bandwidth of the symmetric real link.
                            adjNodeInfo.at(dst, src)[LinkType::Real].capacity += traffic.bandwidth;
                        }
                    }

                    #if SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                    if (demand <= 0) { break; }
                    #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_LATE_EXIT
                }

                return totalCost;
            }


            void reserve() {
                for (auto adjVec = radjList.begin(); adjVec != radjList.end(); ++adjVec) {
                    // TODO[szx][1]: actually it should be (in-degree + out-degree).
                    //               it only works on symmentric graphs (all links are bidirectional).
                    adjVec->reserve(radjList.size() * 2);
                }
            }


            /// input.
            ID nodeNum;
            ID producer;
            ID consumer;
            // reversed adjacency list of the residual network.
            AdjList radjList;
            /// output.
            Weight totalCost;
            Capacity totalFlow;
            /// auxiliary.
            // adjNodePos[src, dst][type] is the index of the `type` link from src to dst in radjList[dst].
            Arr2D<LinkPosPair> adjNodePos;
            // adjNodeInfo[src, dst][type] are the weight and capacity from src to dst in radjList[dst].
            AdjMat<AdjInfoPair> adjNodeInfo;
            // adjNodeId is the list of  adjNode ID.
            std::vector<std::vector<ID> > radjNodeId;
        };
        #endif // SZX_CPPUTILIBS_GRAPH_MIN_COST_FLOW_SUCCESSIVE_SHORTEST_PATH_ADJ_MATRIX
    };


    template<template<typename...> class List = std::vector>
    static AdjList toAdjList(const List<Edge> &edges, ID nodeNum) {
        AdjList adjList(nodeNum);

        for (auto iter = edges.begin(); iter != edges.end(); ++iter) {
            adjList[iter->src].push_back(AdjNode(iter->dst, iter->weight, iter->capacity));
        }

        return adjList;
    }
};

}


#endif // SZX_CPPUTILIBS_GRAPH_H
