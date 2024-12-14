#include "PreProcess.h"

#include <cmath>

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
  std::cout << "bUseMinMax: " << bUseMinMax << std::endl;
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
  m_pParams.PrintParam();

  // 判断图像是否为空
  if (mSrc.empty()) return -1;

  // step1: resize
  cv::Mat mResize;
  if (m_pParams.bIsBorder) {
    if (m_pParams.bIsScale) {
      // 按照比例计算border的宽度
      double dWidthScale =
          static_cast<double>(m_pParams.iDstWidth) / m_pParams.iSrcWidth;
      double dHeightScale =
          static_cast<double>(m_pParams.iDstHeight) / m_pParams.iSrcHeight;
      double dScale = std::min(dWidthScale, dHeightScale);

      int iMidWidth =
          static_cast<int>(std::round(m_pParams.iSrcWidth * dScale));
      int iMidHeight =
          static_cast<int>(std::round(m_pParams.iSrcHeight * dScale));

      cv::resize(mSrc, mResize, cv::Size(iMidWidth, iMidHeight));

      // Calculate border sizes
      int iTop = (m_pParams.iDstHeight - iMidHeight) / 2;
      int iBottom = m_pParams.iDstHeight - iMidHeight - iTop;
      int iLeft = (m_pParams.iDstWidth - iMidWidth) / 2;
      int iRight = m_pParams.iDstWidth - iMidWidth - iLeft;

      cv::copyMakeBorder(mResize, mResize, iTop, iBottom, iLeft, iRight,
                         cv::BORDER_CONSTANT, cv::Scalar(128, 128, 128));
    } else {
      // 按照输入的参数进行border填充
      int iMidWidth = m_pParams.iDstWidth - 2 * m_pParams.iBorderWidth;
      int iMidHeight = m_pParams.iDstHeight - 2 * m_pParams.iBorderHeight;

      cv::resize(mSrc, mResize, cv::Size(iMidWidth, iMidHeight));
      cv::copyMakeBorder(mResize, mResize, m_pParams.iBorderHeight,
                         m_pParams.iBorderHeight, m_pParams.iBorderWidth,
                         m_pParams.iBorderWidth, cv::BORDER_CONSTANT,
                         cv::Scalar(128, 128, 128));
    }
  } else {
    // 不进行border填充，直接resize
    cv::resize(mSrc, mResize,
               cv::Size(m_pParams.iDstWidth, m_pParams.iDstHeight));
  }

  // step2：调整图片通道
  cv::Mat mConvert;
  if (m_pParams.iDstChannels == 1 && m_pParams.iSrcChannels != 1) {
    cv::cvtColor(mResize, mConvert, cv::COLOR_BGR2GRAY);
  } else if (m_pParams.iDstChannels == 3 && m_pParams.iSrcChannels == 1) {
    cv::cvtColor(mResize, mConvert, cv::COLOR_GRAY2BGR);
  } else {
    mConvert = mResize;
  }

  // step3: 归一化
  if (m_pParams.bIsNorm) {
    // 使用均值和标准差归一化
    if (!m_pParams.bUseMinMax && !m_pParams.vMean.empty() &&
        !m_pParams.vStd.empty()) {
      std::vector<cv::Mat> vChannels;
      cv::split(mConvert, vChannels);

      for (size_t i = 0; i < vChannels.size(); ++i) {
        vChannels[i].convertTo(vChannels[i], CV_64F);
        vChannels[i] = (vChannels[i] - m_pParams.vMean[i]) / m_pParams.vStd[i];
      }

      cv::merge(vChannels, mDst);
    } else {
      // 使用最大最小值归一化
      mConvert.convertTo(mConvert, CV_32F, 1.0 / 255.0);
    }
    mConvert.convertTo(mConvert, CV_8U);
  }

  mDst = mConvert;
  return 0;
}  // namespace PreProcess

Img2ImgParams& PreProcess<cv::Mat, cv::Mat>::GetParams() { return m_pParams; }

int PreProcess<cv::Mat, cv::Mat>::SetParams(Img2ImgParams& pParams) {
  m_pParams = Img2ImgParams(pParams);
  // m_pParams.PrintParam();
  return 0;
}
}  // namespace PreProcess