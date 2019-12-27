#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

// Not used
// hoes be multiplexing
class TSemaphore {
public:
    TSemaphore(int init = 0) : count(init) {}

    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }

    void post()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return count != 0; });
        --count;
    }

private:
    std::mutex              mtx;
    std::condition_variable cv;
    int                     count;
};

#endif