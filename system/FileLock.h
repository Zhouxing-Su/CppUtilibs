/**
*   usage : 1.  locking mechanism implemented by create and remove file.
*           2.  method 1 : loop to tryLock until checkLock return true, unlock after close file.
*               method 2 : call lock, unlock after close file.
*
*   note :  1.  signature collision.
*           2.  still got problems on massive thread running.
*/


#ifndef FILE_LOCK_H
#define FILE_LOCK_H


#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdio>
#include <cstdlib>


namespace szx
{
    class FileLock
    {
    public:
        static const int TRY_LOCK_INTERVAL = 250;   // in milliseconds
        static const int BIT_NUM_OF_INT = 32;

        static const std::string LockName;
        static const std::ios_base::openmode ReadMode;
        static const std::ios_base::openmode WriteMode;


        // used in initialization, in case unexpected termination 
        // which leaves the lock file not removed.
        static void unlock( const std::string &filename );


        // specify your own lock id to make the chance of signature collision smaller
        FileLock( const std::string &filename, unsigned id = std::random_device()() );

        // return true if the file is really locked by the caller, else return false
        bool checkLock() const;
        // if the file is free, then lock it, else do nothing
        // so you must call checkLock to get the lock state
        void tryLock();
        // spin lock which will block the thread until success
        void lock();
        // release the lock
        void unlock();

    private:
        const std::string lockFileName;
        const long long signature;
        const unsigned retryInterval;

    private:    // forbidden operators
        FileLock( const FileLock & ) : lockFileName(), signature(), retryInterval() {}
        FileLock& operator=(const FileLock &) { return *this; }
    };
}


#endif
