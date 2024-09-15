//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include "MyMath.h"
#include "Material.h"

struct Ray
{
    Vec3f origin, direction, directionInv;
    std::array<int, 3> dirIsNeg;

#ifdef DEBUG
    mutable size_t boundsTestCount = 0;
    mutable size_t triangleTestCount = 0;
#endif

    Ray(Vec3f ori, Vec3f dir): origin(ori), direction(dir)
    {
        directionInv = Vec3f(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
        dirIsNeg = {direction.x < 0, direction.y < 0, direction.z < 0};
    };

    Vec3f HitPos(float t) const { return origin + t * direction; }

    Ray RayFromWorldToModel(const Mat4f& worldToModel) const;
};

struct HitInfo
{
    float t;
    Vec3f hitPos;
    Vec3f normal;
    Material* material = nullptr;
};
