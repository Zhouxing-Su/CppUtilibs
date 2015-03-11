#include "Directory.h"


using namespace std;


namespace szx
{
    const std::string Directory::mkdir_unixshell( "mkdir " );
    const std::string Directory::mkdir_win32cmd( "mkdir -p " );


    void Directory::MakeSureDirExist( const std::string &dir )
    {
        system( (mkdir_win32cmd + dir).c_str() );
        system( (mkdir_unixshell + dir).c_str() );
    }
}