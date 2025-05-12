#include "PostProcessTask.h"

#include <opencv2/opencv.hpp>

#include "ConfigManager.h"
#include "InferManager.h"
#include "PostProcessMan.h"

const std::string sConfigName = "postprocess";
const std::string sConfigPath = "../config/config.csv";

const std::string sGroupPost = "postprocess";
const std::string sType = "Type";
const std::string sClassNum = "ClassNum";
const std::string sClassName = "ClassName";
const std::string sClassColor = "ClassColor";
const std::string sInputWidth = "InputWidth";
const std::string sInputHeight = "InputHeight";
const std::string sInputChannels = "InputChannels";
const std::string sOutputWidth = "OutputWidth";
const std::string sOutputHeight = "OutputHeight";
const std::string sOutputChannels = "OutputChannels";
const std::string sConfThresh = "ConfThresh";
const std::string sNmsThresh = "NmsThresh";
const std::string sNeedSigmoid = "NeedSigmoid";

const std::string sPostProcessName = "Yolo";

namespace Task {
void PostProcessTask::PreTask() {
  // 配置文件
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  rConfigManager.AddConfig(sConfigName, sConfigPath, Config::eConfigType::CSV);

  //   后处理模块
  PostProcess::PostProcessMan& rPostProcessMan =
      PostProcess::PostProcessMan::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);
  std::string sPostProcessType;
  pConfig->GetString(sGroupPost, sType, sPostProcessType);
  rPostProcessMan.AddPostProcess<ModelInfer::YoloOutput, cv::Mat>(
      sPostProcessName, sPostProcessType);
}

void PostProcessTask::DoTask() {
  // 加载配置文件指针
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);

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

  std::shared_ptr<PostProcess::YoloParams> pParams =
      std::make_shared<PostProcess::YoloParams>();
  pParams->iClassNum = iClassNum;
  pParams->vClassNames = std::move(vClassNames);
  pParams->vClassColors = std::move(vClassColors);
  pConfig->GetInt(sGroupPost, sInputWidth, pParams->iInputWidth);
  pConfig->GetInt(sGroupPost, sInputHeight, pParams->iInputHeight);
  pConfig->GetInt(sGroupPost, sInputChannels, pParams->iInputChannels);
  pConfig->GetInt(sGroupPost, sOutputWidth, pParams->iOutputWidth);
  pConfig->GetInt(sGroupPost, sOutputHeight, pParams->iOutputHeight);
  pConfig->GetInt(sGroupPost, sOutputChannels, pParams->iOutputChannels);
  pConfig->GetDouble(sGroupPost, sConfThresh, pParams->dConfThresh);
  pConfig->GetDouble(sGroupPost, sNmsThresh, pParams->dNmsThresh);
  pConfig->GetBool(sGroupPost, sNeedSigmoid, pParams->bNeedSigmoid);

  pPostProcess->SetParams(pParams);
}

void PostProcessTask::PostTask() {}

TaskBase* CreateTask() { return new PostProcessTask(); }
}  // namespace Task