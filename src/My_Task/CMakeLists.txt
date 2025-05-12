cmake_minimum_required(VERSION 3.10)
project(My_Task)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(TASK_NAME PostProcessTask) # 每次编译新业务时修改成文件夹名

find_package(OpenCV REQUIRED)

include_directories(${TASK_NAME}
                    ${OpenCV_INCLUDE_DIRS}
                    ../Task
                    ../Config
                    ../PreProcess
                    ../ModelInfer
                    ../PostProcess)

add_library(Task SHARED ${TASK_NAME}/${TASK_NAME}.cpp) 

target_link_libraries(Task PRIVATE 
                        ${OpenCV_LIBS} 
                        )          

set(INSTALL_LIB_DIR ${CMAKE_SOURCE_DIR}/../../install/lib) # 指定安装路径
install(TARGETS Task
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}) # 将动态库安装到指定路径

# 如果存在config文件夹，则将config文件夹安装到指定路径
if(EXISTS ${CMAKE_SOURCE_DIR}/${TASK_NAME}/config)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/${TASK_NAME}/config
            DESTINATION ${INSTALL_LIB_DIR}/..)
endif()

# 如果存在data文件夹，则将data文件夹安装到指定路径
if(EXISTS ${CMAKE_SOURCE_DIR}/${TASK_NAME}/data)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/${TASK_NAME}/data
            DESTINATION ${INSTALL_LIB_DIR}/..)
endif()

# 如果存在model文件夹，则将model文件夹安装到指定路径
if(EXISTS ${CMAKE_SOURCE_DIR}/${TASK_NAME}/model)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/${TASK_NAME}/model
            DESTINATION ${INSTALL_LIB_DIR}/..)
endif()