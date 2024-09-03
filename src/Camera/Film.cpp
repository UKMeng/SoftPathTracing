//
// Created by UKMeng on 2024/8/17.
//

#include "Film.h"
#include "ColorRGB.h"
#include "ThreadPool.h"

Film::Film(size_t width, size_t height) : m_Width(width), m_Height(height)
{
    m_Pixels.resize(width * height);
}

void Film::Save(const std::filesystem::path &filename)
{
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << m_Width << " " << m_Height << "\n255\n";  // use P6 standard PPM format save binary data

    std::vector<uint8_t> buffer(m_Width * m_Height * 3);

    g_ThreadPool.ParallelFor(m_Width, m_Height, [&](size_t x, size_t y)
    {
        auto pixel = GetPixel(x, y);
        ColorRGB rgb(pixel.color / static_cast<float>(pixel.sampleCount));
        size_t index = (y * m_Width + x) * 3;
        buffer[index + 0] = static_cast<uint8_t>(rgb.r);
        buffer[index + 1] = static_cast<uint8_t>(rgb.g);
        buffer[index + 2] = static_cast<uint8_t>(rgb.b);
    }, false);

    g_ThreadPool.Wait();

    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}
