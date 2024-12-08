#include "InferManager.h"

#include <opencv2/opencv.hpp>
#include <vector>

#include "ONNXInfer.h"

namespace ModelInfer {
InferManager::InferManager() { m_mapInfer.clear(); }
InferManager::~InferManager() {}

template <typename InputType, typename OutputType>
int InferManager::AddInfer(const std::string& sName, const std::string& sPath,
                           eModelType eType) {
  // 添加onnx模型
  if (eType == eModelType::ONNX) {
    // 判断是否存在该输入输出的特化模版
    if (typeid(InputType) == typeid(cv::Mat) &&
        typeid(OutputType) == typeid(std::vector<float>)) {
      // 判断是否已存在
      if (m_mapInfer.find(sName) == m_mapInfer.end()) {
        m_mapInfer[sName] =
            std::make_shared<ONNXInfer<InputType, OutputType>>(sName, sPath);

        if (!m_mapInfer[sName]->IsLoaded()) {
          std::cout << "模型" << sName << ":" << sPath << "加载失败"
                    << std::endl;
          return -1;
        } else {
          std::cout << "模型" << sName << ":" << sPath << "加载成功"
                    << std::endl;
          return 0;
        }
      }

      std::cout << "模型" << sName << ":" << sPath << "已存在" << std::endl;
      return -1;
    }

    std::cout << "当前输入类型：" << typeid(InputType).name() << "，输出类型："
              << typeid(OutputType).name() << "不支持" << std::endl;
    return -1;

    // 添加rknn模型
  } else if (eType == eModelType::RKNN) {
    std::cout << "敬请期待" << std::endl;
    return -1;
  }

  // 模型类型不支持
  std::cout << "该模型类型暂不支持" << std::endl;
  return -1;
}

template <typename InputType, typename OutputType>
std::shared_ptr<BaseInfer<InputType, OutputType>> GetInfer(
    const std::string& sName) {
  if (m_mapInfer.find(sName) == m_mapInfer.end()) return nullptr;
  return std::static_pointer_cast<BaseInfer<InputType, OutputType>>(
      m_mapInfer[sName]);
}

}  // namespace ModelInfer
