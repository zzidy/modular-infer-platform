#include "ONNXTask.h"

#include <opencv2/opencv.hpp>

#include "ConfigManager.h"
#include "InferManager.h"
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
const std::string sIsBorder = "IsBorder";
const std::string sIsScale = "IsScale";

const std::string sGroupInfer = "modelinfer";
const std::string sModelPath = "ModelPath";
const std::string sModelName = "ModelName";
const std::string sModelType = "ModelType";

const std::string sGroupPost = "postprocess";

const std::string sPreProcessName = "Img2Img";
const std::string sModelInferName = "YOLOv5";

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

  PreProcess::Img2ImgParams& pParams = pPreProcess->GetParams();

  pParams.iSrcWidth = mSrcImg.cols;
  pParams.iSrcHeight = mSrcImg.rows;
  pParams.iSrcChannels = mSrcImg.channels();
  std::cout << "src width: " << pParams.iSrcWidth << std::endl;
  std::cout << "src height: " << pParams.iSrcHeight << std::endl;
  std::cout << "src channels: " << pParams.iSrcChannels << std::endl;

  pConfig->GetInt(sGroupPre, sDstWidth, pParams.iDstWidth);
  pConfig->GetInt(sGroupPre, sDstHeight, pParams.iDstHeight);
  pConfig->GetInt(sGroupPre, sDstChannels, pParams.iDstChannels);
  pConfig->GetBool(sGroupPre, sIsNorm, pParams.bIsNorm);
  pConfig->GetBool(sGroupPre, sIsBorder, pParams.bIsBorder);
  pConfig->GetBool(sGroupPre, sIsScale, pParams.bIsScale);

  // 执行预处理操作
  pPreProcess->Process(mSrcImg, mDstImg);

  cv::imshow("dst", mDstImg);
  cv::waitKey(0);
  cv::imwrite(sDstImgPath, mDstImg);

  // 执行模型推理操作
  ModelInfer::YoloOutput vfInferOut;
  ModelInfer::InferManager& rInferManager =
      ModelInfer::InferManager::GetInstance();
  auto pModelInfer =
      rInferManager.GetInfer<cv::Mat, ModelInfer::YoloOutput>(sModelInferName);
  pModelInfer->Infer(mDstImg, vfInferOut);
}

void ONNXTask::PostTask() {}

TaskBase* CreateTask() { return new ONNXTask(); }

}  // namespace Task