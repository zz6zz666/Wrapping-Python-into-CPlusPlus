# Wrapping Python into C++ 🐍➕🐀

## 📋 项目概述

本项目提供了一个**在C++中嵌入Python代码并支持独立打包分发**的完整模板。它详细展示了如何将Python解释器与C++程序无缝集成，并确保整个应用程序可以作为一个独立包分发，无需终端用户单独安装Python环境。

> **重要说明**：项目中包含的Eigen与NumPy矩阵运算性能对比，作为展示C++与Python交互的示例应用。

## ⚙️ 嵌入式Python环境配置详解

### 📥 1. 下载嵌入式Python包

1. 访问 [Python官方下载页面](https://www.python.org/downloads/windows/)
2. 下载适合您系统的 **Windows embeddable package (64-bit)** 版本
3. 解压下载的`python-3.XX.X-embed-amd64.zip`文件到项目根目录下

### 🔑 2. 配置Python标准库（关键步骤）

**官方嵌入式包的问题**：
- 官方提供的嵌入式包中，标准库被打包在`python3XX.zip`文件中
- 这可能导致在某些情况下出现"找不到unzip模块"的错误

**解决方案**：
1. 不要直接使用zip文件形式的库
2. 手动解压`python3XX.zip`中的所有内容到`Lib`文件夹中
3. 确保`python-3.XX.X-embed/Lib`目录包含所有必要的Python模块

### 🛤️ 3. 配置Python路径文件

编辑`python-3.XX.X-embed/python3XX._pth`文件，确保其内容如下：

```
Lib
Lib/site-packages
.

# 取消下面这行的注释以启用第三方包支持
import site
```

> **注意**：必须取消`import site`行的注释，否则将无法使用第三方包。

### 📦 4. 安装pip（用于管理第三方库）

1. 确保`get-pip.py`脚本存在于嵌入式Python目录中（如不存在，可从[Python官网](https://bootstrap.pypa.io/get-pip.py)下载）
2. 在命令行中执行以下命令安装pip：
   ```bash
   python-3.13.3-embed/python.exe get-pip.py
   ```
3. 安装完成后，将自动在嵌入式Python目录下创建`Scripts`文件夹

### 🔗 5. 安装必要的第三方库

使用刚安装的pip安装项目所需的库，例如NumPy：

```bash
python-3.13.3-embed/Scripts/pip.exe install numpy
```

## 📂 项目结构

```
Wrapping-Python-into-CPlusPlus/
├── compare.cpp       # 主程序文件，展示C++嵌入Python的完整实现
├── CMakeLists.txt    # CMake构建配置，包含必要的链接和依赖设置
├── README.md         # 项目说明文档
├── LICENSE           # 许可证文件
└── python-3.13.3-embed/  # 配置好的嵌入式Python解释器
    ├── Lib/          # 解压后的Python标准库
    │   └── site-packages/ # 第三方库（如NumPy）
    ├── Scripts/      # pip等脚本工具（安装pip后生成）
    ├── get-pip.py    # pip安装脚本
    ├── python313.dll # Python解释器核心DLL
    ├── python313._pth # Python路径配置文件（已配置）
    └── python.exe    # Python解释器可执行文件
```

## 🛠️ CMake配置说明

### 🔨 编译环境设置

要成功编译项目，您需要：

1. **安装完整的Python开发环境**（与嵌入式版本匹配）
2. 正确配置`CMakeLists.txt`中的包含目录和库文件路径

### 📝 关键配置部分

```cmake
# 设置包含目录
target_include_directories(compare PRIVATE
    # Python开发库的包含目录（改为你的python路径，确保版本与嵌入式版本匹配）
    "D:/python-envs/Python-3.XX.X/include"
    ```
)

# 链接必要的库
target_link_libraries(compare
    # Python库文件
    "D:/python-envs/Python-3.XX.X/libs/python3XX.lib"
    ```
)

> **重要提示**：
> - 确保使用的Python开发库版本与嵌入式Python版本完全匹配
> - 根据您的实际安装路径修改上述配置中的路径
> - 如果您不使用Eigen库，可以移除相应的包含目录

## 🔌 C++中嵌入Python的核心实现

### 🚀 1. Python解释器初始化

```cpp
// 设置Python主页环境变量，指向嵌入式Python目录
_putenv_s("PYTHONHOME", ".\\python-3.XX.X-embed");

// 初始化Python解释器
Py_Initialize();

// 配置Python路径，确保能找到嵌入式Python的库
const char* set_path_code = R"(
import sys
sys.path.append('./python-3.XX.X-embed/Lib/site-packages')
sys.path.append('./python-3.XX.X-embed/Lib')
sys.path.append('./python-3.XX.X-embed')
)";
```

### ▶️ 2. Python代码执行

```cpp
// 执行Python代码块的函数
bool run_python_code(const char* code) {
    int result = PyRun_SimpleString(code);
    if (result != 0) {
        std::cerr << "Python code execution failed!" << std::endl;
        PyErr_Print();
        return false;
    }
    return true;
}
```

### 🧹 3. 资源清理

```cpp
// 程序结束时清理Python环境
Py_Finalize();
```

## 🏗️ 构建步骤

1. 创建构建目录：
   ```bash
   mkdir build
   cd build
   ```

2. 运行CMake配置（根据需要调整路径）：
   ```bash
   cmake ..
   ```

3. 编译项目：
   ```bash
   cmake --build . --config Release
   ```

## ⏱️ 示例程序用法

编译完成后，您可以运行示例程序来比较NumPy与Eigen库在矩阵运算上的性能差异。

### 运行命令

```bash
./compare <matrix_size>
```

其中 `<matrix_size>` 是要测试的矩阵大小（行列数相同的方阵）。

### 功能说明

示例程序 `compare.cpp` 实现了以下功能：

1. **矩阵生成性能测试**：比较Eigen和NumPy生成随机矩阵的速度
2. **矩阵加法性能测试**：比较Eigen和NumPy执行矩阵加法的速度
3. **矩阵乘法性能测试**：比较Eigen和NumPy执行矩阵乘法的速度

### 测量方法

- 对于Eigen，直接在C++代码中使用`std::chrono`库测量执行时间
- 对于NumPy，同时提供了两种测量结果：
  - Python内部使用`time.time()`测量的纯计算时间
  - C++外部使用`std::chrono`测量的总执行时间（包括Python解释器开销）

## 📊 测试运行结果展示

以下是在10000x10000矩阵大小下的性能测试结果：

```
================= Matrix Operation Speed Comparison =================

Matrix size: 10000 x 10000

================= Eigen Matrix Operations =================
Eigen matrix generation time: 8.64674 seconds
Eigen matrix addition time: 0.140348 seconds
Eigen matrix multiplication time: 4.48394 seconds

================= NumPy Matrix Operations =================
Matrix generation time: 0.949181 seconds  [C++ external: 0.949402 seconds]
Matrix addition time: 0.147931 seconds  [C++ external: 0.148281 seconds]
Matrix multiplication time: 4.992873 seconds  [C++ external: 4.99308 seconds]

================= Matrix Operations Completed =================
```

### 结果分析

1. **矩阵生成**：NumPy在矩阵生成方面明显优于Eigen（~0.95秒 vs ~8.65秒）
2. **矩阵加法**：两者性能相近，Eigen略快（~0.14秒 vs ~0.148秒）
3. **矩阵乘法**：Eigen在大规模矩阵乘法上略占优势（~4.48秒 vs ~4.99秒）

### 性能特点

- **NumPy优势**：在矩阵生成等初始化操作上表现出色
- **Eigen优势**：在复杂计算操作上展现出更好的性能优化
- **Python解释器开销**：从结果可以看出，Python内部测量时间与C++外部测量时间非常接近，说明Python解释器在NumPy计算上的开销较小，这主要归功于NumPy的底层C实现

## 📦 打包分发指南

### ✅ 分发前准备

1. **确认嵌入式Python环境完整**：确保`python-3.XX.X-embed`目录包含所有必要文件
2. **验证路径配置**：检查`python3XX._pth`文件配置正确
3. **确认第三方库已安装**：确保所有必需的第三方库（如NumPy）已正确安装

### 📋 分发清单

分发时应包含以下文件和目录：

- 编译后的C++可执行文件（如`compare.exe`）
- `python-3.XX.X-embed/` 目录（完整，包含所有文件和子目录）
- 其他必要的运行时文件（如将`python3XX.dll`放置在与可执行文件相同目录）

### ⚠️ 注意事项

- 确保保持相对路径结构不变
- 分发的可执行文件会在当前工作目录查找`python-3.XX.X-embed`目录
- 无需终端用户安装Python环境，所有依赖都包含在分发包中

## ❓ 常见问题解决

### 📁 1. "找不到unzip模块"错误

**解决方案**：确保已将`python3XX.zip`的内容解压到`Lib`文件夹中，而不是直接使用zip文件。

### 📚 2. 无法导入第三方库

**解决方案**：
- 确认`python3XX._pth`文件中已取消`import site`行的注释
- 确认第三方库已正确安装在`site-packages`目录中
- 检查Python路径设置是否正确

### 🔧 3. 编译错误（找不到Python.h或python3XX.lib）

**解决方案**：
- 确保已安装完整的Python开发环境
- 在`CMakeLists.txt`中正确配置Python的包含目录和库文件路径
- 确保使用的Python版本与嵌入式版本完全匹配

## 🔄 扩展到自己的项目

要将此模板用于您自己的项目：

1. **修改业务逻辑**：编辑`compare.cpp`，保留Python嵌入的核心部分，替换示例代码
2. **调整CMake配置**：根据您的项目需求修改`CMakeLists.txt`
3. **配置Python依赖**：使用嵌入式Python的pip安装您项目所需的库
4. **更新路径设置**：根据需要调整代码中的Python路径设置
5. **构建和测试**：确保项目能正确构建并运行

## 📜 许可证

本项目采用项目根目录中的LICENSE文件所规定的许可证。请参考LICENSE文件获取详细的许可证条款和条件。