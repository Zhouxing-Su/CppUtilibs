////////////////////////////////
/// usage : 1.	coarse-grained date and time calculation.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_DATE_TIME_H
#define SZX_CPPUTILIBS_DATE_TIME_H


#include <chrono>
#include <ctime>


// [off] use chrono instead of ctime.
#define UTILITY_DATE_TIME_CPP_STYLE  0


namespace szx {

class DateTime {
public:
    static constexpr int MinutesPerDay = 60 * 24;
    static constexpr int MinutesPerHour = 60;
    static constexpr int SecondsPerMinute = 60;


    // TODO[szx][8]: use different names for the arguments.
    DateTime(int year = 0, int month = 0, int day = 0, int hour = 0, int minute = 0, int second = 0)
        : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}
    DateTime(tm &t) : DateTime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec) {}
    DateTime(time_t t) : DateTime(*std::localtime(&t)) {}

    // get an inconsistent tm struct which requires std::mktime() to revise.
    operator std::tm() const {
        std::tm datetime;

        datetime.tm_year = year - 1900;
        datetime.tm_mon = month - 1;
        datetime.tm_mday = day;
        datetime.tm_hour = hour;
        datetime.tm_min = minute;
        datetime.tm_sec = second;

        datetime.tm_isdst = -1;

        //datetime.tm_wday = 0; // ignored by mktime().
        //datetime.tm_yday = 0; // ignored by mktime().

        return datetime;
    }

    operator time_t() const {
        std::tm t = static_cast<std::tm>(*this);
        return std::mktime(&t);
    }

    friend DateTime operator+(const DateTime &dateTime, time_t second) {
        time_t t = static_cast<time_t>(dateTime);
        t += second;
        return DateTime(t);
    }
    friend DateTime operator-(const DateTime &dateTime, time_t second) { return (dateTime + (-second)); }

    friend std::ostream& operator<<(std::ostream &os, DateTime &dateTime) {
        os << dateTime.year << '-'
            << std::setw(2) << std::setfill('0') << dateTime.month << '-'
            << std::setw(2) << std::setfill('0') << dateTime.day << ' '
            << std::setw(2) << std::setfill('0') << dateTime.hour << ':'
            << std::setw(2) << std::setfill('0') << dateTime.minute << ':'
            << std::setw(2) << std::setfill('0') << dateTime.second;
    }

    static double durationInSecond(const DateTime &start, const DateTime &end) {
        #if UTILITY_DATE_TIME_CPP_STYLE
        using Clock = std::chrono::system_clock;
        using TimePoint = Clock::time_point;
        TimePoint tpl = Clock::from_time_t(static_cast<time_t>(start));
        TimePoint tpr = Clock::from_time_t(static_cast<time_t>(end));
        return std::chrono::duration_cast<std::chrono::seconds>(tpl - tpr).count();
        #else
        return std::difftime(static_cast<time_t>(end), static_cast<time_t>(start));
        #endif // UTILITY_DATE_TIME_CPP_STYLE
    };


    int year; // year since Common Era.
    int month; // months in year.
    int day; // days in month.
    int hour; // hours in day.
    int minute; // minutes in hour.
    int second; // seconds in minute.
};

}


#endif // SZX_CPPUTILIBS_DATE_TIME_H
