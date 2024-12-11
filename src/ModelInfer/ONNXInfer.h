#ifndef ONNXINFER_H
#define ONNXINFER_H

#include <iostream>

#include "BaseInfer.h"

namespace ModelInfer {

// 推理onnx模型模版接口类
template <typename InputType, typename OutputType>
class ONNXInfer : public BaseInfer<InputType, OutputType> {
 public:
  ONNXInfer(std::string sName, std::string sPath) : BaseInfer(sName, sPath){};
  virtual ~ONNXInfer() = default;

  bool LoadModel() = 0 override;
  int Infer(InputType& rInput, OutputType& rOutput) = 0 override;
};

// 用于图片输入，数组输出的onnx模型推理的模版特化
template <>
class ONNXInfer<cv::Mat, std::vector<float>> {};
}  // namespace ModelInfer

#endif
