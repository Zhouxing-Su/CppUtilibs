/**
*   usage : 1. call isTimeOut() regularly to check if time is up.
*
*   note :  1.
*/

#ifndef TIMER_H
#define TIMER_H


#include <ctime>


namespace szx
{
    class Timer
    {
    public:
        Timer( clock_t duration, clock_t startTime = clock() )
            :endTime( startTime + duration )
        {
        }

        bool isTimeOut() const
        {
            return (clock() >= endTime);
        }

        clock_t restTime() const
        {
            return (endTime - clock());
        }

    private:
        clock_t endTime;
    };
}


#endif