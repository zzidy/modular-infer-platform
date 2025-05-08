#ifndef BASEINFER_H
#define BASEINFER_H

#include <iostream>
#include <string>
#include <vector>

namespace ModelInfer {

template <typename InputType, typename OutputType>
class BaseInfer {
 public:
  BaseInfer(std::string sModelName, std::string sModelPath)
      : m_sModelName(sModelName),
        m_sModelPath(sModelPath),
        m_bIsLoaded(false){};
  virtual ~BaseInfer() = default;

  // 模型推理相关接口
  virtual bool LoadModel() = 0;
  virtual int Infer(InputType& rInput, OutputType& rOutput) = 0;

  // 读取私有成员变量
  std::string GetModelName() { return m_sModelName; }
  std::string GetModelPath() { return m_sModelPath; }
  bool IsLoaded() { return m_bIsLoaded; }
  void SetIsLoaded(bool bIsLoaded) { m_bIsLoaded = bIsLoaded; }

 private:
  std::string m_sModelName;
  std::string m_sModelPath;
  bool m_bIsLoaded;
};

// yolo的输出数据格式
struct YoloAnchor {
  float x, y, w, h;                 // bbox
  float objectness;                 // 置信度
  std::vector<float> class_scores;  // 分类得分
};
using YoloOutput = std::vector<std::vector<std::vector<YoloAnchor>>>;
// 第1层vector：3个特征层
// 第2层vector：每个特征层里有 3 个 anchor
// 第3层vector：每个 anchor 上的 HxW 个格子

}  // namespace ModelInfer
#endif