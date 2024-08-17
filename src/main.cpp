#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"
#include "Film.h"

class SimpleTask : public Task
{
public:
    void Run() override
    {
        std::cout << "Hello, World!" << std::endl;
    }
};

int main()
{
    Vec3 test = {0, 1, 2};
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


    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    return 0;
}
