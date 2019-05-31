////////////////////////////////
/// usage : 1.	
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_KNAPSACK_H
#define SZX_CPPUTILIBS_KNAPSACK_H


#include <algorithm>
#include <map>
#include <vector>

#include "Container.h"


namespace szx {

struct KnapsackBase {
    using ID = int;
    template<typename T>
    using List = std::vector<T>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;

    #if SZX_CPPUTILIBS_KNAPSACK_ITEM_PARTIAL_SPECIALIZATION
    template<typename Weight, typename Value = void>
    struct Item {
        Item() {}
        Item(Weight itemWeight, Value itemValue) : w(itemWeight), v(itemValue) {}

        Weight weight() const { return w; }
        Value value() const { return v; }

        Weight w;
        Value v;
    };
    template<typename Weight>
    struct Item<Weight, void> {
        Item() {}
        Item(Weight itemWeight) : w(itemWeight) {}

        Weight weight() const { return w; }
        Weight value() const { return w; }

        Weight w;
    };
    #else // SZX_CPPUTILIBS_KNAPSACK_ITEM_PARAMETER_PACK
    template<typename ...T>
    struct Item;
    template<typename Weight, typename Value>
    struct Item<Weight, Value> {
        Item() {}
        Item(Weight itemWeight, Value itemValue) : w(itemWeight), v(itemValue) {}

        Weight weight() const { return w; }
        Value value() const { return v; }

        Weight w;
        Value v;
    };
    template<typename Weight>
    struct Item<Weight> {
        Item() {}
        Item(Weight itemWeight) : w(itemWeight) {}

        Weight weight() const { return w; }
        Weight value() const { return w; }

        Weight w;
    };
    #endif // SZX_CPPUTILIBS_KNAPSACK_ITEM_PARTIAL_SPECIALIZATION

    struct IdSet : public List<bool> {
        using List<bool>::List;

        void addItem(ID i) { operator[](i) = true; }
    };
    struct IdList : public List<ID> {
        using List<ID>::List;
        void addItem(ID i) { push_back(i); }
    };
};

struct Knapsack : public KnapsackBase {
    // 0-1 knapsack solver.
    // `capacity` is the inclusive maximal weight of picked items.
    // `lowerBound` is the worst value of the optima, and it will be updated with the best solution found by this method.
    // `candidateNum` is the maximal number of optima to record in `sln`.
    // return the total weight of items of the best solution.
    // it will be better if items are ordered by weight in increasing order.
    template<typename Item, typename Weight, typename Value = Weight, typename Candidate>
    static Weight dynamicProgramming(const List<Item> &items, Weight capacity, Value &lowerBound, List<Candidate> &sln, const Candidate &emptyCandidate, ID candidateNum = 1) {
        Weight capacityRange = capacity + 1;
        List<List<Candidate>> slns(capacityRange, List<Candidate>(1, emptyCandidate));

        List<Value> values(capacityRange, 0);
        Weight weightSum = 0;
        ID i = 0;
        for (auto item = items.begin(); item != items.end(); ++item, ++i) {
            weightSum += item->weight();
            for (Weight w = (std::min)(weightSum, capacity); w >= item->weight(); --w) {
                Weight weightBeforePick = w - item->weight();
                Value valueAfterPick = values[weightBeforePick] + item->value();
                Value &bestValue = values[w];
                if (valueAfterPick < bestValue) { continue; } // skip is better.
                List<Candidate> &slnsAfterPick(slns[w]);
                if (valueAfterPick > bestValue) { // pick is better.
                    bestValue = valueAfterPick;
                    slnsAfterPick.clear();
                }
                List<Candidate> &slnsBeforePick(slns[weightBeforePick]);
                ID restCandidateNum = candidateNum - slnsAfterPick.size();
                for (auto s = slnsBeforePick.begin(); (s != slnsBeforePick.end()) && (--restCandidateNum >= 0); ++s) {
                    slnsAfterPick.push_back(*s);
                    slnsAfterPick.back().addItem(i);
                }
                if ((w >= capacity) && (restCandidateNum <= 0)) {
                    lowerBound = values[capacity];
                    sln = slnsAfterPick;
                    return capacity;
                }
            }
        }

        for (; (capacity > 0) && slns[capacity].empty(); --capacity) {}
        lowerBound = values[capacity];
        sln = slns[capacity]; // EXTEND[szx][5]: merge sub-optima if the number of optima is less than candidateNum.
        return capacity;
    }
    template<typename Item, typename Weight, typename Value = Weight>
    static Weight dynamicProgramming(const List<Item> &items, Weight capacity, Value &lowerBound, List<IdSet> &sln, ID candidateNum = 1) {
        return dynamicProgramming(items, capacity, lowerBound, sln, IdSet(items.size(), false), candidateNum);
    }
    template<typename Item, typename Weight, typename Value = Weight>
    static Weight dynamicProgramming(const List<Item> &items, Weight capacity, Value &lowerBound, List<IdList> &sln, ID candidateNum = 1) {
        return dynamicProgramming(items, capacity, lowerBound, sln, IdList(), candidateNum);
    }

