//
// Created by UKMeng on 2024/9/3.
//

#pragma once

#include <random>

class RNG
{
public:
    RNG()
    {
        std::random_device dev;
        SetSeed(dev());
    }
    RNG(size_t seed) { SetSeed(seed); }

    void SetSeed(size_t seed) { gen.seed(seed); }
    float Uniform() { return dist(gen); }

    Vec4f CosineSampleHemisphere(Vec2f E)
    {
        float Phi = 2 * M_PI * E.x;
        float CosTheta = Sqrt(E.y);
        float SinTheta = Sqrt(1 - CosTheta * CosTheta);

        float x = SinTheta * Cos(Phi);
        float y = CosTheta;
        float z = SinTheta * Sin(Phi);

        float PDF = CosTheta * INV_PI;

        return Vec4f{ x, y, z, PDF };
    }

    /// Importance Sampling GGX
    /// \param E two random numbers between 0 and 1
    /// \param a2 roughness * roughness
    /// \return Sampling direction and PDF
    Vec4f ImportanceSampleGGX(Vec2f E, float a2)
    {
        float Phi = 2.0f * M_PI * E.x;
        float CosTheta = Sqrt((1.0f - E.y) / (1.0f + (a2 - 1.0f) * E.y));
        float SinTheta = Sqrt(1.0f - CosTheta * CosTheta);

        float x = SinTheta * Cos(Phi);
        float y = CosTheta;
        float z = SinTheta * Sin(Phi);

        float d = (a2 * CosTheta - CosTheta) * CosTheta + 1;
        float D = a2 / (M_PI * d * d);
        float PDF = D * CosTheta;

        return Vec4f {x, y, z, PDF};
    }

    // reference: https://learnopengl-cn.github.io/07%20PBR/03%20IBL/02%20Specular%20IBL/
    // Hammersley Sequence
    float RadicalInverse_VdC(uint32_t bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }

    Vec2f Hammersley(uint32_t i, uint32_t N)
    {
        return Vec2f(float(i)/float(N), RadicalInverse_VdC(i));
    }

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist {0.f, 1.f};
};
