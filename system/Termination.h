/**
*   usage : 1.  provide interface about program termination control.
*
*   note :  1.
*/

#ifndef TERMINATION_H
#define TERMINATION_H


#include <iostream>
#include <string>
#include <thread>


namespace szx
{
    class Termination
    {
    public:
        // block current thread until correct code was read
        static void waitTerminationCode( const std::string &terminationCode );
        // wait in a new thread without blocking current thread
        static void peekTerminationCode( const std::string &terminationCode );
    };
}


#endif
