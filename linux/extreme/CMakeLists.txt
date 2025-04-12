cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(extreme C)

# 找到 MPI
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

# 设置 MPI 的 C 编译器（使用 mpicc）
set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
add_compile_options(-O3 -march=native -ffast-math -fopt-info-vec -Wall)

# 添加源文件
add_executable(extreme main.c compute.c)

# 添加 OpenMP 选项
if(OpenMP_C_FOUND)
    target_compile_options(extreme PRIVATE ${OpenMP_C_FLAGS})
    target_link_libraries(extreme PRIVATE ${OpenMP_C_LIBRARIES} m)
else()
    message(FATAL_ERROR "openmp not found!")
endif()
