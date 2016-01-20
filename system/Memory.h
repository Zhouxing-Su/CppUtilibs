////////////////////////////////
/// usage : 1.	query memory size and usage.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_H
#define SZX_CPPUTILIBS_H


#ifdef WIN32
#include <Windows.h>
#else // unix like systems
#include <unistd.h>
#endif // WIN32


namespace szx {

class Memory {
public:
    using Size = long long;

    #ifdef WIN32
    static MEMORYSTATUSEX getMemoryStatus() {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status;
    }

    static Size getTotalPhysicalMemory() { return getMemoryStatus().ullTotalPhys; }

    static Size getAvailablePhysicalMemory() { return getMemoryStatus().ullAvailPhys; }
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
