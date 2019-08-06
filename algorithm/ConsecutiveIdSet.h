////////////////////////////////
/// usage : 1.	bidirection index for fixed size of numbers.
///             for each number i, there should be (i == item[index[i]]).
/// 
/// note  : 1.  "capacity" indicate the max or total valid item number which is different from std container,
///             while "itemNum" and "size" indicate the current inserted items.
///         2.  index is count from 0, while item is between [min, max].
///         3.  it will not consider index out of range error.
////////////////////////////////

#ifndef SZX_CPPUTILIBS_BIDIRECTION_INDEX_H
#define SZX_CPPUTILIBS_BIDIRECTION_INDEX_H


#include "../type/Arr.h"


namespace szx {

class ConsecutiveIdSet {
public:
    using Index = int;
    using Item = Index;


    static constexpr Index InvalidIndex = -1;


    ConsecutiveIdSet(Index capacity, Item minValue = 0)
        : lowerBound(minValue), upperBound(minValue + capacity - 1), itemNum(0),
        items(capacity), index(capacity, InvalidIndex) {
    }


    // if the item has been inserted, return true.
    bool isItemExist(Item e) const { return (index[e - lowerBound] != InvalidIndex); }
    // if the item is in range [min, max], return true.
    bool isItemValid(Item e) const { return ((e <= upperBound) && (e >= lowerBound)); }
    // if the index is in range [0, itemNum), return true.
    bool isIndexValid(Index i) const { return ((i >= 0) && (i < itemNum)); }

    // insert item e and update index.
    void insert(Item e) {
        items[itemNum] = e;
        index[e - lowerBound] = itemNum;
        ++itemNum;
    }
    // return item at the index of i.
    Item itemAt(Index i) const { return items[i]; }
    // return index of item e.
    Index indexOf(Item e) const { return index[e - lowerBound]; }
    // remove item and update index.
    void eraseItem(Item e) {
        Index i = index[e - lowerBound];
        --itemNum;
        index[items[itemNum] - lowerBound] = i;
        items[i] = items[itemNum];
        index[e - lowerBound] = InvalidIndex;
    }
    // remove index and update item.
    void eraseIndex(Index i) {
        Item e = items[i];
        --itemNum;
        index[items[itemNum] - lowerBound] = i;
        items[i] = items[itemNum];
        index[e - lowerBound] = InvalidIndex;
    }

    // return number of inserted items.
    Index size() const { return itemNum; }

    // keep the capacity but invalidate all items and reset the size.
    void clear() {
        index.reset(Arr<Index>::ResetOption::AllBits1);
        itemNum = 0;
    }
    // OPTIMIZE[szx][5]: this implementation may be more efficient for a sparse data set.
    //void clear() {
    //    for (Index i = 0; i < itemNum; ++i) { index[items[i] - lowerBound] = InvalidIndex; }
    //    itemNum = 0;
    //}

protected:
    Item lowerBound; // min value of items.
    Item upperBound; // max value of items.

    Index itemNum; // current number of items.
    Arr<Item> items; // items value, itemNum valid items in it.
    Arr<Index> index; // items index in item.
};


class ConsecutiveIdSet_Safe : public ConsecutiveIdSet {
public:
    ConsecutiveIdSet_Safe(Index size, Item minValue = 0) : ConsecutiveIdSet(size, minValue) {}


    // insert item e and update index.
    void insert(Item e) {
        if (isItemValid(e) && !isItemExist(e)) {
            items[itemNum] = e;
            index[e - lowerBound] = itemNum;
            ++itemNum;
        }
    }
    // return item at the index of i (e.g., the No. i inserted item if no remove).
    Item itemAt(Index i) const { // return (min-1) if index out of range.
        if (isIndexValid(i)) {
            return items[i];
        } else {
            return (lowerBound - 1);
        }
    }
        // return index of item e (e.g., the sequence number of the e of the insertion if no remove).
    Index indexOf(Item e) const { // return InvalidIndex if no such item inserted.
        if (isItemValid(e) && isItemExist(e)) {
            return index[e - lowerBound];
        } else {
            return InvalidIndex;
        }
    }
    // remove item and update index.
    void eraseItem(Item e) {
        if (isItemValid(e) && isItemExist(e)) {
            Index i = index[e - lowerBound];
            --itemNum;
            index[items[itemNum] - lowerBound] = i;
            items[i] = items[itemNum];
            index[e - lowerBound] = InvalidIndex;
        }
    }
    // remove index and update item.
    void eraseIndex(Index i) {
        if (isIndexValid(i)) {
            Item e = items[i];
            --itemNum;
            index[items[itemNum] - lowerBound] = i;
            items[i] = items[itemNum];
            index[e - lowerBound] = InvalidIndex;
        }
    }
};

}


#endif // SZX_CPPUTILIBS_BIDIRECTION_INDEX_H