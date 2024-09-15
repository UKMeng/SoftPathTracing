//
// Created by UKMeng on 2024/8/18.
//

#include "Plane.h"

Plane::Plane(const Vec3f &centroid, const Vec3f &normal)
    : centroid(centroid), normal(Normalize(normal))
{
//    Vec3f up = Abs(normal.y) < 0.99 ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0);
//    Vec3f u = Normalize(Cross(up, normal));
//    Vec3f v = Normalize(Cross(normal, u));
//
//    Vec3f p1 = centroid + u * size.x / 2 + v * size.y / 2;
//    Vec3f p2 = centroid - u * size.x / 2 + v * size.y / 2;
//    Vec3f p3 = centroid - u * size.x / 2 - v * size.y / 2;
//    Vec3f p4 = centroid + u * size.x / 2 - v * size.y / 2;
//
//    Vec3f pMin = Vec3f::Min(p1, Vec3f::Min(p2, Vec3f::Min(p3, p4)));
//    Vec3f pMax = Vec3f::Max(p1, Vec3f::Max(p2, Vec3f::Max(p3, p4)));
//    boundingBox = AABB(pMin, pMax);
}


std::optional<HitInfo> Plane::Intersect(const Ray &ray, float tMin, float tMax) const
{
//    if (!boundingBox.HasIntersect(ray, tMin, tMax)) return {};

    float t = Dot(centroid - ray.origin, normal) / Dot(ray.direction, normal);
    if (t > tMin && t < tMax)
    {
        return HitInfo(t, ray.HitPos(t), normal);
    }
    return {};
}
