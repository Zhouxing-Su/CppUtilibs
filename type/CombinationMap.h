////////////////////////////////
/// usage : 1.	cache for combinations.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SMART_SZX_CPPUTILIBS_COMBINATION_MAP_H
#define SMART_SZX_CPPUTILIBS_COMBINATION_MAP_H


#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <functional>


namespace szx {

template<typename Data, typename Item = int>
struct CombinationMapBase {
    template<typename T>
    using List = std::vector<T>;

    using ItemList = List<Item>; // `ItemList` is a list of items, usually sorted in increasing order.
    using ItemSet = List<bool>; // `ItemSet[i]` is true if item `i` is included in the set.
    using TraverseEvent = std::function<bool(const Data &data)>; // return true if it should break the traverse.
    using CmpData = std::function<bool(const Data &oldData, const Data &newData)>; // return true if the new data is better.


    static const Data& emptyData() {
        static const Data es;
        return es;
    }
    static bool alwaysTrue(const Data&, const Data&) { return true; }
    static bool alwaysFalse(const Data&, const Data&) { return false; }


    CombinationMapBase() {}
    CombinationMapBase(Item itemNumber) : itemNum(itemNumber) {}


    void toItemSet(const ItemList &items, ItemSet &containItem) const {
        std::fill(containItem.begin(), containItem.end(), false);
        for (auto i = items.begin(); i != items.end(); ++i) { containItem[*i] = true; }
    }
    ItemSet toItemSet(const ItemList &items) const {
        ItemSet containItem(itemNum, false);
        for (auto i = items.begin(); i != items.end(); ++i) { containItem[*i] = true; }
        return containItem;
    }
    void toItemList(const ItemSet &containItem, ItemList &items) const {
        items.clear();
        for (Item i = 0; i < itemNum; ++i) {
            if (containItem[i] == true) { items.push_back(i); }
        }
    }
    ItemList toItemList(const ItemSet &containItem) const {
        ItemList items;
        toItemList(containItem, items);
        return items;
    }

    // return a non-empty data if there is an entry for such item set.
    // for a returned `str`, call `str.empty()` to check the status.
    virtual const Data& get(const ItemSet &containItem) const = 0;
    virtual const Data& get(const ItemList &orderedItems) const = 0; // the orderedItems is a list of items in increasing order.

    // return true if overwriting happens or a new entry is added.
    virtual bool set(const ItemSet &containItem, const Data &str, CmpData shouldOverwrite = alwaysFalse) = 0;
    virtual bool set(const ItemList &orderedItems, const Data &str, CmpData shouldOverwrite = alwaysFalse) = 0; // the orderedItems is a list of items in increasing order.

    // `onCombination` return true if the traverse should be breaked, otherwise the loop will continue.
    // return true if no break happens.
    virtual bool forEach(TraverseEvent onCombination) const = 0;

    // the number of cached combinations.
    virtual Item dataNum() const = 0;


    Item itemNum;
};


template<typename Data, typename Item = int>
struct CombinationMap_BinTreeImpl : public CombinationMapBase<Data, Item> {
    using ItemList = CombinationMapBase<Data, Item>::ItemList; // `itemList` is a list of items in increasing order.
    using ItemSet = CombinationMapBase<Data, Item>::ItemSet; // `itemSet[i]` is true if item `i` is included in the set.
    using TraverseEvent = CombinationMapBase<Data, Item>::TraverseEvent;
    using CmpData = CombinationMapBase<Data, Item>::CmpData;
    using CombinationMapBase<Data, Item>::emptyData;
    using CombinationMapBase<Data, Item>::alwaysTrue;
    using CombinationMapBase<Data, Item>::alwaysFalse;
    using CombinationMapBase<Data, Item>::toItemSet;

protected:
    using TreeNodeId = int;
    // for non-leaf node, child[0]/child[1] leads to the sub-tree where the node is excluded/included.
    // for leaf node, child[0]/child[1] are the indices for the data (where the last node is excluded/included) in the dataPool.
    // (child[0] < 0) means the sub-tree or data is not cached.
    using BinTreeNode = std::array<TreeNodeId, 2>;


