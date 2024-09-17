//
// Created by UKMeng on 2024/9/3.
//

#include "NormalRenderer.h"

Vec3f NormalRenderer::RenderPixel(const Vec2i &pixelCoords, const Vec2f& xi, const size_t& currentSpp)
{
    auto ray = camera.GenerateRay(pixelCoords);
    auto result = scene.Intersect(ray);
    if (result.has_value())
    {
        return result->normal * 0.5f + 0.5f;
    }
    return {};
}
