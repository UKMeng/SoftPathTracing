//
// Created by UKMeng on 2024/9/3.
//

#pragma once

#include "Renderer.h"

class NormalRenderer: public Renderer
{
public:
    NormalRenderer(Camera& camera, Scene& scene) : Renderer(camera, scene) {}
private:
    Vec3f RenderPixel(const Vec2i& pixelCoords) override;
};
