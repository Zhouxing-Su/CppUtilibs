#include "Timer.h"

using namespace std;

const char Timer::logLevelMsg[3][12] = { "[ERROR] ", "[WARNING] ", "[INFO] " };

Timer::TimeStamp::TimeStamp( std::clock_t t, const std::string &m )
: time( t ), msg( m )
{
}

Timer::TimeStamp::~TimeStamp()
{
}




string Timer::getLocalTime()
{
    char buf[64];

    time_t t = time( NULL );
    tm *date = localtime( &t );
    strftime( buf, 64, "%Y-%m-%d %a %H:%M:%S", date );

    return string( buf );
}

string Timer::getLocalTimeNO( )
{
    char buf[64];

    time_t t = time( NULL );
    tm *date = localtime( &t );
    strftime( buf, 64, "%Y%m%d%a%H%M%S", date );

    return string( buf );
}

Timer::Timer() : timestamps( 1, TimeStamp( clock(), string() ) )
{
}


Timer::~Timer()
{
}

void Timer::reset()
{
    timestamps.clear();
    timestamps.push_back( TimeStamp( clock(), string() ) );
}

void Timer::record( LogLevel level, const std::string &msg )
{
    timestamps.push_back( TimeStamp( clock(), (logLevelMsg[level] + msg) ) );
}

void Timer::recordAndPrint( LogLevel level, const std::string &msg, std::ostream &os )
{
    clock_t lastTime = timestamps.back().time;
    record( level, msg );
    os << "\n\t[duration: " << ((timestamps.back().time - lastTime) / static_cast<double>(CLOCKS_PER_SEC)) << "s]\n";
    os << "\n[time: " << ((timestamps.back().time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC)) << "s] " << timestamps.back().msg << endl;
}

void Timer::printAll( ostream &os ) const
{
    for (vector<TimeStamp>::const_iterator iter = (timestamps.begin() + 1); iter != timestamps.end(); iter++) {
        os << "\n\t[duration: " << ((iter->time - (iter - 1)->time) / static_cast<double>(CLOCKS_PER_SEC)) << "s]\n";
        os << "\n[time: " << ((iter->time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC)) << "s] " << (iter->msg) << endl;
    }
}

double Timer::getTotalDuration() const
{
    return ((timestamps.back().time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC));
}

double Timer::getDuration( int start, int end ) const
{
    return ((timestamps[end].time - timestamps[start].time) / static_cast<double>(CLOCKS_PER_SEC));
}