    static BinTreeNode newTreeNode() { return { -1, -1 }; }

public:
    static constexpr Item DefaultHintDataNum = (1 << 12);


    CombinationMap_BinTreeImpl() {}
    CombinationMap_BinTreeImpl(Item itemNumber, Item hintDataNum = DefaultHintDataNum)
        : CombinationMapBase<Data, Item>(itemNumber) {
        nodePool.reserve(itemNumber * hintDataNum);
        dataPool.reserve(hintDataNum);
        nodePool.push_back(newTreeNode()); // the root.
    }


    virtual const Data& get(const ItemSet &containItem) const override {
        if (static_cast<Item>(containItem.size()) != itemNum) { return emptyData(); }

        TreeNodeId treeNode = 0;
        for (auto i = containItem.begin(); i != containItem.end(); ++i) {
            treeNode = nodePool[treeNode][*i];
            if (treeNode < 0) { return emptyData(); } // cache miss.
        }
        return dataPool[treeNode]; // data found.
    }

    virtual const Data& get(const ItemList &orderedItems) const override {
        return get(toItemSet(orderedItems));
    }

    virtual bool set(const ItemSet &containItem, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        if (static_cast<Item>(containItem.size()) != itemNum) { return false; }

        TreeNodeId treeNode = 0;
        auto lastItem = containItem.end() - 1;
        for (auto i = containItem.begin(); i != lastItem; ++i) {
            TreeNodeId &child = nodePool[treeNode][*i];
            if (child < 0) {
                treeNode = static_cast<TreeNodeId>(nodePool.size());
                child = treeNode;
                nodePool.push_back(newTreeNode());
            } else {
                treeNode = child;
            }
        }

        TreeNodeId &leaf = nodePool[treeNode][*lastItem];
        if (leaf < 0) {
            leaf = static_cast<TreeNodeId>(dataPool.size());
            dataPool.push_back(data);
        } else if (shouldOverwrite(dataPool[leaf], data)) {
            dataPool[leaf] = data;
        } else {
            return false;
        }

        return true;
    }
    virtual bool set(const ItemList &orderedItems, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        return set(toItemSet(orderedItems), data, shouldOverwrite);
    }

    virtual bool forEach(std::function<bool(const Data&)> onCombination) const override {
        for (auto d = dataPool.begin(); d != dataPool.end(); ++d) {
            if (onCombination(*d)) { return false; }
        }
        return false;
    }

    virtual Item dataNum() const { return static_cast<Item>(dataPool.size()); }


    std::vector<BinTreeNode> nodePool;
    std::vector<Data> dataPool;
};


template<typename Data, typename Item = int>
struct CombinationMap_TrieImpl : public CombinationMapBase<Data, Item> {
    using ItemList = CombinationMapBase<Data, Item>::ItemList; // `itemList` is a list of items in increasing order.
    using ItemSet = CombinationMapBase<Data, Item>::ItemSet; // `itemSet[i]` is true if item `i` is included in the set.
    using TraverseEvent = CombinationMapBase<Data, Item>::TraverseEvent;
    using CmpData = CombinationMapBase<Data, Item>::CmpData;
    using CombinationMapBase<Data, Item>::emptyData;
    using CombinationMapBase<Data, Item>::alwaysTrue;
    using CombinationMapBase<Data, Item>::alwaysFalse;
    using CombinationMapBase<Data, Item>::toItemSet;

protected:
    using DataId = int;

    struct TreeNode {
        DataId dataId = -1;
        std::map<Item, TreeNode> children;
    };

public:
    static constexpr Item DefaultHintDataNum = (1 << 12);


    CombinationMap_TrieImpl() {}
    CombinationMap_TrieImpl(Item itemNumber, Item hintDataNum = DefaultHintDataNum)
        : CombinationMapBase<Data, Item>(itemNumber) {
        dataPool.reserve(hintDataNum);
    }


    virtual const Data& get(const ItemSet &containItem) const override {
        return get(toItemList(containItem));
    }

