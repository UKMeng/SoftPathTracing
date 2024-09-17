//
// Created by UKMeng on 2024/9/16.
//

#include "SpecularMaterial.h"


Vec3f SpecularMaterial::BRDF(const Vec3f &wi, const Vec3f &wo)
{
    return albedo / wi.y;
}

Vec4f SpecularMaterial::Sample(const Vec3f &wo, const Vec2f& xi)
{
    return Vec4f{-wo.x, wo.y, -wo.z, 1.0f};
}