//
// Created by UKMeng on 2024/8/17.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount) : m_Alive(true)
{
    if (threadCount == 0)
    {
        threadCount = std::thread::hardware_concurrency();
    }

    for (size_t i = 0; i < threadCount; i++)
    {
        m_Threads.emplace_back(ThreadPool::Worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    // wait all task done
    while (!m_TaskList.empty())
    {
        std::this_thread::yield();
    }

    // when all task done, set m_Alive false to stop threads
    m_Alive = false;
    for (auto& thread: m_Threads)
    {
        thread.join();
    }

}

void ThreadPool::AddTask(Task *task)
{
    std::lock_guard<std::mutex> guard(m_Mutex);
    m_TaskList.emplace_back(task);
}

Task *ThreadPool::GetTask()
{
    std::lock_guard<std::mutex> guard(m_Mutex);
    if (m_TaskList.empty())
    {
        return nullptr;
    }

    Task *task = m_TaskList.front();
    m_TaskList.pop_front();
    return task;
}

void ThreadPool::Worker(ThreadPool *master)
{
    while (master->m_Alive)
    {
        Task *task = master->GetTask();
        if (task != nullptr)
        {
            task->Run();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}


