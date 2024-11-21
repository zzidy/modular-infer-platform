#include <iostream>

#include "Task.h"

namespace TaskSpace
{
    Task::Task() : m_pTask(TaskBase::CreateTask())
    {
        std::cout << "Task::Task()" << std::endl;

        m_pTask->PreTask();
        m_pTask->DoTask();
        m_pTask->DoneTask();
    }

    Task::~Task()
    {
        std::cout << "Task::~Task()" << std::endl;
    }

    void TaskBase::PreTask()
    {
        std::cout << "TaskBase::PreTask()" << std::endl;
    }

    void TaskBase::DoTask()
    {
        std::cout << "TaskBase::DoTask()" << std::endl;
    }

    void TaskBase::DoneTask()
    {
        std::cout << "TaskBase::DoneTask()" << std::endl;
    }

    TaskBase* TaskBase::CreateTask()
    {
        return new TaskBase();
    }
}
