#include <iostream>
#include <thread>

#include "Termination.h"


using namespace std;


namespace szx
{

void Termination::waitForCode( const string &terminationCode )
{
    waitForCode( terminationCode, [](){ return 0; } );
}

template<typename OnTermination>
void Termination::waitForCode( const string &terminationCode, OnTermination onTermination )
{
    string s;
    do {
        cin >> s;
    } while (s != terminationCode);
    exit( onTermination() );
}

void Termination::waitForCodeAsync( const string &terminationCode )
{
    waitForCodeAsync( terminationCode, [](){ return 0; } );
}

template<typename OnTermination>
void Termination::waitForCodeAsync( const string &terminationCode, OnTermination onTermination )
{
    thread( waitForCode<OnTermination>, terminationCode, onTermination ).detach();
}

}
