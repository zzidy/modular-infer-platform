#ifndef PREPROCESSMAN_H
#define PREPROCESSMAN_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "PreProcess.h"

namespace PreProcess {
class PreProcessMan {
 private:
  PreProcessMan();
  std::unordered_map<std::string, std::shared_ptr<void>> m_mapPreProcess;

 public:
  ~PreProcessMan();
  PreProcessMan(const PreProcessMan&) = delete;
  PreProcessMan& operator=(const PreProcessMan&) = delete;

  static PreProcessMan& GetInstance() {
    static PreProcessMan instance;
    return instance;
  }

  template <typename InputType, typename OutputType>
  int AddPreProcess(const std::string& sName);

  template <typename InputType, typename OutputType>
  std::shared_ptr<PreProcess<InputType, OutputType>> GetPreProcess(
      const std::string& sName);
};

}  // namespace PreProcess

#endif