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
    const unsigned vertexNum;

protected:
    Graph( unsigned vn ) :vertexNum( vn ) {}
    ~Graph() {}
};

class GeometricalGraph : public Graph
{
public:
    typedef double Distance;
    typedef double Coord;
    class Point
    {
    public:
        Point() {}
        Point( Coord cx, Coord cy ) :x( cx ), y( cy ) {}

        Coord x;
        Coord y;
    };
    typedef std::vector<Point> PointList;

    GeometricalGraph( const PointList &pl ) : Graph(pl.size()), pointList( pl ) {}
    ~GeometricalGraph() {}

    const Point& point( int i ) const
    {
        return pointList[i];
    }

private:
    PointList pointList;
};

class TopologicalGraph : public Graph
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
    int findVertexWithinRadius( int start, TopologicalGraph::Distance radius ) const;
    // find the number of vertices whose distance to start is shorter the radius
    // this number can be used to get all vertices within radius by distSeq
    int findVertexNumWithinRadius( int start, TopologicalGraph::Distance radius ) const;

    const DistanceMatrix& getShortestPath();
    void printShortestDist( std::ostream &os ) const;
    const DistSeqTable& getDistSeqTable();
    void printDistSeqTable( std::ostream &os ) const;

protected:
    TopologicalGraph( unsigned vertexNumber, int minVertexIndex );
    virtual ~TopologicalGraph();


    // essential elements ( must be initialized in constructor )
    DistanceMatrix adjMat;          // use elements within [minVertexIndex,maxVertexIndex]

    // optional elements ( can be generated later by calling member methods )
    DistanceMatrix shortestDist;    // use elements within [minVertexIndex,maxVertexIndex]
    DistSeqTable distSeq;           // use elements within [minVertexIndex,maxVertexIndex]
    bool shortestDistSolved;
    bool distSeqSolved;

private:
    const DistanceMatrix& getShortestPathByDijkstra();
    const DistanceMatrix& getShortestPathByFloyd();
    const DistSeqTable& getDistSeqTableBySTLsort();
    const DistSeqTable& getDistSeqTableByInsertSort();
};



class UndirectedGraph : public TopologicalGraph
{
public:
    // get a symmetrical adjMat
    UndirectedGraph( const ArcList &arcList, unsigned vertexNumber, int minVertexIndex = DEFAULT_MIN_VERTEX_INDEX );
    // automatically generate the shortestDist
    UndirectedGraph( const GeometricalGraph& gg );
    ~UndirectedGraph();
};


class DirectedGraph : public TopologicalGraph
{
public:
    // get a asymmetrical adjMat
    DirectedGraph( const ArcList &arcList, unsigned vertexNumber, int minVertexIndex = DEFAULT_MIN_VERTEX_INDEX );
    ~DirectedGraph();
};


#endif