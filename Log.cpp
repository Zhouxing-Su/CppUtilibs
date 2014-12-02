#include "Log.h"


template <class T_OUTPUT = const char *>
static std::terminate_handler Trace::userTerminateHandler;

template <class T_OUTPUT = const char *>
static std::vector<T_OUTPUT> Trace::callStack;