    virtual const Data& get(const ItemList &orderedItems) const override {
        const TreeNode *treeNode = &root;
        for (auto i = orderedItems.begin(); i != orderedItems.end(); ++i) {
            auto child = treeNode->children.find(*i);
            if (child == treeNode->children.end()) { return emptyData(); } // cache miss.
            treeNode = &child->second;
        }
        if (treeNode->dataId < 0) { return emptyData(); } // cache miss.
        return dataPool[treeNode->dataId]; // data found.
    }

    virtual bool set(const ItemSet &containItem, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        return set(toItemList(containItem), data, shouldOverwrite);
    }
    virtual bool set(const ItemList &orderedItems, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        TreeNode *treeNode = &root;
        for (auto i = orderedItems.begin(); i != orderedItems.end(); ++i) {
            treeNode = &(treeNode->children[*i]);
        }
        if (treeNode->dataId < 0) {
            treeNode->dataId = static_cast<DataId>(dataPool.size());
            dataPool.push_back(data);
        } else if (shouldOverwrite(dataPool[treeNode->dataId], data)) {
            dataPool[treeNode->dataId] = data;
        } else {
            return false;
        }

        return true;
    }

    virtual bool forEach(std::function<bool(const Data&)> onCombination) const override {
        for (auto d = dataPool.begin(); d != dataPool.end(); ++d) {
            if (onCombination(*d)) { return false; }
        }
        return false;
    }

    virtual Item dataNum() const { return static_cast<Item>(dataPool.size()); }


    TreeNode root;
    std::vector<Data> dataPool;
};


template<typename Data, typename Item = int>
struct CombinationMap_HashImpl : public CombinationMapBase<Data, Item> {
    using ItemList = CombinationMapBase<Data, Item>::ItemList; // `itemList` is a list of items in increasing order.
    using ItemSet = CombinationMapBase<Data, Item>::ItemSet; // `itemSet[i]` is true if item `i` is included in the set.
    using TraverseEvent = CombinationMapBase<Data, Item>::TraverseEvent;
    using CmpData = CombinationMapBase<Data, Item>::CmpData;
    using CombinationMapBase<Data, Item>::emptyData;
    using CombinationMapBase<Data, Item>::alwaysTrue;
    using CombinationMapBase<Data, Item>::alwaysFalse;
    using CombinationMapBase<Data, Item>::toItemSet;


    CombinationMap_HashImpl() {}
    CombinationMap_HashImpl(Item itemNumber) : CombinationMapBase<Data, Item>(itemNumber) {}
    CombinationMap_HashImpl(Item itemNumber, Item hintDataNum)
        : CombinationMapBase<Data, Item>(itemNumber), dataMap(hintDataNum) {}


    virtual const Data& get(const ItemSet &containItem) const override {
        auto d = dataMap.find(containItem);
        if (d == dataMap.end()) { return emptyData(); } // cache miss.
        return d->second; // data found.
    }

    virtual const Data& get(const ItemList &orderedItems) const override {
        return get(toItemSet(orderedItems));
    }

    virtual bool set(const ItemSet &containItem, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        auto d = dataMap.find(containItem);
        if (d == dataMap.end()) {
            dataMap[containItem] = data;
        } else if (shouldOverwrite(d->second, data)) {
            d->second = data;
        } else {
            return false;
        }

        return true;
    }
    virtual bool set(const ItemList &orderedItems, const Data &data, CmpData shouldOverwrite = alwaysFalse) override {
        return set(toItemSet(orderedItems), data, shouldOverwrite);
    }

    virtual bool forEach(std::function<bool(const Data&)> onCombination) const override {
        for (auto d = dataMap.begin(); d != dataMap.end(); ++d) {
            if (onCombination(d->second)) { return false; }
        }
        return false;
    }

    virtual Item dataNum() const { return static_cast<Item>(dataMap.size()); }


    std::unordered_map<ItemSet, Data> dataMap;
};


template<typename Data, typename Item = int>
//using CombinationMap = CombinationMap_BinTreeImpl<Data, Item>;
//using CombinationMap = CombinationMap_TrieImpl<Data, Item>;
using CombinationMap = CombinationMap_HashImpl<Data, Item>;

}


#endif // SMART_SZX_CPPUTILIBS_COMBINATION_MAP_H
