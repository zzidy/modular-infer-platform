#include "PostProcessMan.h"

#include "YoloPostProcess.h"

namespace PostProcess {

PostProcessMan::PostProcessMan() { m_mapPostProcess.clear(); }
PostProcessMan::~PostProcessMan() {}

template <typename InputType, typename OutputType>
int PostProcessMan::AddPostProcess(const std::string& sName,
                                   ePostProcessType eType) {
  if (eType == ePostProcessType::Yolo) {
    if (typeid(InputType) == typeid(ModelInfer::YoloOutput) &&
        typeid(OutputType) == typeid(cv::Mat)) {
      if (m_mapPostProcess.find(sName) == m_mapPostProcess.end()) {
        m_mapPostProcess[sName] =
            std::make_shared<YoloPostProcess<InputType, OutputType>>(sName);
        std::cout << "Add PostProcess: " << sName << std::endl;
        return 0;
      }
      std::cout << "PostProcess: " << sName << " 已存在" << std::endl;
      return -1;
    }
    std::cout << "当前Yolo后处理还不支持输入：" << typeid(InputType)
              << "，输出:" << typeid(OutputType) << "的格式" << std::endl;
    return -1;
  }

  std::cout << "当前后处理格式还不支持" << std::endl;
  return -1;
}
template <typename InputType, typename OutputType>
std::shared_ptr<BasePostProcess<InputType, OutputType>>
PostProcessMan::GetPostProcess(const std::string& sName) {
  if (m_mapPostProcess.find(sName) == m_mapPostProcess.end()) return nullptr;
  return std::static_pointer_cast<BasePostProcess<InputType, OutputType>>(
      m_mapPostProcess[sName]);
}

// 显示实例化
template int PostProcessMan::AddPostProcess<ModelInfer::YoloOutput, cv::Mat>(
    const std::string& sName, ePostProcessType eType);
template std::shared_ptr<BasePostProcess<ModelInfer::YoloOutput, cv::Mat>>
PostProcessMan::GetPostProcess(const std::string& sName);
}  // namespace PostProcess