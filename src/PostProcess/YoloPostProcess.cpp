#include "YoloPostProcess.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace PostProcess {
void YoloParams::PrintParam() {
  std::cout << "YoloParams:" << std::endl;
  std::cout << "iClassNum:" << iClassNum << std::endl;
  std::cout << "vClassNames:";
  for (auto& str : vClassNames) std::cout << str << " ";
  std::cout << std::endl;
  std::cout << "vClassColors:";
  for (auto& vColor : vClassColors)
    std::cout << "(" << vColor[0] << "," << vColor[1] << "," << vColor[2]
              << ") "
              << " ";
  std::cout << std::endl;
  std::cout << "iInputWidth:" << iInputWidth << std::endl;
  std::cout << "iInputHeight:" << iInputHeight << std::endl;
  std::cout << "iInputChannels:" << iInputChannels << std::endl;
  std::cout << "iOutputWidth:" << iOutputWidth << std::endl;
  std::cout << "iOutputHeight:" << iOutputHeight << std::endl;
  std::cout << "iOutputChannels:" << iOutputChannels << std::endl;
  std::cout << "dConfThresh:" << dConfThresh << std::endl;
  std::cout << "dNmsThresh:" << dNmsThresh << std::endl;
  std::cout << "bNeedSigmoid:" << bNeedSigmoid << std::endl;
  std::cout << "bIsBorder: " << bIsBorder << std::endl;
  std::cout << "bIsScale: " << bIsScale << std::endl;
  std::cout << "iBorderWidth: " << iBorderWidth << std::endl;
  std::cout << "iBorderHeight: " << iBorderHeight << std::endl;
}

int YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::Process(
    ModelInfer::YoloOutput& rYoloOutput, cv::Mat& mImg) {
  auto pParams = std::dynamic_pointer_cast<YoloParams>(m_pParams);
  if (!pParams) {
    std::cout << "后处理模块参数缺失" << std::endl;
    return -1;
  }
  if ((pParams->iClassNum != pParams->vClassNames.size()) ||
      (pParams->iClassNum != pParams->vClassColors.size())) {
    std::cout << "后处理模块参数iClassNum与其他输入不匹配" << std::endl;
    return -1;
  }

  pParams->PrintParam();

  if (mImg.empty()) {
    std::cout << "输入图片为空" << std::endl;
    return -1;
  }

  if (rYoloOutput.empty()) {
    std::cout << "输入数据为空" << std::endl;
    return -1;
  }

  // 解码输出数据
  DecodeOutput(rYoloOutput);

  std::vector<cv::Rect2f> vAllBoxes, vOutBoxes;
  std::vector<int> vAllClasses, vOutClasses;
  std::vector<float> vAllScores, vOutScores;

  for (const auto& vLayer : rYoloOutput) {
    // vLayer: size = 3 (Anchors)
    for (const auto& AnchorPreds : vLayer) {
      // AnchorPreds: size = H × W
      for (const auto& Anchor : AnchorPreds) {
        float fObjConf = Anchor.fObjectness;
        if (fObjConf < pParams->dConfThresh) continue;

        const std::vector<float>& vClassScores = Anchor.vClassScores;
        auto pMaxIt =
            std::max_element(vClassScores.begin(), vClassScores.end());
        int iClsId =
            static_cast<int>(std::distance(vClassScores.begin(), pMaxIt));
        float fClsConf = *pMaxIt;

        float fScore = fObjConf * fClsConf;
        // if (fScore < pParams->dConfThresh) continue;

        cv::Rect2f box = xywh2xyxy(Anchor.fX, Anchor.fY, Anchor.fW, Anchor.fH);

        vAllBoxes.push_back(box);
        vAllClasses.push_back(iClsId);
        vAllScores.push_back(fScore);
      }
    }
  }

  // NMS 按类分别处理
  std::map<int, std::vector<int>> vClsIndices;
  for (size_t i = 0; i < vAllClasses.size(); ++i) {
    vClsIndices[vAllClasses[i]].push_back(i);
  }

  for (const auto& KV : vClsIndices) {
    int iCls = KV.first;
    const std::vector<int>& vIndices = KV.second;

    std::vector<cv::Rect2f> vClsBoxes;
    std::vector<float> vClsScores;
    for (int idx : vIndices) {
      vClsBoxes.push_back(vAllBoxes[idx]);
      vClsScores.push_back(vAllScores[idx]);
    }

    std::vector<int> vKeep = Nms(vClsBoxes, vClsScores);
    for (int i : vKeep) {
      vOutBoxes.push_back(vClsBoxes[i]);
      vOutClasses.push_back(iCls);
      vOutScores.push_back(vClsScores[i]);
    }
  }

  Draw(mImg, vOutBoxes, vOutClasses, vOutScores);

  return 0;
}
cv::Rect2f YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::xywh2xyxy(
    float fCX, float fCY, float fW, float fH) {
  return {fCX - fW / 2.0f, fCY - fH / 2.0f, fW, fH};
}

