//
// Created by UKMeng on 2024/8/18.
//

#pragma once

#include <optional>

#include "Ray.h"
#include "MyMath.h"

class Sphere
{
public:
    Sphere(const Vec3f& center, float radius) : m_Center(center), m_Radius(radius) {}
    std::optional<float> Intersect(const Ray& ray) const;
private:
    Vec3f m_Center;
    float m_Radius;
};
