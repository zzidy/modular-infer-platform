#include "PreProcess.h"

namespace PreProcess {

void PreProcess<cv::Mat, cv::Mat>::Img2ImgParams::PrintParam() {
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

PreProcess<cv::Mat, cv::Mat>::PreProcess() : m_pParams(nullptr) {}
PreProcess<cv::Mat, cv::Mat>::~PreProcess() {
  if (m_pParams) delete m_pParams;
}

int PreProcess<cv::Mat, cv::Mat>::Process(const cv::Mat& mSrc, cv::Mat& mDst) {
  if (!m_pParams) {
    std::cout << "参数未加载，请先加载参数" << std::endl;
    return -1;
  }

  // 先判断需要不要border

  // 确定色彩空间再归一化，归一化参数的个数不确定
  // 1->3,3->1,1->1,3->3
  return 0;
}

int PreProcess<cv::Mat, cv::Mat>::SetParams(void* pParams) {
  if (m_pParams) delete m_pParams;
  m_pParams = new Params(*static_cast<Params*>(pParams));
  m_pParams->PrintParam();
  return 0;
}
}  // namespace PreProcess