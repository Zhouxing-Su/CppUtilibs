////////////////////////////////
/// usage : 1.	command line utility.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_SYSTEM_H
#define SZX_CPPUTILIBS_SYSTEM_H


#include <string>


#pragma region PlatformCheck
#ifdef _MSC_VER // use (_WIN16 || _WIN32 || _WIN64)?
#define _OS_MS_WINDOWS  1
#define _CC_MSVC  1
#else
#define _OS_MS_WINDOWS  0
#define _CC_MSVC  0
#endif // _MSC_VER

#ifdef __unix__
#define _OS_UNIX  1
#else
#define _OS_UNIX  0
#endif // __unix__

#ifdef __linux__
#define _OS_GNU_LINUX  1
#else
#define _OS_GNU_LINUX  0
#endif // __linux__

#ifdef __MACH__ // use __APPLE__?
#define _OS_APPLE_MAC  1
#else
#define _OS_APPLE_MAC  0
#endif // __MACH__

#ifdef __GNUC__
#define _CC_GNU_GCC  1
#else
#define _CC_GNU_GCC  0
#endif // __GNUC__

#ifdef __llvm__
#define _CC_LLVM  1
#else
#define _CC_LLVM  0
#endif // __llvm__

#ifdef __clang__
#define _CC_CLANG  1
#else
#define _CC_CLANG  0
#endif // __clang__
#pragma endregion PlatformCheck


/// if there is "#define x  y", VERBATIM_STRINGIFY(x) will get "x".
#define VERBATIM_STRINGIFY(x)  #x
/// if there is "#define x  y", RESOLVED_STRINGIFY(x) will get "y".
#define RESOLVED_STRINGIFY(x)  VERBATIM_STRINGIFY(x)

#define VERBATIM_CONCAT(a, b)  a##b
#define VERBATIM_CONCAT2(a, b, c)  a##b##c
#define VERBATIM_CONCAT3(a, b, c, d)  a##b##c##d
#define RESOLVED_CONCAT(a, b)  VERBATIM_CONCAT(a, b)
#define RESOLVED_CONCAT2(a, b, c)  VERBATIM_CONCAT2(a, b, c)
#define RESOLVED_CONCAT3(a, b, c, d)  VERBATIM_CONCAT3(a, b, c, d)


namespace szx {

namespace cmd {

namespace common {

extern const std::string redirectStdin;
extern const std::string redirectStdout;
extern const std::string redirectStderr;
extern const std::string redirectStdout_app;
extern const std::string redirectStderr_app;


std::string quote(const std::string &s);

}

namespace win32 {

extern const std::string mkdir;
extern const std::string nullDev;

}

namespace unix {

extern const std::string mkdir;
extern const std::string nullDev;

}

}

}


#endif // SZX_CPPUTILIBS_SYSTEM_H