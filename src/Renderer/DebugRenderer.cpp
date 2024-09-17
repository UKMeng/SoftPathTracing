//
// Created by UKMeng on 2024/9/11.
//

#include "DebugRenderer.h"
#include "ColorRGB.h"

Vec3f DebugBTCRenderer::RenderPixel(const Vec2i &pixelCoords, const Vec2f& xi, const size_t& currentSpp)
{
#ifdef DEBUG
    auto ray = camera.GenerateRay(pixelCoords);
    auto result = scene.Intersect(ray);
    return ColorRGB::GenerateHeatMapRGB(ray.boundsTestCount / 150.f);
#else
    return {};
#endif
}

Vec3f DebugTTCRenderer::RenderPixel(const Vec2i &pixelCoords, const Vec2f& xi, const size_t& currentSpp)
{
#ifdef DEBUG
    auto ray = camera.GenerateRay(pixelCoords);
    auto result = scene.Intersect(ray);
    return ColorRGB::GenerateHeatMapRGB(ray.triangleTestCount / 7.f);
#else
    return {};
#endif
}
