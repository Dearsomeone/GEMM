#!/bin/bash

# 执行编译脚本
python auto_build.py
echo "Program build completed!"

if [ -f "timing.txt" ]; then
    echo "timing.txt exists. Deleting..."
    rm "timing.txt"
else
    echo "timing.txt does not exist."
fi

# 设定重复执行的次数
N=5

# 执行命令 N 次
for i in $(seq 1 $N); do
    echo "Run #$i - $(date)"
    
    # 运行 MPI 命令
    mpirun -n 28 ./mpi 3000
done

echo "All runs completed!"

