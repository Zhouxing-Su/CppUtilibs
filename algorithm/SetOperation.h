////////////////////////////////
/// usage : 1.	set operations.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_SET_OPERATION_H
#define SZX_CPPUTILIBS_SET_OPERATION_H


#include <algorithm>


namespace szx {

// return true if the container contains element.
template <typename T_Set, typename T>
static bool contain(const T_Set &container, const T &element) {
    return (std::end(container) !=
        std::find(std::begin(container), std::end(container), element));
}

// return true if the container contains element.
template <typename T>
static bool contain(const T &container, const T &element) {
    return (container == element);
}

}


#endif // SZX_CPPUTILIBS_SET_OPERATION_H