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

  // border
  bool bIsBorder;
  bool bIsScale;  // 按比例自动计算border长宽
  int iBorderWidth;
  int iBorderHeight;

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
        bNeedSigmoid(false),
        bIsBorder(false),
        bIsScale(false),
        iBorderWidth(0),
        iBorderHeight(0) {}

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
        bNeedSigmoid(bNeedSigmoid),
        bIsBorder(bIsBorder),
        bIsScale(bIsScale),
        iBorderWidth(iBorderWidth),
        iBorderHeight(iBorderHeight) {}
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
        bNeedSigmoid(rParams.bNeedSigmoid),
        bIsBorder(rParams.bIsBorder),
        bIsScale(rParams.bIsScale),
        iBorderWidth(rParams.iBorderWidth),
        iBorderHeight(rParams.iBorderHeight) {}
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
        bNeedSigmoid(pParams->bNeedSigmoid),
        bIsBorder(pParams->bIsBorder),
        bIsScale(pParams->bIsScale),
        iBorderWidth(pParams->iBorderWidth),
        iBorderHeight(pParams->iBorderHeight) {}

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
  virtual int Process(ModelInfer::YoloOutput& rYoloOutput,
                      cv::Mat& mImg) override;

 private:
  inline float Sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }
  const std::vector<std::vector<int>> m_vMasks = {
      {0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  const std::vector<std::pair<float, float>> m_vAnchors = {
      {10, 13},  {16, 30},  {33, 23},   {30, 61},  {62, 45},
      {59, 119}, {116, 90}, {156, 198}, {373, 326}};
  cv::Rect2f xywh2xyxy(float fCX, float fCY, float fW, float fH);
  std::vector<int> Nms(const std::vector<cv::Rect2f>& vBoxes,
                       const std::vector<float>& vScores);
  // 用于将原始数据解码为检测框
  void DecodeOutput(ModelInfer::YoloOutput& rYoloOutput);
  void Draw(cv::Mat& mImg, const std::vector<cv::Rect2f>& vBoxes,
            const std::vector<int>& vClasses,
            const std::vector<float>& vScores);
};
}  // namespace PostProcess

#endif