    template<typename Item, typename Weight, typename Value = Weight, typename Candidate>
    static Weight dynamicProgrammingForSparseWeightDistribution(const List<Item> &items, Weight capacity, Value &lowerBound, List<Candidate> &sln, const Candidate &emptyCandidate, ID candidateNum = 1) {
        Map<Weight, List<Candidate>> slns;

        Map<Weight, Value> values = { { 0, 0 } };
        ID i = 0;
        for (auto item = items.begin(); item != items.end(); ++item, ++i) {
            for (auto wv = values.end(); wv != values.begin();) {
                --wv;
                Weight w = wv->first + item->weight();
                if (w > capacity) { continue; }
                Weight weightBeforePick = wv->first;
                Value valueAfterPick = wv->second + item->value();
                Value &bestValue = values.emplace(w, 0).first->second; // equivalent to `values[w]` but it allows setting default value other than `Weight()`.
                if (valueAfterPick < bestValue) { continue; } // skip is better.
                List<Candidate> &slnsAfterPick(slns[w]);
                if (valueAfterPick > bestValue) { // pick is better.
                    bestValue = valueAfterPick;
                    slnsAfterPick.clear();
                }
                List<Candidate> &slnsBeforePick(slns[weightBeforePick]);
                if (slnsBeforePick.empty()) { slnsBeforePick.push_back(emptyCandidate); }
                ID restCandidateNum = candidateNum - slnsAfterPick.size();
                for (auto s = slnsBeforePick.begin(); (s != slnsBeforePick.end()) && (--restCandidateNum >= 0); ++s) {
                    slnsAfterPick.push_back(*s);
                    slnsAfterPick.back().addItem(i);
                }
                if ((w >= capacity) && (restCandidateNum <= 0)) {
                    lowerBound = values[capacity];
                    sln = slnsAfterPick;
                    return capacity;
                }
            }
        }

        lowerBound = values.rbegin()->second;
        sln = slns.rbegin()->second; // EXTEND[szx][5]: merge sub-optima if the number of optima is less than candidateNum.
        return values.rbegin()->first;
    }
    template<typename Item, typename Weight, typename Value = Weight>
    static Weight dynamicProgrammingForSparseWeightDistribution(const List<Item> &items, Weight capacity, Value &lowerBound, List<IdSet> &sln, ID candidateNum = 1) {
        return dynamicProgrammingForSparseWeightDistribution(items, capacity, lowerBound, sln, IdSet(items.size(), false), candidateNum);
    }
    template<typename Item, typename Weight, typename Value = Weight>
    static Weight dynamicProgrammingForSparseWeightDistribution(const List<Item> &items, Weight capacity, Value &lowerBound, List<IdList> &sln, ID candidateNum = 1) {
        return dynamicProgrammingForSparseWeightDistribution(items, capacity, lowerBound, sln, IdList(), candidateNum);
    }

    // grouped 0-1 knapsack solver.
    // `items` is a list of items where sum(items, itemNumInGroup[g - 1], itemNumInGroup[g])
    // `itemNumInGroup[g]` is the number of items in the g_th group.
    // `capacity` is the inclusive maximal weight of picked items.
    // `lowerBound` is the worst value of the optima, and it will be updated with the best solution found by this method.
    // `candidateNum` is the maximal number of optima to record in `sln`.
    // return the total weight of items of the best solution.
    // it will be better if items are ordered by weight in increasing order.
    template<typename Item, typename Weight, typename Value = Weight, typename Candidate>
    static Weight dynamicProgramming(const List<Item> &items, const List<ID> itemNumInGroup, Weight capacity, Value &lowerBound, List<Candidate> &sln, const Candidate &emptyCandidate, ID candidateNum = 1) {
        Weight capacityRange = capacity + 1;
        List<List<Candidate>> slns(capacityRange, List<Candidate>(1, emptyCandidate));

        List<Value> values(capacityRange, 0);
        Weight weightSum = 0;
        ID i = 0;
        for (auto group = itemNumInGroup.begin(); group != itemNumInGroup.end(); ++group) {
            weightSum += item->weight();
            for (Weight w = (std::min)(weightSum, capacity); w >= item->weight(); --w) {
                for (auto item = items.begin(); item != items.end(); ++item, ++i) {
                    Weight weightBeforePick = w - item->weight();
                    Value valueAfterPick = values[weightBeforePick] + item->value();
                    Value &bestValue = values[w];
                    if (valueAfterPick < bestValue) { continue; } // skip is better.
                    List<Candidate> &slnsAfterPick(slns[w]);
                    if (valueAfterPick > bestValue) { // pick is better.
                        bestValue = valueAfterPick;
                        slnsAfterPick.clear();
                    }
                    List<Candidate> &slnsBeforePick(slns[weightBeforePick]);
                    ID restCandidateNum = candidateNum - slnsAfterPick.size();
                    for (auto s = slnsBeforePick.begin(); (s != slnsBeforePick.end()) && (--restCandidateNum >= 0); ++s) {
                        slnsAfterPick.push_back(*s);
                        slnsAfterPick.back().addItem(i);
                    }
                    if ((w >= capacity) && (restCandidateNum <= 0)) {
                        lowerBound = values[capacity];
                        sln = slnsAfterPick;
                        return capacity;
                    }
                }
            }
        }

        for (; (capacity > 0) && slns[capacity].empty(); --capacity) {}
        lowerBound = values[capacity];
        sln = slns[capacity]; // EXTEND[szx][5]: merge sub-optima if the number of optima is less than candidateNum.
        return capacity;
    }

    template<typename Item, typename Weight, typename Value = Weight, typename Candidate = IdSet>
    static List<Candidate> branchAndBound(const List<Item> &items, Weight capacity, ID candidateNum = 1) {
    }

    // focus on the nodes with best unit value (value per weight).
    template<typename Item, typename Weight, typename Value = Weight, typename Candidate = IdSet>
    static List<Candidate> bestFirstSearch(const List<Item> &items, Weight capacity, ID candidateNum = 1) {
    }

    template<typename Item, typename Weight, typename Value = Weight, typename Candidate = IdSet>
    static List<Candidate> greedy(const List<Item> &items, Weight capacity, ID candidateNum = 1) {
    }
};


}


#endif // SZX_CPPUTILIBS_KNAPSACK_H
