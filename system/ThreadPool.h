////////////////////////////////
/// usage : 1.	a simple hread pool without return value retrieval and argument passing.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef SZX_CPPUTILIBS_THREAD_POOL_H
#define SZX_CPPUTILIBS_THREAD_POOL_H


#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>
#include <utility>


namespace szx {

namespace impl {

// [NoReturnValueRetrieval][NotExceptionSafe]
// [ManualStart][ManualPend][ManualStop]
class ThreadPoolBase {
public:
    enum State { Run, Pend, Stop };


    using Size = int;
    using Job = std::function<void(void)>;
    using Worker = std::thread;
    using Lock = std::unique_lock<std::mutex>;


    static Size getDefaultWorkerNum() { return Worker::hardware_concurrency(); }


    // you may want to start() once the pool is constructed in the derived classes.
    ThreadPoolBase(Size threadNum) : workerPool(threadNum) {}
    // you may want to pend() once the pool is destructed in the derived classes.
    virtual ~ThreadPoolBase() {}


    // make all workers ready to work.
    // you could restart after pend() or stop().
    virtual void start() {
        setState(State::Run);
        launchAll();
    }
    // terminate all workers after all pushed jobs are done.
    virtual void pend() {
        setState(State::Pend);
        waitAll(); // OPTIMIZE[szx][0]: detach and move on instead of waiting?
    }
    // terminate all workers after all taken jobs are done.
    virtual void stop() {
        setState(State::Stop);
        waitAll();
    }

    virtual void push(Job &&newJob) = 0;

    virtual State getState() = 0;

protected:
    // the main loop of taking and executeing jobs for single worker thread.
    virtual void work() = 0;

    virtual void setState(State state) = 0;

    void launchAll() {
        for (auto worker = workerPool.begin(); worker != workerPool.end(); ++worker) {
            *worker = Worker([this]() { work(); });
        }
    }
    void waitAll() {
        for (auto worker = workerPool.begin(); worker != workerPool.end(); ++worker) {
            if (worker->joinable()) { worker->join(); }
        }
    }

    static bool isValidJob(const Job &job) { return static_cast<bool>(job); }

    static void dummyJob() {}


    std::vector<Worker> workerPool;
};

class ThreadPool {
public:
// [NoReturnValueRetrieval][NotExceptionSafe]
// [ManualStart][ManualPend][ManualStop]
    class QueueImpl : public ThreadPoolBase {
    public:
        using ThreadPoolBase::ThreadPoolBase;


        virtual void stop() override {
            setState(State::Stop);
            jobCv.notify_all(); // terminate sleeping workers.
            waitAll();
        }
        virtual void pend() override {
            setState(State::Pend);
            jobCv.notify_all(); // terminate sleeping workers.
            waitAll();
        }

        virtual void push(Job &&newJob) override {
            Lock jobLock(jobMutex);
            jobQueue.push(newJob);
            jobLock.unlock();

            jobCv.notify_one();
        }

        virtual State getState() override {
            Lock jobLock(jobMutex);
            return state;
        }

    protected:
        virtual void work() override {
            for (;;) {
                Lock jobLock(jobMutex);

                if (jobQueue.empty()) {
                    if (state != State::Run) { return; } // all pending jobs finished.
                    jobCv.wait(jobLock);
                    if (jobQueue.empty()) {
                        if (state != State::Run) { return; } // all pending jobs finished.
                        continue;  // skip spurious wake up.
                    }
                }
                if (state == State::Stop) { return; }

                Job newJob(std::move(jobQueue.front()));
                jobQueue.pop();

                jobLock.unlock();

                newJob();
            }
        }

        virtual void setState(State newState) override {
            Lock jobLock(jobMutex);
            state = newState;
        }


        State state;

        std::queue<Job> jobQueue;

        std::mutex jobMutex;
        std::condition_variable jobCv;
    };


    // [NoReturnValueRetrieval][NotExceptionSafe]
    // [ManualStart][ManualPend][ManualStop]
    class SingleSlotImpl : public ThreadPoolBase {
    public:
        using ThreadPoolBase::ThreadPoolBase;


        virtual void stop() override {
            state = State::Stop;
            jobCv.notify_all();
            waitAll();
        }
        virtual void pend() override {
            state = State::Pend;
            jobCv.notify_all();
            waitAll();
        }

        virtual void push(Job &&newJob) override {
            Lock workerLock(workerMutex);
            workerCv.wait(workerLock, [this]() { return isSlotEmpty(); }); // OPTIMIZE[szx][0]: assume spurious wake up will never happen?
            nextJob = newJob; // make the new job available for taking.
            workerLock.unlock();

            jobCv.notify_one(); // declare that a new job is available (wake up a worker to take the new job).
        }

        virtual State getState() override { return state; }

    protected:
        virtual void work() override {
            for (Job newJob; state != State::Stop; newJob()) { // do the job.
                Lock jobLock(jobMutex); // wait until a new job is available.

                if (isSlotEmpty()) {
                    if (state != State::Run) { return; } // all pending jobs finished.
                    jobCv.wait(jobLock);
                    if (isSlotEmpty()) {
                        if (state != State::Run) { return; } // all pending jobs finished.
                        continue;  // skip spurious wake up.
                    }
                }
                if (state == State::Stop) { return; }

                newJob = std::move(nextJob); // take the new job.
                jobLock.unlock();

                workerCv.notify_one(); // declare that the job has been taken (wake up the dispatcher to enable a new job).
            }
        }

        virtual void setState(State newState) override { state = newState; }

        bool isSlotEmpty() const { return !isValidJob(nextJob); }


        std::atomic<State> state;

        Job nextJob;

        std::mutex jobMutex;
        std::condition_variable jobCv;
        std::mutex workerMutex;
        std::condition_variable workerCv;
    };
};

}

// [NoReturnValueRetrieval][NotExceptionSafe]
// [AutoStart][AutoPend][ManualStop]
template<typename ThreadPoolImpl = impl::ThreadPool::QueueImpl>
class ThreadPool : public ThreadPoolImpl {
public:
    ThreadPool(int threadNum) : ThreadPoolImpl(threadNum) { start(); }
    ThreadPool() : ThreadPool(getDefaultWorkerNum()) {}
    virtual ~ThreadPool() { pend(); }


    using ThreadPoolImpl::push;
    // avoid copying function objects. the const reference can be handled automatically.
    template<typename Functor>
    void push(Functor &newJob) { push(std::ref(newJob)); } // or use `push([&newJob]() { newJob(); });`.
    // EXTEND[szx][9]: provide std::future?
};

}


#endif // SZX_CPPUTILIBS_THREAD_POOL_H
