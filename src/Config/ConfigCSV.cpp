#include "ConfigCSV.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Config {
ConfigCSV::ConfigCSV(std::string sName, std::string sPath)
    : ConfigBase(sName, sPath) {
  bool bIsLoaded = LoadFromFile();
  SetIsLoaded(bIsLoaded);

  if (bIsLoaded) {
    std::cout << "配置文件 " << GetCfgName() << " 加载成功" << std::endl;
    PrintConfig();
  } else {
    std::cout << "配置文件 " << GetCfgName() << " 加载失败" << std::endl;
  }
}

ConfigCSV::~ConfigCSV() {}

void ConfigCSV::PrintConfig() {
  if (!IsLoaded()) return;

  std::cout << "配置文件 " << GetCfgName() << " 内容如下：" << std::endl;
  for (auto& itGroup : m_mapCfgData) {
    for (auto& itItem : itGroup.second) {
      std::cout << itGroup.first << " " << itItem.first << " "
                << itItem.second.sType << " " << itItem.second.sValue
                << std::endl;
    }
  }
}

bool ConfigCSV::LoadFromFile() {
  if (IsLoaded()) {
    std::cout << "配置文件 " << GetCfgName() << " 已加载" << std::endl;
    return false;
  }

  std::ifstream fCSV(GetCfgPath());
  if (!fCSV.is_open()) {
    std::cout << "打开配置文件 " << GetCfgName() << " 失败" << std::endl;
    return false;
  }

  // 核对CSV标题行格式是否正确
  std::string sHeader;
  std::getline(fCSV, sHeader);
  if (sHeader != "Group,Name,Type,Value") {
    std::cout << "配置文件 " << GetCfgName() << " 格式错误" << std::endl;
    return false;
  }

  // 逐行解析csv文件
  std::string sLine;
  while (std::getline(fCSV, sLine)) {
    if (_ParseCsvLine(sLine) != 0) {
      return false;
    }
  }

  fCSV.close();
  return true;
}

int ConfigCSV::GetString(const std::string& sGroup, const std::string& sName,
                         std::string& sOut) {
  if (!IsLoaded()) return -1;
  if (!HasKey(sGroup, sName)) return -1;
  if (m_mapCfgData[sGroup][sName].sType != "string") return -1;
  sOut = m_mapCfgData[sGroup][sName].sValue;
  return 0;
}

int ConfigCSV::GetInt(const std::string& sGroup, const std::string& sName,
                      int& iOut) {
  if (!IsLoaded()) return -1;
  if (!HasKey(sGroup, sName)) return -1;
  if (m_mapCfgData[sGroup][sName].sType != "int") return -1;
  iOut = std::stoi(m_mapCfgData[sGroup][sName].sValue);
  return 0;
}

int ConfigCSV::GetDouble(const std::string& sGroup, const std::string& sName,
                         double& dOut) {
  if (!IsLoaded()) return -1;
  if (!HasKey(sGroup, sName)) return -1;
  if (m_mapCfgData[sGroup][sName].sType != "double") return -1;
  dOut = std::stod(m_mapCfgData[sGroup][sName].sValue);
  return 0;
}

int ConfigCSV::GetBool(const std::string& sGroup, const std::string& sName,
                       bool& bOut) {
  if (!IsLoaded()) return -1;
  if (!HasKey(sGroup, sName)) return -1;
  if (m_mapCfgData[sGroup][sName].sType != "bool") return -1;
  bOut = std::stoi(m_mapCfgData[sGroup][sName].sValue);
  return 0;
}

bool ConfigCSV::HasKey(const std::string& sGroup, const std::string& sName) {
  if (!IsLoaded()) return false;
  if ((m_mapCfgData.find(sGroup) != m_mapCfgData.end()) &&
      m_mapCfgData[sGroup].find(sName) != m_mapCfgData[sGroup].end())
    return true;
  return false;
}

int ConfigCSV::_ParseCsvLine(const std::string& sLine) {
  std::istringstream ssLine(sLine);
  std::string sGroup, sName, sType, sValue;

  if (std::getline(ssLine, sGroup, ',') && std::getline(ssLine, sName, ',') &&
      std::getline(ssLine, sType, ',') && std::getline(ssLine, sValue, ',')) {
    std::transform(sType.begin(), sType.end(), sType.begin(),
                   [](char c) { return std::tolower(c); });
    m_mapCfgData[sGroup][sName] = CsvData(sGroup, sName, sType, sValue);
  }

  return 0;
}

}  // namespace Config