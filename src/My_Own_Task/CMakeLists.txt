cmake_minimum_required(VERSION 3.10)
project(Platform)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(TaskTest
                    ../Task)

add_library(Tasklib SHARED TaskTest/TaskTest.cpp)
target_link_libraries(Tasklib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/TaskTest)