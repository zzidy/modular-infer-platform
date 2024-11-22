#ifndef TASK_H
#define TASK_H

#include <iostream>

#include "TaskBase.h"

class Task : public TaskBase
{
private:
    /* data */
public:
    Task() = default;
    ~Task() = default;

    virtual void PreTask();  // 执行任务前准备
    virtual void DoTask();   // 执行任务
    virtual void DoneTask(); // 执行任务后清理
};





// extern "C" Task* CreateTask();  // 工厂函数

#endif
