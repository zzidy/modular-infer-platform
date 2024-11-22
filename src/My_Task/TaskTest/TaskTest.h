#include <iostream>

#include "TaskBase.h"

namespace Task {
class MyTask : public TaskBase {
 public:
  MyTask() { std::cout << "MyTask" << std::endl; }
  ~MyTask() { std::cout << "~MyTask" << std::endl; }
  void PreTask() override;
  void DoTask() override;
  void DoneTask() override;
};
}  // namespace Task