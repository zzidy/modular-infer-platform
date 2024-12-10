#include <iostream>

#include "TaskBase.h"

namespace Task {
class PreProcessTask : public TaskBase {
 public:
  PreProcessTask() = default;
  ~PreProcessTask() = default;
  void DoTask() override;
  void PostTask() override;
  void PreTask() override;
};
}  // namespace Task