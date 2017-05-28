////////////////////////////////
/// usage : 1.	a simple wrapper for a random number generator.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_RANDOM_H
#define SZX_CPPUTILIBS_RANDOM_H


#include <random>

#include <ctime>


namespace szx {

class Random {
public:
    using Generator = std::mt19937;


    Random(int seed) : rgen(seed) {}
    Random() : rgen(generateSeed()) {}


    static int generateSeed() {
        return static_cast<int>(std::time(nullptr) + std::clock());
    }

    Generator::result_type operator()() { return rgen(); }

    // pick with probability of (numerator / denominator).
    bool isPicked(unsigned numerator, unsigned denominator) {
        return ((rgen() % denominator) < numerator);
    }

    // pick from [min, max).
    int pick(int min, int max) {
        return ((rgen() % (max - min)) + min);
    }
    // pick from [0, max).
    int pick(int max) {
        return (rgen() % max);
    }

protected:
    Generator rgen;
};

}


#endif // SZX_CPPUTILIBS_RANDOM_H