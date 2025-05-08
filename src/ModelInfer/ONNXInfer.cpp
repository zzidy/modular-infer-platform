#include "ONNXInfer.h"

namespace ModelInfer {

// 单例模式管理Ort::Env实例
class ONNXEnv {
 private:
  Ort::Env m_pEnv;

  ONNXEnv() {
    // 创建ONNXRuntime环境
    m_pEnv = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, "ONNXInfer");
  };

 public:
  ~ONNXEnv() = default;
  ONNXEnv& operator=(const ONNXEnv&) = delete;
  Ort::Env& GetEnv() { return m_pEnv; }
  static ONNXEnv& GetInstance() {
    static ONNXEnv pONNXEnv;
    return pONNXEnv;
  }
};

ONNXInfer<cv::Mat, std::vector<float>>::ONNXInfer(std::string sName,
                                                  std::string sPath)
    : BaseInfer<cv::Mat, std::vector<float>>(sName, sPath) {
  // 初始化ONNXRuntime环境
  ONNXEnv& pEnv = ONNXEnv::GetInstance();
  LoadModel();
};
bool ONNXInfer<cv::Mat, std::vector<float>>::LoadModel() {
  if (IsLoaded()) {
    std::cout << "模型 " << GetModelName() << " 已加载" << std::endl;
    return true;
  }

  if (!GetModelName().empty()) {
    Ort::SessionOptions pSession_options;

    pSession_options.SetGraphOptimizationLevel(
        GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    m_pSession = std::make_unique<Ort::Session>(ONNXEnv::GetInstance().GetEnv(),
                                                GetModelPath().c_str(),
                                                pSession_options);
    SetIsLoaded(m_pSession != nullptr);
    return m_pSession != nullptr;
  }

  return false;
}

int ONNXInfer<cv::Mat, std::vector<float>>::Infer(cv::Mat& rInput,
                                                  std::vector<float>& rOutput) {
  std::cout << "test" << std::endl;
  return 0;
}

}  // namespace ModelInfer
