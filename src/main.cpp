#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"
#include "Camera.h"
#include "Sphere.h"

int main()
{
    Film film { 1920, 1080 };

    Camera camera { film, { 0, 0, 1 }, { 0, 0, 0 }, 90};

    Sphere sphere { {0, 0, 0}, 0.5f};

    ThreadPool threadPool {};

    Vec3f lightPos {-1, 1, 1};

    threadPool.ParallelFor(film.GetWidth(), film.GetHeight(),
                           [&](size_t x, size_t y)
                           {
                               auto ray = camera.GenerateRay(Vec2i(x, y));
                               auto result = sphere.Intersect(ray);
                               if (result.has_value())
                               {
                                   Vec3f hitPos = ray.HitPos(result.value());
                                   Vec3f normal = Normalize(hitPos - sphere.GetCenter());
                                   Vec3f lightDir = Normalize(lightPos - hitPos);
                                   float cosine = std::max(0.0f, Dot(normal, lightDir));
                                   film.SetPixel(x, y, {cosine, cosine, cosine});
                               }
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
