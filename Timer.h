/**
*   usage:  (example code)
====================================================================
#include <iostream>
#include "Timer.h"

using namespace std;

void A() {}   // some procedure written by you.
void B() {}   // some procedure written by you.

int main( int argc, char **argv )
{
    Timer timer;

    timer.record( Timer::INFO, string( "A start" ) );
    A();    // some procedure writen by you.
    timer.record( Timer::INFO, string( "A end" ) );
    timer.printAll( cout );

    timer.reset();
    timer.recordAndPrint( Timer::INFO, string( "B start" ), cout );
    B();
    timer.recordAndPrint( Timer::INFO, string( "B end" ), cout );

    return 0;
}
====================================================================
*/

#ifndef TIMER_H
#define TIMER_H


#include <ctime>
#include <vector>
#include <string>
#include <iostream>



class Timer
{
public:
    // type definitions ===================
    struct TimeStamp
    {
    public:
        TimeStamp( std::clock_t time, const std::string &msg );
        ~TimeStamp();

        const std::clock_t time;
        const std::string msg;
    };

    enum LogLevel { ERROR = 0, WARNING, INFO };

    // static constant ===================
    static const char logLevelMsg[3][12];

    // member constant ===================

    // static methods ===================

    // member methods ===================
    Timer();    // get the start time
    ~Timer();

    // clear all records and reset the start time
    void reset();
    // record current time and a message for the related event
    void record( LogLevel level, const std::string &msg );
    // call record() and print the record just inserted
    void recordAndPrint( LogLevel level, const std::string &msg, std::ostream &os );
    // print all record
    void printAll( std::ostream &os ) const;

private:
    std::vector<TimeStamp> timestamps;
};


#endif