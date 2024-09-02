//
// Created by UKMeng on 2024/9/2.
//

#pragma once

#include "MyMath.h"

// Local Reflection Space
class Frame
{
public:
    Frame(const Vec3f& normal);

    Vec3f GetLocalFromWorld(const Vec3f& worldDir) const;
    Vec3f GetWorldFromLocal(const Vec3f& localDir) const;
private:
    Vec3f xAxis, yAxis, zAxis;
};
