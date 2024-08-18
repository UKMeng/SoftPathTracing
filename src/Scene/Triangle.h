//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include "Object.h"
#include "MyMath.h"

class Triangle : public Object
{
public:
    Vec3f v0, v1, v2; // vertices, counter-clockwise order
    Vec3f e1, e2;     // 2 edges, v1-v0, v2-v0
    Vec3f normal;     // normal vector

    Triangle(Vec3f v0, Vec3f v1, Vec3f v2) : v0(v0), v1(v1), v2(v2)
    {
        e1 = v1 - v0;
        e2 = v2 - v0;
        normal = Normalize(Cross(e1, e2));
    }

    virtual std::optional<HitInfo> Intersect(const Ray& ray, float tMin, float tMax) const override;

    virtual AABB GetAABB() override;
};
