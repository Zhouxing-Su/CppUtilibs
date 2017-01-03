////////////////////////////////
/// usage : 1.	an online algorithm for sampling targetNum items from a data stream.
/// 
/// note  : 1.	https://en.wikipedia.org/wiki/Reservoir_sampling
////////////////////////////////

#ifndef SZX_CPPUTILIBS_RAND_SAMPLE_H
#define SZX_CPPUTILIBS_RAND_SAMPLE_H


#include <random>


namespace szx {

class RandSample {
public:
    RandSample(std::mt19937 &randomNumberGenerator, int targetNumber)
        : rgen(randomNumberGenerator), targetNum(targetNumber), pickCount(0) {}

    int isPicked() {
        if ((++pickCount) <= targetNum) {
            return pickCount;
        } else {
            int i = (rgen() % pickCount) + 1;
            return (i <= targetNum) ? i : 0;
        }
    }

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
