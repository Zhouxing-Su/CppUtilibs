#include "FileLock.h"


using namespace std;


const std::string FileLock::LockName( ".lock" );
const std::ios_base::openmode FileLock::ReadMode( ios::in | ios::binary );
const std::ios_base::openmode FileLock::WriteMode( ios::out | ios::binary );


void FileLock::unlock( const std::string &filename )
{
    remove( (LockName + filename).c_str() );
}


FileLock::FileLock( const string &filename, int id )
    : lockFileName( filename + LockName ),
    signature( (static_cast<long long>(rand() + time( NULL ) + clock()) << BIT_NUM_OF_INT) | id ),
    retryInterval( TRY_LOCK_INTERVAL + (id % TRY_LOCK_INTERVAL) )
{
}

bool FileLock::checkLock() const
{
    long long sign = (static_cast<long long>((rand() + time( NULL ) + clock())) << BIT_NUM_OF_INT) | random_device()();
    fstream lockFile( lockFileName, ReadMode );
    if (lockFile.is_open()) {
        lockFile.read( reinterpret_cast<char*>(&sign), sizeof( sign ) );
        lockFile.close();
    }

    return (sign == signature);
}

void FileLock::tryLock()
{
    fstream lockFile( lockFileName, ReadMode );
    if (lockFile.is_open()) {
        lockFile.close();
    } else {
        lockFile.open( lockFileName, WriteMode );
        lockFile.clear();
        lockFile.write( reinterpret_cast<const char*>(&signature), sizeof( signature ) );
        lockFile.close();
    }
}

void FileLock::lock()
{
    do {
        tryLock();  // result can be 1.success; 2.fail; 3.share lock
        // wait in case others write signature right after checkLock
        this_thread::sleep_for( chrono::milliseconds( retryInterval ) );
    } while (!checkLock());
}

void FileLock::unlock()
{
    remove( lockFileName.c_str() );
}
