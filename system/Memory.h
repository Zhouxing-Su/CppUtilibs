////////////////////////////////
/// usage : 1.	query memory size and usage.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_H
#define SZX_CPPUTILIBS_H


#include <iostream>
#include <iomanip>

#include "System.h"


namespace szx {

class Memory {
public:
    using Unit = long long;

    static constexpr Unit Base = 1024;

    enum Scale : Unit { B = 1, KB = Base * B, MB = Base * KB, GB = Base * MB, TB = Base * GB };

    struct Size {
        friend std::ostream& operator<<(std::ostream &os, const Size &memSize) {
            auto units = { "B", "KB", "MB", "GB", "TB", "PB" };
            double size = static_cast<double>(memSize.size);
            for (auto u = units.begin(); u != units.end(); ++u, size /= Base) {
                if (size < Base) {
                    os << std::setprecision(4) << size << *u;
                    break;
                }
            }

            return os;
        }

        Size(Unit memSize) : size(memSize) {}

        operator Unit() const { return size; }
        operator Unit&() { return size; }
        operator const Unit&() const { return size; }

        Unit size;
    };

    struct Usage {
        Size physicalMemory;
        Size virtualMemory;
    };

    static bool setMaxMemorySize(Unit byteSize); // the working set will also be expanded.
    static bool setMaxMemorySize(Unit size, Scale unit) { return setMaxMemorySize(static_cast<Unit>(size * unit)); }

    static Usage memoryUsage();
    static Usage peakMemoryUsage();

    static Unit getPhysicalMemoryByteSize();
    static Unit getFreePhysicalMemoryByteSize();

    static Unit getPhysicalMemoryKbSize();
};

}


#endif // SZX_CPPUTILIBS_H
