////////////////////////////////
/// usage : 1.	an interval implementation with some basic operations.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_INTERVAL_H
#define SZX_CPPUTILIBS_INTERVAL_H


namespace szx {

template<typename Number = int>
struct Interval {
    Interval() {}
    Interval(Number intervalBegin, Number intervalEnd) : begin(intervalBegin), end(intervalEnd) {}

    bool cover(Number x) const { return ((begin <= x) && (x < end)); }
    bool beginBefore(Number x) const { return (begin < x); }
    bool endBefore(Number x) const { return (end <= x); }
    bool beginBefore(const Interval &i) const { return (begin < i.begin); }
    bool endBefore(const Interval &i) const { return (end < i.end); }
    /// return true if this is strictly before i (no overlap).
    bool before(const Interval &i) const { return (end < i.begin); }

    bool isValid() const { return (begin < end); }
    static bool isValid(const Interval& i) { return i.isValid(); }

    static bool isOverlaped(const Interval& l, const Interval &r) {
        return ((l.begin < r.end) && (r.begin < l.end));
    }

    /// define `\cap` as intersection, this method do result = l \cap r.
    static Interval intersect(const Interval& l, const Interval &r) {
        Interval result;
        result.begin = std::max(l.begin, r.begin);
        result.end = std::min(l.end, r.end);
        return result;
    }

    Number begin;
    Number end;
};

}


#endif // SZX_CPPUTILIBS_INTERVAL_H
