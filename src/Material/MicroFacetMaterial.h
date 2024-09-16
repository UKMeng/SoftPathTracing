//
// Created by UKMeng on 2024/9/16.
//

#pragma once

#include "Material.h"

class MicroFacetMaterial : public Material
{
public:
    MicroFacetMaterial(Vec3f albedo, float roughness, float metallic) : albedo(albedo), roughness(roughness), metallic(metallic) {}

    virtual Vec3f BRDF(const Vec3f& wi, const Vec3f& wo) override;
    virtual Vec4f Sample(const Vec3f& wo, RNG& rng) override;

private:
    float GeometrySchlickGGX(float NdotV);
    float GeometrySmith(float NdotV, float NdotL);
    Vec3f FresnelSchlick(float VdotH, Vec3f F0);
    float DistributionGGX(float NdotH);
private:
    Vec3f albedo;
    float roughness;
    float metallic;
};
