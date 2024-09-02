//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include "MyMath.h"
#include "Material.h"

struct Ray
{
    Vec3f origin, direction;

    Ray(Vec3f ori, Vec3f dir): origin(ori), direction(dir) {};

    Vec3f HitPos(float t) const { return origin + t * direction; }

    Ray RayFromWorldToModel(const Mat4f& worldToModel) const;
};

struct HitInfo
{
    float t;
    Vec3f hitPos;
    Vec3f normal;
    const Material* material = nullptr;
};
