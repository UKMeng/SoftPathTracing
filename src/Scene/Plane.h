//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include "Object.h"

class Plane : public Object
{
public:
    Vec3f centroid, normal;
    Vec2f size;
    AABB boundingBox;
    float area;
    Vec3f u, v;

     Plane(const Vec3f& centroid, const Vec3f& normal, const Vec2f& size);
    // Plane(const Vec3f& centroid, const Vec3f& normal);

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual std::optional<HitInfo> Sample(float& pdf, RNG& rng) const override;

    virtual AABB GetAABB() const override
    {
//        return AABB();
        return boundingBox;
    }

    virtual float GetArea() const override
    {
        return area;
    }
};
