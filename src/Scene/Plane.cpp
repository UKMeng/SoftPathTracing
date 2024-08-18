//
// Created by UKMeng on 2024/8/18.
//

#include "Plane.h"

std::optional<HitInfo> Plane::Intersect(const Ray &ray, float tMin, float tMax) const
{
    float t = Dot(point - ray.origin, normal) / Dot(ray.direction, normal);
    if (t > tMin && t < tMax)
    {
        return HitInfo(t, ray.HitPos(t), normal);
    }
    return {};
}

AABB Plane::GetAABB()
{
    return AABB();
}
