#include "ConfigManager.h"

namespace Config {

ConfigManager::ConfigManager() { m_mapCfg.clear(); }

ConfigManager::~ConfigManager() {}

bool ConfigManager::AddConfig(const std::string& sName,
                              const std::string& sPath) {
  m_mapCfg[sName] = std::make_shared<ConfigBase>(sName, sPath);
  return false;
}

}  // namespace Config