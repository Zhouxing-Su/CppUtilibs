#include "system.h"


using namespace std;


namespace szx
{

namespace cmd
{

namespace common
{

const string redirectStdin( " 0< " );
const string redirectStdout( " 1> " );
const string redirectStderr( " 2> " );
const string redirectStdout_app( " 1>> " );
const string redirectStderr_app( " 2>> " );

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

string quote( const string &s )
{
    return ('\"' + s + '\"');
}

}