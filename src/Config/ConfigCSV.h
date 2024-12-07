#ifndef CONFIGCSV_H
#define CONFIGCSV_H

#include <unordered_map>

#include "ConfigBase.h"

namespace Config {

// CSV的配置文件格式为：Group,Name,Type,Value
struct CsvData {
  std::string sGroup;
  std::string sName;
  std::string sType;
  std::string sValue;

  CsvData(std::string sGroup, std::string sName, std::string sType,
          std::string sValue)
      : sGroup(sGroup), sName(sName), sType(sType), sValue(sValue) {}

  CsvData() = default;
};

class ConfigCSV : public ConfigBase {
 private:
  std::unordered_map<std::string, std::unordered_map<std::string, CsvData>>
      m_mapCfgData;
  int _ParseCsvLine(const std::string& sLine);

 public:
  ConfigCSV(std::string sName, std::string sPath);
  ~ConfigCSV();

  //  打印配置文件
  void PrintConfig() override;

  //   加载配置文件
  bool LoadFromFile() override;

  //   读取配置文件

  int GetString(const std::string& sGroup, const std::string& sName,
                std::string& sOut) override;
  int GetInt(const std::string& sGroup, const std::string& sName,
             int& iOut) override;
  int GetDouble(const std::string& sGroup, const std::string& sName,
                double& dOut) override;
  int GetBool(const std::string& sGroup, const std::string& sName,
              bool& bOut) override;

  //   判断配置项是否存在
  bool HasKey(const std::string& sGroup, const std::string& sName) override;
};

}  // namespace Config

#endif