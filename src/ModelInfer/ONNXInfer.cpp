#include "ONNXInfer.h"

#include <opencv2/opencv.hpp>
#include <vector>

namespace ModelInfer {

// 用于图片输入，数组输出的onnx模型推理的模版特化
template <>
class ONNXInfer<cv::Mat, std::vector<float>> {};
}  // namespace ModelInfer
