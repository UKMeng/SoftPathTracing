#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"

int main()
{
    Vec3f test = {0, 1, 2};
    test.Print();

    Film film { 1920, 1080 };

    ThreadPool threadPool {};

    threadPool.ParallelFor(200, 100,
                           [&](size_t x, size_t y)
                           {
                               film.SetPixel(x, y, {1.0, 0.0, 0.0});
                           }
    );

    threadPool.Wait();
    film.Save("test.ppm");

    return 0;
}
