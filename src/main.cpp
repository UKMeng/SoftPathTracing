#include <iostream>
#include <atomic>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"
#include "Camera.h"
#include "Sphere.h"
#include "ProgressBar.h"
#include "Model.h"

int main()
{
    const int width = 1920;
    const int height = 1080;

    Film film { width, height };

    Camera camera { film, { -1, 0, 0 }, { 0, 0, 0 }, 90};

    Sphere sphere { {0, 0, 0}, 0.5f};

    Model bunny("models/simple_dragon.obj");

    ThreadPool threadPool {};

    Vec3f lightPos {-1, 2, 1};

    std::atomic<int> finishedCount = 0;

    Object& obj = bunny;

    threadPool.ParallelFor(film.GetWidth(), film.GetHeight(),
                           [&](size_t x, size_t y)
                           {
                               auto ray = camera.GenerateRay(Vec2i(x, y));
                               auto result = obj.Intersect(ray);
                               if (result.has_value())
                               {
                                   Vec3f hitPos = result->hitPos;
                                   Vec3f normal = result->normal;
                                   Vec3f lightDir = Normalize(lightPos - hitPos);
                                   float cosine = std::max(0.0f, Dot(normal, lightDir));
                                   film.SetPixel(x, y, {cosine, cosine, cosine});
                               }
                               finishedCount++;
                               if (finishedCount % (film.GetWidth() * 10) == 0) {
                                   UpdateProgress( static_cast<float>(finishedCount) / static_cast<float>(film.GetWidth() * film.GetHeight()));
                               }
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
