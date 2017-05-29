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


namespace szx {

namespace cmd {

namespace common {

constexpr char RedirectStdin[] = " 0< ";
constexpr char RedirectStdout[] = " 1> ";
constexpr char RedirectStderr[] = " 2> ";
constexpr char RedirectStdout_app[] = " 1>> ";
constexpr char RedirectStderr_app[] = " 2>> ";

}

namespace win32 {

constexpr char Mkdir[] = " mkdir ";
constexpr char NullDev[] = " nul ";

}

namespace unix {

constexpr char Mkdir[] = " mkdir -p ";
constexpr char NullDev[] = " /dev/null ";

}

}

}


#endif // SZX_CPPUTILIBS_SYSTEM_H