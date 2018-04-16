////////////////////////////////
/// usage : 1.	an interval implementation with some basic operations.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_INTERVAL_H
#define SZX_CPPUTILIBS_INTERVAL_H


#include <algorithm>


namespace szx {

template<typename Unit>
struct Interval {
    Interval() {}
    Interval(Unit intervalBegin, Unit intervalEnd) : begin(intervalBegin), end(intervalEnd) {}

    bool cover(Unit x) const { return ((begin <= x) && (x < end)); }
    bool cover(const Interval &i) const { return ((begin <= i.begin) && (i.end <= end)); }
    bool beginBefore(Unit x) const { return (begin < x); }
    bool beginBefore(const Interval &i) const { return (begin < i.begin); }
    bool endBefore(Unit x) const { return (end <= x); }
    bool endBefore(const Interval &i) const { return (end < i.end); }
    // return true if this is strictly before i (no overlap).
    bool before(const Interval &i) const { return (end <= i.begin); }

    bool isValid() const { return (begin < end); }
    static bool isValid(const Interval &i) { return i.isValid(); }

    static bool isOverlapped(const Interval &l, const Interval &r) {
        return ((l.begin < r.end) && (r.begin < l.end));
    }

    // vector measurement of the interval span.
    Unit displacement() const { return (end - begin); }
    // scalar measurement of the interval span.
    Unit length() const { return std::abs(end - begin); }

    // return the intersection of l and r if they are overlapped,
    // or the reversed gap between them if there is no intersection.
    static Interval overlap(const Interval &l, const Interval &r) {
        return Interval((std::max)(l.begin, r.begin), (std::min)(l.end, r.end));
    }

    // return the length of the blank space between l and r if they are not interseted,
    // or the opposite number of the minimal distance to make them mutually exclusive.
    static Unit gap(const Interval &l, const Interval &r) {
        if (l.begin < r.begin) {
            if (l.end < r.end) {
                return r.begin - l.end;
            } else { // if (l.end >= r.end)
                return (std::max)(r.begin - l.end, l.begin - r.end);
            }
        } else { // if (l.begin >= r.end)
            if (l.end < r.end) {
                return (std::max)(r.begin - l.end, l.begin - r.end);
            } else { // if (l.end >= r.end)
                return l.begin - r.end;
            }
        }
    }

    Unit begin;
    Unit end;
};

}


#endif // SZX_CPPUTILIBS_INTERVAL_H
