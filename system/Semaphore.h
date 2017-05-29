////////////////////////////////
/// usage : 1.	a simple hread pool without return value retrieval and argument passing.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_SEMAPHORE_H
#define SZX_CPPUTILIBS_SEMAPHORE_H


#include <mutex>
#include <condition_variable>


namespace szx {

class Semaphore {
public:
    Semaphore(int count = 0) : count(count) {}

    // a.k.a. release/up/post/vacate/signal.
    void notify() {
        std::unique_lock<std::mutex> lock(m);
        ++count;
        cv.notify_one();
    }

    // a.k.a. acquire/down/pend/procure.
    void wait() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]() { return (count > 0); });
        --count;
    }

private:
    std::mutex m;
    std::condition_variable cv;
    int count;
};

}


#endif // SZX_CPPUTILIBS_SEMAPHORE_H
