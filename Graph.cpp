#include "Graph.h"

using namespace std;

// GeometricalGraph =======================


// TopologicalGraph =======================
template <typename T_DIST>
TopologicalGraph<T_DIST>::TopologicalGraph( unsigned vn, int mvi )
: Graph( vn ), minVertexIndex( mvi ), maxVertexIndex( vn + mvi - 1 ), vertexAllocNum( vn + mvi ),
adjMat( vn + mvi, vector<Distance>( vn + mvi, MAX_DISTANCE ) ), shortestDistSolved( false ), distSeqSolved( false )
{
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        adjMat[i][i] = 0;
    }
}

template <typename T_DIST>
TopologicalGraph<T_DIST>::~TopologicalGraph( )
{
}

template <typename T_DIST>
int TopologicalGraph<T_DIST>::findVertexWithinRadius( int start, typename TopologicalGraph<T_DIST>::Distance radius ) const
{
    RandSelect rs( 1 );
    int vertex;
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        if (distance( start, nthClosestVertex( start, i ) ) < radius) {
            if (rs.isSelected()) {
                vertex = nthClosestVertex( start, i );
            }
        } else {
            break;
        }
    }

    return vertex;
}

template <typename T_DIST>
int TopologicalGraph<T_DIST>::findVertexNumWithinRadius( int start, typename TopologicalGraph<T_DIST>::Distance radius ) const
{
    int i = minVertexIndex;
    for (; i <= maxVertexIndex; i++) {
        if (distance( start, nthClosestVertex( start, i ) ) >= radius) {
            break;
        }
    }

    return i;
}

template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistanceMatrix& TopologicalGraph<T_DIST>::getShortestPath( )
{
    if (!shortestDistSolved) {
        getShortestPathByFloyd();   /// put your own specified algorithm here

        shortestDistSolved = true;
        distSeqSolved = false;
    }

    return shortestDist;
}

template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistSeqTable& TopologicalGraph<T_DIST>::getDistSeqTable( )
{
    if (!distSeqSolved) {
        getShortestPath();  // prerequisite  
        /// put your own specified algorithm below
        getDistSeqTableBySTLsort();

        distSeqSolved = true;
    }

    return distSeq;
}

template <typename T_DIST>
void TopologicalGraph<T_DIST>::printDistSeqTable( std::ostream &os ) const
{
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        for (int j = minVertexIndex; j < maxVertexIndex; j++) {
            os << shortestDist[i][j] << ',';
        }
        os << shortestDist[i][maxVertexIndex] << '\n';
    }
}

template <typename T_DIST>
void TopologicalGraph<T_DIST>::printShortestDist( ostream &os ) const
{
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        for (int j = minVertexIndex; j < maxVertexIndex; j++) {
            if (shortestDist[i][j] == MAX_DISTANCE) {
                os << "x,";
            } else {
                os << shortestDist[i][j] << ',';
            }
        }

        if (shortestDist[i][maxVertexIndex] == MAX_DISTANCE) {
            os << "x\n";
        } else {
            os << shortestDist[i][maxVertexIndex] << '\n';
        }
    }
}


template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistanceMatrix& TopologicalGraph<T_DIST>::getShortestPathByDijkstra()
{
    enum VertexState { IN_SET, OUT_OF_SET };

    shortestDist = adjMat;

    // find the shortest path start from each vertex
    for (int startVertex = minVertexIndex; startVertex <= maxVertexIndex; startVertex++) {
        // create an array of flag to record whether a vertex is in the final set
        vector<VertexState> vertexState( vertexAllocNum, VertexState::OUT_OF_SET );
        vertexState[startVertex] = VertexState::IN_SET;
        // loop (vertexNum-1) times to add every vertex into the final set
        for (int k = minVertexIndex; k < maxVertexIndex; k++) {
            int closestVertex = 0;    // vertex with index of 0 is not used, so the distance will always be MAX_DISTANCE
            // find the closest vertex outside the final set
            for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
                if (vertexState[i] == VertexState::OUT_OF_SET &&
                    shortestDist[startVertex][i] < shortestDist[startVertex][closestVertex]) {
                    closestVertex = i;
                }
            }
            // add it into the final set
            vertexState[closestVertex] = VertexState::IN_SET;
            // update current shortest path from start vertex to vertexs outside of the final set
            for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
                Distance newDist = shortestDist[startVertex][closestVertex] + shortestDist[closestVertex][i];
                if (vertexState[i] == VertexState::OUT_OF_SET &&
                    newDist < shortestDist[startVertex][i]) {
                    shortestDist[startVertex][i] = newDist;
                }
            }
        }
    }


    return shortestDist;
}

