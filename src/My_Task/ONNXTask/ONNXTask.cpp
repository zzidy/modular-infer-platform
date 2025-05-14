#include "ONNXTask.h"

#include <opencv2/opencv.hpp>

#include "ConfigManager.h"
#include "InferManager.h"
#include "PostProcessMan.h"
#include "PreProcessMan.h"

const std::string sConfigName = "onnx";
const std::string sConfigPath = "../config/config.csv";

const std::string sGroupPre = "preprocess";
const std::string sInputPath = "InputPath";
const std::string sOutputPath = "OutputPath";
const std::string sDstWidth = "DstWidth";
const std::string sDstHeight = "DstHeight";
const std::string sDstChannels = "DstChannels";
const std::string sIsNorm = "IsNorm";
const std::string sUseMeanStd = "UseMeanStd";
const std::string sIsBorder = "IsBorder";
const std::string sIsScale = "IsScale";

const std::string sGroupInfer = "modelinfer";
const std::string sModelPath = "ModelPath";
const std::string sModelName = "ModelName";
const std::string sModelType = "ModelType";

const std::string sGroupPost = "postprocess";
const std::string sType = "Type";
const std::string sClassNum = "ClassNum";
const std::string sClassName = "ClassName";
const std::string sClassColor = "ClassColor";
const std::string sConfThresh = "ConfThresh";
const std::string sNmsThresh = "NmsThresh";
const std::string sNeedSigmoid = "NeedSigmoid";

const std::string sPreProcessName = "Img2Img";
const std::string sModelInferName = "YOLOv5";
const std::string sPostProcessName = "Yolo";

namespace Task {

void ONNXTask::PreTask() {
  // 配置文件
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  rConfigManager.AddConfig(sConfigName, sConfigPath, Config::eConfigType::CSV);

  //   预处理模块
  PreProcess::PreProcessMan& rPreProcessMan =
      PreProcess::PreProcessMan::GetInstance();
  rPreProcessMan.AddPreProcess<cv::Mat, cv::Mat>(sPreProcessName);

  // 模型推理模块
  ModelInfer::InferManager& rInferManager =
      ModelInfer::InferManager::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);
  std::string sModelInferPath, sModelInferType;
  pConfig->GetString(sGroupInfer, sModelPath, sModelInferPath);
  pConfig->GetString(sGroupInfer, sModelType, sModelInferType);
  rInferManager.AddInfer<cv::Mat, ModelInfer::YoloOutput>(
      sModelInferName, sModelInferPath, sModelInferType);

  // 后处理模块
  PostProcess::PostProcessMan& rPostProcessMan =
      PostProcess::PostProcessMan::GetInstance();
  std::string sPostProcessType;
  pConfig->GetString(sGroupPost, sType, sPostProcessType);
  rPostProcessMan.AddPostProcess<ModelInfer::YoloOutput, cv::Mat>(
      sPostProcessName, sPostProcessType);
}

