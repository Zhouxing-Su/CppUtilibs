////////////////////////////////
/// usage : 1.	query memory size and usage.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_H
#define SZX_CPPUTILIBS_H


#include <iostream>
#include <iomanip>

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#else // unix like systems
#include <unistd.h>
#endif // WIN32


namespace szx {

class Memory {
public:
    struct Size {
        using Unit = long long;

        static constexpr Unit Base = 1024;

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

        Unit size;
    };

    struct MemoryUsage {
        Size physicalMemory;
        Size virtualMemory;
    };

    #ifdef WIN32
    static MemoryUsage memoryUsage() {
        MemoryUsage mu = { 0, 0 };

        PROCESS_MEMORY_COUNTERS pmc;
        HANDLE hProcess = GetCurrentProcess();
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            mu.physicalMemory.size = pmc.WorkingSetSize;
            mu.virtualMemory.size = pmc.PagefileUsage;
        }
        CloseHandle(hProcess);

        return mu;
    }
    static MemoryUsage peakMemoryUsage() {
        MemoryUsage mu = { 0, 0 };

        PROCESS_MEMORY_COUNTERS pmc;
        HANDLE hProcess = GetCurrentProcess();
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            mu.physicalMemory.size = pmc.PeakWorkingSetSize;
            mu.virtualMemory.size = pmc.PeakPagefileUsage;
        }
        CloseHandle(hProcess);

        return mu;
    }

    static MEMORYSTATUSEX getMemoryStatus() {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status;
    }

    static Size::Unit getTotalPhysicalMemory() { return getMemoryStatus().ullTotalPhys; }

    static Size::Unit getAvailablePhysicalMemory() { return getMemoryStatus().ullAvailPhys; }
    #else // unix like systems
    static Size getTotalPhysicalMemory() {
        Size pages = sysconf(_SC_PHYS_PAGES);
        Size pageSize = sysconf(_SC_PAGE_SIZE);
        return pages * pageSize;
    }
    #endif // WIN32
};

}


#endif // SZX_CPPUTILIBS_H
