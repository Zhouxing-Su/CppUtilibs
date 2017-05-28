////////////////////////////////
/// usage : 1.	map arbitrary numeric id into non-negative numbers.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_CONSECUTIVE_NON_NEGATIVE_ID_MAP_H
#define SZX_CPPUTILIBS_CONSECUTIVE_NON_NEGATIVE_ID_MAP_H


#include <vector>
#include <map>


namespace szx {

template<typename ID = int, const ID DefaultIdNumberHint = 1024>
class ConsecutiveNonNegativeIdMap {
public:
    ConsecutiveNonNegativeIdMap(ID idNumberHint = DefaultIdNumberHint) : count(-1) {
        idList.reserve(static_cast<size_t>(idNumberHint));
    }


    // track a new arbitrary ID or return the sequence of a tracked one.
    ID toConsecutiveId(ID arbitraryId) {
        auto iter = idMap.find(arbitraryId);
        if (iter != idMap.end()) { return iter->second; }
        idList.push_back(arbitraryId);
        return (idMap[arbitraryId] = (++count));
    }

    // return the consecutiveId_th tracked arbitrary ID.
    ID toArbitraryId(ID consecutiveId) const { return idList[static_cast<size_t>(consecutiveId)]; }


    // number of tracked IDs.
    ID count;
    // idList[consecutiveId] == arbitraryId.
    std::vector<ID> idList;
    // idMap[arbitraryId] == consecutiveId.
    std::map<ID, ID> idMap;
};

}


#endif // SZX_CPPUTILIBS_CONSECUTIVE_NON_NEGATIVE_ID_MAP_H