std::vector<int> YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::Nms(
    const std::vector<cv::Rect2f>& vBoxes, const std::vector<float>& vScores) {
  auto pParams = std::dynamic_pointer_cast<YoloParams>(m_pParams);

  std::vector<int> vIndices;
  std::vector<int> vOrder(vBoxes.size());
  std::iota(vOrder.begin(), vOrder.end(), 0);

  std::sort(vOrder.begin(), vOrder.end(),
            [&](int i, int j) { return vScores[i] > vScores[j]; });

  while (!vOrder.empty()) {
    int i = vOrder.front();
    vIndices.push_back(i);
    vOrder.erase(vOrder.begin());

    std::vector<int> vNewOrder;
    for (int j : vOrder) {
      float fInter_area = (vBoxes[i] & vBoxes[j]).area();
      float fUnion_area = vBoxes[i].area() + vBoxes[j].area() - fInter_area;
      float fIou = fInter_area / fUnion_area;
      if (fIou <= pParams->dNmsThresh) {
        vNewOrder.push_back(j);
      }
    }
    vOrder = vNewOrder;
  }
  return vIndices;
}

void YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::DecodeOutput(
    ModelInfer::YoloOutput& rYoloOutput) {
  auto pParams = std::dynamic_pointer_cast<YoloParams>(m_pParams);

  for (size_t iLayerIdx = 0; iLayerIdx < rYoloOutput.size(); ++iLayerIdx) {
    auto& vLayerAnchors = rYoloOutput[iLayerIdx];
    const std::vector<int>& vLayerMask = m_vMasks[iLayerIdx];

    int iAnchorsNum = vLayerAnchors.size();
    int iGridNum = vLayerAnchors[0].size();  // Anchor 上 HxW 的格子数量
    int iGridSize = static_cast<int>(std::sqrt(iGridNum));

    for (int a = 0; a < iAnchorsNum; ++a) {
      auto AnchorWH = m_vAnchors[vLayerMask[a]];
      for (int i = 0; i < iGridNum; ++i) {
        int iRow = i / iGridSize;
        int iCol = i % iGridSize;

        auto& Anchor = vLayerAnchors[a][i];

        // 1. xy 解码
        if (pParams->bNeedSigmoid) {
          Anchor.fX = (Sigmoid(Anchor.fX) * 2.f - 0.5f + iCol) *
                      (float(pParams->iInputWidth) / iGridSize);
          Anchor.fY = (Sigmoid(Anchor.fY) * 2.f - 0.5f + iRow) *
                      (float(pParams->iInputHeight) / iGridSize);
        } else {
          Anchor.fX = (Anchor.fX * 2.f - 0.5f + iCol) *
                      (float(pParams->iInputWidth) / iGridSize);
          Anchor.fY = (Anchor.fY * 2.f - 0.5f + iRow) *
                      (float(pParams->iInputHeight) / iGridSize);
        }

        // 2. wh 解码
        float fPW = AnchorWH.first;
        float fPH = AnchorWH.second;
        if (pParams->bNeedSigmoid) {
          Anchor.fW = std::pow(Sigmoid(Anchor.fW) * 2.f, 2.f) * fPW;
          Anchor.fH = std::pow(Sigmoid(Anchor.fH) * 2.f, 2.f) * fPH;
        } else {
          Anchor.fW = std::pow(Anchor.fW * 2.f, 2.f) * fPW;
          Anchor.fH = std::pow(Anchor.fH * 2.f, 2.f) * fPH;
        }

        // 3. 尺度变换
        if (pParams->bIsBorder) {
          int iMidWidth, iMidHeight, iBorderWidth, iBorderHeight;
          if (pParams->bIsScale) {
            // 按照比例计算border的宽度
            double dWidthScale = static_cast<double>(pParams->iInputWidth) /
                                 pParams->iOutputWidth;
            double dHeightScale = static_cast<double>(pParams->iInputHeight) /
                                  pParams->iOutputHeight;
            double dScale = std::min(dWidthScale, dHeightScale);

            iMidWidth =
                static_cast<int>(std::round(pParams->iOutputWidth * dScale));
            iMidHeight =
                static_cast<int>(std::round(pParams->iOutputHeight * dScale));
            iBorderWidth = (pParams->iInputWidth - iMidWidth) / 2;
            iBorderHeight = (pParams->iInputHeight - iMidHeight) / 2;
          } else {
            // 按照输入的参数进行border填充
            iMidWidth = pParams->iInputWidth - 2 * pParams->iBorderWidth;
            iMidHeight = pParams->iInputHeight - 2 * pParams->iBorderHeight;
            iBorderWidth = pParams->iBorderWidth;
            iBorderHeight = pParams->iBorderHeight;
          }
          Anchor.fX = (Anchor.fX - iBorderWidth) *
                      float(pParams->iOutputWidth) / iMidWidth;
          Anchor.fY = (Anchor.fY - iBorderHeight) *
                      float(pParams->iOutputHeight) / iMidHeight;
          Anchor.fW = Anchor.fW * float(pParams->iOutputWidth) / iMidWidth;
          Anchor.fH = Anchor.fH * float(pParams->iOutputHeight) / iMidHeight;
        }

        // 4. objectness
        if (pParams->bNeedSigmoid) {
          Anchor.fObjectness = Sigmoid(Anchor.fObjectness);
        }

        // 5. class scores
        if (pParams->bNeedSigmoid) {
          for (auto& fScore : Anchor.vClassScores) {
            fScore = Sigmoid(fScore);
          }
        }
      }
    }
  }
}

