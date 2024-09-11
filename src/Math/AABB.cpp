//
// Created by UKMeng on 2024/8/18.
//

#include "AABB.h"

bool AABB::HasIntersect(const Ray &ray, float tMin, float tMax) const
{
    float tMin1 = (pMin.x - ray.origin.x) * ray.directionInv.x;
    float tMax1 = (pMax.x - ray.origin.x) * ray.directionInv.x;
    if (ray.dirIsNeg[0]) std::swap(tMin1, tMax1);

    float tMin2 = (pMin.y - ray.origin.y) * ray.directionInv.y;
    float tMax2 = (pMax.y - ray.origin.y) * ray.directionInv.y;
    if (ray.dirIsNeg[1]) std::swap(tMin2, tMax2);

    float tMin3 = (pMin.z - ray.origin.z) * ray.directionInv.z;
    float tMax3 = (pMax.z - ray.origin.z) * ray.directionInv.z;
    if (ray.dirIsNeg[2]) std::swap(tMin3, tMax3);

    float tEnter = std::max({tMin1, tMin2, tMin3});
    float tExit = std::min({tMax1, tMax2, tMax3});

    if (tEnter <= tMin && tExit >= tMax) return false;

    tEnter = std::max(tEnter, tMin);
    tExit = std::min(tExit, tMax);

    return (tEnter <= tExit) && (tExit >= 0.0);
}

AABB AABB::Transform(const Mat4f &m) const
{
    Vec3f worldMin = Vec3f(std::numeric_limits<float>::max());
    Vec3f worldMax = Vec3f(-std::numeric_limits<float>::max());
    for (int i = 0; i < 7; i++)
    {
        Vec3f corner = m * Vec4f(GetCornerPoint(i), 1.0f);
        worldMin = Vec3f::Min(worldMin, corner);
        worldMax = Vec3f::Max(worldMax, corner);
    }
    return AABB(worldMin, worldMax);
}
