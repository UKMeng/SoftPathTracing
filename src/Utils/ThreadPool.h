//
// Created by UKMeng on 2024/8/17.
//

#pragma once

#include <vector>
#include <list>
#include <thread>
#include <mutex>

class Task
{
public:
    virtual void Run() = 0;
};

class ThreadPool
{
public:
    ThreadPool(size_t threadCount = 0);
    ~ThreadPool();

    void AddTask(Task* task);
    Task* GetTask();

    static void Worker(ThreadPool* master);

private:
    std::vector<std::thread> m_Threads;
    std::list<Task*> m_TaskList;
    std::mutex m_Mutex;

    bool m_Alive;
};
