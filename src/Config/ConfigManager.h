#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "ConfigBase.h"

namespace Config {

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
    static ConfigManager pCfgInstance;
    return pCfgInstance;
  }

  bool AddConfig(const std::string& sName, const std::string& sPath);
};

}  // namespace Config

#endif