cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(serial C)

# 指定gcc编译器
set(CMAKE_C_COMPILER "/usr/bin/gcc")
add_compile_options(-O0 -fopt-info-vec -Wall)

# 添加源文件
add_executable(serial main.c compute.c)

# 链接数学库 (math.h 需要链接 libm)
target_link_libraries(serial m)