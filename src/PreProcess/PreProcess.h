#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

namespace PreProcess {
template <typename IntputType, typename OutputType>
class PreProcess {
 public:
  PreProcess() = default;
  virtual ~PreProcess() = default;
  virtual int Process(const IntputType& pInput, OutputType& pOutput) = 0;
  virtual int SetParams(void* pParams) = 0;
};

template <>
class PreProcess<cv::Mat, cv::Mat> {
 public:
  struct Img2ImgParams {
    // 原图信息
    int iSrcWidth;
    int iSrcHeight;
    int iSrcChannels;

    // 输出图片宽高
    int iDstWidth;
    int iDstHeight;
    int iDstChannels;

    // 归一化
    bool bIsNorm;
    std::vector<double> vMean;
    std::vector<double> vStd;

    // border
    bool bIsBorder;
    bool bIsScale;
    int iBorderWidth;
    int iBorderHeight;

    Img2ImgParams() {
      iSrcWidth = 0;
      iSrcHeight = 0;
      iSrcChannels = 0;

      iDstWidth = 0;
      iDstHeight = 0;
      iDstChannels = 0;

      bIsNorm = false;
      bIsBorder = false;
      bIsScale = false;
    }

    Img2ImgParams(int iSrcWidth, int iSrcHeight, int iSrcChannels,
                  int iDstWidth, int iDstHeight, int iDstChannels, bool bIsNorm,
                  std::vector<double> vMean, std::vector<double> vStd,
                  bool bIsBorder, bool bIsScale, int iBorderWidth,
                  int iBorderHeight)
        : iSrcWidth(iSrcWidth),
          iSrcHeight(iSrcHeight),
          iSrcChannels(iSrcChannels),
          iDstWidth(iDstWidth),
          iDstHeight(iDstHeight),
          iDstChannels(iDstChannels),
          bIsNorm(bIsNorm),
          vMean(vMean),
          vStd(vStd),
          bIsBorder(bIsBorder),
          bIsScale(bIsScale),
          iBorderWidth(iBorderWidth),
          iBorderHeight(iBorderHeight) {}
    Img2ImgParams(const Img2ImgParams& pParams)
        : iSrcWidth(pParams.iSrcWidth),
          iSrcHeight(pParams.iSrcHeight),
          iSrcChannels(pParams.iSrcChannels),
          iDstWidth(pParams.iDstWidth),
          iDstHeight(pParams.iDstHeight),
          iDstChannels(pParams.iDstChannels),
          bIsNorm(pParams.bIsNorm),
          vMean(pParams.vMean),
          vStd(pParams.vStd),
          bIsBorder(pParams.bIsBorder),
          bIsScale(pParams.bIsScale),
          iBorderWidth(pParams.iBorderWidth),
          iBorderHeight(pParams.iBorderHeight) {}
    void PrintParam();
  };

  using Params = Img2ImgParams;

  PreProcess();
  virtual ~PreProcess();
  virtual int Process(const cv::Mat& pInput, cv::Mat& pOutput);
  virtual int SetParams(void* pParams);

 private:
  Params* m_pParams;
};

}  // namespace PreProcess

#endif