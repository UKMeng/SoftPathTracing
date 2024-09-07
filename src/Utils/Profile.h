//
// Created by UKMeng on 2024/9/7.
//

#pragma once

#include <chrono>

class Profile
{
public:
    Profile(std::string name);
    ~Profile();
private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};
