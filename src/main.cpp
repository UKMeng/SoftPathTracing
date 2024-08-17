#include <iostream>
#include "MyMath.h"
#include "ThreadPool.h"

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

    ThreadPool threadPool {};
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    threadPool.AddTask(new SimpleTask());
    return 0;
}
