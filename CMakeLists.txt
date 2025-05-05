cmake_minimum_required(VERSION 3.10)
project(Platform)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/bin)

# 编译选项
option(USE_ONNX "Enable ONNX Runtime support" ON)

# opencv库
find_package(OpenCV REQUIRED)

# 设置onnxruntime库地址
if(USE_ONNX)
        message(STATUS "ONNX Runtime support is ENABLED")
        set(ONNXRUNTIME_DIR /usr/local)
        set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_DIR}/include)
        set(ONNXRUNTIME_LIBRARIES ${ONNXRUNTIME_DIR}/lib/libonnxruntime.so)
else()
        message(STATUS "ONNX Runtime support is DISABLED")
endif()

add_subdirectory(src/Config)
add_subdirectory(src/Task)
add_subdirectory(src/PreProcess)
add_subdirectory(src/ModelInfer)

include_directories(src/Config)
include_directories(src/Task)
include_directories(src/PreProcess)
include_directories(src/ModelInfer)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Platform
        src/Platform.cpp)

target_link_libraries(Platform PRIVATE
        dl ${OpenCV_LIBS}
        Config PreProcess Task ModelInfer)

# 显示编译内容
message(STATUS "")
message(STATUS "============ Build Configuration ============")
message(STATUS "C++ Standard          : ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type            : ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Bin Directory : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Install Lib Directory : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "---------------------------------------------")

if(OpenCV_FOUND)
        message(STATUS "OpenCV Found          : on")
        message(STATUS "OpenCV Version        : ${OpenCV_VERSION}")
else()
        message(STATUS "OpenCV Found          : off")
endif()

message(STATUS "ONNX Runtime Enabled  : ${USE_ONNX}")
message(STATUS "ONNX Runtime Dir      : ${ONNXRUNTIME_DIR}")

if(NOT USE_ONNX)
        message(STATUS "---------------------------------------------")
        message(STATUS "WARNING : ModelInfer is lack  of ONNX Runtime")
endif()

message(STATUS "=============================================")
message(STATUS "")