template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistanceMatrix& TopologicalGraph<T_DIST>::getShortestPathByFloyd()
{
    shortestDist = adjMat;

    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        for (int j = minVertexIndex; j <= maxVertexIndex; j++) {
            for (int k = minVertexIndex; k <= maxVertexIndex; k++) {
                Distance newDist = shortestDist[j][i] + shortestDist[i][k];
                if (newDist < shortestDist[j][k]) {
                    shortestDist[j][k] = newDist;
                }
            }
        }
    }

    return shortestDist;
}

template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistSeqTable& TopologicalGraph<T_DIST>::getDistSeqTableBySTLsort()
{
    class DistCmp
    {
    public:
        DistCmp( const TopologicalGraph<T_DIST> &g, int sv ) : graph( g ), startVertex( sv ) {}
        ~DistCmp() {}

        bool operator() ( const int &a, const int &b )
        {
            return graph.distance( startVertex, a ) < graph.distance( startVertex, b );
        }

    private:
        const TopologicalGraph<T_DIST> &graph;
        int startVertex;
    };

    // init distSeq
    vector<int> seq( vertexAllocNum );
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        seq[i] = i;
    }
    distSeq = DistSeqTable( vertexAllocNum, seq );

    // do sort
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        DistCmp cmp( *this, i );
        sort( distSeq[i].begin() + minVertexIndex, distSeq[i].end(), cmp );
    }

    return distSeq;
}

template <typename T_DIST>
const typename TopologicalGraph<T_DIST>::DistSeqTable& TopologicalGraph<T_DIST>::getDistSeqTableByInsertSort()
{
    // init distSeq
    vector<int> seq( vertexAllocNum );
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        seq[i] = i;
    }
    distSeq = DistSeqTable( vertexAllocNum, seq );

    // do sort
    for (int i = minVertexIndex; i <= maxVertexIndex; i++) {
        //  do insertion sort on each vertex
        for (int j = minVertexIndex; j <= maxVertexIndex; j++) {
            int k = j;
            for (Distance d = distance( i, j ); k > 0; k--) {
                if (d < distance( i, distSeq[i][k - 1] )) {
                    distSeq[i][k] = distSeq[i][k - 1];
                } else {
                    distSeq[i][k] = j;
                    break;
                }
            }
            if (k == 0) {
                distSeq[i][0] = j;
            }
        }
    }

    return distSeq;
}



// UndirectedGraph =======================
template <typename T_DIST>
UndirectedGraph<T_DIST>::UndirectedGraph( const ArcList &arcList, unsigned vn, int mvi )
: TopologicalGraph( vn, mvi )
{
    for (ArcList::const_iterator iter = arcList.begin(); iter != arcList.end(); iter++) {
        adjMat[iter->startVertex][iter->endVertex] = iter->dist;
        adjMat[iter->endVertex][iter->startVertex] = iter->dist;
    }
}

template <typename T_DIST>
UndirectedGraph<T_DIST>::UndirectedGraph( const GeometricalGraph &gg )
: TopologicalGraph( gg.vertexNum, 0 )
{
    // calculate distance between each pair of points
    for (int i = 0; i < gg.vertexNum; i++) {
        for (int j = 0; j < i; j++) {
            GeometricalGraph::Coord dx = gg.point( i ).x - gg.point( j ).x;
            GeometricalGraph::Coord dy = gg.point( i ).y - gg.point( j ).y;
            adjMat[i][j] = adjMat[j][i] = sqrt(dx*dx + dy*dy);
        }
        adjMat[i][i] = 0;
    }
    shortestDist = adjMat;
    shortestDistSolved = true;
}

template <typename T_DIST>
UndirectedGraph<T_DIST>::~UndirectedGraph( )
{
}


// DirectedGraph =======================
template <typename T_DIST>
DirectedGraph<T_DIST>::DirectedGraph( const ArcList &arcList, unsigned vn, int mvi )
: TopologicalGraph( vn, mvi )
{
    for (ArcList::const_iterator iter = arcList.begin(); iter != arcList.end(); iter++) {
        adjMat[iter->startVertex][iter->endVertex] = iter->dist;
    }
}

template <typename T_DIST>
DirectedGraph<T_DIST>::~DirectedGraph( )
{
}