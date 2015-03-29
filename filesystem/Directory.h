/**
*   usage : 1. portable simplify directory operation.
*
*   note :  1. not efficient and secure.
*           2. add space on the border of commands.
*/

#ifndef DIRECTORY_H
#define DIRECTORY_H


#include <cstdlib>
#include <string>

#include "../system/system.h"


namespace szx
{
    class Directory
    {
    public:
        static void makeSureDirExist( const std::string &dir );

    private:

    };
}


#endif