//
// Created by UKMeng on 2024/8/17.
//

#include "Film.h"
#include "RGB.h"

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
            RGB rgb(GetPixel(x, y));
//            const Vec3f& color = GetPixel(x, y);
//            uint8_t r = static_cast<uint8_t>(Clamp(color.x, 0.0, 1.0) * 255.0);
//            uint8_t g = static_cast<uint8_t>(Clamp(color.y, 0.0, 1.0) * 255.0);
//            uint8_t b = static_cast<uint8_t>(Clamp(color.z, 0.0, 1.0) * 255.0);
            file << static_cast<uint8_t>(rgb.r) << static_cast<uint8_t>(rgb.g) << static_cast<uint8_t>(rgb.b);
        }
    }
}
