#include <TaskTest.h>

void MyTask::PreTask() {
    std::cout << "MyTest::PreTask()" << std::endl;
}
void MyTask::DoTask() {
    std::cout << "MyTest::DoTask()" << std::endl;
}
void MyTask::DoneTask() {
    std::cout << "MyTest::DoneTask()" << std::endl;
}

// TaskSpace::TaskBase* TaskSpace::TaskBase::CreateTask()
// {
//     return new MyTask();
// }

namespace TaskSpace {
    TaskBase* TaskBase::CreateTask()
    {
        return new MyTask();
    }
}