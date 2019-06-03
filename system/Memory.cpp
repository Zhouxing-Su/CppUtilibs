#include "Memory.h"

#ifdef _OS_MS_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#else // unix like systems
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif // _OS_MS_WINDOWS


using namespace std;


namespace szx {

#if _OS_MS_WINDOWS
bool Memory::setMaxMemorySize(Memory::Unit byteSize) {
    HANDLE hProc = GetCurrentProcess();
    SIZE_T minWorkingSetSize, maxWorkingSetSize;
    GetProcessWorkingSetSize(hProc, &minWorkingSetSize, &maxWorkingSetSize);

    HANDLE hJob = CreateJobObjectA(NULL, NULL);
    if (!AssignProcessToJobObject(hJob, hProc)) { return false; }

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit;
    limit.JobMemoryLimit = static_cast<SIZE_T>(byteSize);
    limit.BasicLimitInformation.MinimumWorkingSetSize = minWorkingSetSize;
    limit.BasicLimitInformation.MaximumWorkingSetSize = static_cast<SIZE_T>(byteSize);
    limit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY | JOB_OBJECT_LIMIT_WORKINGSET;
    return SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &limit, sizeof(limit)) != 0;
}

Memory::Usage Memory::memoryUsage() {
    Memory::Usage mu = { 0, 0 };

    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE hProcess = GetCurrentProcess();
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        mu.physicalMemory.size = pmc.WorkingSetSize;
        mu.virtualMemory.size = pmc.PagefileUsage;
    }
    CloseHandle(hProcess);

    return mu;
}
Memory::Usage Memory::peakMemoryUsage() {
    Memory::Usage mu = { 0, 0 };

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

Memory::Unit Memory::getPhysicalMemoryByteSize() { return getMemoryStatus().ullTotalPhys; }

Memory::Unit Memory::getFreePhysicalMemoryByteSize() { return getMemoryStatus().ullAvailPhys; }

Memory::Unit Memory::getPhysicalMemoryKbSize() {
    ULONGLONG physicalMemorySize;
    GetPhysicallyInstalledSystemMemory(&physicalMemorySize);
    return physicalMemorySize;
}
#else // unix like systems.
bool Memory::setMaxMemorySize(Memory::Unit byteSize) {
    struct rlimit memlimit;

    memlimit.rlim_cur = byteSize;
    memlimit.rlim_max = byteSize;
    setrlimit(RLIMIT_AS, &memlimit);

    return true;
}

Memory::Unit Memory::getPhysicalMemoryByteSize() {
    Memory::Unit pages = sysconf(_SC_PHYS_PAGES);
    Memory::Unit pageSize = sysconf(_SC_PAGE_SIZE);
    return pages * pageSize;
}
#endif // _OS_MS_WINDOWS

}
