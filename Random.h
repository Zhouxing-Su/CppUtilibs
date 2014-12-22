/**
*   usage : singleton class with function object rand.
*           other class using rand() should inherit this class to ensure that
*           the global random seed is only initialized once.
*           1. setSeed() for release build, getSeed() for logging. this is the default behavior.
*           2. setSeq() for cross-platform debuging when you need same random sequence.
*              ( rand() in windows and linux will produce different sequence even they got the same seed )
*
*   problem :
*           1.
*/

#ifndef RANDOM_H

#include <ctime>
#include <cstdlib>
#include <vector>

class Random
{
public:
    typedef int( *RandGenerator )();


    static unsigned int genSeed()
    {
        return static_cast<unsigned int>(time( NULL ) + clock());
    }

    static int setSeed( unsigned int s = genSeed() )
    {
        rand.seed = s;
        rand.gen = std::rand;
        std::srand( s );
        // return int for initializing random seed in default behavior
        return static_cast<int>(s);
    }

    static unsigned int getSeed()
    {
        return rand.seed;
    }

    static void setSeq( const std::vector<int> &seq )
    {
        rand.randSeq = seq;
        rand.seqPointer = 0;
        rand.gen = getIntFromRandSeq;
    }

    static const std::vector<int>& getSeq()
    {
        return rand.randSeq;
    }

    int operator()()
    {
        return gen();
    }


    static Random rand;

protected:
    Random( int i = 0 ) {} // for inherited classed and static rand init

private:
    static int getIntFromRandSeq()
    {
        rand.seqPointer %= rand.randSeq.size();
        return rand.randSeq[rand.seqPointer++];
    }

    RandGenerator gen;

    unsigned int seed;

    std::vector<int> randSeq;
    int seqPointer;

private:    // forbid to be used
    Random( const Random& r ) {}
    Random& operator=(const Random& r) { return *this; }

};

#define RANDOM_H
#endif