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

template<typename T>
class PriorityQueueBase {
public:
    virtual void add(const T &e) = 0;
    virtual void remove(const T &e) = 0;
    virtual void reorder() = 0;

    virtual T top() const = 0;
    virtual void pop() = 0;
    virtual void push(const T &e) = 0;

    virtual void clear() = 0;
    virtual bool empty() const = 0;
};

template<typename T, typename GreaterPriorityPred = std::greater<T>>
class PriorityQueueUsingSet : public PriorityQueueBase<T> {
public:
    using ContainerType = std::set<T, GreaterPriorityPred>;


    explicit PriorityQueueUsingSet(const GreaterPriorityPred &greaterPred = std::greater<T>()) :
        container(greaterPred) {}

    explicit PriorityQueueUsingSet(const ContainerType itemContainer, const GreaterPriorityPred &greaterPred = std::greater<T>()) :
        container(itemContainer.begin(), itemContainer.end(), greaterPred) {}

    void add(const T &e) { container.insert(e); }

    void remove(const T &e) { container.erase(e); }

    void reorder() {}

    T top() const { return *container.begin(); }

    void pop() { container.erase(container.begin()); }

    void push(const T &e) { container.insert(e); }

    void clear() { container.clear(); }

    bool empty() const { return container.empty(); }

    const ContainerType& getContainer() const { return container; }

private:
    ContainerType container;
};

template<typename T, typename LessPriorityPred = std::less<T>>
class PriorityQueueUsingHeap : public PriorityQueueBase<T> {
public:
    using ContainerType = std::vector<T>;


    explicit PriorityQueueUsingHeap(const LessPriorityPred &lessPred = std::less<T>()) :
        container(), pred(lessPred) {}

    explicit PriorityQueueUsingHeap(const ContainerType itemContainer, const LessPriorityPred &lessPred = std::less<T>()) :
        container(itemContainer), pred(lessPred) {}

    void setLessPred(const LessPriorityPred &lessPred) {
        pred = lessPred;
        reorder();
    }

    /// reorder() must be invoked before [Ordered] methods calls after add() is executed.
    void add(const T &e) { container.push_back(e); }

    /// reorder() must be invoked before [Ordered] methods calls after remove() is executed.
    void remove(const T &e) { 
        std::swap(*std::find(container.begin(), container.end(), e), container.back()); 
        container.pop_back();
    }

    void reorder() { std::make_heap(container.begin(), container.end(), pred); }

    T top() const { return container.front(); }

    void pop() {
        std::pop_heap(container.begin(), container.end(), pred);
        container.pop_back();
    }

    void push(const T &e) {
        container.push_back(e);
        std::push_heap(container.begin(), container.end(), pred);
    }

    void clear() { container.clear(); }

    bool empty() const { return container.empty(); }

    const ContainerType& getContainer() const { return container; }

private:
    ContainerType container;
    LessPriorityPred pred;
};

template<typename T, typename GreaterPriorityPred = std::greater<T>>
class PriorityQueue : public PriorityQueueUsingSet<T, std::greater<T>()> {};

}


#endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_H
