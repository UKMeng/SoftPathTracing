//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include <optional>

#include "Ray.h"
#include "AABB.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    virtual std::optional<HitInfo> Intersect(const Ray &ray, float tMin = 1e-5, float tMax = std::numeric_limits<float>::infinity()) const = 0;
    virtual AABB GetAABB() = 0;
};
