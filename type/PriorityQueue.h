////////////////////////////////
/// usage : 1.	configurable priority queue.
/// 
/// note  : 1.	use heap for dense graph.
////////////////////////////////

#ifndef SZX_CPPUTILIBS_PRIORITY_QUEUE_H
#define SZX_CPPUTILIBS_PRIORITY_QUEUE_H


#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <map>


// [?] use `log2(priority)` as the group index instead of uniform grouping.
#define SZX_CPPUTILIBS_PRIORITY_QUEUE_BUCKET_IMP_LOG2_GROUP_MODE  1


namespace szx {

namespace impl {

class PriorityQueue {
public:
    // less/greater key means higher/lower priority.
    // it is effective on radix based implementations only and will not bother compare based ones.
    using Priority = int;


    // [CompareBased]
    template<typename T, typename LessKeyPred = std::less<T>>
    class SetImpl {
    public:
        using ContainerType = std::set<T, LessKeyPred>;


        explicit SetImpl(const LessKeyPred &lessPred = LessKeyPred())
            : container(lessPred) {}

        explicit SetImpl(const ContainerType itemContainer, const LessKeyPred &lessPred = LessKeyPred())
            : container(itemContainer.begin(), itemContainer.end(), lessPred) {}

        explicit SetImpl(const SetImpl &pq) : container(pq.container) {}


        // remove the given item.
        void remove(const T &e) { container.erase(e); }

        // reorganize to fit the properties of the priority queue.
        void reorder() {} // nothing to be done.

        // peek the item with minimal priority (highest priority).
        T top() const { return *container.begin(); }

        // remove the item with minimal priority (highest priority).
        void pop() { container.erase(container.begin()); }

        // put the item in proper place.
        void push(const T &e, Priority /*priority*/) { container.insert(e); }

        void clear() { container.clear(); }

        bool empty() const { return container.empty(); }

        const ContainerType& getContainer() const { return container; }

    protected:
        ContainerType container;
    };


    // [CompareBased]
    template<typename T, typename LessKeyPred = std::less<T>, typename LessValuePred = std::less<T>>
    class MapImpl {
    public:
        using ContainerType = std::map<Priority, std::vector<T>, LessKeyPred>;
        // EXTEND[szx][8]: using std::set to enable remove operation?
        //                 using ContainerType = std::map<Priority, std::set<T, LessValuePred>, LessKeyPred>;


        explicit MapImpl(const LessKeyPred &lessPred = LessKeyPred())
            : container(lessPred) {}

        explicit MapImpl(const ContainerType itemContainer, const LessKeyPred &lessPred = LessKeyPred())
            : container(itemContainer.begin(), itemContainer.end(), lessPred) {}

        explicit MapImpl(const MapImpl &pq)
            : container(pq.container) {}


        // remove the given item.
        void remove(const T &e); // { throw NotImplementedException(); }

        // reorganize to fit the properties of the priority queue.
        void reorder() {} // nothing to be done.

        Priority topPriority() {
            //if (empty()) { throw PeekEmptyContainerException(); }
            return container.begin()->first;
        }

        // peek the item with minimal priority (highest priority).
        T top() const { return *(container.begin()->rbegin()); }

        // remove the item with minimal priority (highest priority).
        void pop() {
            container.erase(container.begin()->rbegin());
            if (container.begin()->empty()) { container.erase(container.begin()); }
        }

        // put the item in proper place.
        void push(const T &e, Priority priority) {
            container[priority].push_back(e);
        }

        void clear() { container.clear(); }

        bool empty() const { return container.empty(); }

        const ContainerType& getContainer() const { return container; }

    protected:
        ContainerType container;
    };


    // [RadixBased]
    template<typename T>
    class BucketL1Impl {
    public:
        static constexpr Priority InvalidIndex = (std::numeric_limits<Priority>::max)() / 2;


        BucketL1Impl(Priority maxBucketNum) : firstNonEmptyIndex(InvalidIndex),
            lastNonEmptyIndex(0), container(maxBucketNum) {}


        // remove the given item.
        void remove(const T &e); // { throw NotImplementedException(); }

        // reorganize to fit the properties of the priority queue.
        void reorder() {} // nothing to be done.

