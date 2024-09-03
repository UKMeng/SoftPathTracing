//
// Created by UKMeng on 2024/8/17.
//

#include "ThreadPool.h"

ThreadPool g_ThreadPool {}; // global thread pool

ThreadPool::ThreadPool(size_t threadCount)
{
    m_Alive = 1;
    m_PendingTaskCount = 0;
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
    m_PendingTaskCount++;
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
            delete task;
            master->m_PendingTaskCount--;
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

void ThreadPool::Wait() const
{
    while (m_PendingTaskCount > 0)
    {
        std::this_thread::yield();
    }
}

class ParallelForTask: public Task
{
public:
    ParallelForTask(size_t x, size_t y, size_t chunkWidth, size_t chunkHeight, const std::function<void(size_t, size_t)> &lambda)
        : x(x), y(y), chunkWidth(chunkWidth), chunkHeight(chunkHeight), lambda(lambda) {}

    void Run() override
    {
        for (size_t i = 0; i < chunkWidth; i++)
        {
            for (size_t j = 0; j < chunkHeight; j++)
            {
                lambda(x + i, y + j);
            }
        }
    }

private:
    size_t x, y, chunkWidth, chunkHeight;
    std::function<void(size_t, size_t)> lambda;
};

void ThreadPool::ParallelFor(size_t width, size_t height, const std::function<void(size_t, size_t)> &lambda, bool complex)
{
    Guard guard(m_SpinLock);

    size_t chunkWidth;
    size_t chunkHeight;

    if (complex)
    {
        // if lambda is a complex task, divide the task into smaller chunks
        chunkWidth = std::ceil(static_cast<float>(width) / std::sqrt(16) / std::sqrt(m_Threads.size()));
        chunkHeight = std::ceil(static_cast<float>(height) / std::sqrt(16) / std::sqrt(m_Threads.size()));
    }
    else
    {
        chunkWidth = std::ceil(static_cast<float>(height) / std::sqrt(m_Threads.size()));
        chunkHeight = std::ceil(static_cast<float>(width) / std::sqrt(m_Threads.size()));
    }

    for (size_t x = 0; x < width; x += chunkWidth)
    {
        for (size_t y = 0; y < height; y += chunkHeight)
        {
            m_PendingTaskCount++;
            if (x + chunkWidth > width) chunkWidth = width - x;
            if (y + chunkHeight > height) chunkHeight = height - y;
            m_TaskList.push(new ParallelForTask(x, y, chunkWidth, chunkHeight, lambda));
        }
    }
}