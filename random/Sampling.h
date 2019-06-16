////////////////////////////////
/// usage : 1.	an online algorithm for sampling targetNum items from a data stream.
///         2.	for the case of single selection, call Sampling1::isPicked() on each element to randomly select equiprobability.
///             this class is usefull when you can not find out how many elements are there before enumerating them all,
///             you will not need to hold all possible selections and use (rand() % N) to select one by using this class.
/// 
/// note  : 1.	https://en.wikipedia.org/wiki/Reservoir_sampling
////////////////////////////////

#ifndef SZX_CPPUTILIBS_RAND_SAMPLE_H
#define SZX_CPPUTILIBS_RAND_SAMPLE_H


#include <random>

#include "Random.h"


namespace szx {

class Sampling1 { // pick single item.
public:
    enum StartCount { NoPresetElement = 0, WithPresetElement = 1 };

    Sampling1(Random &randomNumberGenerator, int startCount = StartCount::WithPresetElement)
        : rgen(randomNumberGenerator) {
        reset(startCount);
    }

    // start a new selection on another N elements.
    // sometimes the first element is pre-selected with the possibility of 1, 
    // so you can pass 1 in this condition to leave out a isPicked() call.
    void reset(int startCount = StartCount::WithPresetElement) { pickCount = startCount; }

    // call this for each of the N elements (N times in total) to judge 
    // whether each of them is selected. 
    // only the last returned "true" means that element is selected finally.
    bool isPicked() { return ((rgen() % (++pickCount)) == 0); }

    // if (newItem != minItem), return as their strict order. else select one
    // as the minimal in the sequence of isMinimal() calls randomly.
    template<typename T>
    bool isMinimal(const T &minItem, const T &newItem) {
        if (newItem > minItem) {
            return false;
        } else if (newItem == minItem) {
            return isPicked();
        } else {
            reset();
            return true;
        }
    }

protected:
    Random &rgen;
    int pickCount; // number of elements that have been considered.
};

// if the TargetNum is not known when compiling, use Sampling_Dynamic.
template<const int TargetNum>
class Sampling_Static {
public:
    Sampling_Static(Random &randomNumberGenerator) : rgen(randomNumberGenerator), pickCount(0) {}

    // return 0 for not picked.
    // return an integer i \in [1, TargetNum] if it is the i_th item in the picked set.
    int isPicked() {
        if ((++pickCount) <= TargetNum) {
            return pickCount;
        } else {
            int i = rgen.pick(pickCount) + 1;
            return (i <= TargetNum) ? i : 0;
        }
    }

    // return -1 for no need to replace any item.
    // return an integer i \in [0, TargetNum) as the index to be replaced in the picked set.
    int replaceIndex() {
        if (pickCount < TargetNum) {
            return pickCount++;
        } else {
            int i = rgen.pick(++pickCount);
            return (i < TargetNum) ? i : -1;
        }
    }

protected:
    Random &rgen;
    int pickCount;
};


// if the TargetNum is known when compiling, use Sampling_Static to speed up.
class Sampling_Dynamic {
public:
    Sampling_Dynamic(Random &randomNumberGenerator, int targetNumber)
        : rgen(randomNumberGenerator), targetNum(targetNumber), pickCount(0) {}

    // return 0 for not picked.
    // return an integer i \in [1, targetNum] if it is the i_th item in the picked set.
    int isPicked() {
        if ((++pickCount) <= targetNum) {
            return pickCount;
        } else {
            int i = rgen.pick(pickCount) + 1;
            return (i <= targetNum) ? i : 0;
        }
    }

    // return -1 for no need to replace any item.
    // return an integer i \in [0, targetNum) as the index to be replaced in the picked set.
    int replaceIndex() {
        if (pickCount < targetNum) {
            return pickCount++;
        } else {
            int i = rgen.pick(++pickCount);
            return (i < targetNum) ? i : -1;
        }
    }

    void reset() {
        pickCount = 0;
    }

protected:
    Random &rgen;
    int targetNum;
    int pickCount;
};


// count | 1 2 3 4 ...  k   k+1   k+2   k+3  ...  n
// ------|------------------------------------------
// index | 0 1 2 3 ... k-1   k    k+1   k+2  ... n-1
// prob. | 1 1 1 1 ...  1  k/k+1 k/k+2 k/k+3 ... k/n
using Sampling = Sampling_Dynamic;

}


#endif // SZX_CPPUTILIBS_RAND_SAMPLE_H
