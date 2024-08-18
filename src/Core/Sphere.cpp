//
// Created by UKMeng on 2024/8/18.
//

#include "Sphere.h"

std::optional<float> Sphere::Intersect(const Ray &ray) const
{
    Vec3f co = ray.origin - m_Center;

    float b = 2 * ray.direction.Dot(co);
    float c = co.Dot(co) - m_Radius * m_Radius;
    float delta = b * b - 4 * c;

    if (delta < 0) return {};

    float hit_t = (-b - sqrt(delta)) * 0.5;
    if (hit_t < 0)
    {
        hit_t = (-b + sqrt(delta)) * 0.5;
    }
    if (hit_t > 0)
    {
        return hit_t;
    }
    return {};
}