void ONNXTask::DoTask() {
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);

  // 从配置文件中读取图片相关参数
  std::string sSrcImgPath, sDstImgPath;
  pConfig->GetString(sGroupPre, sInputPath, sSrcImgPath);
  pConfig->GetString(sGroupPre, sOutputPath, sDstImgPath);

  cv::Mat mSrcImg = cv::imread(sSrcImgPath);
  cv::Mat mDstImg;

  if (mSrcImg.empty()) {
    std::cout << "图片加载失败" << std::endl;
    return;
  }

  // 为预处理模块读入参数
  PreProcess::PreProcessMan& rPreProcessMan =
      PreProcess::PreProcessMan::GetInstance();
  auto pPreProcess =
      rPreProcessMan.GetPreProcess<cv::Mat, cv::Mat>(sPreProcessName);

  PreProcess::Img2ImgParams& pPreParams = pPreProcess->GetParams();

  pPreParams.iSrcWidth = mSrcImg.cols;
  pPreParams.iSrcHeight = mSrcImg.rows;
  pPreParams.iSrcChannels = mSrcImg.channels();
  std::cout << "src width: " << pPreParams.iSrcWidth << std::endl;
  std::cout << "src height: " << pPreParams.iSrcHeight << std::endl;
  std::cout << "src channels: " << pPreParams.iSrcChannels << std::endl;

  pConfig->GetInt(sGroupPre, sDstWidth, pPreParams.iDstWidth);
  pConfig->GetInt(sGroupPre, sDstHeight, pPreParams.iDstHeight);
  pConfig->GetInt(sGroupPre, sDstChannels, pPreParams.iDstChannels);
  pConfig->GetBool(sGroupPre, sIsNorm, pPreParams.bIsNorm);
  pConfig->GetBool(sGroupPre, sUseMeanStd, pPreParams.bUseMeanStd);
  pConfig->GetBool(sGroupPre, sIsBorder, pPreParams.bIsBorder);
  pConfig->GetBool(sGroupPre, sIsScale, pPreParams.bIsScale);

  // 执行预处理操作
  pPreProcess->Process(mSrcImg, mDstImg);

  // 执行模型推理操作
  ModelInfer::YoloOutput vfInferOut;
  ModelInfer::InferManager& rInferManager =
      ModelInfer::InferManager::GetInstance();
  auto pModelInfer =
      rInferManager.GetInfer<cv::Mat, ModelInfer::YoloOutput>(sModelInferName);
  pModelInfer->Infer(mDstImg, vfInferOut);

  // 后处理
  int iClassNum;
  pConfig->GetInt(sGroupPost, sClassNum, iClassNum);

  std::vector<std::string> vClassNames(iClassNum);

  std::vector<std::vector<int>> vClassColors;
  vClassColors.reserve(iClassNum);

  std::vector<int> vClassColori(3, 0);

  // 读取ClassName和ClassColor
  for (int i = 1; i <= iClassNum; ++i) {
    std::string sClassNamei = sClassName + std::to_string(i);
    pConfig->GetString(sGroupPost, sClassNamei, vClassNames[i - 1]);

    std::string sClassColoriR = sClassColor + std::to_string(i) + "R";
    std::string sClassColoriG = sClassColor + std::to_string(i) + "G";
    std::string sClassColoriB = sClassColor + std::to_string(i) + "B";
    pConfig->GetInt(sGroupPost, sClassColoriR, vClassColori[0]);
    pConfig->GetInt(sGroupPost, sClassColoriG, vClassColori[1]);
    pConfig->GetInt(sGroupPost, sClassColoriB, vClassColori[2]);
    vClassColors.push_back(vClassColori);
  }

  std::cout << vClassColors.size() << std::endl;

  // 为后处理模块读入参数
  PostProcess::PostProcessMan& rPostProcessMan =
      PostProcess::PostProcessMan::GetInstance();
  auto pPostProcess =
      rPostProcessMan.GetPostProcess<ModelInfer::YoloOutput, cv::Mat>(
          sPostProcessName);

  std::shared_ptr<PostProcess::YoloParams> pPostParams =
      std::make_shared<PostProcess::YoloParams>();
  pPostParams->iClassNum = iClassNum;
  pPostParams->vClassNames = std::move(vClassNames);
  pPostParams->vClassColors = std::move(vClassColors);
  pPostParams->iInputWidth = mDstImg.cols;
  pPostParams->iInputHeight = mDstImg.rows;
  pPostParams->iInputChannels = mDstImg.channels();
  pPostParams->iOutputWidth = mSrcImg.cols;
  pPostParams->iOutputHeight = mSrcImg.rows;
  pPostParams->iOutputChannels = mSrcImg.channels();
  pConfig->GetDouble(sGroupPost, sConfThresh, pPostParams->dConfThresh);
  pConfig->GetDouble(sGroupPost, sNmsThresh, pPostParams->dNmsThresh);
  pConfig->GetBool(sGroupPost, sNeedSigmoid, pPostParams->bNeedSigmoid);
  pPostParams->bIsBorder = pPreParams.bIsBorder;
  pPostParams->bIsScale = pPreParams.bIsScale;
  pPostParams->iBorderWidth = pPreParams.iBorderWidth;
  pPostParams->iBorderHeight = pPreParams.iBorderHeight;

  pPostProcess->SetParams(pPostParams);

  pPostProcess->Process(vfInferOut, mSrcImg);
  cv::imshow("out", mSrcImg);
  cv::waitKey(0);
  cv::imwrite(sDstImgPath, mSrcImg);
}

void ONNXTask::PostTask() {}

TaskBase* CreateTask() { return new ONNXTask(); }

}  // namespace Task