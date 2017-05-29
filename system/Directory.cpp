#include "Directory.h"

#include <cstdlib>

#include "System.h"


using namespace std;


namespace szx {

using namespace cmd::common;
#ifdef _OS_MS_WINDOWS
using namespace cmd::win32;
#else
using namespace cmd::unix;
#endif // _OS_MS_WINDOWS


void Directory::makeSureDirExist(const string &dir) {
    string cmd(Mkdir + quote(dir) + RedirectStderr + NullDev);
    system(cmd.c_str());
}

}
