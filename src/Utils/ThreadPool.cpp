//
// Created by UKMeng on 2024/8/17.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount)
{
    m_Alive = 1;
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
    Wait();

    // when all task done, set m_Alive false to stop threads
    m_Alive = 0;
    for (auto& thread: m_Threads)
    {
        thread.join();
    }

}

void ThreadPool::AddTask(Task *task)
{
    // std::lock_guard<std::mutex> guard(m_Mutex);
    Guard guard(m_SpinLock);
    m_TaskList.push(task);
}

Task *ThreadPool::GetTask()
{
    // std::lock_guard<std::mutex> guard(m_Mutex);
    Guard guard(m_SpinLock);
    if (m_TaskList.empty())
    {
        return nullptr;
    }

    Task *task = m_TaskList.front();
    m_TaskList.pop();
    return task;
}

void ThreadPool::Worker(ThreadPool *master)
{
    while (master->m_Alive == 1)
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

void ThreadPool::Wait() const
{
    while (!m_TaskList.empty())
    {
        ThreadPool::UpdateProgress(1.0 - m_TaskList.size() / m_TotalTaskCount);
        std::this_thread::yield();
    }
}

class ParallelForTask: public Task
{
public:
    ParallelForTask(size_t x, size_t y, const std::function<void(size_t, size_t)> &lambda): x(x), y(y), lambda(lambda) {}

    void Run() override
    {
        lambda(x, y);
    }
private:
    size_t x, y;
    std::function<void(size_t, size_t)> lambda;
};

void ThreadPool::ParallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)> &lambda)
{
    Guard guard(m_SpinLock);

    m_TotalTaskCount = width * height;

    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            m_TaskList.push(new ParallelForTask(x, y, lambda));
        }
    }
}