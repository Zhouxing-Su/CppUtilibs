/**
*   usage : 1. command line utility.
*
*   note :  1.
*/

#ifndef SYSTEM_H
#define SYSTEM_H


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
            extern const std::string redirectStdout_app;
            extern const std::string redirectStderr_app;
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

    std::string quote( const std::string &s );
}


#endif