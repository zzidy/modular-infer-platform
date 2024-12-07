#include "ConfigManager.h"

#include "ConfigCSV.h"

namespace Config {

ConfigManager::ConfigManager() { m_mapCfg.clear(); }

ConfigManager::~ConfigManager() {}

int ConfigManager::AddConfig(const std::string& sName, const std::string& sPath,
                             eConfigType eType) {
  // 添加CSV类型的配置文件
  if (eType == eConfigType::CSV) {
    if (m_mapCfg.find(sName) == m_mapCfg.end()) {
      m_mapCfg[sName] = std::make_shared<ConfigCSV>(sName, sPath);

      if (!m_mapCfg[sName]->IsLoaded()) {
        std::cout << "加载配置文件 " << sName << ":" << sPath << " 失败"
                  << std::endl;
      } else {
        std::cout << "加载配置文件 " << sName << ":" << sPath << " 成功"
                  << std::endl;
      }

      if (m_mapCfg[sName]->IsLoaded())
        return 0;
      else
        return -1;
    }

    std::cout << "该CSV配置文件 " << sName << ":" << sPath << " 已存在"
              << std::endl;
    return -1;
  }

  std::cout << "当前配置文件格式还不支持" << std::endl;
  return -1;
}

std::shared_ptr<ConfigBase> ConfigManager::GetConfig(const std::string& sName) {
  if (m_mapCfg.find(sName) == m_mapCfg.end()) {
    return nullptr;
  }
  return m_mapCfg[sName];
}

}  // namespace Config