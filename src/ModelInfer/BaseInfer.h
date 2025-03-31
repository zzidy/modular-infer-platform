#ifndef BASEINFER_H
#define BASEINFER_H

#include <iostream>
#include <string>

namespace ModelInfer {

template <typename InputType, typename OutputType>
class BaseInfer {
 public:
  BaseInfer(std::string sModelName, std::string sModelPath)
      : m_sModelName(sModelName), m_sModelPath(sModelPath){};
  virtual ~BaseInfer() = default;

  // 模型推理相关接口
  virtual bool LoadModel() = 0;
  virtual int Infer(InputType& rInput, OutputType& rOutput) = 0;

  // 读取私有成员变量
  std::string GetModelName() { return m_sModelName; }
  std::string GetModelPath() { return m_sModelPath; }
  bool IsLoaded() { return m_bIsLoaded; }
  void SetIsLoaded(bool bIsLoaded) { m_bIsLoaded = bIsLoaded; }

 private:
  std::string m_sModelName;
  std::string m_sModelPath;
  bool m_bIsLoaded;
};
}  // namespace ModelInfer
#endif