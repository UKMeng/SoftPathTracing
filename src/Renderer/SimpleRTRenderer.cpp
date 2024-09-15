//
// Created by UKMeng on 2024/9/3.
//

#include "SimpleRTRenderer.h"
#include "Frame.h"

Vec3f SimpleRTRenderer::RenderPixel(const Vec2i &pixelCoords)
{
    auto ray = camera.GenerateRay(pixelCoords, { rng.Uniform(), rng.Uniform() });
    Vec3f beta = {1, 1, 1}; // total albedo
    Vec3f color = {0, 0, 0};
    size_t maxBounce = 32;

    while (maxBounce--)
    {
       auto result = scene.Intersect(ray);
       if (result.has_value())
       {
           color += beta * result->material->emissive;
           beta *= result->material->albedo;

//           ray.origin = result->hitPos;

           Frame frame(result->normal);
           Vec3f lightDir;

           if (result->material->isSpecular)
           {
               // specular
               Vec3f viewDir = frame.GetLocalFromWorld(-ray.direction);
               lightDir = {-viewDir.x, viewDir.y, -viewDir.z};
           } else
           {
               // diffuse
               // Acceptance-Rejection Sampling
               do
               {
                   lightDir = { rng.Uniform(), rng.Uniform(), rng.Uniform() };
                   lightDir = lightDir * 2.0f - 1.0f;
               } while (lightDir.Length() > 1.0f);
               if (lightDir.y < 0) lightDir.y = -lightDir.y;

           }
           ray = Ray(result->hitPos, frame.GetWorldFromLocal(lightDir));
       } else break;
    }

    return color;
}
