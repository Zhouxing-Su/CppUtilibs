/**
*   usage : call isSelected() method N times to randomly select 1 out of N elements with equiprobability.
*           this class is usefull when you can find out how many elements are there before enumerating them all,
*           you will not need to hold all posible selections and use (rand() % N) to select one by using this class.

(example code)
====================================================================
#include <iostream>
#include "RandSelect.h"

using namespace std;

int main( int argc, char **argv )
{
int result = 0;
for (int k = 0; k < 10; k++) {
RandSelect rs;
for (int i = 0; i < 10; i++) {
if (rs.isSelected()) {
result = i;
}
}
cout << result << endl;
}
return 0;
}
====================================================================
*/

#ifndef RAND_SELECT_H


#include <cstdlib>
#include <ctime>

class RandSelect
{
public:
    RandSelect();
    ~RandSelect();

    // call this for each of the N elements (N times in total) to judge whether each of them is selected.
    bool isSelected();  // only the last returned "true" means that element is selected finally.
    // start a new selection on another N elements
    void reset()
    {
        count = 1;
    }

private:
    int count;

    static int seed;    // this is used for calling srand() only once and should not be used elsewhere !
};



#define RAND_SELECT_H
#endif