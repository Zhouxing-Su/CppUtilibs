/**
*   usage : bidirection index for fixed size of numbers.
*           for each number i, there should be (i == element[index[i]])
*
*   note :  1.  "capacity" indicate the max or total valid element number which is different from std container,
*               while "elementNum" and "size" indicate the current inserted elements.
*           2.  index is count from 0, while element is between [min, max].
*           3.  BidirectionIndex will not consider index out of range error.
*/

#ifndef BIDIRECTION_INDEX_H


#include <vector>


class BidirectionIndex
{
public:
    static const int INVALID_INDEX = -1;

    BidirectionIndex( int capacity, int minValue = 0 )
        :min( minValue ), max( minValue + capacity - 1 ), elementNum( 0 ),
        element( capacity ), index( capacity, INVALID_INDEX )
    {
    }

    // insert element e and update index
    void insert( int e )
    {
        element[elementNum] = e;
        index[e - min] = elementNum;
        ++elementNum;
    }
    // return element at the index of i
    int elementAt( int i ) const { return element[i]; }
    // return index of element e
    int indexOf( int e ) const { return index[e - min]; }
    // remove element and update index
    void eraseElement( int e )
    {
        int i = index[e - min];
        --elementNum;
        index[element[elementNum] - min] = i;
        element[i] = element[elementNum];
        index[e - min] = INVALID_INDEX;
    }
    // remove index and update element
    void eraseIndex( int i )
    {
        int e = element[i];
        --elementNum;
        index[element[elementNum] - min] = i;
        element[i] = element[elementNum];
        index[e - min] = INVALID_INDEX;
    }

    // return number of inserted elements
    int size() const { return elementNum; }

    // keep the capacity but invalidate all elements and reset the size
    void clear()
    {
        index = std::vector<int>( element.size(), INVALID_INDEX );
        elementNum = 0;
    }
    // this implementation may be more efficient for a sparse data set
    //void clear()
    //{
    //    for (int i = 0; i < elementNum; i++) {
    //        index[element[i] - min] = INVALID_INDEX;
    //    }
    //    elementNum = 0;
    //}

protected:
    int min;              // min value of elements
    int max;              // max value of elements

    int elementNum;             // current number of elements
    std::vector<int> element;   // elements value, elementNum valid elements in it
    std::vector<int> index;     // elements index in element
};

class BidirectionIndex_Safe : public BidirectionIndex
{
public:
    BidirectionIndex_Safe( int size, int minValue = 0 ) :BidirectionIndex( size, minValue ) {}

    // insert element e and update index
    void insert( int e );
    // return element at the index of i (e.g., the No. i inserted element if no remove)
    int elementAt( int i ) const;   // return (min-1) if index out of range
    // return index of element e (e.g., the sequence number of the e of the insertion if no remove)
    int indexOf( int e ) const;   // return INVALID_INDEX if no such element inserted
    // remove element and update index
    void eraseElement( int e );
    // remove index and update element
    void eraseIndex( int i );

    // if the element has been inserted, return true
    bool isElementExist( int e ) const { return (index[e - min] != INVALID_INDEX); }
    // if the element is in range [min, max], return true
    bool isElementValid( int e ) const { return ((e <= max) && (e >= min)); }
    // if the index is in range [0, elementNum), return true
    bool isIndexValid( int i ) const { return ((i >= 0) && (i < elementNum)); }
};



#define BIDIRECTION_INDEX_H
#endif