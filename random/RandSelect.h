////////////////////////////////
/// usage : 1.	call isSelected() method N times to randomly select 1 out of N elements with equiprobability.
///             this class is usefull when you can not find out how many elements are there before enumerating them all,
///             you will not need to hold all posible selections and use (rand() % N) to select one by using this class.
///
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_RAND_SELECT_H
#define SZX_CPPUTILIBS_RAND_SELECT_H


#include "Random.h"


namespace szx
{

class RandSelect {
public:
    enum StartCount { NoPreseletedElement = 0, WithPreseletedElement = 1 };

    RandSelect(Random &randomNumberGenerator, int startCount = StartCount::WithPreseletedElement)
        : rgen(randomNumberGenerator) {
        reset(startCount);
    }

    /// start a new selection on another N elements.
    /// sometimes the first element is pre-selected with the possibility of 1, 
    /// so you can pass 1 in this condition to leave out a isSelected() call.
    void reset(int startCount = StartCount::WithPreseletedElement) { count = startCount; }

    /// call this for each of the N elements (N times in total) to judge 
    /// whether each of them is selected. 
    /// only the last returned "true" means that element is selected finally.
    bool isSelected() { return ((rgen() % (++count)) == 0); }

    /// if (newItem != minItem), return as their strict order. else select one
    /// as the minimal in the sequence of isMinimal() calls randomly.
    template<typename T>
    bool isMinimal(const T &minItem, const T &newItem) {
        if (newItem > minItem) {
            return false;
        } else if (newItem == minItem) {
            return isSelected();
        } else {
            reset();
            return true;
        }
    }

protected:
    Random &rgen;
    int count; /// number of elements that have been considered.
};

}


#endif // SZX_CPPUTILIBS_RAND_SELECT_H