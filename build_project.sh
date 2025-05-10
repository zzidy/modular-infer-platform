#!/bin/bash

# 定义颜色代码，用于美化输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # 无颜色

# 错误处理函数
handle_error() {
    echo -e "${RED}错误: 步骤 $1 失败!${NC}"
    exit 1
}

# 主项目编译
echo -e "${YELLOW}开始编译主项目...${NC}"
if [ ! -d "build" ]; then
    echo -e "${GREEN}创建build目录...${NC}"
    mkdir build || handle_error "创建主项目build目录"
fi

cd build || handle_error "进入主项目build目录"
echo -e "${GREEN}运行CMake...${NC}"
cmake .. || handle_error "运行主项目CMake"
echo -e "${GREEN}开始编译主项目 (使用8个线程)...${NC}"
make -j8 || handle_error "编译主项目"
cd .. || handle_error "返回主目录"
echo -e "${GREEN}主项目编译完成!${NC}"

# My_Task子项目编译
echo -e "${YELLOW}开始编译My_Task子项目...${NC}"
cd src/My_Task || handle_error "进入My_Task目录"
if [ ! -d "build" ]; then
    echo -e "${GREEN}创建My_Task build目录...${NC}"
    mkdir build || handle_error "创建My_Task build目录"
fi

cd build || handle_error "进入My_Task build目录"
echo -e "${GREEN}运行CMake...${NC}"
cmake .. || handle_error "运行My_Task CMake"
echo -e "${GREEN}开始编译My_Task (使用8个线程)...${NC}"
make -j8 || handle_error "编译My_Task"
echo -e "${GREEN}安装My_Task...${NC}"
make install || handle_error "安装My_Task"
cd ../../../ || handle_error "返回项目根目录"
echo -e "${GREEN}My_Task子项目编译并安装完成!${NC}"

echo -e "${GREEN}=========================${NC}"
echo -e "${GREEN}项目编译全部完成!${NC}"
echo -e "${GREEN}=========================${NC}"    