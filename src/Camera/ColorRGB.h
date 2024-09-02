//
// Created by UKMeng on 2024/9/1.
//

#pragma once

#include <MyMath.h>

class ColorRGB
{
public:
    int r, g, b;
    ColorRGB(int r, int g, int b) : r(r), g(g), b(b) {}

    ColorRGB(const Vec3f& intensity)
    {
        r = Clamp(Pow(intensity.x, 1 / 2.2) * 255, 0, 255);
        g = Clamp(Pow(intensity.y, 1 / 2.2) * 255, 0, 255);
        b = Clamp(Pow(intensity.z, 1 / 2.2) * 255, 0, 255);
    }


    operator Vec3f() const
    {
        return Vec3f {
            Pow(static_cast<float>(r) / 255.f, 2.2),
            Pow(static_cast<float>(g) / 255.f, 2.2),
            Pow(static_cast<float>(b) / 255.f, 2.2),
        };
    }
};
