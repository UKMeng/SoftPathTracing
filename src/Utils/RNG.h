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

        Vec3f H;
        H.x = SinTheta * Cos(Phi);
        H.y = CosTheta;
        H.z = SinTheta * Sin(Phi);

        float PDF = CosTheta * INV_PI;

        return Vec4f(H, PDF);
    }

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist {0.f, 1.f};
};
