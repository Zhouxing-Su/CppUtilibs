#include "BidirectionIndex.h"

using namespace std;


void BidirectionIndex_Safe::insert( int e )
{
    if (isElementValid( e ) && !isElementExist( e )) {
        element[elementNum] = e;
        index[e - min] = elementNum;
        ++elementNum;
    }
}

int BidirectionIndex_Safe::elementAt( int i ) const
{
    if (isIndexValid( i )) {
        return element[i];
    } else {
        return (min - 1);
    }
}

int BidirectionIndex_Safe::indexOf( int e ) const
{
    if (isElementValid( e ) && isElementExist( e )) {
        return index[e - min];
    } else {
        return INVALID_INDEX;
    }
}

void BidirectionIndex_Safe::eraseElement( int e )
{
    if (isElementValid( e ) && isElementExist( e )) {
        int i = index[e - min];
        --elementNum;
        index[element[elementNum] - min] = i;
        element[i] = element[elementNum];
        index[e - min] = INVALID_INDEX;
    }
}

void BidirectionIndex_Safe::eraseIndex( int i )
{
    if (isIndexValid( i )) {
        int e = element[i];
        --elementNum;
        index[element[elementNum] - min] = i;
        element[i] = element[elementNum];
        index[e - min] = INVALID_INDEX;
    }
}