#include "ConfigTask.h"

#include "ConfigManager.h"

const std::string sConfigName = "ConfigTest";
const std::string sConfigPath = "../config/test.csv";
const std::string sGroup1 = "1";
const std::string sGroup2 = "2";
const std::string sGroup3 = "3";
const std::string sNameA = "A";
const std::string sNameB = "B";
const std::string sNameC = "C";

namespace Task {
void ConfigTask::PreTask() {
  // 加载配置文件
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  rConfigManager.AddConfig(sConfigName, sConfigPath, Config::eConfigType::CSV);
}

void ConfigTask::DoTask() {
  Config::ConfigManager& rConfigManager = Config::ConfigManager::GetInstance();
  auto pConfig = rConfigManager.GetConfig(sConfigName);
  if (!pConfig) {
    return;
  }
  std::string str;
  int i;
  double d;
  bool b;

  pConfig->GetString(sGroup1, sNameA, str);
  std::cout << "Group1 A: " << str << std::endl;
  pConfig->GetInt(sGroup1, sNameB, i);
  std::cout << "Group1 B: " << i << std::endl;
  pConfig->GetDouble(sGroup2, sNameA, d);
  std::cout << "Group2 A: " << d << std::endl;
  pConfig->GetDouble(sGroup2, sNameB, d);
  std::cout << "Group2 B: " << d << std::endl;
  pConfig->GetBool(sGroup3, sNameC, b);
  std::cout << "Group3 C: " << b << std::endl;
  std::cout << pConfig->GetString(sGroup3, sNameA, str) << std::endl;
  std::cout << pConfig->GetString(sGroup1, sNameB, str) << std::endl;
}

void ConfigTask::PostTask() {}

TaskBase* CreateTask() { return new ConfigTask(); }
}  // namespace Task