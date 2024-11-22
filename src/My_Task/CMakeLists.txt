cmake_minimum_required(VERSION 3.10)
project(Platform)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib) # 业务动态库存放地址，复制去替换原有业务库

include_directories(TaskTest
                    ../Task)

add_library(Task SHARED TaskTest/TaskTest.cpp) # 每次编译新业务时修改此地址
target_link_libraries(Task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/TaskTest) # 每次编译新业务时修改此地址

set(INSTALL_LIB_DIR ${CMAKE_SOURCE_DIR}/../../install/lib) # 指定安装路径
install(TARGETS Task
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}) # 将动态库安装到指定路径