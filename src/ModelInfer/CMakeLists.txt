add_library(ModelInfer SHARED InferManager.cpp ONNXInfer.cpp)
target_include_directories(ModelInfer PRIVATE ${ONNXRUNTIME_INCLUDE_DIRS})
target_link_libraries(ModelInfer PUBLIC ${ONNXRUNTIME_LIBRARIES})