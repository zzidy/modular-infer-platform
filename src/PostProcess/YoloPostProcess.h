#ifndef YOLOPOSTPROCESS_H
#define YOLOPOSTPROCESS_H

#include <opencv2/opencv.hpp>

#include "../ModelInfer/BaseInfer.h"
#include "BasePostProcess.h"

namespace PostProcess {

struct YoloParams : public BaseParams {
  // class
  int iClassNum;
  std::vector<std::string> vClassNames;
  std::vector<std::vector<int>> vClassColors;

  // 输入图片信息
  int iInputWidth;
  int iInputHeight;
  int iInputChannels;

  // 输出图片信息
  int iOutputWidth;
  int iOutputHeight;
  int iOutputChannels;

  // 阈值
  double dConfThresh;
  double dNmsThresh;

  // sigmoid
  bool bNeedSigmoid;

  YoloParams()
      : iClassNum(0),
        vClassNames({}),
        vClassColors({}),
        iInputWidth(0),
        iInputHeight(0),
        iInputChannels(0),
        iOutputWidth(0),
        iOutputHeight(0),
        iOutputChannels(0),
        dConfThresh(0.0f),
        dNmsThresh(0.0f),
        bNeedSigmoid(false) {}

  YoloParams(int iClassNum, std::vector<std::string> vClassNames,
             std::vector<std::vector<int>> vClassColors, int iInputWidth,
             int iInputHeight, int iInputChannels, int iOutputWidth,
             int iOutputHeight, int iOutputChannels, float dConfThresh,
             float dNmsThresh, bool bNeedSigmoid)
      : iClassNum(iClassNum),
        vClassNames(vClassNames),
        vClassColors(vClassColors),
        iInputWidth(iInputWidth),
        iInputHeight(iInputHeight),
        iInputChannels(iInputChannels),
        iOutputWidth(iOutputWidth),
        iOutputHeight(iOutputHeight),
        iOutputChannels(iOutputChannels),
        dConfThresh(dConfThresh),
        dNmsThresh(dNmsThresh),
        bNeedSigmoid(bNeedSigmoid) {}
  YoloParams(YoloParams& rParams)
      : iClassNum(rParams.iClassNum),
        vClassNames(rParams.vClassNames),
        vClassColors(rParams.vClassColors),
        iInputWidth(rParams.iInputWidth),
        iInputHeight(rParams.iInputHeight),
        iInputChannels(rParams.iInputChannels),
        iOutputWidth(rParams.iOutputWidth),
        iOutputHeight(rParams.iOutputHeight),
        iOutputChannels(rParams.iOutputChannels),
        dConfThresh(rParams.dConfThresh),
        dNmsThresh(rParams.dNmsThresh),
        bNeedSigmoid(rParams.bNeedSigmoid) {}
  YoloParams(YoloParams* pParams)
      : iClassNum(pParams->iClassNum),
        iInputWidth(pParams->iInputWidth),
        iInputHeight(pParams->iInputHeight),
        iInputChannels(pParams->iInputChannels),
        iOutputWidth(pParams->iOutputWidth),
        iOutputHeight(pParams->iOutputHeight),
        iOutputChannels(pParams->iOutputChannels),
        dConfThresh(pParams->dConfThresh),
        dNmsThresh(pParams->dNmsThresh),
        bNeedSigmoid(pParams->bNeedSigmoid) {}

  virtual void PrintParam() override;
};

template <typename InputType, typename OutputType>
class YoloPostProcess;

template <>
class YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>
    : public BasePostProcess<ModelInfer::YoloOutput, cv::Mat> {
 public:
  YoloPostProcess() { this->m_pParams = std::make_shared<YoloParams>(); }
  virtual ~YoloPostProcess() = default;
  virtual int Process(const ModelInfer::YoloOutput& rYoloOutput,
                      cv::Mat& mImg) override;
};
}  // namespace PostProcess

#endif