//
// Created by UKMeng on 2024/9/10.
//

#pragma once

#include "Renderer.h"

class PathTracingRenderer : public Renderer
{
public:
    PathTracingRenderer(Camera& camera, Scene& scene) : Renderer(camera, scene) {}
private:
    Vec3f RenderPixel(const Vec2i& pixelCoords, const Vec2f& xi, const size_t& currentSpp) override;

    Vec3f CastRay(const Ray &ray, int depth); // use max depth to terminate the ray
    Vec3f RRCastRay(const Ray& ray, int depth); // use Russian Roulette to terminate the ray
    Vec3f MISCastRay(const Ray& ray, int depth); // use Multiple Importance Sampling
    float MISMixWeight(const float& a, const float &b)
    {
        float t = a * a;
        return t / (t + b * b);
    }

    // Russian Roulette
    float P_RR = 0.8f;

    // Max depth
    int maxDepth = 4;
    size_t m_Spp;
    Vec2i m_PixelCoords;
};