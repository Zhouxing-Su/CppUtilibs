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


namespace szx
{
    namespace cmd
    {
        namespace common
        {
            extern const std::string redirectStdin;
            extern const std::string redirectStdout;
            extern const std::string redirectStderr;
        }

        namespace win32
        {
            extern const std::string mkdir;
            extern const std::string nullDev;
        }

        namespace unix
        {
            extern const std::string mkdir;
            extern const std::string nullDev;
        }
    }

    class Directory
    {
    public:
        static std::string quote( const std::string &s )
        {
            return ('\"' + s + '\"');
        }
        static void makeSureDirExist( const std::string &dir );

    private:

    };
}


#endif