cmake_minimum_required(VERSION 3.10)
project(Platform)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install/bin)

# opencv库
find_package(OpenCV REQUIRED)

# 设置onnxruntime库地址
set(ONNXRUNTIME_DIR /usr/local)
set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_DIR}/include)
set(ONNXRUNTIME_LIBRARIES ${ONNXRUNTIME_DIR}/lib/libonnxruntime.so)

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