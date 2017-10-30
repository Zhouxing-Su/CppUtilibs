////////////////////////////////
/// usage : 1.	map arbitrary identifiers into zero-based numbers.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_ZERO_BASED_CONSECUTIVE_ID_MAP_H
#define SZX_CPPUTILIBS_ZERO_BASED_CONSECUTIVE_ID_MAP_H


#include <vector>
#include <map>


namespace szx {

template<typename ArbitraryId = int, typename ConsecutiveId = int, const ConsecutiveId DefaultIdNumberHint = 1024>
class ZeroBasedConsecutiveIdMap {
public:
    ZeroBasedConsecutiveIdMap(ConsecutiveId idNumberHint = DefaultIdNumberHint) : count(-1) {
        idList.reserve(static_cast<size_t>(idNumberHint));
    }


    // track a new arbitrary ID or return the sequence of a tracked one.
    ConsecutiveId toConsecutiveId(ArbitraryId arbitraryId) {
        auto iter = idMap.find(arbitraryId);
        if (iter != idMap.end()) { return iter->second; }
        idList.push_back(arbitraryId);
        return (idMap[arbitraryId] = (++count));
    }

    // return the consecutiveId_th tracked arbitrary ID.
    ArbitraryId toArbitraryId(ConsecutiveId consecutiveId) const { return idList[consecutiveId]; }


    // number of tracked IDs.
    ConsecutiveId count;
    // idList[consecutiveId] == arbitraryId.
    std::vector<ArbitraryId> idList;
    // idMap[arbitraryId] == consecutiveId.
    std::map<ArbitraryId, ConsecutiveId> idMap;
};

}


#endif // SZX_CPPUTILIBS_ZERO_BASED_CONSECUTIVE_ID_MAP_H
