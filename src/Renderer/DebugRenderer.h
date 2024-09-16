//
// Created by UKMeng on 2024/9/11.
//

#pragma once
#include "Renderer.h"

class DebugBTCRenderer: public Renderer
{
public:
    DebugBTCRenderer(Camera& camera, Scene& scene) : Renderer(camera, scene) {}
private:
    Vec3f RenderPixel(const Vec2i& pixelCoords, const Vec2f& xi) override;
};

class DebugTTCRenderer: public Renderer
{
public:
    DebugTTCRenderer(Camera& camera, Scene& scene) : Renderer(camera, scene) {}
private:
    Vec3f RenderPixel(const Vec2i& pixelCoords, const Vec2f& xi) override;
};