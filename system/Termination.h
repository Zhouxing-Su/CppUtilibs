/**
*   usage : 1.  provide interface about program termination control.
*
*   note :  1.
*/

#ifndef TERMINATION_H
#define TERMINATION_H


#include <string>


namespace szx
{

class Termination
{
public:
    // block current thread until correct code was read.
    static void waitForCode( const std::string &terminationCode );

    // block current thread until correct code was read, execute function object onTermination before exit.
    template<typename OnTermination>
    static void waitForCode( const std::string &terminationCode, OnTermination onTermination );

    // wait in a new thread without blocking current thread.
    static void waitForCodeAsync( const std::string &terminationCode );

    // wait in a new thread without blocking current thread, execute function object onTermination before exit.
    template<typename OnTermination>
    static void waitForCodeAsync( const std::string &terminationCode, OnTermination onTermination );
};

}


#endif
