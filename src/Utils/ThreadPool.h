//
// Created by UKMeng on 2024/8/17.
//

#pragma once

#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

class SpinLock
{
public:
    void Lock()
    {
        while (m_Flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    void Unlock()
    {
        m_Flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag m_Flag {};
};

class Guard
{
public:
    Guard(SpinLock& lock) : m_SpinLock(lock) { m_SpinLock.Lock(); }
    ~Guard() { m_SpinLock.Unlock();}
private:
    SpinLock& m_SpinLock;
};


class Task
{
public:
    virtual ~Task() = default;
    virtual void Run() = 0;
};

class ThreadPool
{
public:
    ThreadPool(size_t threadCount = 0);
    ~ThreadPool();

    void Wait() const;

    void ParallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)>& lambda);

    void AddTask(Task* task);
    Task* GetTask();

    static void Worker(ThreadPool* master);

private:
    std::vector<std::thread> m_Threads;
    std::list<Task*> m_TaskList;
    // std::mutex m_Mutex;
    SpinLock m_SpinLock {};
    std::atomic<int> m_Alive;
};
