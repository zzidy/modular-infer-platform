#ifndef INFERMANAGER_H
#define INFERMANAGER_H

#include <iostream>
#include <memory>
#include <unordered_map>

#include "BaseInfer.h"

namespace ModelInfer {

// 支持的模型类型
enum class eModelType { ONNX, RKNN };

// 单例模式的管理者
class InferManager {
 private:
  InferManager();
  std::unordered_map<std::string, std::shared_ptr<void>> m_mapInfer;

 public:
  ~InferManager();
  InferManager(const InferManager&) = delete;
  InferManager& operator=(const InferManager&) = delete;

  static InferManager& GetInstance() {
    static InferManager rInferManInstance;
    return rInferManInstance;
  }

  template <typename InputType, typename OutputType>
  int AddInfer(const std::string& sName, const std::string& sPath,
               eModelType eType);

  template <typename InputType, typename OutputType>
  std::shared_ptr<BaseInfer<InputType, OutputType>> GetInfer(
      const std::string& sName);
};

}  // namespace ModelInfer

#endif