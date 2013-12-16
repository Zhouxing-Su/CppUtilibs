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
#include <set>
#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "RandSelect.h"

class Graph
{
public:
    // the distance or weight on edges
    typedef std::set<int> VertexSet;
    typedef unsigned Distance;
    struct Arc
    {
        class CmpClass
        {
        public:
            bool operator() ( const Arc& lhs, const Arc& rhs ) const
            {
                return (lhs.startVertex < rhs.startVertex);
            }
        };

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
    typedef std::set<Arc, Arc::CmpClass> ArcSet;
    // the No. N element on line M is the distance from vertex N to vertex M
    typedef std::vector< std::vector<Distance> > DistanceMatrix;
    // for each line N, it records the index of vertices sorted by their distance to vertex N with ascending order
    typedef std::vector< std::vector<int> > DistSeqTable;


    static const Distance MAX_DISTANCE = 0x3fffffff;
    static const Distance MIN_DISTANCE = 0;
    static const int DEFAULT_MIN_VERTEX_INDEX = 1;


    const int vertexNum;
    const int minVertexIndex;
    const int maxVertexIndex;
    const int vertexAllocNum;


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

    // find a vertex whose distance to start is shorter the radius randomly
    int findVertexWithinRadius( int start, Graph::Distance radius ) const;
    // find the number of vertices whose distance to start is shorter the radius
    // this number can be used to get all vertices within radius by distSeq
    int findVertexNumWithinRadius( int start, Graph::Distance radius ) const;

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