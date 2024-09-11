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

Vec4f Vec3f::EularAngleToQuaternion()
{
    float phi = Radians(x);
//    float theta = Radians(y);
//    float psi = Radians(z);
    float psi = Radians(y);
    float theta = Radians(z);
    float cosX = std::cos(phi / 2), sinX = std::sin(phi / 2);
    float cosY = std::cos(theta / 2), sinY = std::sin(theta / 2);
    float cosZ = std::cos(psi / 2), sinZ = std::sin(psi / 2);
    return Vec4f {cosX * cosY * cosZ + sinX * sinY * sinZ,
                  cosX * sinY * cosZ + sinX * cosY * sinZ,
                  cosX * cosY * sinZ - sinX * sinY * cosZ,
                  sinX * cosY * cosZ - cosX * sinY * sinZ,
    };
}

