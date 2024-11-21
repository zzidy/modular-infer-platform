#include <iostream>
#include <Task.h>

class MyTask : public TaskSpace::TaskBase
{
public:
    MyTask()
    {
        std::cout << "MyTask" << std::endl;
    }
    ~MyTask() {
        std::cout << "~MyTask" << std::endl;
    }
    void PreTask() override;
    void DoTask() override;
    void DoneTask() override;
};