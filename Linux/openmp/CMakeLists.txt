cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(openmp C)

# 查找 OpenMP
find_package(OpenMP REQUIRED)

# 指定gcc编译器
set(CMAKE_C_COMPILER "/usr/bin/gcc")

# 添加源文件
add_executable(openmp main.c compute.c)

# 添加 OpenMP 选项
if(OpenMP_C_FOUND)
    target_compile_options(openmp PRIVATE ${OpenMP_C_FLAGS})
    target_link_libraries(openmp PRIVATE ${OpenMP_C_LIBRARIES} m)
else()
    message(FATAL_ERROR "openmp not found!")
endif()