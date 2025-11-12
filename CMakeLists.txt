cmake_minimum_required(VERSION 3.16)
project(compare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 正确启用 OpenMP 的方式
find_package(OpenMP REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(compare compare.cpp)

target_include_directories(compare PRIVATE
    "D:/msys64/mingw64/include/eigen3"
    "D:/python-envs/Python-3.13.3/include"
)

# 链接库：注意 OpenMP 要在这里加！
target_link_libraries(compare
    "D:/python-envs/Python-3.13.3/libs/python313.lib"
    OpenMP::OpenMP_CXX   # 关键：自动处理 -fopenmp 编译和链接
)

# 其他编译选项
target_compile_options(compare PRIVATE -march=native -O3)
