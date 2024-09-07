//
// Created by UKMeng on 2024/9/7.
//

#include "Profile.h"

#include <utility>
#include <iostream>

Profile::Profile(std::string name) : m_Name(std::move(name)), m_StartTime(std::chrono::high_resolution_clock::now())
{

}

Profile::~Profile()
{
    auto duration = std::chrono::high_resolution_clock::now() - m_StartTime;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << m_Name << ": " << ms << "ms" << std::endl;
}
