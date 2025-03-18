cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(mpi C)

# 找到 MPI
find_package(MPI REQUIRED)

# 设置 MPI 的 C 编译器（使用 mpicc）
set(CMAKE_C_COMPILER ${MPI_C_COMPILER})

# 添加源文件
add_executable(mpi main.c compute.c)

# 链接数学库 (math.h 需要链接 libm)
target_link_libraries(mpi m)