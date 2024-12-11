#include "PreProcessMan.h"

#include <opencv2/opencv.hpp>

namespace PreProcess {
PreProcessMan::PreProcessMan() { m_mapPreProcess.clear(); }
PreProcessMan::~PreProcessMan() {}

template <typename InputType, typename OutputType>
int PreProcessMan::AddPreProcess(const std::string& sName) {
  if (typeid(InputType) == typeid(cv::Mat) &&
      typeid(OutputType) == typeid(cv::Mat)) {
    if (m_mapPreProcess.find(sName) == m_mapPreProcess.end()) {
      m_mapPreProcess[sName] =
          std::make_shared<PreProcess<InputType, OutputType>>();
      std::cout << "Add PreProcess: " << sName << std::endl;
      return 0;
    }
    std::cout << "PreProcess: " << sName << " 已存在" << std::endl;
    return -1;
  }
  std::cout << "不支持的预处理类型" << std::endl;
  return -1;
}

template <typename InputType, typename OutputType>
std::shared_ptr<PreProcess<InputType, OutputType>> PreProcessMan::GetPreProcess(
    const std::string& sName) {
  if (m_mapPreProcess.find(sName) == m_mapPreProcess.end()) return nullptr;
  return std::static_pointer_cast<PreProcess<InputType, OutputType>>(
      m_mapPreProcess[sName]);
}

// 显示实例化
template int PreProcessMan::AddPreProcess<cv::Mat, cv::Mat>(
    const std::string& sName);
template std::shared_ptr<PreProcess<cv::Mat, cv::Mat>>
PreProcessMan::GetPreProcess<cv::Mat, cv::Mat>(const std::string& sName);

}  // namespace PreProcess