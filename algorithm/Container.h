////////////////////////////////
/// usage : 1.	container wrapper.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_CONTAINER_H
#define SZX_CPPUTILIBS_CONTAINER_H


#include <map>
#include <set>


namespace szx {

template<typename T>
static bool contains(const std::set<T> &s, const T &value) {
    return s.find(value) != s.end();
}

template<typename Key, typename Value>
static typename std::map<Key, Value>::iterator findLessEqual(std::map<Key, Value> &m, const Key &key, const Value &defaultValue) {
    auto iter = m.lower_bound(key);
    if (iter->first == key) { return iter; } // find equal key.
    if (iter != m.begin()) { return --iter; } // find less key.
    return m.emplace_hint(m.begin(), key, defaultValue); // no less equal key.
}
template<typename Key, typename Value>
static typename std::map<Key, Value>::iterator findLessEqual(std::map<Key, Value> &m, const Key &key) {
    auto iter = m.lower_bound(key);
    if (iter->first == key) { return iter; } // find equal key.
    if (iter != m.begin()) { --iter; } // find less key if there is.
    return iter;
}
template<typename Key, typename Value>
static bool findLessEqual(std::map<Key, Value> &m, const Key &key, typename std::map<Key, Value>::iterator &result) {
    result = m.lower_bound(key);
    if (result->first == key) { return true; } // find equal key.
    if (result == m.begin()) { return false; } // no less key.
    --result; // find less key.
    return true;
}

}


#endif // SZX_CPPUTILIBS_CONTAINER_H