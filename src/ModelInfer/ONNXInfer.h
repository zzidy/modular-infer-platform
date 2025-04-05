#ifndef ONNXINFER_H
#define ONNXINFER_H

#include <opencv2/opencv.hpp>

#include "BaseInfer.h"

namespace ModelInfer {

// 推理onnx模型模版接口类
template <typename InputType, typename OutputType>
class ONNXInfer : public BaseInfer<InputType, OutputType> {
 public:
  ONNXInfer(std::string sName, std::string sPath)
      : BaseInfer<InputType, OutputType>(sName, sPath){};
  virtual ~ONNXInfer() = default;

  bool LoadModel() = 0;
  int Infer(InputType& rInput, OutputType& rOutput) = 0;
};

// 用于图片输入，数组输出的onnx模型推理的模版特化
template <>
class ONNXInfer<cv::Mat, std::vector<float>>
    : public BaseInfer<cv::Mat, std::vector<float>> {
 public:
  ONNXInfer(std::string sName, std::string sPath);
  virtual ~ONNXInfer() = default;
  bool LoadModel() override;
  int Infer(cv::Mat& rInput, std::vector<float>& rOutput) override;
};

}  // namespace ModelInfer

#endif
