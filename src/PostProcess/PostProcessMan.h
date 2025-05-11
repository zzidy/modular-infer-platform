#ifndef POSTPROCESSMAN_H
#define POSTPROCESSMAN_H

#include <iostream>
#include <unordered_map>

#include "BasePostProcess.h"

namespace PostProcess {

enum class PostProcessType { Yolo };

class PostProcessMan {
 private:
  PostProcessMan();
  std::unordered_map<std::string, std::shared_ptr<void>> postProcessMap;

 public:
  ~PostProcessMan();
  PostProcessMan(const PostProcessMan&) = delete;
  PostProcessMan& operator=(const PostProcessMan&) = delete;
  static PostProcessMan& GetInstance() {
    static PostProcessMan instance;
    return instance;
  }

  template <typename InputType, typename OutputType>
  int AddPostProcess(const std::string& sName);

  template <typename InputType, typename OutputType>
  std::shared_ptr<BasePostProcess<InputType, OutputType>> GetPostProcess(
      const std::string& sName);
};
}  // namespace PostProcess

#endif