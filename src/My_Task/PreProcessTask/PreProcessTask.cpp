#include "PreProcessTask.h"

#include <opencv2/opencv.hpp>

#include "ConfigManager.h"
#include "PreProcessMan.h"

const std::string sConfigName = "preprocess";
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

const std::string sPreProcessName = "Img2Img";

namespace Task {
void PreProcessTask::PreTask() {
  // 配置文件
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  rConfigManager.AddConfig(sConfigName, sConfigPath, Config::eConfigType::CSV);

  //   预处理模块
  PreProcess::PreProcessMan& rPreProcessMan =
      PreProcess::PreProcessMan::GetInstance();
  rPreProcessMan.AddPreProcess<cv::Mat, cv::Mat>(sPreProcessName);
}

void PreProcessTask::DoTask() {
  // 加载配置文件指针
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
}

void PreProcessTask::PostTask() {}

TaskBase* CreateTask() { return new PreProcessTask(); }
}  // namespace Task