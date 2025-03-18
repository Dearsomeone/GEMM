cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(openmp C)

# 查找 OpenMP
find_package(OpenMP REQUIRED)

# 指定 MinGW-w64 的 gcc 编译器
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")

# 编译选项
add_compile_options(-O0 -Wall -g)

# 添加源文件
add_executable(openmp main.c compute.c)

# 添加 OpenMP 选项
if(OpenMP_C_FOUND)
    # 添加 OpenMP 编译选项
    target_compile_options(openmp PRIVATE ${OpenMP_C_FLAGS})
    
    # 显式链接 libgomp
    target_link_libraries(openmp PRIVATE ${OpenMP_C_LIBRARIES} gomp)
else()
    message(FATAL_ERROR "OpenMP not found!")
endif()
