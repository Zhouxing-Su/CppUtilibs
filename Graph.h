/**
*   usage:

(example code)
====================================================================
#include <iostream>
#include "Graph.h"

using namespace std;

int main( int argc, char **argv )
{
return 0;
}
====================================================================
*/

#ifndef GRAPH_H
#define GRAPH_H



#include <vector>
#include <array>
#include <iostream>
#include <iomanip>

class Graph
{
public:
    // the distance or weight on edges
    typedef unsigned Distance;
    struct Arc
    {
        Arc() {}
        Arc( int sv, int ev, Distance d )
            : startVertex( sv ), endVertex( ev ), dist( d )
        {
        }
        ~Arc() {}

        int startVertex;
        int endVertex;
        Distance dist;
    };
    typedef std::vector<Arc> ArcList;
    // the No. N element on line M is the distance from vertex N to vertex M
    typedef std::vector< std::vector<Distance> > DistanceMatrix;
    // for each line N, it records the index of vertices sorted by their distance to vertex N with ascending order
    typedef std::vector< std::vector<int> > DistSeqTable;


    static const Distance MAX_DISTANCE = 0x3fffffff;
    static const Distance MIN_DISTANCE = 0;
    static const int DEFAULT_MIN_VERTEX_INDEX = 1;


    const int vertexNum;
    const int vertexAllocNum;
    const int minVertexIndex;
    const int maxVertexIndex;


    // get the shortest distance between start and end
    Distance distance( int start, int end ) const
    {
        return shortestDist[start][end];
    }

    // get the Nth closest vertex from start
    // shiftedN = (N-1) + minVertexIndex;
    int nthClosestVertex( int start, int shiftedN ) const
    {
        return distSeq[start][shiftedN];
    }

    const DistanceMatrix& getShortestPath();
    void printShortestDist( std::ostream &os ) const;
    const DistSeqTable& getDistSeqTable();
    void printDistSeqTable( std::ostream &os ) const;

protected:
    Graph( int vertexNumber, int minVertexIndex );
    virtual ~Graph();
    const DistanceMatrix& getShortestPathByDijkstra();
    const DistanceMatrix& getShortestPathByFloyd();
    const DistSeqTable& getDistSeqTableBySTLsort();
    const DistSeqTable& getDistSeqTableByInsertSort();


    // essential elements ( must be initialized in constructor )
    DistanceMatrix adjMat;          // use elements within [minVertexIndex,maxVertexIndex]
    bool shortestDistSolved;
    bool distSeqSolved;

    // optional elements ( can be generated later by calling member methods )
    DistanceMatrix shortestDist;    // use elements within [minVertexIndex,maxVertexIndex]
    DistSeqTable distSeq;           // use elements within [minVertexIndex,maxVertexIndex]
};



class UndirectedGraph : public Graph
{
public:
    UndirectedGraph( const ArcList &arcList, int vertexNumber, int minVertexIndex = DEFAULT_MIN_VERTEX_INDEX );
    ~UndirectedGraph();

};


class DirectedGraph : public Graph
{
public:
    DirectedGraph( const ArcList &arcList, int vertexNumber, int minVertexIndex = DEFAULT_MIN_VERTEX_INDEX );
    ~DirectedGraph();
};


#endif