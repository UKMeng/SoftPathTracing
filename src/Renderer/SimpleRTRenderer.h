//
// Created by UKMeng on 2024/9/3.
//

#pragma once

#include "Renderer.h"

class SimpleRTRenderer : public Renderer
{
public:
    SimpleRTRenderer(Camera& camera, Scene& scene) : Renderer(camera, scene) {}
private:
    Vec3f RenderPixel(const Vec2i& pixelCoords) override;
};
