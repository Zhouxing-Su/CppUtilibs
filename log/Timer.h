/**
*   usage : 1. call isTimeOut() regularly to check if time is up.
*
*   note :  1.
*/

#ifndef TIMER_H
#define TIMER_H


#ifdef _WIN32   // Windows
#include <Windows.h>
#else   // Posix/Linux
#include <sys/resource.h>
#include <sys/times.h>
#endif

#include <chrono>
#include <string>
#include <ctime>


namespace szx
{

class Timer
{
public:
    static const int MICROSECOND_PER_SECOND = 1000000;


    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::milliseconds Duration;
    typedef std::chrono::steady_clock::time_point TimePoint;

    typedef unsigned long long Microsecond;


    static Microsecond getCPUtime()
    {
#ifdef _WIN32   // Windows
        FILETIME creationTime, exitTime, kernelTime, userTime;
        GetProcessTimes( GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime );
        return ((static_cast<unsigned long long>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime) / 10;
#else   // Posix/Linux
        rusage r;
        getrusage( RUSAGE_SELF, &r );
        return r.ru_utime.tv_sec * MICROSECOND_PER_SECOND + r.ru_utime.tv_usec;
#endif
    }

    static std::string getLocalTime()
    {
        char buf[64];

        time_t t = time( NULL );
        tm *date = localtime( &t );
        strftime( buf, 64, "%Y-%m-%d %a %H:%M:%S", date );

        return std::string( buf );
    }
    static std::string getLocalTime_NumberOnly()
    {
        char buf[64];

        time_t t = time( NULL );
        tm *date = localtime( &t );
        strftime( buf, 64, "%Y%m%d%a%H%M%S", date );

        return std::string( buf );
    }

    Timer( Duration duration, TimePoint startTime = Clock::now() )
        : endTime( startTime + duration )
    {
    }

    bool isTimeOut() const
    {
        return (Clock::now() >= endTime);
    }

    Duration restTime() const
    {
        return std::chrono::duration_cast<Duration>(
            endTime - Clock::now());
    }

private:
    TimePoint endTime;
};

}


#endif