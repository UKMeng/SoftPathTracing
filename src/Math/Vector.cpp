//
// Created by UKMeng on 2024/8/17.
//

#include "Vector.h"

void Vec3f::Print() const
{
    std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
}

Vec3f Vec3f::Normalize(const Vec3f &a)
{
    return a.Normalize();
}
