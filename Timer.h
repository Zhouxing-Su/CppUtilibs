/**
*   usage : 1. record time in millisecond and record the related information into a vector.
*           2. get current date and time.
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
    static std::string getLocalTime();
    static std::string getLocalTimeNO();    // with numbers only

    // member methods ===================
    Timer();    // get the start time
    ~Timer();

    // clear all records and reset the start time
    void reset();
    // record current time and a message for the related event
    void record( LogLevel level = LogLevel::INFO, const std::string &msg = std::string() );
    // call record() and print the record just inserted
    void recordAndPrint( LogLevel level, const std::string &msg, std::ostream &os );
    // print all record
    void printAll( std::ostream &os ) const;
    // get duration from the first record to the last
    double getTotalDuration() const;
    // get duration from the record indexed start to end
    double getDuration( int start, int end ) const;

private:
    std::vector<TimeStamp> timestamps;
};


#endif