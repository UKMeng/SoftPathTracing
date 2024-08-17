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
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            film.SetPixel(i, j, {1.0, 0.0, 0.0});
        }
    }
    film.Save("test.ppm");

    ThreadPool threadPool {};
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    return 0;
}
