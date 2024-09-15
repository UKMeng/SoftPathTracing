//
// Created by UKMeng on 2024/9/16.
//

#pragma once

#include "Material.h"

class LambertianMaterial : public Material
{
public:
    LambertianMaterial(const Vec3f& albedo) : albedo(albedo) {}

    virtual Vec3f BRDF(const Vec3f& wi, const Vec3f& wo) override;
    virtual Vec4f Sample(const Vec3f& wo, RNG& rng) override;
private:
    Vec3f albedo;
};
