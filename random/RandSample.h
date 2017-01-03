////////////////////////////////
/// usage : 1.	an online algorithm for sampling targetNum items from a data stream.
/// 
/// note  : 1.	https://en.wikipedia.org/wiki/Reservoir_sampling
////////////////////////////////

#ifndef SZX_CPPUTILIBS_RAND_SAMPLE_H
#define SZX_CPPUTILIBS_RAND_SAMPLE_H


#include <random>


namespace szx {

// count | 1 2 3 4 ...  k   k+1   k+2   k+3  ...  n
// ------|------------------------------------------
// index | 0 1 2 3 ... k-1   k    k+1   k+2  ... n-1
// prob. | 1 1 1 1 ...  1  k/k+1 k/k+2 k/k+3 ... k/n
class RandSample {
public:
    RandSample(std::mt19937 &randomNumberGenerator, int targetNumber)
        : rgen(randomNumberGenerator), targetNum(targetNumber), pickCount(0) {}

    // return 0 for not picked.
    // return an integer i \in [1, targetNum] if it is the i_th item in the picked set.
    int isPicked() {
        if ((++pickCount) <= targetNum) {
            return pickCount;
        } else {
            int i = (rgen() % pickCount) + 1;
            return (i <= targetNum) ? i : 0;
        }
    }

    // return -1 for no need to replace any item.
    // return an integer i \in [0, targetNum) as the index to be replaced in the picked set.
    int replaceIndex() {
        if (pickCount < targetNum) {
            return pickCount++;
        } else {
            int i = (rgen() % (++pickCount));
            return (i < targetNum) ? i : -1;
        }
    }

private:
    std::mt19937 &rgen;
    int pickCount;
    int targetNum;
};

}


#endif // SZX_CPPUTILIBS_RAND_SAMPLE_H
