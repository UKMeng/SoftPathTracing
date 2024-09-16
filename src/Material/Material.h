//
// Created by UKMeng on 2024/9/1.
//

#pragma once

#include "MyMath.h"
#include "RNG.h"

class Material
{
public:
    virtual ~Material() = default;

    /// Evaluate the BRDF
    /// \param wi ray direction in the local coordinate system (from hit point)
    /// \param wo view direction in the local coordinate system (from hit point)
    /// \return the BRDF value
    virtual Vec3f BRDF(const Vec3f& wi, const Vec3f& wo) = 0;

    /// Sample a new ray direction (wi) in the local coordinate system
    /// \param wo view direction in the local coordinate system
    /// \param rng random number generator
    /// \return a new ray direction (vec4f.xyz) in the local coordinate system and the pdf(vec4f.w) of the sample
    virtual Vec4f Sample(const Vec3f& wo, RNG& rng) = 0;

    inline void SetEmissive(Vec3f&& emissive)
    {
        isEmissive = true;
        this->emissive = std::move(emissive);
    }

    bool isEmissive = false;
    Vec3f emissive = {0, 0, 0};
};
