#ifndef BASEPOSTPROCESS_H
#define BASEPOSTPROCESS_H

#include <memory>
#include <string>
#include <vector>

namespace PostProcess {

// 参数结构体
struct BaseParams {
  BaseParams() = default;
  virtual ~BaseParams() = default;
  virtual void PrintParam() = 0;
};

template <typename InputType, typename OutputType>
class BasePostProcess {
 public:
  BasePostProcess() {}
  virtual ~BasePostProcess() {}
  virtual int Process(const InputType& rInput, OutputType& rOutput) = 0;
  int SetParams(std::shared_ptr<BaseParams> pParams) {
    m_pParams = std::move(pParams);
    m_pParams->PrintParam();
    return 0;
  };

  std::shared_ptr<BaseParams> GetParams() { return m_pParams; }

 protected:
  std::shared_ptr<BaseParams> m_pParams;
};
}  // namespace PostProcess

#endif