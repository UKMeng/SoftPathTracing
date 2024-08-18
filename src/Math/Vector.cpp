//
// Created by UKMeng on 2024/8/17.
//

#include "Vector.h"

void Vec3f::Print() const
{
    std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
}

Vec3f Vec3f::Min(const Vec3f &a, const Vec3f &b)
{
    return Vec3f(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vec3f Vec3f::Max(const Vec3f &a, const Vec3f &b)
{
    return Vec3f(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}
