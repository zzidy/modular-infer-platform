#include "ONNXInfer.h"

#include <onnxruntime/core/session/onnxruntime_cxx_api.h>

#include <vector>

namespace ModelInfer {

// 单例模式管理Ort::Env实例
class ONNXEnv {
 private:
  ONNXEnv() {
    Ort::Env env =
        Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "ONNXInfer");
  };

 public:
  ~ONNXEnv() = default;
  ONNXEnv& operator=(const ONNXEnv&) = delete;
  static ONNXEnv GetInstance() {
    static ONNXEnv env;
    return env;
  }
};

ONNXInfer<cv::Mat, std::vector<float>>::ONNXInfer(std::string sName,
                                                  std::string sPath)
    : BaseInfer<cv::Mat, std::vector<float>>(sName, sPath) {
  ONNXEnv pEnv = ONNXEnv::GetInstance();
};
bool ONNXInfer<cv::Mat, std::vector<float>>::LoadModel() { return false; }

int ONNXInfer<cv::Mat, std::vector<float>>::Infer(cv::Mat& rInput,
                                                  std::vector<float>& rOutput) {
  return 0;
}

}  // namespace ModelInfer
