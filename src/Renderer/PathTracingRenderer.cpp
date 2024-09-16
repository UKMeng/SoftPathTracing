//
// Created by UKMeng on 2024/9/10.
//

#include "PathTracingRenderer.h"
#include "Frame.h"

Vec3f PathTracingRenderer::RenderPixel(const Vec2i &pixelCoords)
{
    auto ray = camera.GenerateRay(pixelCoords, { rng.Uniform(), rng.Uniform() });
    Vec3f beta = {1, 1, 1}; // total albedo
    Vec3f radiance = {0, 0, 0};

    float q = 0.8f; // Ruassian Roulette

    while (true)
    {
        auto result = scene.Intersect(ray);
        if (result.has_value())
        {
            if (result->material->isEmissive)
            {
                radiance += beta * result->material->emissive;
                break;
            }

            if (rng.Uniform() > q)
            {
                break;
            }
            beta /= q;

            if (result->material)
            {
                Frame frame(result->normal);
                Vec3f viewDir = frame.GetLocalFromWorld(-ray.direction);
                Vec4f sample = result->material->Sample(viewDir, this->rng);
                Vec3f rayDir = sample.xyz();
                float pdf = sample.w;
                Vec3f brdf = result->material->BRDF(rayDir, viewDir);
                beta *= brdf * rayDir.y / pdf;
                ray = Ray(result->hitPos, frame.GetWorldFromLocal(rayDir));
            }
        }
        else break;
    }

    return radiance;
}
