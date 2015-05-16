#include "Termination.h"


using namespace std;


namespace szx
{
    void Termination::waitTerminationCode( const string &terminationCode )
    {
        string s;
        do {
            cin >> s;
        } while (s != terminationCode);
        exit( 0 );
    }

    void Termination::peekTerminationCode( const std::string &terminationCode )
    {
        thread( waitTerminationCode, terminationCode ).detach();
    }
}
