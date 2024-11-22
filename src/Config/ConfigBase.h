#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <iostream>
#include <string>

namespace Config {

class ConfigBase {
 public:
  ConfigBase(std::string sName, std::string sPath)
      : m_sName(sName), m_sPath(sPath){};
  virtual ~ConfigBase() = default;

  //   获取配置文件名和地址
  std::string GetCfgName() { return m_sName; }
  std::string GetCfgPath() { return m_sPath; }

  //   加载配置文件
  virtual bool LoadFromFile(const std::string sFilePath) = 0;
  virtual bool LoadFromString(const std::string sCfgData) = 0;

  virtual bool isLoaded() { return m_bIsLoaded; };

  virtual void Print() = 0;

  //   读取配置文件
  virtual std::string GetString(const std::string& sKey) = 0;
  virtual int GetInt(const std::string& sKey) = 0;
  virtual double GetDouble(const std::string& sKey) = 0;
  virtual bool GetBool(const std::string& sKey) = 0;
  virtual bool HasKey(const std::string& sKey) = 0;

 private:
  std::string m_sName;
  std::string m_sPath;
  bool m_bIsLoaded = false;
};

}  // namespace Config

#endif