#include "RandSelect.h"

using namespace std;


RandSelect::RandSelect( int startCount ) : count( startCount )
{
}


RandSelect::~RandSelect()
{
}

bool RandSelect::isSelected()
{
    return ((rand() % (count++)) == 0);
}

vector<int> RandSelect::randSelect( int totalNum, int selectNum )
{
    vector<int> rIndex( selectNum );
    vector<int> originIndex( totalNum );

    for (int i = 0; i < totalNum; i++) {
        originIndex[i] = i;
    }

    while (--selectNum >= 0) {
        int s = rand() % totalNum;
        rIndex[selectNum] = originIndex[s];
        originIndex[s] = originIndex[--totalNum];
    }

    return rIndex;
}