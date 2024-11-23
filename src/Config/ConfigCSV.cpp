#include "ConfigCSV.h"

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

}  // namespace Config