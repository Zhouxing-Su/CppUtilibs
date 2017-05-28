////////////////////////////////
/// usage : 1.	a timer with timeout check and duration calculation.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_TIMER_H
#define SZX_CPPUTILIBS_TIMER_H


#include <chrono>
#include <string>
#include <ctime>


// [on] use chrono instead of ctime.
#define SZX_CPPUTILIBS_TIMER_CPP_STYLE  1
// [off] enable functions for getting cpu time.
#define SZX_CPPUTILIBS_TIMER_GET_CPU_TIME  0

#if SZX_CPPUTILIBS_TIMER_GET_CPU_TIME
#ifdef _WIN32   // Windows
#include <Windows.h>
#else   // Posix/Linux
#include <sys/resource.h>
#include <sys/times.h>
#endif
#endif // SZX_CPPUTILIBS_TIMER_GET_CPU_TIME


namespace szx {

class Timer {
public:
    #if SZX_CPPUTILIBS_TIMER_CPP_STYLE
    using Millisecond = std::chrono::milliseconds;
    using TimePoint = std::chrono::steady_clock::time_point;
    using Clock = std::chrono::steady_clock;
    #else
    using Millisecond = int;
    using TimePoint = int;
    struct Clock {
        static TimePoint now() { return clock(); }
    };
    #endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE


    static constexpr double MillisecondsPerSecond = 1000;
    static constexpr double ClocksPerSecond = CLOCKS_PER_SEC;
    static constexpr int ClocksPerMillisecond = static_cast<int>(ClocksPerSecond / MillisecondsPerSecond);


    #if SZX_CPPUTILIBS_TIMER_CPP_STYLE
    Timer(const Millisecond &duration, const TimePoint &st = Clock::now())
        : startTime(st), endTime(startTime + duration) {}
    #else
    Timer(const Millisecond &duration, const TimePoint &st = Clock::now())
        : startTime(st), endTime(startTime + duration * ClocksPerMillisecond) {}
    #endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE


    static Millisecond durationInMillisecond(const TimePoint &start, const TimePoint &end) {
        #if SZX_CPPUTILIBS_TIMER_CPP_STYLE
        return std::chrono::duration_cast<Millisecond>(end - start);
        #else
        return (end - start) / ClocksPerMillisecond;
        #endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE
    }

    static double durationInSecond(const TimePoint &start, const TimePoint &end) {
        #if SZX_CPPUTILIBS_TIMER_CPP_STYLE
        return std::chrono::duration_cast<Millisecond>(end - start).count() / MillisecondsPerSecond;
        #else
        return (end - start) / ClocksPerSecond;
        #endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE
    }

    static Millisecond toMillisecond(double second) {
        #if SZX_CPPUTILIBS_TIMER_CPP_STYLE
        return Millisecond(static_cast<int>(second * MillisecondsPerSecond));
        #else
        return static_cast<Millisecond>(second * MillisecondsPerSecond);
        #endif // SZX_CPPUTILIBS_TIMER_CPP_STYLE
    }

    // there is no need to free the pointer. the format of the format string is 
    // the same as std::strftime() in http://en.cppreference.com/w/cpp/chrono/c/strftime.
    static const char* getLocalTime(const char *format = "%Y-%m-%d(%a)%H:%M:%S") {
        static constexpr int DateBufSize = 64;
        static char buf[DateBufSize];
        time_t t = time(NULL);
        tm *date = localtime(&t);
        strftime(buf, DateBufSize, format, date);
        return buf;
    }

    #if SZX_CPPUTILIBS_TIMER_GET_CPU_TIME
    using Microsecond = unsigned long long;
    static constexpr Microsecond MicrosecondsPerSecond = 1000000;

    static Microsecond getCPUtime() {
        #ifdef _WIN32 // Windows
        FILETIME creationTime, exitTime, kernelTime, userTime;
        GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime);
        return ((static_cast<Microsecond>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime) / 10;
        #else // Posix/Linux
        rusage r;
        getrusage(RUSAGE_SELF, &r);
        return r.ru_utime.tv_sec * MicrosecondsPerSecond + r.ru_utime.tv_usec;
        #endif
    }
    #endif // SZX_CPPUTILIBS_TIMER_GET_CPU_TIME


    bool isTimeOut() const {
        return (Clock::now() > endTime);
    }

    Millisecond restMilliseconds() const {
        return durationInMillisecond(Clock::now(), endTime);
    }

    double restSeconds() const {
        return durationInSecond(Clock::now(), endTime);
    }

    Millisecond elapsedMilliseconds() const {
        return durationInMillisecond(startTime, Clock::now());
    }

    double elapsedSeconds() const {
        return durationInSecond(startTime, Clock::now());
    }

    const TimePoint& getStartTime() const { return startTime; }
    const TimePoint& getEndTime() const { return endTime; }

protected:
    TimePoint startTime;
    TimePoint endTime;
};

}


#endif // SZX_CPPUTILIBS_TIMER_H