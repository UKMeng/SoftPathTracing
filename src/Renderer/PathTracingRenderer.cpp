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
            beta *= result->material->albedo / q;


            Frame frame(result->normal);
            Vec3f lightDir;

            Vec3f brdf;
            float pdf;

            if (result->material->isSpecular)
            {
                // specular
                Vec3f viewDir = frame.GetLocalFromWorld(-ray.direction);
                lightDir = {-viewDir.x, viewDir.y, -viewDir.z};
                brdf = result->material->albedo; // / lightDir.y;
                // pdf = 1;
                beta *= brdf; // * lightDir.y;
            } else
            {
                // Lambertian diffuse
                // TODO: Cosine Hemisphere Importance Sampling
                // Acceptance-Rejection Sampling
                do
                {
                    lightDir = { rng.Uniform(), rng.Uniform(), rng.Uniform() };
                    lightDir = lightDir * 2.0f - 1.0f;
                } while (lightDir.Length() > 1.0f);
                if (lightDir.y < 0) lightDir.y = -lightDir.y;
                brdf = result->material->albedo; // /PI
                // pdf = 0.5; // /PI
                // abs(Dot(lightDir, Vec3f(0, 1, 0))) = lightDir.y
                beta *= brdf * lightDir.y * 2; // / pdf;
            }
            ray = Ray(result->hitPos, frame.GetWorldFromLocal(lightDir));
        } else break;
    }

    return radiance;
}