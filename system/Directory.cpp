#include "Directory.h"


using namespace std;


namespace szx
{

using namespace cmd::common;
#ifdef WIN32
using cmd::win32::mkdir;
using cmd::win32::nullDev;
#else
using cmd::Unix::mkdir;
using cmd::Unix::nullDev;
#endif


void Directory::makeSureDirExist( const string &dir )
{
    string cmd( mkdir + quote( dir ) + redirectStderr + nullDev );
    system( cmd.c_str() );
}

}