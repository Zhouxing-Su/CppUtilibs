/**
*   usage : 1.  define auto-incremental counter. can be used with std::generate or std::fill
*
*   note :  1.  T must be a numeric type and implement operator+=().
*           2.  return const T& or T& in operater() ?
*/

#ifndef COUNTER_H
#define COUNTER_H


namespace szx
{
    template <typename T = int>
    class Counter
    {
    public:
        Counter( const T &start = -1, const T &stepNum = 1 ) : count( start ), step( stepNum ) {}

        T operator()()
        {
            return (count += step);
        }

    private:
        T count;
        T step;
    };

    template<typename T = int>
    class LoopCounter : public Counter < T >
    {
    public:
        LoopCounter( const T &modular, const T &start = -1, const T&stepNum = 1 )
            : Counter( start, stepNum ), mod( modular )
        {
        }

        T operator()()
        {
            return (count += step) %= mod;
        }

    private:
        T mod;
    };
}


#endif