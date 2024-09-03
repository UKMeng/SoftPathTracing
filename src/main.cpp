#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"
#include "Camera.h"
#include "Sphere.h"
#include "ProgressBar.h"
#include "Model.h"
#include "Plane.h"
#include "Scene.h"
#include "Frame.h"
#include "ColorRGB.h"

int main()
{
    const int width = 192 * 4;
    const int height = 108 * 4;

    Film film { width, height };

    ProgressBar progress { width * height };

    Camera camera { film, { -3.6, 0.0, 0 }, { 0, 0, 0 }, 45};

    Sphere sphere { {0, 0, 0}, 1.0f};

    Plane plane { {0, 0, 0}, {0, 1, 0}};

    Model model("models/simple_dragon.obj");

    ThreadPool threadPool {};

    Vec3f lightPos {-1, 2, 1};

    Scene scene {};

    scene.AddObject(model, {ColorRGB(202,159,117)}, {0, 0, 0}, {0, 0, 0}, {1, 3, 2});
    scene.AddObject(
            sphere,
            {{1, 1, 1}, false, ColorRGB(255, 128, 128)},
            {0, 0.0, 2.5}
            );

    scene.AddObject(
            sphere,
            {{1, 1, 1}, false, ColorRGB(128, 128, 255)},
            {0, 0.0, -2.5}
            );

    scene.AddObject(
            sphere,
            {{1, 1, 1}, true },
            {3, 0.5, -2}
            );

    scene.AddObject(plane, {ColorRGB(120, 204, 157)}, { 0, -0.5, 0});

    int spp = 8;

    threadPool.ParallelFor(film.GetWidth(), film.GetHeight(),
                           [&](size_t x, size_t y)
                           {
                               for (int i = 0; i < spp; ++i)
                               {
                                   auto ray = camera.GenerateRay(Vec2i(x, y), { Abs(GetRandomFloat()), Abs(GetRandomFloat()) });

                                   Vec3f beta = {1, 1, 1}; // total albedo
                                   Vec3f color = {0, 0, 0};

                                   while (true)
                                   {
                                       auto result = scene.Intersect(ray);
                                       if (result.has_value())
                                       {
                                           color += beta * result->material->emissive;
                                           beta *= result->material->albedo;

                                           ray.origin = result->hitPos;

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
                                                   lightDir = {GetRandomFloat(), GetRandomFloat(), GetRandomFloat()};
                                               } while (lightDir.Length() > 1.0f);
                                               if (lightDir.y < 0) lightDir.y = -lightDir.y;

                                           }
                                           ray.direction = frame.GetWorldFromLocal(lightDir);
                                       } else break;
                                   }

                                   film.AddSample(x, y, color);
                               }

//                               if (result.has_value())
//                               {
//                                   Vec3f hitPos = result->hitPos;
//                                   Vec3f normal = result->normal;
//                                   Vec3f lightDir = Normalize(lightPos - hitPos);
//                                   float cosine = std::max(0.0f, Dot(normal, lightDir));
//
//                               }
                               progress.Update(1);
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
