#include <iostream>
#include <string>

#include "TaskBase.h"

namespace Task {
class ConfigTask : public TaskBase {
 public:
  ConfigTask() { std::cout << "ConfigTask::ConfigTask()" << std::endl; }
  ~ConfigTask() {}
  void PreTask() override;
  void DoTask() override;
  void PostTask() override;
};

}  // namespace Task