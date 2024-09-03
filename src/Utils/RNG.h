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

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist {0.f, 1.f};
};
