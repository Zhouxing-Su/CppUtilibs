/**
*   usage : call isSelected() method N times to randomly select 1 out of N elements with equiprobability.
*           this class is usefull when you can find out how many elements are there before enumerating them all,
*           you will not need to hold all posible selections and use (rand() % N) to select one by using this class.
*
*   note : 1.   for select(), the procedure is :
*               r is the indices of selected elements in original array,
*               o is the indices of non-selected elements in original array.
*               every time select one element's index and remove it from o.
*               r:| |           o:| 1 2 3 4 5 6 7 8 |
*               r:| 4 |         o:| 1 2 3 8 5 6 7 | 4
*               r:| 4 1 |       o:| 7 2 3 8 5 6 | 1 4
*               r:| 4 1 5 |     o:| 7 2 3 8 6 | 5 1 4
*/

#ifndef RAND_SELECT_H
#define RAND_SELECT_H


#include <cstdlib>
#include <ctime>
#include <vector>

#include "Random.h"

class RandSelect : Random
{
public:
    // reandomly select selectNum elements from totalNum elements and return their indices
    static std::vector<int> randSelect( int totalNum, int selectNum );


    // sometimes the first element is pre-selected with the posibility of 1,
    // so you can pass 2 into the constructor in this condition to leave out a isSelected() call.
    RandSelect( int startCount = 2 );
    ~RandSelect();

    // call this for each of the N elements (N times in total) to judge whether each of them is selected.
    bool isSelected();  // only the last returned "true" means that element is selected finally.
    // start a new selection on another N elements
    void reset( int startCount = 2 )
    {
        count = startCount;
    }

private:
    int count;
};



#endif