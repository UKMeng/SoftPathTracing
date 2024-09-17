//
// Created by UKMeng on 2024/8/18.
//

#include "Sphere.h"

std::optional<HitInfo> Sphere::Intersect(const Ray& ray, float tMin, float tMax) const
{
    Vec3f co = ray.origin - center;

    float a = Dot(ray.direction, ray.direction);
    float b = 2 * Dot(ray.direction, co);
    float c = Dot(co, co) - radius * radius;
    float delta = b * b - 4 * a * c;

    if (delta < 0) return {};

    float hit_t = (-b - sqrt(delta)) * 0.5 / a;
    if (hit_t < 0)
    {
        hit_t = (-b + sqrt(delta)) * 0.5 / a;
    }
    if (hit_t > tMin && hit_t < tMax)
    {
        Vec3f hitPos = ray.HitPos(hit_t);
        Vec3f normal = Normalize(hitPos - center);
        return HitInfo { hit_t, hitPos, normal};
    }
    return {};
}

std::optional<HitInfo> Sphere::Sample(float &pdf, RNG &rng) const
{
    pdf = 1.0f / area;

    float theta = 2.0 * M_PI * rng.Uniform(), phi = M_PI * rng.Uniform();
    Vec3f dir = Vec3f(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
    return HitInfo { -1, center + radius * dir, dir };
}
