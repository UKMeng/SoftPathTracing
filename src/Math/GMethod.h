//
// Created by UKMeng on 2024/8/17.
//

#pragma once

// global math method

#undef M_PI
#define M_PI 3.141592653589793f

inline float Clamp(const float& val, const float& minVal, const float& maxVal)
{
    return std::max(minVal, std::min(maxVal, val));
}

inline float Radians(const float& degree)
{
    return degree * M_PI / 180.0f;
}

// reference: https://en.wikipedia.org/wiki/Fast_inverse_square_root
inline float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}