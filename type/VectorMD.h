/**
*   usage : 1. simplify multi-dimensional vector representation.
*
*   note :  1. conversion between std::vector to VectorMD?
*           2. implement other constructor of std::vector?
*           3.
*/

#ifndef VECTORMD_H
#define VECTORMD_H


#include <vector>


template <typename T, const int Dimension = 1>
class VectorMD_FullInit : public std::vector < VectorMD_FullInit < T, Dimension - 1 > >
{
public:
    VectorMD_FullInit() {}
    // for traditional way of initialization
    VectorMD_FullInit( int size, const VectorMD_FullInit<T, Dimension - 1>& val )
        : std::vector< VectorMD_FullInit<T, Dimension - 1> >( size, val ) {}
    // for new way of initialization
    // make sure (sizes.size() == Dimension)
    VectorMD_FullInit( const std::vector<int> &sizes, const T& val )
        : std::vector< VectorMD_FullInit<T, Dimension - 1> >( sizes[sizes.size() - Dimension],
        VectorMD_FullInit<T, Dimension - 1>( sizes, val ) )
    {
    }
};

template <typename T>
class VectorMD_FullInit<T, 1> : public std::vector < T >
{
public:
    VectorMD_FullInit() {}
    // for traditional way of initialization
    VectorMD_FullInit( int size, const T& val = T() ) : std::vector<T>( size, val ) {}
    // for new way of initialization
    // make sure (sizes.size() == Dimension)
    VectorMD_FullInit( const std::vector<int> &sizes, const T& val )
        : std::vector<T>( sizes[sizes.size() - 1], val ) {}
};





template <typename T, const int Dimension = 1>
class VectorMD_PartInit : public std::vector < VectorMD_PartInit < T, Dimension - 1 > >
{
public:
    VectorMD_PartInit() {}
    // for traditional way of initialization
    VectorMD_PartInit( int size, const VectorMD_PartInit<T, Dimension - 1>& val )
        : std::vector< VectorMD_PartInit<T, Dimension - 1> >( size, val ) {}
    // for new way of initialization
    // sizes.size() can be any value
    VectorMD_PartInit( const std::vector<int> &sizes, const T& val, int sizeIndex = 0 )
        : std::vector< VectorMD_PartInit<T, Dimension - 1> >( ((sizeIndex < sizes.size()) ? sizes[sizeIndex] : 0),
        VectorMD_PartInit<T, Dimension - 1>( sizes, val, sizeIndex + 1 ) )
    {
    }
};

template <typename T>
class VectorMD_PartInit<T, 1> : public std::vector < T >
{
public:
    VectorMD_PartInit() {}
    // for traditional way of initialization
    VectorMD_PartInit( int size, const T& val = T() ) : std::vector<T>( size, val ) {}
    // for new way of initialization
    // sizes.size() can be any value
    VectorMD_PartInit( const std::vector<int> &sizes, const T& val, int sizeIndex = 0 )
        : std::vector<T>( ((sizeIndex < sizes.size()) ? sizes[sizeIndex] : 0), val ) {}
};




template <typename T, const int Dimension = 1>
using VectorMD = VectorMD_FullInit < T, Dimension > ;


#endif
