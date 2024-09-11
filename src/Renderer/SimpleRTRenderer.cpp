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
//           if (result->material->emissive.x != 0.0 && maxBounce < 30)
//           {
//               std::cout << "Bounce" << maxBounce << std::endl;
//               std::cout << "Emissive" << result->material->emissive.x << " " << result->material->emissive.y << " " << result->material->emissive.z << std::endl;
//               std::cout << "color" << color.x << " " << color.y << " " << color.z << std::endl;
//               std::cout << "beta" << beta.x << " " << beta.y << " " << beta.z << std::endl;
//           }

           color += beta * result->material->emissive;
           beta *= result->material->albedo;

           ray.origin = result->hitPos;

           Frame frame(result->normal);
           Vec3f lightDir;

           if (result->material->isSpecular)
           {
               // specular
//               std::cout << "Specular" << std::endl;
//               std::cout << "color" << color.x << " " << color.y << " " << color.z << std::endl;
//               std::cout << "beta" << beta.x << " " << beta.y << " " << beta.z << std::endl;
//               std::cout << result->material->albedo.x << result->material->albedo.y << result->material->albedo.z << std::endl;
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
           ray.direction = frame.GetWorldFromLocal(lightDir);
       } else break;
    }

    return color;
}
