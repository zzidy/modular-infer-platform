#include <iostream>

#include "TaskBase.h"

namespace Task {
class PostProcessTask : public TaskBase {
 public:
  PostProcessTask() = default;
  ~PostProcessTask() = default;
  void DoTask() override;
  void PostTask() override;
  void PreTask() override;
};
}  // namespace Task