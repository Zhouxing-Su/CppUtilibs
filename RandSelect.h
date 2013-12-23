/**
*   usage : call isSelected() method N times to randomly select 1 out of N elements with equiprobability.
*           this class is usefull when you can find out how many elements are there before enumerating them all,
*           you will not need to hold all posible selections and use (rand() % N) to select one by using this class.
*/

#ifndef RAND_SELECT_H
#define RAND_SELECT_H


#include <cstdlib>
#include <ctime>

class RandSelect
{
public:
    // sometimes the first element is pre-selected with the posibility of 1,
    // so you can pass 2 into the constructor in this condition to leave out a isSelected() call.
    RandSelect(int startCount = 1);
    ~RandSelect();

    // call this for each of the N elements (N times in total) to judge whether each of them is selected.
    bool isSelected();  // only the last returned "true" means that element is selected finally.
    // start a new selection on another N elements
    void reset( int startCount = 1 )
    {
        count = startCount;
    }

private:
    int count;

    static int seed;    // this is used for calling srand() only once and should not be used elsewhere !
};



#endif