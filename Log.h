/**
*   usage : 1.  show debug infomation when NDEBUG macro is not defined.
*               compatible with visual studio when switching build target between Debug and Release,
*               or you can toggle NDEBUG macro below to switch it.
*           2.  show log.
*
*   note :  1.  the T_OUTPUT must get an "operator<<" to print information about it.
*/

#ifndef LOG_H


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>


// in main(), call setHandler() to register the crash handler
// in every block you want to trace, declare local variable by "Trace t(SomeInfo)" to trace.
// explicitly call printStack() or automatically print stack after crash.
template <class T_OUTPUT = const char *>
class Trace
{
public:
    static void setHandler( std::terminate_handler uth = printStack )
    {
        userTerminateHandler = uth;
        set_terminate( terminateHandler );
    }

    static void printStack( std::ostream &os = std::cout )
    {
        for (int i = callStack.size() - 1; i >= 0; i--) {
            os << callStack[i] << std::endl;
        }
    }

    Trace( const T_OUTPUT &info )
    {
        callStack.push_back( info );
    }

    ~Trace()
    {
        callStack.pop_back();
    }

private:
    static void terminateHandler()
    {
        userTerminateHandler();
        std::abort();
    }

    static std::terminate_handler userTerminateHandler;

    static std::vector<T_OUTPUT> callStack;
};





template <class T_OUTPUT = const char *>
class Log
{
public:
    static void write( const T_OUTPUT &outputObject, bool condition = true, std::ostream &os = std::cout )
    {
        if (condition) {
            os << outputObject;
        }
    }

    static void writeln( const T_OUTPUT &outputObject, bool condition = true, std::ostream &os = std::cout )
    {
        if (condition) {
            os << outputObject << std::endl;
        }
    }
};


// toggle comment on this macro to switch whether log or not
//#define NDEBUG
template <class T_OUTPUT = const char *>
class Debug
{
public:
    static void write( const T_OUTPUT &outputObject, bool condition = true, std::ostream &os = std::cout )
    {
#ifndef NDEBUG
        if (condition) {
            os << outputObject;
        }
#endif
    }

    static void writeln( const T_OUTPUT &outputObject, bool condition = true, std::ostream &os = std::cout )
    {
#ifndef NDEBUG
        if (condition) {
            os << outputObject << std::endl;
        }
#endif
    }
};

#define LOG_H
#endif