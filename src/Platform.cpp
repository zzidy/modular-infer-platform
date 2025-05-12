#include <dlfcn.h>

#include <iostream>

#include "ConfigManager.h"
#include "InferManager.h"
#include "PostProcessMan.h"
#include "PreProcessMan.h"
#include "Task.h"

// 初始化
void Init() {
  Config::ConfigManager::GetInstance();
  ModelInfer::InferManager::GetInstance();
  PreProcess::PreProcessMan::GetInstance();
  PostProcess::PostProcessMan::GetInstance();
}

int main() {
  // 业务库路径
  const char* sLibPath = "../lib/libTask.so";

  // 加载业务库
  void* pHandle = dlopen(sLibPath, RTLD_LAZY);
  if (!pHandle) {
    std::cerr << "无法加载业务库: " << dlerror() << "\n";
    return -1;
  }

  // 获取工厂函数
  using CreateTaskFunc = Task::Task* (*)();
  CreateTaskFunc fCreateTask = (CreateTaskFunc)dlsym(pHandle, "CreateTask");
  if (!fCreateTask) {
    std::cerr << "无法加载CreateTask函数: " << dlerror() << "\n";
    dlclose(pHandle);
    return -1;
  }

  // 创建任务对象
  Task::Task* pTask = fCreateTask();
  if (!pTask) {
    std::cerr << "无法创建业务对象.\n";
    dlclose(pHandle);
    return -1;
  }

  // 执行任务
  pTask->PreTask();
  pTask->DoTask();
  pTask->PostTask();

  // 释放资源
  delete pTask;
  dlclose(pHandle);

  return 0;
}