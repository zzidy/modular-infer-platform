#include "YoloPostProcess.h"

namespace PostProcess {
void YoloParams::PrintParam() {
  std::cout << "YoloParams:" << std::endl;
  std::cout << "iClassNum:" << iClassNum << std::endl;
  std::cout << "vClassNames:";
  for (auto& str : vClassNames) std::cout << str << " ";
  std::cout << std::endl;
  std::cout << "vClassColors:";
  for (auto& vColor : vClassColors)
    for (int iVlaue : vColor)
      std::cout << "(" << vColor[0] << "," << vColor[1] << "," << vColor[2]
                << ") "
                << " ";
  std::cout << std::endl;
  std::cout << "iInputWidth:" << iInputWidth << std::endl;
  std::cout << "iInputHeight:" << iInputHeight << std::endl;
  std::cout << "iInputChannels:" << iInputChannels << std::endl;
  std::cout << "iOutputWidth:" << iOutputWidth << std::endl;
  std::cout << "iOutputHeight:" << iOutputHeight << std::endl;
  std::cout << "iOutputChannels:" << iOutputChannels << std::endl;
  std::cout << "fConfThresh:" << fConfThresh << std::endl;
  std::cout << "fNmsThresh:" << fNmsThresh << std::endl;
  std::cout << "bNeedSigmoid:" << bNeedSigmoid << std::endl;
}

int YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::Process(
    const ModelInfer::YoloOutput& rYoloOutput, cv::Mat& mImg) {
  auto pParams = std::dynamic_pointer_cast<YoloParams>(m_pParams);
  if (!pParams) {
    std::cout << "后处理模块参数缺失" << std::endl;
    return -1;
  }
  if ((pParams->iClassNum != pParams->vClassNames.size()) ||
      (pParams->iClassNum != pParams->vClassColors.size())) {
    std::cout << "后处理模块参数iClassNum与其他输入不匹配" << std::endl;
    return -1;
  }

  pParams->PrintParam();

  if (mImg.empty()) {
    std::cout << "输入图片为空" << std::endl;
    return -1;
  }

  if (rYoloOutput.empty()) {
    std::cout << "输入数据为空" << std::endl;
    return -1;
  }

  // 具体后处理流程

  return 0;
}
}  // namespace PostProcess