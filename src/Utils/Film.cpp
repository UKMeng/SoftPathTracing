//
// Created by UKMeng on 2024/8/17.
//

#include "Film.h"

Film::Film(size_t width, size_t height) : m_Width(width), m_Height(height)
{
    m_Pixels.resize(width * height);
}

void Film::Save(const std::filesystem::path &filename)
{
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << m_Width << " " << m_Height << "\n255\n";  // use P6 standard PPM format save binary data
    for (size_t y = 0; y < m_Height; ++y) {
        for (size_t x = 0; x < m_Width; ++x) {
            const Vec3& color = GetPixel(x, y);
            uint8_t r = static_cast<uint8_t>(clamp(color.x, 0.0, 1.0) * 255.0);
            uint8_t g = static_cast<uint8_t>(clamp(color.y, 0.0, 1.0) * 255.0);
            uint8_t b = static_cast<uint8_t>(clamp(color.z, 0.0, 1.0) * 255.0);
            file << r << g << b;
        }
    }
}
