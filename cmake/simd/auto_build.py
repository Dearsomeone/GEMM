import os
import shutil
import subprocess

build_path = "build/"
# 若存在则删除
if os.path.exists(build_path):
    shutil.rmtree(build_path)
    print(f"文件夹 '{build_path}' 已删除。")

# 创建目录
os.makedirs(build_path)
print(f"文件夹 '{build_path}' 已创建。")

# 切换到 build 目录
os.chdir(build_path)

# 运行 cmake 命令
try:
    # 假设 CMakeLists.txt 在上级目录
    subprocess.run(["cmake", ".."], check=True)
    print("CMake 配置成功！")
except subprocess.CalledProcessError as e:
    print(f"CMake 配置失败: {e}")
    exit(1)

# 运行 make 命令
try:
    subprocess.run(["make"], check=True)
    print("编译成功！")
except subprocess.CalledProcessError as e:
    print(f"编译失败: {e}")
    exit(1)

# 移动可执行文件
if os.path.exists("../simd"):
    os.remove("../simd")
shutil.move("simd", "../")
# 返回上级目录
os.chdir("..")

