# add_library(ModelInfer SHARED InferManager.cpp ONNXInfer.cpp)
# target_include_directories(ModelInfer PRIVATE ${ONNXRUNTIME_INCLUDE_DIRS})
# target_link_libraries(ModelInfer PUBLIC ${ONNXRUNTIME_LIBRARIES})

set(USE_ONNX ${USE_ONNX} CACHE BOOL "Use ONNXRuntime")

# 设置源文件列表
set(SOURCES InferManager.cpp)

if(USE_ONNX)
    list(APPEND SOURCES ONNXInfer.cpp)
endif()

add_library(ModelInfer SHARED ${SOURCES})

if(USE_ONNX)
    target_include_directories(ModelInfer PRIVATE ${ONNXRUNTIME_INCLUDE_DIRS})
    target_link_libraries(ModelInfer PUBLIC ${ONNXRUNTIME_LIBRARIES})
endif()