#include "TaskTest.h"

namespace Task {
void MyTask::PreTask() { std::cout << "MyTest::PreTask()" << std::endl; }
void MyTask::DoTask() { std::cout << "MyTest::DoTask()" << std::endl; }
void MyTask::DoneTask() {
  std::cout << "MyTest::DoneTask()" << std::endl;
  print();
}

void MyTask::print() { std::cout << "MyTest::print()" << std::endl; }

TaskBase* CreateTask() { return new MyTask(); }
}  // namespace Task