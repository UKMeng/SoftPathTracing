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
    float area;
    AABB bounds;

    Sphere(const Vec3f& center, float radius) : center(center), radius(radius)
    {
        bounds = AABB(Vec3f {center.x - radius, center.y - radius, center.z - radius},
                      Vec3f {center.x + radius, center.y + radius, center.z + radius});
        area = 4 * M_PI * radius * radius;
    }

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual std::optional<HitInfo> Sample(RNG& rng) const override;

    virtual AABB GetAABB() const override
    {
        return bounds;
    }

    virtual float GetArea() const override
    {
        return area;
    }
};
