#include "File.h"


using namespace std;


namespace szx
{

string File::getLastNoneEmptyLine( const string &filename )
{
    string lastLine;
    ifstream fin( filename );

    if (fin.is_open()) {
        // leave out last '\r' or '\n'
        locate_last_of<true>( fin, "\n\r", 0, ios_base::end );
        // search for last end of line
        locate_last_of( fin, "\n\r" );

        if (contain( "\n\r", fin.peek() )) {
            fin.get();
        }
        getline( fin, lastLine );
        fin.close();
    }

    return lastLine;
}

}