#include "PreProcess.h"

namespace PreProcess {

void Img2ImgParams::PrintParam() {
  std::cout << "Img2ImgParams:" << std::endl;
  std::cout << "iSrcWidth: " << iSrcWidth << std::endl;
  std::cout << "iSrcHeight: " << iSrcHeight << std::endl;
  std::cout << "iSrcChannels: " << iSrcChannels << std::endl;
  std::cout << "iDstWidth: " << iDstWidth << std::endl;
  std::cout << "iDstHeight: " << iDstHeight << std::endl;
  std::cout << "iDstChannels: " << iDstChannels << std::endl;
  std::cout << "bIsNorm: " << bIsNorm << std::endl;
  std::cout << "vMean: ";
  for (int i = 0; i < vMean.size(); i++) std::cout << vMean[i] << " ";
  std::cout << std::endl;
  std::cout << "vStd: ";
  for (int i = 0; i < vStd.size(); i++) std::cout << vStd[i] << " ";
  std::cout << std::endl;
  std::cout << "bIsBorder: " << bIsBorder << std::endl;
  std::cout << "bIsScale: " << bIsScale << std::endl;
  std::cout << "iBorderWidth: " << iBorderWidth << std::endl;
  std::cout << "iBorderHeight: " << iBorderHeight << std::endl;
}

PreProcess<cv::Mat, cv::Mat>::PreProcess() {}
PreProcess<cv::Mat, cv::Mat>::~PreProcess() {}

int PreProcess<cv::Mat, cv::Mat>::Process(const cv::Mat& mSrc, cv::Mat& mDst) {
  // 先判断需要不要border

  // 确定色彩空间再归一化，归一化参数的个数不确定
  // 1->3,3->1,1->1,3->3
  return 0;
}

Img2ImgParams& PreProcess<cv::Mat, cv::Mat>::GetParams() { return m_pParams; }

int PreProcess<cv::Mat, cv::Mat>::SetParams(Img2ImgParams& pParams) {
  m_pParams = Img2ImgParams(pParams);
  m_pParams.PrintParam();
  return 0;
}
}  // namespace PreProcess