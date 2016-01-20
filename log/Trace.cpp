#include "Trace.h"


using namespace std;


namespace szx {

const string Trace::DUMP_FILE_NAME = "crashdump.txt";

vector<string> Trace::callStack;


#if WIN32
LONG Trace::defaultHandler_Win32(struct _EXCEPTION_POINTERS *) {
    dumpCallStack();
    return EXCEPTION_EXECUTE_HANDLER;
}
#else   // unix like systems
void Trace::defaultHandler_Unix(int signum, siginfo_t* info, void*ptr) {
    dumpCallStack();
}
#endif  // WIN32

void Trace::dumpCallStack(ostream &dumpFile) {
    for (size_t i = 0; i < callStack.size(); ++i) {
        dumpFile << '[' << setw(3) << i << "] " << callStack[i] << std::endl;
    }
}

void Trace::dumpCallStack(const std::string &dumpFileName) {
    ofstream ofs(dumpFileName);
    dumpCallStack(ofs);
    ofs.close();
}

}