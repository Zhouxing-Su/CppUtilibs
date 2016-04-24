////////////////////////////////
/// usage : 1.	portable and configurable dijkstra shortest path generator.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_DIJKSTRA_PATH_GENERATOR_H
#define SZX_CPPUTILIBS_DIJKSTRA_PATH_GENERATOR_H


#include <vector>

#include "../type/PriorityQueue.h"


namespace szx {

template<typename ID = int, typename Weight = int>
class DijkstraPathGenerator {
public:
    static constexpr int InvalidId = -1;
    static constexpr int MaxWeight = (1 << 20);


    struct Edge {
        ID dst;
        ID weight;
    };

    struct Cmp {
        explicit Cmp(const std::vector<Weight> &d) : dist(d) {}
        #if USE_HEAP
        bool operator()(ID l, ID r) { return (dist[l] > dist[r]); } // greater weight means less priority.
        #else
        bool operator()(ID l, ID r) const { return ((dist[l] != dist[r]) ? (dist[l] < dist[r]) : (l < r)); } // greater weight means less priority.
        #endif // USE_HEAP
        const std::vector<Weight> &dist;
    };


    DijkstraPathGenerator(const std::vector<std::vector<Edge>> &adjList, ID src, const std::vector<bool> &isNodeRemoved)
        : adjacencyList(adjList), source(src), nodeRemoved(isNodeRemoved),
        dist(adjacencyList.size()), prevNode(adjacencyList.size()), unvisitedNodes(Cmp(dist)) {
        clear();
    }
    DijkstraPathGenerator(const std::vector<std::vector<Edge>> &adjList, ID src)
        : DijkstraPathGenerator(adjList, src, std::vector<bool>(adjList.size(), false)) {}

    DijkstraPathGenerator(const std::vector<std::vector<Edge>> &adjList, ID src, const std::vector<bool> &isNodeRemoved,
        const std::vector<Weight> &distList, const std::vector<ID> &prevNodeList, const PriorityQueue<ID, Cmp> &unvisitedNodeQueue)
        : adjacencyList(adjList), source(src), nodeRemoved(isNodeRemoved), dist(distList),
        prevNode(prevNodeList), unvisitedNodes(unvisitedNodeQueue.getContainer(), Cmp(dist)) {}

    DijkstraPathGenerator(const DijkstraPathGenerator &dpg)
        : adjacencyList(dpg.adjacencyList), source(dpg.source), nodeRemoved(dpg.nodeRemoved),
        dist(dpg.dist), prevNode(dpg.prevNode), unvisitedNodes(dpg.getUnvisitedNodes().getContainer(), Cmp(dist)) {}

    void reset(const std::vector<bool> &isNodeRemoved) {
        clear();
        nodeRemoved = isNodeRemoved;
    }
    void reset(const std::vector<bool> &isNodeRemoved, ID src) {
        source = src;
        reset(isNodeRemoved);
    }

    const std::vector<Weight>& getDistList() const { return dist; }
    Weight getDist(ID dst) const { return dist[dst]; }

    ID getPrevNode(ID node) const { return prevNode[node]; }
    const std::vector<ID>& getPrevNodeList() const { return prevNode; }
    std::vector<ID> getPath(ID dst) const {
        std::vector<ID> path;
        for (; dst != InvalidId; dst = prevNode[dst]) { path.push_back(dst); }
        std::reverse(path.begin(), path.end());
        return path;
    }
    std::vector<ID> getPathWithoutSrc(ID dst) const {
        std::vector<ID> path;
        for (; dst != source; dst = prevNode[dst]) { path.push_back(dst); }
        std::reverse(path.begin(), path.end());
        return path;
    }
    std::vector<ID> getPathWithoutDst(ID dst) const {
        std::vector<ID> path;
        for (dst = prevNode[dst]; dst != source; dst = prevNode[dst]) { path.push_back(dst); }
        std::reverse(path.begin(), path.end());
        return path;
    }

    const std::vector<std::vector<Edge>>& getAdjacencyList() const { return adjacencyList; }
    ID getSource() const { return source; }
    const std::vector<bool>& getNodeRemoved() const { return nodeRemoved; }
    const PriorityQueue<ID, Cmp>& getUnvisitedNodes() const { return unvisitedNodes; }
    ID getLastNode() const { return unvisitedNodes.top(); }

    template<typename T_Pred>
    ID next(const T_Pred &isTarget) {
        if (unvisitedNodes.empty()) { return InvalidId; }
        for (;;) {
            ID lastNode = unvisitedNodes.top();

            nodeRemoved[lastNode] = true;
            unvisitedNodes.pop();

            for (auto i = adjacencyList[lastNode].begin(); i != adjacencyList[lastNode].end(); ++i) {
                if (nodeRemoved[i->dst]) { continue; }
                Weight newDist = dist[lastNode] + i->weight;
                if (newDist < dist[i->dst]) {
                    #if USE_HEAP
                    #else
                    unvisitedNodes.remove(i->dst);
                    #endif // USE_HEAP

                    dist[i->dst] = newDist;
                    prevNode[i->dst] = lastNode;

                    #if USE_HEAP
                    unvisitedNodes.add(i->dst);
                    #else
                    unvisitedNodes.push(i->dst);
                    #endif // USE_HEAP
                }
            }

            #if USE_HEAP
            unvisitedNodes.reorder();
            while (nodeRemoved[unvisitedNodes.top()]) {
                unvisitedNodes.pop();
                if (unvisitedNodes.empty()) { return InvalidId; }
            }
            #else
            #endif // USE_HEAP

            if (unvisitedNodes.empty()) { return InvalidId; }
            if (isTarget(unvisitedNodes.top())) { return unvisitedNodes.top(); }
        }
    }
    ID nextClosest() { return next([](ID) { return true; }); }
    void genPath(ID dst) { next([dst](ID node) { return (node == dst); }); }
    void genAllPaths() { next([](ID) { return false; }); }

private:
    void clear() {
        std::fill(dist.begin(), dist.end(), MaxWeight);
        dist[source] = 0;
        std::fill(prevNode.begin(), prevNode.end(), InvalidId);
        unvisitedNodes.clear();
        unvisitedNodes.push(source);
    }


    const std::vector<std::vector<Edge>> &adjacencyList;
    ID source;
    std::vector<bool> nodeRemoved;

    std::vector<Weight> dist;
    std::vector<ID> prevNode;
    #if USE_HEAP
    PriorityQueueUsingHeap<ID, Cmp> unvisitedNodes;
    #else
    PriorityQueueUsingSet<ID, Cmp> unvisitedNodes;
    #endif // USE_HEAP
};

}


#endif // SZX_CPPUTILIBS_DIJKSTRA_PATH_GENERATOR_H
