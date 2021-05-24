#pragma once
#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <condition_variable>
#include <mutex>
#include <queue>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue() : q(), m() {}

    ~SafeQueue() {}

    bool empty() const { return q.empty(); }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
    }

    // Get the front element (if there is one).
    T dequeue(void)
    {
        T val = nullptr;
        std::lock_guard<std::mutex> lock(m);
        if (!q.empty())
        {
            val = q.front();
            q.pop();
        }
        return val;
    }

private:
    std::queue<T> q;
    mutable std::mutex m;
};
#endif