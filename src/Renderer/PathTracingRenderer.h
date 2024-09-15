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
    Vec3f RenderPixel(const Vec2i& pixelCoords) override;
};