#include "Directory.h"


using namespace std;


namespace szx
{
    const std::string Directory::mkdir_win32cmd( "mkdir -p " );
    const std::string Directory::mkdir_unixshell( "mkdir " );


    void Directory::MakeSureDirExist( const std::string &dir )
    {
#ifdef WIN32
        system( (mkdir_win32cmd + dir).c_str() );
#else
        system( (mkdir_unixshell + dir).c_str() );
#endif
    }
}