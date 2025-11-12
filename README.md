# Wrapping Python into C++

## 项目概述

本项目展示了如何在C++程序中嵌入Python解释器，并比较C++ Eigen库与Python NumPy库在矩阵运算上的性能差异。通过直接在C++代码中调用Python代码，实现了两种编程语言之间的无缝集成和性能对比。

## 主要功能

- **C++ Eigen矩阵运算**：使用Eigen库进行矩阵生成、加法和乘法运算，并进行精确计时
- **Python NumPy矩阵运算**：在C++中嵌入Python解释器，调用NumPy进行相同的矩阵运算
- **性能对比**：同时提供Python内部计时和C++外部计时，全面评估两种实现的性能差异
- **可配置矩阵大小**：支持通过命令行参数指定矩阵维度，便于测试不同规模下的性能表现

## 技术依赖

- **C++ 20**：使用现代C++特性
- **Eigen库**：C++矩阵运算库
- **Python 3.13.3**：嵌入版本的Python解释器
- **NumPy**：Python科学计算库
- **OpenMP**：用于并行计算优化
- **CMake**：构建系统

## 项目结构

```
Wrapping-Python-into-CPlusPlus/
├── compare.cpp       # 主程序文件，包含Eigen和Python调用实现
├── CMakeLists.txt    # CMake构建配置
├── README.md         # 项目说明文档
├── LICENSE           # 许可证文件
└── python-3.13.3-embed/  # 嵌入版本的Python解释器
```

## 构建说明

### 前提条件

1. 已安装CMake 3.16或更高版本
2. 已安装C++编译器（支持C++20标准）
3. 已安装Eigen库（项目中指向`D:/msys64/mingw64/include/eigen3`）
4. 已安装Python开发库（项目中指向`D:/python-envs/Python-3.13.3`）
5. 已安装NumPy（放置在嵌入Python的site-packages目录中）

### 构建步骤

1. 创建构建目录：
   ```bash
   mkdir build
   cd build
   ```

2. 运行CMake配置：
   ```bash
   cmake ..
   ```

3. 编译项目：
   ```bash
   cmake --build . --config Release
   ```

## 使用方法

编译完成后，可以通过以下方式运行程序：

```bash
compare.exe <matrix_size>
```

其中 `<matrix_size>` 是要测试的矩阵维度大小（如1000表示1000x1000的矩阵）。

### 示例

```bash
compare.exe 1000
```

程序将输出类似以下结果：

```
================= Matrix Operation Speed Comparison =================

Matrix size: 1000 x 1000

================= Eigen Matrix Operations =================
Eigen matrix generation time: 0.012345 seconds
Eigen matrix addition time: 0.001234 seconds
Eigen matrix multiplication time: 0.234567 seconds

================= NumPy Matrix Operations =================
Matrix generation time: 0.023456 seconds  [C++ external: 0.025678 seconds]
Matrix addition time: 0.002345 seconds  [C++ external: 0.003456 seconds]
Matrix multiplication time: 0.345678 seconds  [C++ external: 0.356789 seconds]

================= Matrix Operations Completed =================
```

## 性能对比说明

- **内部计时**：Python代码内部使用`time.time()`测量运算时间
- **外部计时**：C++代码使用`std::chrono`测量从调用Python开始到返回的总时间
- 外部计时包含了Python解释器开销和数据传递开销，通常会略高于内部计时

## 注意事项

1. 确保嵌入的Python解释器中已正确安装NumPy库
2. 程序需要正确设置Python路径，当前配置指向`./python-3.13.3-embed`目录
3. 对于大矩阵运算，确保系统有足够的内存资源
4. 性能测试结果会因硬件配置、编译器优化等因素而有所差异

## 许可证

本项目采用项目根目录中的LICENSE文件所规定的许可证。请参考LICENSE文件获取详细的许可证条款和条件。