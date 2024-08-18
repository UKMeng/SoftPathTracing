//
// Created by UKMeng on 2024/8/18.
//

#pragma once
#include "Object.h"

class Plane : public Object
{
public:
    Vec3f point, normal;

    Plane(const Vec3f& point, const Vec3f& normal) : point(point), normal(Normalize(normal)) {}

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual AABB GetAABB() override;
};
