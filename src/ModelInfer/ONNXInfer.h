#ifndef ONNXINFER_H
#define ONNXINFER_H

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

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

// 用于yolo的onnx模型推理的模版特化
template <>
class ONNXInfer<cv::Mat, YoloOutput> : public BaseInfer<cv::Mat, YoloOutput> {
 public:
  ONNXInfer(std::string sName, std::string sPath);
  virtual ~ONNXInfer() = default;
  bool LoadModel() override;
  int Infer(cv::Mat& rInput, YoloOutput& rOutput) override;

 private:
  std::unique_ptr<Ort::Session> m_pSession;
};

}  // namespace ModelInfer

#endif
