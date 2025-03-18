import os
import shutil

build_path = "build/"
exe_name = "serial_opt.exe"  # Windows 可执行文件名

# 若存在 build 目录，则删除
if os.path.exists(build_path):
    shutil.rmtree(build_path)
    print(f"文件夹 '{build_path}' 已删除。")

# 创建 build 目录
os.makedirs(build_path)
print(f"文件夹 '{build_path}' 已创建。")

# 切换到 build 目录
os.chdir(build_path)

# 运行 CMake
try:
    cmake_command = "cmake -G \"MinGW Makefiles\" .."
    if os.system(cmake_command) != 0:
        raise RuntimeError("CMake 配置失败！")
    print("CMake 配置成功！")
except RuntimeError as e:
    print(e)
    exit(1)

# 运行 MinGW-w64 的 mingw32-make
try:
    make_command = "mingw32-make"
    if os.system(make_command) != 0:
        raise RuntimeError("编译失败！")
    print("编译成功！")
except RuntimeError as e:
    print(e)
    exit(1)

# 返回上级目录
os.chdir("..")

# 移动可执行文件
exe_path = os.path.join(build_path, exe_name)
if os.path.exists(exe_name):
    os.remove(exe_name)
shutil.move(exe_path, exe_name)
print(f"可执行文件 '{exe_name}' 已移动至项目根目录。")
