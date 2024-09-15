//
// Created by UKMeng on 2024/9/16.
//

#include "LambertianMaterial.h"

Vec3f LambertianMaterial::BRDF(const Vec3f &wi, const Vec3f &wo)
{
    return albedo * INV_PI;
}

Vec4f LambertianMaterial::Sample(const Vec3f& wo, RNG& rng)
{
    return rng.CosineSampleHemisphere( { rng.Uniform(), rng.Uniform() });
}
