cmake_minimum_required(VERSION 2.8)
project(main)

set(CMAKE_CXX_COMPILER      "g++" )         # 显示指定使用的C++编译器

set(CMAKE_CXX_FLAGS "-std=c++11")   # c++11
set(CMAKE_CXX_FLAGS "-g")           # 调试信息
set(CMAKE_CXX_FLAGS "-Wall")        # 开启所有警告
# 指定目标文件及库文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 指定要包含的头文件所在目录
include_directories(${CMAKE_CURRENT_LIST_DIR}/header)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指名项目包含一个子目录 Project，这样 Project 目录下的 CMakeLists.txt文件和源代码也会被处理
add_subdirectory(source)

# add_executable(${PROJECT_NAME} ${DIR_SRCS})
add_executable(main main.cpp)
# 添加链接库
# target_link_libraries(${PROJECT_NAME} appRatp)
target_link_libraries(main appRatp)
