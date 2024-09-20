cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(quickjs-demo)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)

# 指定 QuickJS 的头文件路径
include_directories(${CMAKE_SOURCE_DIR}/quickjs)

# 指定 QuickJS 库的路径
set(QUICKJS_LIB ${CMAKE_SOURCE_DIR}/quickjs/libquickjs.a)

# 添加可执行文件
add_executable(quickjs-demo main.c)

# 将 QuickJS 库链接到项目中
target_link_libraries(quickjs-demo ${QUICKJS_LIB})

# 如果你使用的是动态库 (.so 文件)，使用以下替代链接：
# target_link_libraries(my_project ${CMAKE_SOURCE_DIR}/quickjs/libquickjs.so)
