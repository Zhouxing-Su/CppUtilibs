#include "Directory.h"


using namespace std;


namespace szx
{
    using namespace cmd;
    using namespace cmd::common;
#ifdef WIN32
    using namespace cmd::win32;
#else
    using namespace cmd::Unix;
#endif

    namespace cmd
    {
        namespace common
        {
            const string redirectStdin( " 0< " );
            const string redirectStdout( " 1> " );
            const string redirectStderr( " 2> " );
        }

        namespace win32
        {
            const string mkdir( " mkdir " );
            const string nullDev( " nul " );
        }

        namespace unix
        {
            const string mkdir( " mkdir -p " );
            const string nullDev( " /dev/null " );
        }
    }


    void Directory::makeSureDirExist( const string &dir )
    {
        string cmd( mkdir + quote( dir ) + redirectStderr + nullDev );
        system( cmd.c_str() );
    }
}