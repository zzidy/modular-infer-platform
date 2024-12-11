#include "PreProcessTask.h"

#include <opencv2/opencv.hpp>

#include "ConfigManager.h"
#include "PreProcessMan.h"

const std::string sConfigName = "preprocess";
const std::string sConfigPath = "../config/config.csv";
const std::string sGroupPre = "preprocess";
const std::string sSrcWidth = "SrcWidth";
const std::string sSrcHeight = "SrcHeight";
const std::string SrcChannels = "SrcChannels";
const std::string sDstWidth = "DstWidth";
const std::string sDstHeight = "DstHeight";
const std::string sDstChannels = "DstChannels";
const std::string sIsNorm = "IsNorm";
const std::string sMean0 = "Mean0";
const std::string sMean1 = "Mean1";
const std::string sMean2 = "Mean2";
const std::string sStd0 = "Std0";
const std::string sStd1 = "Std1";
const std::string sStd2 = "Std2";
const std::string sIsBorder = "IsBorder";
const std::string sIsScale = "IsScale";
const std::string sBorderWidth = "BorderWidth";
const std::string sBorderHeight = "BorderHeight";

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
  // 从配置文件中获取参数，并传入预处理模块
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);

  PreProcess::PreProcessMan& rPreProcessMan =
      PreProcess::PreProcessMan::GetInstance();
  auto pPreProcess =
      rPreProcessMan.GetPreProcess<cv::Mat, cv::Mat>(sPreProcessName);

  PreProcess::Img2ImgParams& pParams = pPreProcess->GetParams();
  pConfig->GetInt(sGroupPre, sSrcWidth, pParams.iSrcWidth);
  pConfig->GetInt(sGroupPre, sSrcHeight, pParams.iSrcHeight);
  pConfig->GetInt(sGroupPre, SrcChannels, pParams.iSrcChannels);
  pConfig->GetInt(sGroupPre, sDstWidth, pParams.iDstWidth);
  pConfig->GetInt(sGroupPre, sDstHeight, pParams.iDstHeight);
  pConfig->GetInt(sGroupPre, sDstChannels, pParams.iDstChannels);
  pConfig->GetBool(sGroupPre, sIsNorm, pParams.bIsNorm);
  std::vector<double> vMean(3);
  pConfig->GetDouble(sGroupPre, sMean0, vMean[0]);
  pConfig->GetDouble(sGroupPre, sMean1, vMean[1]);
  pConfig->GetDouble(sGroupPre, sMean2, vMean[2]);
  pParams.vMean = vMean;
  std::vector<double> vStd(3);
  pConfig->GetDouble(sGroupPre, sStd0, vStd[0]);
  pConfig->GetDouble(sGroupPre, sStd1, vStd[1]);
  pConfig->GetDouble(sGroupPre, sStd2, vStd[2]);
  pParams.vStd = vStd;
  pConfig->GetBool(sGroupPre, sIsBorder, pParams.bIsBorder);
  pConfig->GetBool(sGroupPre, sIsScale, pParams.bIsScale);
  pConfig->GetInt(sGroupPre, sBorderWidth, pParams.iBorderWidth);
  pConfig->GetInt(sGroupPre, sBorderHeight, pParams.iBorderHeight);

  //   pParams.PrintParam();
  //   pPreProcess->SetParams(pParams);
  pPreProcess->GetParams().PrintParam();
}

void PreProcessTask::PostTask() {}

TaskBase* CreateTask() { return new PreProcessTask(); }
}  // namespace Task