        // TODO[szx][2]: what if the priority is negative?
        //               pass an offset in constructor and add it to the priority in each push?
        // put item into the queue.
        void push(const T &e, Priority priority) {
            container[priority].push_back(e);
            if (priority < firstNonEmptyIndex) { firstNonEmptyIndex = priority; }
            if (priority > lastNonEmptyIndex) { lastNonEmptyIndex = priority; }
        }

        Priority topPriority() {
            //if (empty()) { throw PeekEmptyContainerException(); }
            updateFirstNonEmptyIndex();
            return firstNonEmptyIndex;
        }

        // peek the item with highest priority.
        T top() {
            //if (empty()) { throw PeekEmptyContainerException(); }
            updateFirstNonEmptyIndex();
            return container[firstNonEmptyIndex].back();
        }

        // remove the item with highest priority.
        void pop() {
            //if (empty()) { return; }
            updateFirstNonEmptyIndex();
            container[firstNonEmptyIndex].pop_back();
        }

        bool empty() {
            updateFirstNonEmptyIndex();
            return (firstNonEmptyIndex > lastNonEmptyIndex);
        }

        void clear() {
            for (Priority i = firstNonEmptyIndex; i <= lastNonEmptyIndex; ++i) { container[i].clear(); }
            firstNonEmptyIndex = InvalidIndex;
            lastNonEmptyIndex = 0;
        }

        void reserve(Priority bucket, int capacity) {
            container[bucket].reserve(capacity);
        }

    protected:
        void updateFirstNonEmptyIndex() {
            for (; firstNonEmptyIndex <= lastNonEmptyIndex; ++firstNonEmptyIndex) {
                if (!container[firstNonEmptyIndex].empty()) { return; }
            }
        }


        // the index of the first non-empty group.
        Priority firstNonEmptyIndex;
        // the index of the last non-empty group.
        Priority lastNonEmptyIndex;
        // `container[bucket][i]` is the i_th item in the bucket.
        std::vector<std::vector<T>> container;
    };


    // [RadixBased]
    template<typename T>
    class BucketL2Impl {
    public:
        static constexpr Priority InvalidIndex = (std::numeric_limits<Priority>::max)() / 2;
        // EXTEND[yc][8]: make it an argument of the constructor.
        static constexpr Priority GroupSize = 64;
        static constexpr Priority GroupSizeBase = 8;


        BucketL2Impl(int maxBucketNum) : firstNonEmptyGroupIndex(InvalidIndex), lastNonEmptyGroupIndex(0),
            bucketGroupNum(getGroupIndex(maxBucketNum) + 1), numbersOfNonEmptyBucket(bucketGroupNum, 0),
            container(maxBucketNum), firstNonEmptyBucketIndices(bucketGroupNum, InvalidIndex) {}


        // remove the given item.
        void remove(const T &e); // { throw NotImplementedException(); }

        // reorganize to fit the properties of the priority queue.
        void reorder() {} // nothing to be done.

        // TODO[szx][2]: what if the priority is negative?
        //               pass an offset in constructor and add it to the priority in each push?
        // initialize the container and bucket group (record the first non-empty bucket).
        void push(const T &e, Priority priority) {
            container[priority].push_back(e);
            Priority i = getGroupIndex(priority);
            if (container[priority].size() == 1) { numbersOfNonEmptyBucket[i]++; }
            firstNonEmptyGroupIndex = std::min(firstNonEmptyGroupIndex, i);
            lastNonEmptyGroupIndex = std::max(lastNonEmptyGroupIndex, i);
            firstNonEmptyBucketIndices[i] = std::min(priority, firstNonEmptyBucketIndices[i]);
        }

        Priority topPriority() {
            //if (empty()) { throw PeekEmptyContainerException(); }
            return getFirstNonEmptyIndex();
        }

        // peek the item with highest priority.
        T top() {
            //if (empty()) { throw PeekEmptyContainerException(); }
            return container[getFirstNonEmptyIndex()].front();
        }

        // remove the item with highest priority and find the next non-empty position.
        void pop() {
            //if (empty()) { return; }
            Priority pos = getFirstNonEmptyIndex();
            std::swap(container[pos].front(), container[pos].back());
            container[pos].pop_back();
        }

