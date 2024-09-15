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
            radiance += beta * result->material->emissive;
            if (rng.Uniform() > q)
            {
                break;
            }
            beta /= q;

            Frame frame(result->normal);
            Vec3f viewDir = frame.GetLocalFromWorld(-ray.direction);
            Vec4f sample = result->material->Sample(viewDir, this->rng);
            Vec3f rayDir = sample.xyz();
            float pdf = sample.w;
            Vec3f brdf = result->material->BRDF(rayDir, viewDir);
            beta *= brdf * rayDir.y / pdf;
//            Vec3f lightDir;
//
//            Vec3f brdf;
//            float pdf;
//
//            if (result->material->isSpecular)
//            {
//                // specular
//
//                lightDir = {-viewDir.x, viewDir.y, -viewDir.z};
//                brdf = result->material->albedo; // / lightDir.y;
//                // pdf = 1;
//                beta *= brdf; // * lightDir.y;
//            }
//            else
//            {
//                // Lambertian diffuse
//                // Cosine Hemisphere Importance Sampling
//                Vec4f sample = rng.CosineSampleHemisphere({rng.Uniform(), rng.Uniform() });
//                lightDir = sample.xyz();
//                pdf = sample.w;
//                brdf = result->material->albedo * INV_PI;
//
//            }
            ray = Ray(result->hitPos, frame.GetWorldFromLocal(rayDir));
        }
        else break;
    }

    return radiance;
}