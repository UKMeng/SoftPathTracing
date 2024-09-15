//
// Created by UKMeng on 2024/8/18.
//

#include "Triangle.h"

std::optional<HitInfo> Triangle::Intersect(const Ray &ray, float tMin, float tMax) const
{
    // Moller-Trumbore Algorithm
    Vec3f s = ray.origin - v0;
    Vec3f s1 = Cross(ray.direction, e2);
    Vec3f s2 = Cross(s, e1);

    float det = Dot(s1, e1);
    float detInv = 1.0 / det;

    float u = Dot(s1, s) * detInv;
    if (u < 0 || u > 1) return {};

    float v = Dot(s2, ray.direction) * detInv;
    if (v < 0 || u + v > 1) return {};

    float t = Dot(s2, e2) * detInv;
    if (t > tMin && t < tMax)
    {
        Vec3f hitPos = ray.HitPos(t);
        return HitInfo { t, hitPos, normal };
    }

    return {};
}
