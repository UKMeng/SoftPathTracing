//
// Created by UKMeng on 2024/9/16.
//

#include "LambertianMaterial.h"

Vec3f LambertianMaterial::BRDF(const Vec3f &wi, const Vec3f &wo)
{
    return albedo * INV_PI;
}

Vec4f LambertianMaterial::Sample(const Vec3f& wo, const Vec2f& xi)
{
    return RNG::CosineSampleHemisphere(xi);
}

float LambertianMaterial::PDF(const Vec3f &wi, const Vec3f &wo)
{
    return wi.y * INV_PI;
}
