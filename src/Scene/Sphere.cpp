//
// Created by UKMeng on 2024/8/18.
//

#include "Sphere.h"

std::optional<HitInfo> Sphere::Intersect(const Ray& ray, float tMin, float tMax) const
{
    Vec3f co = ray.origin - center;

    float b = 2 * ray.direction.Dot(co);
    float c = co.Dot(co) - radius * radius;
    float delta = b * b - 4 * c;

    if (delta < 0) return {};

    float hit_t = (-b - sqrt(delta)) * 0.5;
    if (hit_t < 0)
    {
        hit_t = (-b + sqrt(delta)) * 0.5;
    }
    if (hit_t > tMin && hit_t < tMax)
    {
        Vec3f hitPos = ray.HitPos(hit_t);
        Vec3f normal = Normalize(hitPos - center);
        return HitInfo { hit_t, hitPos, normal};
    }
    return {};
}

AABB Sphere::GetAABB()
{
    return AABB(Vec3f {center.x - radius, center.y - radius, center.z - radius},
                Vec3f {center.x + radius, center.y + radius, center.z + radius});
}
