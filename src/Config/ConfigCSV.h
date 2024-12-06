#ifndef CONFIGCSV_H
#define CONFIGCSV_H

#include "ConfigBase.h"

namespace Config {

// CSV的配置文件格式为：Group,Name,Type,Value
struct CsvData {
  std::string sGroup;
  std::string sName;
  std::string sType;
  std::string sValue;
};

class ConfigCSV : public ConfigBase {
 private:
  std::unordered_map<std::string, std::unordered_map<std::string, CsvData>>
      m_mapCfgData;
  void ParseCsvLine(const std::string& sLine);

 public:
  ConfigCSV(std::string sName, std::string sPath);
  ~ConfigCSV();

  //  打印配置文件
  void Print() override;

  //   加载配置文件
  bool LoadFromFile() override;

  //   读取配置文件
  std::string GetString(const std::string& sKey) override;
  int GetInt(const std::string& sKey) override;
  double GetDouble(const std::string& sKey) override;
  bool GetBool(const std::string& sKey) override;

  //   判断配置项是否存在
  bool HasKey(const std::string& sKey) override;
};

}  // namespace Config

#endif