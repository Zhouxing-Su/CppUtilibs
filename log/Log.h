////////////////////////////////
/// usage : 1.	switchable log printer.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_LOG_H
#define SZX_CPPUTILIBS_LOG_H


#include <iostream>


namespace szx {

class Log {
public:
    using Manipulator = std::ostream& (*)(std::ostream&);


    enum Level {
        On,
        Off, // the default state if not specified.

        Fatal = On,
        Error = On,
        Warning = On,
        Debug = On,
        Info, // = Off.
    };


    static bool isTurnedOn(int level) { return (level == On); }
    static bool isTurnedOff(int level) { return !isTurnedOn(level); }


    Log(int logLevel, std::ostream &logFile) : level(logLevel), os(logFile) {}
    Log(int logLevel) : Log(logLevel, std::cerr) {}


    template<typename T>
    Log& operator<<(const T &obj) {
        if (isTurnedOn(level)) { os << obj; }
        return *this;
    }
    Log& operator<<(Manipulator manip) {
        if (isTurnedOn(level)) { os << manip; }
        return *this;
    }

protected:
    int level;
    std::ostream &os;
};

}


#endif // SZX_CPPUTILIBS_LOG_H