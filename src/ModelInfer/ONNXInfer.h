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
  ONNXInfer(std::string sName, std::string sPath);
  virtual ~ONNXInfer() = default;

  bool LoadModel() override;
  int Infer(InputType& rInput, OutputType& rOutput);

 private:
  std::unique_ptr<Ort::Session> m_pSession;
};

}  // namespace ModelInfer

#endif
