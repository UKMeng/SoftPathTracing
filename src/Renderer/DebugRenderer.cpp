//
// Created by UKMeng on 2024/9/11.
//

#include "DebugRenderer.h"
#include "ColorRGB.h"

Vec3f DebugBTCRenderer::RenderPixel(const Vec2i &pixelCoords)
{
#ifdef DEBUG
    auto ray = camera.GenerateRay(pixelCoords);
    auto result = scene.Intersect(ray);
    if (result.has_value())
    {
        return ColorRGB::GenerateHeatMapRGB(result->boundsTestCount / 200.f);
    }
    return {};
#else
    return {};
#endif
}

Vec3f DebugTTCRenderer::RenderPixel(const Vec2i &pixelCoords)
{
#ifdef DEBUG
    auto ray = camera.GenerateRay(pixelCoords);
    auto result = scene.Intersect(ray);
    if (result.has_value())
    {
        return ColorRGB::GenerateHeatMapRGB(result->triangleTestCount / 640.f);
    }
    return {};
#else
    return {};
#endif
}
