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

int main()
{
    const int width = 1920;
    const int height = 1080;

    Film film { width, height };

    ProgressBar progress { width * height };

    Camera camera { film, { -1.6, 0.0, 0 }, { 0, 0, 0 }, 90};

    Sphere sphere { {0, 0, 0}, 0.5f};

    Plane plane { {0, 0, 0}, {0, 1, 0}};

    Model model("models/simple_dragon.obj");

    ThreadPool threadPool {};

    Vec3f lightPos {-1, 2, 1};

    Scene scene {};

    scene.AddObject(&model, {0, 0, 0}, {0, 45, 0}, {1, 3, 2});
    scene.AddObject(&sphere, {0, 0.0, 1.6}, {0, 0, 0}, {0.3, 0.3, 0.3});

    scene.AddObject(&plane, { 0, -0.5, 0});

    threadPool.ParallelFor(film.GetWidth(), film.GetHeight(),
                           [&](size_t x, size_t y)
                           {
                               auto ray = camera.GenerateRay(Vec2i(x, y));
                               auto result = scene.Intersect(ray);
                               if (result.has_value())
                               {
                                   Vec3f hitPos = result->hitPos;
                                   Vec3f normal = result->normal;
                                   Vec3f lightDir = Normalize(lightPos - hitPos);
                                   float cosine = std::max(0.0f, Dot(normal, lightDir));
                                   film.SetPixel(x, y, {cosine, cosine, cosine});
                               }
                               progress.Update(1);
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
