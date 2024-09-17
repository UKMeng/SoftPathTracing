//
// Created by UKMeng on 2024/9/3.
//

#pragma once

#include "Camera.h"
#include "Scene.h"
#include "RNG.h"
#include "MyMath.h"

class Renderer
{
public:
    Renderer(Camera& camera, Scene& scene) : camera(camera), scene(scene) {}
    virtual ~Renderer() = default;
    void Render(size_t spp, const std::filesystem::path& fileName);
private:
    virtual Vec3f RenderPixel(const Vec2i& pixelCoords, const Vec2f& xi) = 0;

protected:
    Camera& camera;
    Scene& scene;
    size_t spp;
    RNG rng {};
};
