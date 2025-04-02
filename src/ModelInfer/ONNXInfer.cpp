#include "ONNXInfer.h"

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include <vector>

namespace ModelInfer {

bool ONNXInfer<cv::Mat, std::vector<float>>::LoadModel() { return false; }

int ONNXInfer<cv::Mat, std::vector<float>>::Infer(cv::Mat& rInput,
                                                  std::vector<float>& rOutput) {
  return 0;
}

}  // namespace ModelInfer
