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

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist {0.f, 1.f};
};
