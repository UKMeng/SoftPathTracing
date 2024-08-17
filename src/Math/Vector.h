//
// Created by UKMeng on 2024/8/17.
//

#pragma once
#include <iostream>

#include "GMethod.h"

class Vec3f
{
public:
    float x, y, z;

    Vec3f(): x(0), y(0), z(0) {};
    Vec3f(float x): x(x), y(x), z(x) {};
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {};

    Vec3f operator-(const Vec3f& other) const
    {
        return Vec3f(x - other.x, y - other.y, z - other.z);
    }

    Vec3f Normalize() const
    {
        float rootInv = Q_rsqrt(x * x + y * y + z * z);
        return Vec3f(x * rootInv, y * rootInv, z * rootInv);
    }

    Vec3f Cross(const Vec3f& other) const
    {
        return Vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    void Print() const;
private:

};

class Vec2f
{
public:
    float x, y;
    Vec2f(): x(0), y(0) {};
    Vec2f(float x): x(x), y(x) {};
    Vec2f(float x, float y) : x(x), y(y) {};
};

class Vec2i
{
public:
    int x, y;
    Vec2i(): x(0), y(0) {};
    Vec2i(int x): x(x), y(x) {};
    Vec2i(int x, int y) : x(x), y(y) {};
};