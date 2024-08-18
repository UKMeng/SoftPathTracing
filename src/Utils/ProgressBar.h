//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <iostream>

#include "SpinLock.h"

class ProgressBar
{
public:
    ProgressBar(size_t total, size_t step = 1) : m_Total(total), m_Current(0), m_Percent(0), m_LastPercent(0), m_Step(step)
    {
        std::cout << "[";
        int pos = 0;
        for (int i = 0; i < m_BarWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << 0 << " %\r";
        std::cout.flush();
    }


    inline void Update(size_t count)
    {
        Guard guard(m_Lock);

        m_Current += count;
        m_Percent = static_cast<float>(m_Current) / static_cast<float>(m_Total);

        if ((m_Percent - m_LastPercent) * 100.0 >= m_Step || m_Percent == 1.0)
        {
            // Copied from GAMES101 Homework7
            m_LastPercent = m_Percent;
            std::cout << "[";
            int pos = m_BarWidth * m_Percent;
            for (int i = 0; i < m_BarWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(m_Percent * 100.0) << " %\r";
            std::cout.flush();
        }
    }


private:
    size_t m_Total, m_Current, m_Step;
    float m_Percent, m_LastPercent;
    size_t m_BarWidth = 70;
    SpinLock m_Lock;
};


