//
// Created by UKMeng on 2024/8/17.
//

#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "MyMath.h"

struct Pixel
{
    Vec3f color { 0, 0, 0 };
    int sampleCount { 0 };
};

class Film
{
public:
    Film(size_t width, size_t height);

    void Save(const std::filesystem::path &filename);

    size_t GetWidth() const { return m_Width; }
    size_t GetHeight() const { return m_Height; }
    Pixel GetPixel(size_t x, size_t y) { return m_Pixels[y * m_Width + x]; }
    void AddSample(size_t x, size_t y, const Vec3f& color)
    {
        m_Pixels[y * m_Width + x].color += color;
        m_Pixels[y * m_Width + x].sampleCount++;
    }

    void Clear()
    {
        m_Pixels.clear();
        m_Pixels.resize(m_Width * m_Height);
    }

private:
    size_t m_Width, m_Height;
    std::vector<Pixel> m_Pixels;
};
