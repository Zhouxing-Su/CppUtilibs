////////////////////////////////
/// usage : 1.	generate numbers greater than or less than init value by turns.
///         2.  on each side, the difference between the generated number and init value increase a given stepValue step by step.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_OSCILLATOR_H
#define SZX_CPPUTILIBS_OSCILLATOR_H


namespace szx {

template<typename T>
class Oscillator {
public:
    enum Direction { Up = -1, Down = 1 };

    Oscillator(T minValue, T maxValue, T initValue, Direction initDirection, T stepValue = 1)
        : lb(minValue), ub(maxValue), step(stepValue), cur(initValue), dir(initDirection), amp(0), invertor(-1) {}

    T operator++() {
        next();
        if ((cur < lb) || (ub < cur)) { // first out of bound, disable one direction.
            if (step == 0) { return (cur = end()); } // second out of bound, end oscilation.
            next();
            invertor = 1;
            amp = step;
            step = 0;
        }
        return cur;
    }
    T operator++(int) {
        T prev = cur;
        operator++();
        return prev;
    }

    operator T() { return cur; }

    T end() { return (lb - 1); }

protected:
    void next() { cur += ((amp += step) * (dir *= invertor)); }

    T lb; // lower bound.
    T ub; // upper bound.
    T step; // diff delta.
    T cur; // current value.
    T dir; // greater or less than init value.
    T amp; // diff between current value and next value.
    T invertor; // control flipping to two sides.
};

}


#endif // SZX_CPPUTILIBS_OSCILLATOR_H
