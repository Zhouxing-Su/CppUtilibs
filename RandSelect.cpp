#include "RandSelect.h"

using namespace std;

int RandSelect::seed = (srand( static_cast<unsigned>(time( NULL )) ), 0);

RandSelect::RandSelect() : count( 1 )
{
}


RandSelect::~RandSelect()
{
}

bool RandSelect::isSelected()
{
    return ((rand() % (count++)) == 0);
}