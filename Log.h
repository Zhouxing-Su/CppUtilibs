/**
*   usage : show log when NDEBUG macro is not defined.
*           compatible with visual studio when switching build target between Debug and Release,
*           or you can toggle NDEBUG macro below to switch it.
*
*   note :  1.  the T_OUTPUT must get an "operator<<" to print information about it.
*/

#ifndef LOG_H

#include <iostream>

// toggle comment on this macro to switch whether log or not
//#define NDEBUG

template <class T_OUTPUT = const char *>
class Log
{
public:
    static void write( const T_OUTPUT &outputObject, bool condition = true, std::ostream &os = std::cout )
    {
        #ifndef NDEBUG
        if( condition ){
            os << outputObject << std::endl;
        }
        #endif
    }
};

#define LOG_H
#endif