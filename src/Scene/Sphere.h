//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include <optional>

#include "Object.h"
#include "MyMath.h"

class Sphere : public Object
{
public:
    Vec3f center;
    float radius;

    Sphere(const Vec3f& center, float radius) : center(center), radius(radius) {}

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual AABB GetAABB() override;

};