void YoloPostProcess<ModelInfer::YoloOutput, cv::Mat>::Draw(
    cv::Mat& mImg, const std::vector<cv::Rect2f>& vBoxes,
    const std::vector<int>& vClasses, const std::vector<float>& vScores) {
  auto pParams = std::dynamic_pointer_cast<YoloParams>(m_pParams);

  for (int i = 0; i < vBoxes.size(); i++) {
    cv::Scalar pColor(pParams->vClassColors[vClasses[i]][0],
                      pParams->vClassColors[vClasses[i]][1],
                      pParams->vClassColors[vClasses[i]][2]);

    cv::rectangle(mImg, vBoxes[i], pColor);

    // 构造标签文本
    std::ostringstream pOss;
    pOss << pParams->vClassNames[vClasses[i]] << " ";
    pOss << std::fixed << std::setprecision(2) << vScores[i];

    std::string sLabel = pOss.str();

    // 画文字背景
    int baseLine = 0;
    cv::Size LabelSize =
        cv::getTextSize(sLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    int iTextY = std::max(int(vBoxes[i].y), LabelSize.height + 2);
    cv::rectangle(
        mImg, cv::Point(int(vBoxes[i].x), iTextY - LabelSize.height - 2),
        cv::Point(int(vBoxes[i].x) + LabelSize.width, iTextY + baseLine),
        pColor, cv::FILLED);

    // 画文字
    cv::putText(mImg, sLabel, cv::Point(int(vBoxes[i].x), iTextY),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
  }
}

}  // namespace PostProcess