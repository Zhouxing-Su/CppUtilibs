////////////////////////////////
/// usage : 1.	bidirection index for fixed size of numbers.
///             for each number i, there should be (i == element[index[i]]).
/// 
/// note  : 1.  "capacity" indicate the max or total valid element number which is different from std container,
///             while "elementNum" and "size" indicate the current inserted elements.
///         2.  index is count from 0, while element is between [min, max].
///         3.  it will not consider index out of range error.
////////////////////////////////

#ifndef SZX_CPPUTILIBS_BIDIRECTION_INDEX_H
#define SZX_CPPUTILIBS_BIDIRECTION_INDEX_H


#include <vector>


namespace szx {

class BidirectionIndex {
public:
    using Index = int;
    using Item = Index;


    static constexpr Index InvalidIndex = -1;


    BidirectionIndex(Index capacity, Item minValue = 0)
        : lowerBound(minValue), upperBound(minValue + capacity - 1), elementNum(0),
        element(capacity), index(capacity, InvalidIndex) {}


    // insert element e and update index.
    void insert(Item e) {
        element[elementNum] = e;
        index[e - lowerBound] = elementNum;
        ++elementNum;
    }
    // return element at the index of i.
    Item elementAt(Index i) const { return element[i]; }
    // return index of element e.
    Index indexOf(Item e) const { return index[e - lowerBound]; }
    // remove element and update index.
    void eraseElement(Item e) {
        Index i = index[e - lowerBound];
        --elementNum;
        index[element[elementNum] - lowerBound] = i;
        element[i] = element[elementNum];
        index[e - lowerBound] = InvalidIndex;
    }
    // remove index and update element.
    void eraseIndex(Index i) {
        Item e = element[i];
        --elementNum;
        index[element[elementNum] - lowerBound] = i;
        element[i] = element[elementNum];
        index[e - lowerBound] = InvalidIndex;
    }

    // return number of inserted elements.
    Index size() const { return elementNum; }

    // keep the capacity but invalidate all elements and reset the size.
    void clear() {
        index = std::vector<Item>(element.size(), InvalidIndex);
        elementNum = 0;
    }
    // OPTIMIZE[szx][5]: this implementation may be more efficient for a sparse data set.
    //void clear() {
    //    for (Index i = 0; i < elementNum; ++i) { index[element[i] - min] = InvalidIndex; }
    //    elementNum = 0;
    //}

protected:
    Item lowerBound; // min value of elements.
    Item upperBound; // max value of elements.

    Index elementNum; // current number of elements.
    std::vector<Item> element; // elements value, elementNum valid elements in it.
    std::vector<Index> index; // elements index in element.
};


class BidirectionIndex_Safe : public BidirectionIndex {
public:
    BidirectionIndex_Safe(Index size, Item minValue = 0) : BidirectionIndex(size, minValue) {}


    // if the element has been inserted, return true.
    bool isElementExist(Item e) const { return (index[e - lowerBound] != InvalidIndex); }
    // if the element is in range [min, max], return true.
    bool isElementValid(Item e) const { return ((e <= upperBound) && (e >= lowerBound)); }
    // if the index is in range [0, elementNum), return true.
    bool isIndexValid(Index i) const { return ((i >= 0) && (i < elementNum)); }

    // insert element e and update index.
    void insert(Item e) {
        if (isElementValid(e) && !isElementExist(e)) {
            element[elementNum] = e;
            index[e - lowerBound] = elementNum;
            ++elementNum;
        }
    }
    // return element at the index of i (e.g., the No. i inserted element if no remove).
    Item elementAt(Index i) const { // return (min-1) if index out of range.
        if (isIndexValid(i)) {
            return element[i];
        } else {
            return (lowerBound - 1);
        }
    }
        // return index of element e (e.g., the sequence number of the e of the insertion if no remove).
    Index indexOf(Item e) const { // return InvalidIndex if no such element inserted.
        if (isElementValid(e) && isElementExist(e)) {
            return index[e - lowerBound];
        } else {
            return InvalidIndex;
        }
    }
    // remove element and update index.
    void eraseElement(Item e) {
        if (isElementValid(e) && isElementExist(e)) {
            Index i = index[e - lowerBound];
            --elementNum;
            index[element[elementNum] - lowerBound] = i;
            element[i] = element[elementNum];
            index[e - lowerBound] = InvalidIndex;
        }
    }
    // remove index and update element.
    void eraseIndex(Index i) {
        if (isIndexValid(i)) {
            Item e = element[i];
            --elementNum;
            index[element[elementNum] - lowerBound] = i;
            element[i] = element[elementNum];
            index[e - lowerBound] = InvalidIndex;
        }
    }
};

}


#endif // SZX_CPPUTILIBS_BIDIRECTION_INDEX_H