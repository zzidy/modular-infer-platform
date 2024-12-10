#ifndef TASKBASE_H
#define TASKBASE_H

#include <iostream>

namespace Task {

class TaskBase {
 private:
  /* data */
 public:
  TaskBase() = default;
  ~TaskBase() = default;

  virtual void PreTask() = 0;   // 执行任务前准备
  virtual void DoTask() = 0;    // 执行任务
  virtual void PostTask() = 0;  // 执行任务后清理
};

extern "C" TaskBase* CreateTask();  // 工厂函数

}  // namespace Task

#endif
