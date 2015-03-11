/**
*   usage : 1. portable simplify directory operation.
*
*   note :  1. not efficient and secure.
*/

#ifndef DIRECTORY_H
#define DIRECTORY_H


#include <cstdlib>
#include <string>


namespace szx
{
    class Directory
    {
    public:
        static const std::string mkdir_win32cmd;
        static const std::string mkdir_unixshell;

        static void MakeSureDirExist( const std::string &dir );

    private:

    };
}


#endif