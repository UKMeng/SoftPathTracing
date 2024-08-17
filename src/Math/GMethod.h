//
// Created by UKMeng on 2024/8/17.
//

#pragma once

// global math method

#undef M_PI
#define M_PI 3.141592653589793f

inline float clamp(const float& val, const float& minVal, const float& maxVal)
{
    return std::max(minVal, std::min(maxVal, val));
}
