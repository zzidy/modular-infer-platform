#include "ConfigCSV.h"

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Config {
ConfigCSV::ConfigCSV(std::string sName, std::string sPath)
    : ConfigBase(sName, sPath) {
  bool bIsLoaded = LoadFromFile();
  SetIsLoaded(bIsLoaded);

  if (bIsLoaded) {
    std::cout << "配置文件 " << GetCfgName() << " 加载成功" << std::endl;
    Print();
  } else {
    std::cout << "配置文件 " << GetCfgName() << " 加载失败" << std::endl;
  }
}

ConfigCSV::~ConfigCSV() {}

void ConfigCSV::Print() {}

bool ConfigCSV::LoadFromFile() {
  std::ifstream fCSV(GetCfgPath());
  if (!fCSV.is_open()) {
    std::cout << "打开配置文件 " << GetCfgName() << " 失败" << std::endl;
    return false;
  }

  // TODO：核对CSV标题行格式是否正确

  std::string line;
  bool isHeader = true;
  while (std::getline(fCSV, line)) {
    if (isHeader) {
      isHeader = false;  // 跳过标题行
      continue;
    }
    parseLine(line);
  }

  file.close();
  return true;

  return false;
}

std::string ConfigCSV::GetString(const std::string& sKey) { return ""; }

int ConfigCSV::GetInt(const std::string& sKey) { return 0; }

double ConfigCSV::GetDouble(const std::string& sKey) { return 0.0; }

bool ConfigCSV::GetBool(const std::string& sKey) { return false; }

bool ConfigCSV::HasKey(const std::string& sKey) { return false; }

void ConfigCSV::ParseCsvLine(const std::string& sLine) {}

}  // namespace Config