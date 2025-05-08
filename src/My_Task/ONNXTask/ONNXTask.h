#include <iostream>

#include "TaskBase.h"

namespace Task {
class ONNXTask : public TaskBase {
 public:
  ONNXTask() = default;
  ~ONNXTask() = default;
  void DoTask() override;
  void PostTask() override;
  void PreTask() override;
};
}  // namespace Task