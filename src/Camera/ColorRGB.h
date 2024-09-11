//
// Created by UKMeng on 2024/9/1.
//

#pragma once

#include "MyMath.h"

class ColorRGB
{
public:
    int r, g, b;
    ColorRGB(int r, int g, int b) : r(r), g(g), b(b) {}

    ColorRGB(const Vec3f& intensity)
    {
        // Gamma correction
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

    inline static ColorRGB ColorLerp(const ColorRGB& a, const ColorRGB& b, float t)
    {
        return ColorRGB{
            static_cast<int>(Clamp(Lerp(a.r, b.r, t), 0, 255)),
            static_cast<int>(Clamp(Lerp(a.g, b.g, t), 0, 255)),
            static_cast<int>(Clamp(Lerp(a.b, b.b, t), 0, 255))
        };
    }

    inline static ColorRGB GenerateHeatMapRGB(float t)
    {
        // reference: https://github.com/HeaoYe/CPUPathTracing/blob/main/source/include/util/rgb.hpp
        std::array<ColorRGB, 25> colorPallet {
                ColorRGB { 68, 1, 84 },
                ColorRGB { 71, 17, 100 },
                ColorRGB { 72, 31, 112 },
                ColorRGB { 71, 45, 123 },
                ColorRGB { 68, 58, 131 },

                ColorRGB { 64, 70, 136 },
                ColorRGB { 59, 82, 139 },
                ColorRGB { 54, 93, 141 },
                ColorRGB { 49, 104, 142 },
                ColorRGB { 44, 114, 142 },

                ColorRGB { 40, 124, 142 },
                ColorRGB { 36, 134, 142 },
                ColorRGB { 33, 144, 140 },
                ColorRGB { 31, 154, 138 },
                ColorRGB { 32, 164, 134 },

                ColorRGB { 39, 173, 129 },
                ColorRGB { 53, 183, 121 },
                ColorRGB { 71, 193, 110 },
                ColorRGB { 93, 200, 99 },
                ColorRGB { 117, 208, 84 },

                ColorRGB { 143, 215, 68 },
                ColorRGB { 170, 220, 50 },
                ColorRGB { 199, 224, 32 },
                ColorRGB { 227, 228, 24 },
                ColorRGB { 253, 231, 37 },
        };

        if (t < 0.0f || t >= 1.0f) return ColorRGB(255, 0, 0);

        float index = t * colorPallet.size();
        int i = static_cast<int>(index);
        return ColorLerp(colorPallet[i], colorPallet[i + 1], index - i);
    }
};
