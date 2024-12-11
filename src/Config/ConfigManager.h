#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "ConfigBase.h"

namespace Config {

enum class eConfigType { CSV };  // 当前支持的配置文件类型

// 单例模式的管理者
class ConfigManager {
 private:
  ConfigManager();
  std::unordered_map<std::string, std::shared_ptr<ConfigBase>> m_mapCfg;

 public:
  ~ConfigManager();
  ConfigManager(const ConfigManager&) = delete;
  ConfigManager& operator=(const ConfigManager&) = delete;
  static ConfigManager& GetInstance() {
    static ConfigManager rCfgInstance;
    return rCfgInstance;
  }

  // 为管理者添加配置文件
  int AddConfig(const std::string& sName, const std::string& sPath,
                eConfigType eType);
  std::shared_ptr<ConfigBase> GetConfig(const std::string& sName);
};

}  // namespace Config

#endif