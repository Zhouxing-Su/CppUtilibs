/**
*   usage : 1.  in main(), call setHandler() to register the crash handler.
*               in every block you want to trace, declare local variable by "Trace t(SomeInfo)" to trace.
*               explicitly call dumpCallStack() or automatically save stack to crashdump.txt after crash.
*               also, you can write your own crash handler, and call dumpCallStack() in it.
*
*   note :  1.
*/

#ifndef TRACE_H
#define TRACE_H


#ifdef WIN32
/*
* Win32FaultHandler
* Install and implement a Windows Exception handler that will be called,
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS


#include <Windows.h>

#else   // unix like systems
/*
* SignalHandler
* Install and implement a unix/posix signal handler that will be called,
* if the OS determines a programm fault.
*/

#include <signal.h>
#include <ucontext.h>

#endif  //WIN32


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>


namespace szx
{
    class Trace
    {
    public:
        static const std::string DUMP_FILE_NAME;

#ifdef WIN32
        typedef LONG CrashHandler( struct _EXCEPTION_POINTERS *ExInfo );

        static CrashHandler defaultHandler_Win32;

        static void setHandler( CrashHandler userCrashHandler = defaultHandler_Win32 )
        {
            SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)userCrashHandler );
        }

#else   // unix like systems
        typedef void CrashHandler( int signum, siginfo_t* info, void*ptr );

        static CrashHandler defaultHandler_Unix;

        static void setHandler( CrashHandler userCrashHandler = defaultHandler_Unix )
        {
            struct sigaction action;
            memset( &action, 0, sizeof( action ) );
            action.sa_sigaction = userCrashHandler;
            action.sa_flags = SA_SIGINFO;
            if (sigaction( SIGSEGV, &action, NULL ) < 0) {
                perror( "sigaction" );
            }
        }

#endif  // WIN32

        static void dumpCallStack( std::ostream &dumpFile );
        static void dumpCallStack( const std::string &dumpFileName = DUMP_FILE_NAME );

        Trace( const std::string &info )
        {
            callStack.push_back( info );
        }

        ~Trace()
        {
            callStack.pop_back();
        }

    private:
        static std::vector<const std::string> callStack;
    };
}


#endif