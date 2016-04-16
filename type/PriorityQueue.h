////////////////////////////////
/// usage : 1.	configurable priority queue.
/// 
/// note  : 1.	use heap for dense graph.
////////////////////////////////

#ifndef SZX_CPPUTILIBS_PRIORITY_QUEUE_H
#define SZX_CPPUTILIBS_PRIORITY_QUEUE_H


#include <algorithm>
#include <vector>
#include <set>


//#define SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP  1


namespace szx {

template<typename T, typename LessPriorityPred = std::less<T>>
class PriorityQueue {
public:
    #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    using ContainerType = std::vector<T>;
    #else
    using ContainerType = std::set<T, LessPriorityPred>;
    #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP


    explicit PriorityQueue(const LessPriorityPred &lessPred = std::less<T>()) :
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        container(), pred(lessPred)
        #else
        container(lessPred)
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    {}

    explicit PriorityQueue(const ContainerType itemContainer, const LessPriorityPred &lessPred = std::less<T>()) :
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        container(itemContainer), pred(lessPred)
        #else
        container(itemContainer.begin(), itemContainer.end(), lessPred)
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    {}

    #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    void setLessPred(const LessPriorityPred &lessPred) {
        pred = lessPred;
        reorder();
    }
    #else
    #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP

    /// [Inordered]
    /// reorder() must be invoked before [Ordered] methods calls after add() is executed.
    void add(const T &e) {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        container.push_back(e);
        #else
        container.insert(e);
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    /// [Inordered]
    /// reorder() must be invoked before [Ordered] methods calls after remove() is executed.
    void remove(const T &e) {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        // TODO[szx][5]: find and erase?
        #else
        container.erase(e);
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    /// [Inordered]
    void reorder() {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        std::make_heap(container.begin(), container.end(), pred);
        #else
        // nothing to be done.
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    /// [Ordered]
    T top() const {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        return container.front();
        #else
        return *container.begin();
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    /// [Ordered]
    void pop() {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        std::pop_heap(container.begin(), container.end(), pred);
        container.pop_back();
        #else
        container.erase(container.begin());
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    /// [Ordered]
    void push(const T &e) {
        #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
        container.push_back(e);
        std::push_heap(container.begin(), container.end(), pred);
        #else
        container.insert(e);
        #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    }

    void clear() { container.clear(); }

    bool empty() const { return container.empty(); }

    const ContainerType& getContainer() const { return container; }

private:
    ContainerType container;
    #if SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
    LessPriorityPred pred;
    #else
    #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_USE_HEAP
};

}


#endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_H
