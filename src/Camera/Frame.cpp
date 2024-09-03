//
// Created by UKMeng on 2024/9/2.
//

#include "Frame.h"

Frame::Frame(const Vec3f &normal)
{
    yAxis = normal;
    Vec3f up = Abs(normal.y) < 0.999f ? Vec3f(0.0f, 1.0f, 0.0f) : Vec3f(0.0f, 0.0f, 1.0f); // in case normal is parallel to y-axis
    xAxis = Normalize(Cross(up, yAxis));
    zAxis = Normalize(Cross(xAxis, yAxis));
}

Vec3f Frame::GetLocalFromWorld(const Vec3f &worldDir) const
{
    return Vec3f(
            Dot(worldDir, xAxis),
            Dot(worldDir, yAxis),
            Dot(worldDir, zAxis)).Normalize();
}

Vec3f Frame::GetWorldFromLocal(const Vec3f &localDir) const
{
    return Normalize(localDir.x * xAxis + localDir.y * yAxis + localDir.z * zAxis);
}
