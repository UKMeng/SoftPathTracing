//
// Created by UKMeng on 2024/8/17.
//

#pragma once

// global math method
#include "Vector.h"

#undef M_PI
#define M_PI 3.141592653589793f
#define INV_PI 0.3183098861837907f

template<typename T>
T Clamp(const T& val, const T& minVal, const T& maxVal)
{
    return std::max(minVal, std::min(maxVal, val));
}

inline float Clamp(const float& val, const float& minVal, const float& maxVal)
{
    return std::max(minVal, std::min(maxVal, val));
}

inline Vec3f Clamp(const Vec3f& val, const float& minVal, const float& maxVal)
{
    return Vec3f(Clamp(val.x, minVal, maxVal), Clamp(val.y, minVal, maxVal), Clamp(val.z, minVal, maxVal));
}

inline float Lerp(const float& val1, const float& val2, const float& t)
{
    return val1 + (val2 - val1) * t;
}

inline float Radians(const float& degree)
{
    return degree * M_PI / 180.0f;
}

inline float Pow(const float& val, const float& power)
{
    return std::pow(val, power);
}

inline float Abs(const float& val)
{
    return std::abs(val);
}

inline float Sqrt(const float& val)
{
    return std::sqrt(val);
}

inline float Cos(const float& val)
{
    return std::cos(val);
}

inline float Sin(const float& val)
{
    return std::sin(val);
}

inline float Max(const float& val1, const float& val2)
{
    return std::max(val1, val2);
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

template<typename T>
T Normalize(const T& vec)
{
    return vec.Normalize();
}

template<typename T>
float Dot(const T& vec1, const T& vec2)
{
    return vec1.Dot(vec2);
}

template<typename T>
T Cross(const T& vec1, const T& vec2)
{
    return vec1.Cross(vec2);
}

template<typename T>
T Mix(const T& a, const T& b, float t)
{
    return a + (b - a) * t;
}