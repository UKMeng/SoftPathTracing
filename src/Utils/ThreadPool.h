//
// Created by UKMeng on 2024/8/17.
//

#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

#include "SpinLock.h"

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
    std::queue<Task*> m_TaskList;
    // std::mutex m_Mutex;
    SpinLock m_SpinLock {};
    std::atomic<int> m_Alive;
    std::atomic<int> m_PendingTaskCount;
};

extern ThreadPool g_ThreadPool;