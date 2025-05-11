#include "ONNXInfer.h"

#include <cstring>

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

template <typename InputType, typename OutputType>
ONNXInfer<InputType, OutputType>::ONNXInfer(std::string sName,
                                            std::string sPath)
    : BaseInfer<InputType, OutputType>(sName, sPath) {
  // 初始化ONNXRuntime环境
  ONNXEnv& pEnv = ONNXEnv::GetInstance();
  LoadModel();
}

template <typename InputType, typename OutputType>
bool ONNXInfer<InputType, OutputType>::LoadModel() {
  if (this->IsLoaded()) {
    std::cout << "模型 " << this->GetModelName() << " 已加载" << std::endl;
    return true;
  }

  if (!this->GetModelName().empty()) {
    Ort::SessionOptions pSession_options;

    pSession_options.SetGraphOptimizationLevel(
        GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    m_pSession = std::make_unique<Ort::Session>(ONNXEnv::GetInstance().GetEnv(),
                                                this->GetModelPath().c_str(),
                                                pSession_options);
    this->SetIsLoaded(m_pSession != nullptr);
    return m_pSession != nullptr;
  }

  return false;
}

template <typename InputType, typename OutputType>
int ONNXInfer<InputType, OutputType>::Infer(InputType& rInput,
                                            OutputType& rOutput) {
  std::cout << "不支持当前输入输出" << std::endl;
  return -1;
}

// 用于yolo的onnx模型推理的模版特化
template <>
int ONNXInfer<cv::Mat, YoloOutput>::Infer(cv::Mat& rInput,
                                          YoloOutput& rOutput) {
  if (!IsLoaded()) {
    std::cout << "模型 " << GetModelName() << " 未加载" << std::endl;
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

  auto pInputName =
      m_pSession->GetInputNameAllocated(0, Ort::AllocatorWithDefaultOptions());
  std::string sInputName = pInputName.get();
  const char* cInputNames[] = {sInputName.c_str()};

  auto iOutputCount = m_pSession->GetOutputCount();

  std::vector<const char*> vOutputNames;
  for (size_t i = 0; i < iOutputCount; ++i) {
    auto pName = m_pSession->GetOutputNameAllocated(
        i, Ort::AllocatorWithDefaultOptions());

    char* pTmp = new char[strlen(pName.get()) + 1];
    strcpy(pTmp, pName.get());
    vOutputNames.push_back(pTmp);
  }

  Ort::RunOptions RunOptions(nullptr);
  auto vOutputTensors = m_pSession->Run(RunOptions, cInputNames, &vInputTensor,
                                        1, vOutputNames.data(), iOutputCount);

  // 释放内存
  for (const char* pName : vOutputNames) {
    delete[] pName;
  }

  // 解析输出数据
  rOutput.clear();
  rOutput.reserve(iOutputCount);  // 3 层

  for (size_t iLayerIdx = 0; iLayerIdx < iOutputCount; ++iLayerIdx) {
    auto& rTensor = vOutputTensors[iLayerIdx];

    float* pTensorData = rTensor.GetTensorMutableData<float>();
    auto vShape = rTensor.GetTensorTypeAndShapeInfo().GetShape();
    if (vShape.size() != 5) {
      std::cout << "模型推理结果的张量形状错误" << std::endl;
      return -1;
    }

    int64_t iB = vShape[0];  // batch
    int64_t iA = vShape[1];  // anchor num
    int64_t iH = vShape[2];
    int64_t iW = vShape[3];
    int64_t iC = vShape[4];  // class num

    if (iB != 1) {
      std::cout << "模型推理结果的batch size错误" << std::endl;
      return -1;
    }

    std::vector<std::vector<YoloAnchor>> anchorOutputs(iA);
    for (int64_t a = 0; a < iA; ++a) {
      anchorOutputs[a].reserve(iH * iW);
      for (int64_t h = 0; h < iH; ++h) {
        for (int64_t w = 0; w < iW; ++w) {
          size_t offset = (((0 * iA + a) * iH + h) * iW + w) * iC;
          float fX = pTensorData[offset + 0];
          float fY = pTensorData[offset + 1];
          float fBoxW = pTensorData[offset + 2];
          float fBoxH = pTensorData[offset + 3];
          float fObjectness = pTensorData[offset + 4];

          std::vector<float> vClassScores(iC - 5);
          for (int i = 5; i < iC; ++i) {
            vClassScores[i - 5] = pTensorData[offset + i];
          }

          anchorOutputs[a].emplace_back(YoloAnchor{
              fX, fY, fBoxW, fBoxH, fObjectness, std::move(vClassScores)});
        }
      }
    }

    rOutput.emplace_back(std::move(anchorOutputs));
  }
  std::cout << "ONNXInfer推理成功" << std::endl;
  return 0;
}

// 显示实例化
template ONNXInfer<cv::Mat, YoloOutput>::ONNXInfer(std::string sName,
                                                   std::string sPath);

}  // namespace ModelInfer
