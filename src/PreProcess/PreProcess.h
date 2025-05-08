#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <vector>

namespace PreProcess {

// 参数结构体
struct Params {
  Params() = default;
  virtual ~Params() = default;
  virtual void PrintParam() = 0;
};

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
  bool bUseMeanStd;
  std::vector<double> vMean;
  std::vector<double> vStd;

  // border
  bool bIsBorder;
  bool bIsScale;  // 按比例自动计算border长宽
  int iBorderWidth;
  int iBorderHeight;

  Img2ImgParams()
      : iSrcWidth(0),
        iSrcHeight(0),
        iSrcChannels(0),
        iDstWidth(0),
        iDstHeight(0),
        iDstChannels(0),
        bIsNorm(false),
        bUseMeanStd(false),
        vMean({}),
        vStd({}),
        bIsBorder(false),
        bIsScale(false),
        iBorderWidth(0),
        iBorderHeight(0) {}

  Img2ImgParams(int iSrcWidth, int iSrcHeight, int iSrcChannels, int iDstWidth,
                int iDstHeight, int iDstChannels, bool bIsNorm,
                bool bUseMeanStd, std::vector<double> vMean,
                std::vector<double> vStd, bool bIsBorder, bool bIsScale,
                int iBorderWidth, int iBorderHeight)
      : iSrcWidth(iSrcWidth),
        iSrcHeight(iSrcHeight),
        iSrcChannels(iSrcChannels),
        iDstWidth(iDstWidth),
        iDstHeight(iDstHeight),
        iDstChannels(iDstChannels),
        bIsNorm(bIsNorm),
        bUseMeanStd(bUseMeanStd),
        vMean(vMean),
        vStd(vStd),
        bIsBorder(bIsBorder),
        bIsScale(bIsScale),
        iBorderWidth(iBorderWidth),
        iBorderHeight(iBorderHeight) {}
  Img2ImgParams(Img2ImgParams& rParams)
      : iSrcWidth(rParams.iSrcWidth),
        iSrcHeight(rParams.iSrcHeight),
        iSrcChannels(rParams.iSrcChannels),
        iDstWidth(rParams.iDstWidth),
        iDstHeight(rParams.iDstHeight),
        iDstChannels(rParams.iDstChannels),
        bIsNorm(rParams.bIsNorm),
        bUseMeanStd(rParams.bUseMeanStd),
        vMean(rParams.vMean),
        vStd(rParams.vStd),
        bIsBorder(rParams.bIsBorder),
        bIsScale(rParams.bIsScale),
        iBorderWidth(rParams.iBorderWidth),
        iBorderHeight(rParams.iBorderHeight) {}
  Img2ImgParams(Img2ImgParams* pParams)
      : iSrcWidth(pParams->iSrcWidth),
        iSrcHeight(pParams->iSrcHeight),
        iSrcChannels(pParams->iSrcChannels),
        iDstWidth(pParams->iDstWidth),
        iDstHeight(pParams->iDstHeight),
        iDstChannels(pParams->iDstChannels),
        bIsNorm(pParams->bIsNorm),
        bUseMeanStd(pParams->bUseMeanStd),
        vMean(pParams->vMean),
        vStd(pParams->vStd),
        bIsBorder(pParams->bIsBorder),
        bIsScale(pParams->bIsScale),
        iBorderWidth(pParams->iBorderWidth),
        iBorderHeight(pParams->iBorderHeight) {}

  virtual void PrintParam();
};

// 预处理模板类
template <typename IntputType, typename OutputType>
class PreProcess {
 public:
  PreProcess() = default;
  virtual ~PreProcess() = default;
  virtual int Process(const IntputType& rInput, OutputType& rOutput) = 0;
  virtual int SetParams(Params& pParams) = 0;
  virtual Params& GetParams() = 0;

 private:
  Params m_pParams;
};

// 图片预处理特化
template <>
class PreProcess<cv::Mat, cv::Mat> {
 public:
  PreProcess();
  virtual ~PreProcess();
  virtual int Process(const cv::Mat& rInput, cv::Mat& rOutput);
  virtual int SetParams(Img2ImgParams& pParams);
  virtual Img2ImgParams& GetParams();

 private:
  Img2ImgParams m_pParams;
};

}  // namespace PreProcess

#endif