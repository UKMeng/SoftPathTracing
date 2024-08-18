//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <thread>

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