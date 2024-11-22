#include "Task.h"

#include <iostream>

namespace Task {
void Task::PreTask() { std::cout << "Task::PreTask()" << std::endl; }

void Task::DoTask() { std::cout << "Task::DoTask()" << std::endl; }

void Task::DoneTask() { std::cout << "Task::DoneTask()" << std::endl; }

TaskBase *CreateTask() { return new Task(); }
}  // namespace Task
