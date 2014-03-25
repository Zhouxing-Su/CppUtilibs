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