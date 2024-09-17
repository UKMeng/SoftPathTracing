//
// Created by UKMeng on 2024/9/16.
//

#include "MicroFacetMaterial.h"

Vec3f MicroFacetMaterial::BRDF(const Vec3f &wi, const Vec3f &wo)
{
    float NdotL = wi.y, NdotV = wo.y;
    Vec3f H = Normalize(wi + wo);
    float VdotH = Max(Dot(wo, H), 0.0f);

    Vec3f F0 = Vec3f(0.04f);
    F0 = Mix(F0, albedo, metallic);

    float NDF = DistributionGGX(H.y);
    float G = GeometrySmith(NdotV, NdotL);
    Vec3f F = FresnelSchlick(VdotH, F0);

    Vec3f numerator = NDF * G * F;
    float denominator = Max(4.0f * NdotL * NdotV, 0.01f);

    return numerator / denominator;
}

Vec4f MicroFacetMaterial::Sample(const Vec3f &wo, const Vec2f& xi)
{
    return RNG::ImportanceSampleGGX(xi, roughness * roughness);
}

float MicroFacetMaterial::GeometrySchlickGGX(float NdotV)
{
    float k = (roughness + 1) * (roughness + 1) * 0.125f;

    return NdotV / (NdotV * (1 - k) + k);
}

float MicroFacetMaterial::GeometrySmith(float NdotV, float NdotL)
{
    float ggx1 = GeometrySchlickGGX(NdotV);
    float ggx2 = GeometrySchlickGGX(NdotL);
    return ggx1 * ggx2;
}

Vec3f MicroFacetMaterial::FresnelSchlick(float VdotH, Vec3f F0)
{
    return F0 + (1.0f - F0) * Pow(1 - VdotH, 5);
}

float MicroFacetMaterial::DistributionGGX(float NdotH)
{
    float a2 = roughness * roughness;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0f;
    return a2 / (M_PI * d * d);
}
