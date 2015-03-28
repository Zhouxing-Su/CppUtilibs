#include "TimeStamp.h"


using namespace std;


namespace szx
{
    const char TimeStamp::logLevelMsg[3][12] = { "[ERROR] ", "[WARNING] ", "[INFO] " };

    TimeStamp::Stamp::Stamp( std::clock_t t, const std::string &m )
        : time( t ), msg( m )
    {
    }

    TimeStamp::Stamp::~Stamp()
    {
    }




    string TimeStamp::getLocalTime()
    {
        char buf[64];

        time_t t = time( NULL );
        tm *date = localtime( &t );
        strftime( buf, 64, "%Y-%m-%d %a %H:%M:%S", date );

        return string( buf );
    }

    string TimeStamp::getLocalTimeNO()
    {
        char buf[64];

        time_t t = time( NULL );
        tm *date = localtime( &t );
        strftime( buf, 64, "%Y%m%d%a%H%M%S", date );

        return string( buf );
    }

    TimeStamp::TimeStamp() : timestamps( 1, Stamp( clock(), string() ) )
    {
    }


    TimeStamp::~TimeStamp()
    {
    }

    void TimeStamp::reset()
    {
        timestamps.clear();
        timestamps.push_back( Stamp( clock(), string() ) );
    }

    void TimeStamp::record( LogLevel level, const std::string &msg )
    {
        timestamps.push_back( Stamp( clock(), (logLevelMsg[level] + msg) ) );
    }

    void TimeStamp::recordAndPrint( LogLevel level, const std::string &msg, std::ostream &os )
    {
        clock_t lastTime = timestamps.back().time;
        record( level, msg );
        os << "\n\t[duration: " << ((timestamps.back().time - lastTime) / static_cast<double>(CLOCKS_PER_SEC)) << "s]\n";
        os << "\n[time: " << ((timestamps.back().time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC)) << "s] " << timestamps.back().msg << endl;
    }

    void TimeStamp::printAll( ostream &os ) const
    {
        for (vector<Stamp>::const_iterator iter = (timestamps.begin() + 1); iter != timestamps.end(); iter++) {
            os << "\n\t[duration: " << ((iter->time - (iter - 1)->time) / static_cast<double>(CLOCKS_PER_SEC)) << "s]\n";
            os << "\n[time: " << ((iter->time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC)) << "s] " << (iter->msg) << endl;
        }
    }

    double TimeStamp::getTotalDuration() const
    {
        return ((timestamps.back().time - timestamps.front().time) / static_cast<double>(CLOCKS_PER_SEC));
    }

    double TimeStamp::getDuration( int start, int end ) const
    {
        return ((timestamps[end].time - timestamps[start].time) / static_cast<double>(CLOCKS_PER_SEC));
    }
}