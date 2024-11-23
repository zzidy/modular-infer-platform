#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <iostream>
#include <string>

namespace Config {

class ConfigBase {
 public:
  ConfigBase(std::string sName, std::string sPath)
      : m_sName(sName), m_sPath(sPath), m_bIsLoaded(false){};
  virtual ~ConfigBase() = default;

  //   获取配置文件名和地址
  std::string GetCfgName() { return m_sName; }
  std::string GetCfgPath() { return m_sPath; }

  //   判断是否加载成功
  bool IsLoaded() { return m_bIsLoaded; }
  void SetIsLoaded(bool bIsLoaded) { m_bIsLoaded = bIsLoaded; }

  //  打印配置文件
  virtual void Print() = 0;

  //   加载配置文件
  virtual bool LoadFromFile() = 0;

  //   读取配置文件
  virtual std::string GetString(const std::string& sKey) = 0;
  virtual int GetInt(const std::string& sKey) = 0;
  virtual double GetDouble(const std::string& sKey) = 0;
  virtual bool GetBool(const std::string& sKey) = 0;

  //   判断配置项是否存在
  virtual bool HasKey(const std::string& sKey) = 0;

 private:
  std::string m_sName;
  std::string m_sPath;
  bool m_bIsLoaded;
};

}  // namespace Config

#endif