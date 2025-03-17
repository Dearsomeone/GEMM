cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(serial C)

# 指定 MinGW-w64 的 gcc 编译器
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")

# 编译选项
add_compile_options(-O0 -Wall -g)

# 添加源文件
add_executable(serial main.c compute.c)

# Windows 下不需要手动链接 libm，math.h 默认包含在 MinGW-w64 里
if(NOT WIN32)
    target_link_libraries(serial m)
endif()
