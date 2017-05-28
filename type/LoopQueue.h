////////////////////////////////
/// usage : 1.	a fix-size loop queue.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_LOOP_QUEUE_H
#define SZX_CPPUTILIBS_LOOP_QUEUE_H


#include <vector>


namespace szx {

template<typename T, typename IndexType = int>
class LoopQueue {
public:
    LoopQueue(IndexType size, IndexType initIndex) : len(size), head(nextIndex(initIndex)), tail(initIndex), q(size) {}
    LoopQueue(IndexType size) : LoopQueue(size, size / 4) {}

    void clear() { head = nextIndex(tail); }


    const T& front() const { return q[head]; }
    const T& back() const { return q[tail]; }

    void pushBack(const T& item) { q[increaseIndex(tail)] = item; }
    void pushFront(const T& item) { q[decreaseIndex(head)] = item; }

    void popBack() { decreaseIndex(tail); }
    void popFront() { increaseIndex(head); }

    bool empty() const { return (head == nextIndex(tail)); }

protected:
    IndexType& increaseIndex(IndexType &index) const {
        if ((++index) >= len) { index -= len; }
        return index;
    }
    IndexType& decreaseIndex(IndexType &index) const {
        if ((--index) < 0) { index += len; }
        return index;
    }
    IndexType nextIndex(IndexType index) const { return increaseIndex(index); }


    IndexType len;
    IndexType head;
    IndexType tail;
    std::vector<T> q;
};

}


#endif // SZX_CPPUTILIBS_LOOP_QUEUE_H
