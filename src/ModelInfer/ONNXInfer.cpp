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

ONNXInfer<cv::Mat, YoloOutput>::ONNXInfer(std::string sName, std::string sPath)
    : BaseInfer<cv::Mat, YoloOutput>(sName, sPath) {
  // 初始化ONNXRuntime环境
  ONNXEnv& pEnv = ONNXEnv::GetInstance();
  LoadModel();
};
bool ONNXInfer<cv::Mat, YoloOutput>::LoadModel() {
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

int ONNXInfer<cv::Mat, YoloOutput>::Infer(cv::Mat& rInput,
                                          YoloOutput& rOutput) {
  if (!IsLoaded()) {
    std::cout << "模型 " << GetModelName() << " 已加载" << std::endl;
    return -1;
  }
  if (rInput.empty()) {
    std::cout << "输入图像为空" << std::endl;
    return -1;
  }

  // CHW to NCHW
  int iIntputChannel = rInput.channels();
  int iIntputHeight = rInput.rows;
  int iIntputWidth = rInput.cols;
  std::vector<float> vInputValues(iIntputChannel * iIntputHeight *
                                  iIntputWidth);
  int idx = 0;
  for (int c = 0; c < iIntputChannel; ++c) {
    for (int i = 0; i < iIntputHeight; ++i) {
      for (int j = 0; j < iIntputWidth; ++j) {
        vInputValues[idx++] = rInput.at<cv::Vec3f>(i, j)[c];
      }
    }
  }

  // 生成ONNX输入数据张量
  Ort::MemoryInfo MemoryInfo =
      Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
  std::array<int64_t, 4> aInputShape = {1, iIntputChannel, iIntputHeight,
                                        iIntputWidth};
  Ort::Value vInputTensor = Ort::Value::CreateTensor<float>(
      MemoryInfo, vInputValues.data(), vInputValues.size(), aInputShape.data(),
      aInputShape.size());

  // 执行推理
  auto InputNames =
      m_pSession->GetInputNameAllocated(0, Ort::AllocatorWithDefaultOptions())
          .get();
  auto iOutputCount = m_pSession->GetOutputCount();
  std::vector<const char*> vOutputNames;
  for (size_t i = 0; i < iOutputCount; ++i) {
    vOutputNames.push_back(
        m_pSession
            ->GetOutputNameAllocated(i, Ort::AllocatorWithDefaultOptions())
            .get());
  }

  Ort::RunOptions RunOptions(nullptr);
  auto vOutputTensors = m_pSession->Run(RunOptions, &InputNames, &vInputTensor,
                                        1, vOutputNames.data(), iOutputCount);

  // 解析输出数据
  rOutput.clear();
  rOutput.reserve(iOutputCount);  // 3 层

  for (size_t layerIdx = 0; layerIdx < iOutputCount; ++layerIdx) {
    auto& tensor = vOutputTensors[layerIdx];

    float* tensorData = tensor.GetTensorMutableData<float>();
    auto shape = tensor.GetTensorTypeAndShapeInfo().GetShape();
    if (shape.size() != 5) {
      std::cout << "模型推理结果的张量形状错误" << std::endl;
      return -1;
    }

    int64_t B = shape[0];  // batch
    int64_t A = shape[1];  // anchor num
    int64_t H = shape[2];
    int64_t W = shape[3];
    int64_t C = shape[4];

    if (B != 1) {
      std::cout << "模型推理结果的batch size错误" << std::endl;
      return -1;
    }

    std::vector<std::vector<YoloAnchor>> anchorOutputs(A);
    for (int64_t a = 0; a < A; ++a) {
      anchorOutputs[a].reserve(H * W);
      for (int64_t h = 0; h < H; ++h) {
        for (int64_t w = 0; w < W; ++w) {
          size_t offset = (((0 * A + a) * H + h) * W + w) * C;
          float x = tensorData[offset + 0];
          float y = tensorData[offset + 1];
          float w_box = tensorData[offset + 2];
          float h_box = tensorData[offset + 3];
          float objectness = tensorData[offset + 4];

          std::vector<float> classScores(C - 5);
          for (int i = 5; i < C; ++i) {
            classScores[i - 5] = tensorData[offset + i];
          }

          anchorOutputs[a].emplace_back(YoloAnchor{
              x, y, w_box, h_box, objectness, std::move(classScores)});
        }
      }
    }

    rOutput.emplace_back(std::move(anchorOutputs));
  }

  return 0;
}

}  // namespace ModelInfer
