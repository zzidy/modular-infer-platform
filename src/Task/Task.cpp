#include <iostream>

#include "Task.h"

Task::Task(/* args */)
{
    std::cout << "Task::Task()" << std::endl;
}

Task::~Task()
{
    std::cout << "Task::~Task()" << std::endl;
}

// int main() {
//     Task task;
//     return 0;
// }