        bool empty() const {
            return ((firstNonEmptyGroupIndex == lastNonEmptyGroupIndex)
                && (numbersOfNonEmptyBucket[firstNonEmptyGroupIndex] == 0));
        }

    protected:
        static Priority getGroupIndex(Priority n) {
            #if SZX_CPPUTILIBS_PRIORITY_QUEUE_BUCKET_IMP_LOG2_GROUP_MODE
            return ((n / GroupSizeBase == 0) ? 0 : Math::log2(n / GroupSizeBase));
            #else
            return (n / GroupSize);
            #endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_BUCKET_IMP_LOG2_GROUP_MODE
        }

        // update the first non-empty index of bucket group and the total non-empty buckets in each group.
        Priority updateFirstNonEmptyIndex(Priority pos) {
            numbersOfNonEmptyBucket[firstNonEmptyGroupIndex]--;
            if (numbersOfNonEmptyBucket[firstNonEmptyGroupIndex] == 0) {
                if (firstNonEmptyGroupIndex == lastNonEmptyGroupIndex) { return InvalidBucketQueue; }
                firstNonEmptyBucketIndices[firstNonEmptyGroupIndex] = InvalidIndex;
                while ((numbersOfNonEmptyBucket[++firstNonEmptyGroupIndex] == 0)
                    && (firstNonEmptyGroupIndex <= lastNonEmptyGroupIndex));
                return firstNonEmptyBucketIndices[firstNonEmptyGroupIndex];
            } else {
                while ((pos < container.size()) && container[pos].empty()) { ++pos; }
                return pos;
            }
        }

        Priority getFirstNonEmptyIndex() {
            Priority pos = firstNonEmptyBucketIndices[firstNonEmptyGroupIndex];
            return (container[pos].empty() ? updateFirstNonEmptyIndex(pos) : pos);
        }

        // the index of the first non-empty group.
        Priority firstNonEmptyGroupIndex;
        // the index of the last non-empty group.
        Priority lastNonEmptyGroupIndex;
        // the total number of bucket group
        int bucketGroupNum;
        // `firstNonEmptyBucketIndices[i]` is the index of the first non-empty bucket in the i_th group.
        std::vector<Priority> firstNonEmptyBucketIndices;
        // `container[bucket][i]` is the i_th item in the bucket.
        std::vector<std::vector<T>> container;
        // `numbersOfNonEmptyBucket[i] is the number of non-empty buckets in i_th group
        std::vector<int> numbersOfNonEmptyBucket;
    };


    // [CompareBased][Deprecated]
    template<typename T, typename LessKeyPred = std::greater<T>>
    class HeapImpl { // use std::greater as the comparer to achieve a min heap.
    public:
        using ContainerType = std::vector<T>;


        explicit HeapImpl(const LessKeyPred &lessPred = LessKeyPred())
            : container(), pred(lessPred) {}

        explicit HeapImpl(const ContainerType itemContainer, const LessKeyPred &lessPred = LessKeyPred())
            : container(itemContainer), pred(lessPred) {}

        explicit HeapImpl(const HeapImpl &pq) : container(pq.container), pred(pq.pred) {}

        void setLessPred(const LessKeyPred &lessPred) {
            pred = lessPred;
            reorder();
        }


        // remove the given item.
        void remove(const T &e); // { throw NotImplementedException(); } // TODO[szx][5]: find and erase and reorder?

        // reorganize to fit the properties of the priority queue.
        void reorder() { std::make_heap(container.begin(), container.end(), pred); }

        // peek the item with minimal priority (highest priority).
        T top() const { return container.front(); }

        // remove the item with minimal priority (highest priority).
        void pop() {
            std::pop_heap(container.begin(), container.end(), pred);
            container.pop_back();
        }

        // put the item in proper place.
        void push(const T &e, Priority /*priority*/) {
            container.push_back(e);
            std::push_heap(container.begin(), container.end(), pred);
        }

        void clear() { container.clear(); }

        bool empty() const { return container.empty(); }

        const ContainerType& getContainer() const { return container; }

    protected:
        ContainerType container;
        LessKeyPred pred;
    };
};

}

template<typename T, typename LessKeyPred = std::less<T>>
using PriorityQueue = impl::PriorityQueue::SetImpl<T, LessKeyPred>;

}


#endif // SZX_CPPUTILIBS_PRIORITY_QUEUE_H
