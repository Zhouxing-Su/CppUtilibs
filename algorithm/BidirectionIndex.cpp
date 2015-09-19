#include "BidirectionIndex.h"


using namespace std;


namespace szx
{

const int BidirectionIndex::INVALID_INDEX = -1;

void BidirectionIndex_Safe::insert( int e )
{
    if (isElementValid( e ) && !isElementExist( e )) {
        element[elementNum] = e;
        index[e - lowerBound] = elementNum;
        ++elementNum;
    }
}

int BidirectionIndex_Safe::elementAt( int i ) const
{
    if (isIndexValid( i )) {
        return element[i];
    } else {
        return (lowerBound - 1);
    }
}

int BidirectionIndex_Safe::indexOf( int e ) const
{
    if (isElementValid( e ) && isElementExist( e )) {
        return index[e - lowerBound];
    } else {
        return INVALID_INDEX;
    }
}

void BidirectionIndex_Safe::eraseElement( int e )
{
    if (isElementValid( e ) && isElementExist( e )) {
        int i = index[e - lowerBound];
        --elementNum;
        index[element[elementNum] - lowerBound] = i;
        element[i] = element[elementNum];
        index[e - lowerBound] = INVALID_INDEX;
    }
}

void BidirectionIndex_Safe::eraseIndex( int i )
{
    if (isIndexValid( i )) {
        int e = element[i];
        --elementNum;
        index[element[elementNum] - lowerBound] = i;
        element[i] = element[elementNum];
        index[e - lowerBound] = INVALID_INDEX;
    }
}

}