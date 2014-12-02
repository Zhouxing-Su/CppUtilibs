#include "Log.h"


const std::string Trace::DUMP_FILE_NAME = "crashdump.txt";


std::terminate_handler Trace::userTerminateHandler;

std::vector<const std::string> Trace::callStack;