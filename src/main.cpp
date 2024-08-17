#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"
#include "Camera.h"
#include "Sphere.h"

int main()
{
    Film film { 192, 108 };

    Camera camera { film, { 0, 0, 1 }, { 0, 0, 0 }, 90};

    Sphere sphere { {0, 0, 0}, 0.5f};

    ThreadPool threadPool {};

    threadPool.ParallelFor(film.GetWidth(), film.GetHeight(),
                           [&](size_t x, size_t y)
                           {
                               auto ray = camera.GenerateRay(Vec2i(x, y));
                               auto result = sphere.Intersect(ray);
                               if (result.has_value())
                               {
                                   film.SetPixel(x, y, {1.0, 0.0, 0.0});
                               }
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
