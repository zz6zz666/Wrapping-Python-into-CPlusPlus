#include <Python.h>
#include <iostream>
#include <chrono>
#include <Eigen/Dense>
#include <string>
#include <Windows.h>


// Eigen matrix operations with step-by-step timing
void eigen_matrix_operations(int size) {
    std::cout << "\n================= Eigen Matrix Operations =================" << std::endl;
    
    // Matrix generation timing
    auto gen_start = std::chrono::high_resolution_clock::now();
    Eigen::MatrixXd matrix_a = Eigen::MatrixXd::Random(size, size);
    Eigen::MatrixXd matrix_b = Eigen::MatrixXd::Random(size, size);
    auto gen_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gen_duration = gen_end - gen_start;
    
    // Matrix addition timing
    auto add_start = std::chrono::high_resolution_clock::now();
    Eigen::MatrixXd add_result = matrix_a + matrix_b;
    auto add_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> add_duration = add_end - add_start;
    
    // Matrix multiplication timing
    auto mul_start = std::chrono::high_resolution_clock::now();
    Eigen::MatrixXd mul_result = matrix_a * matrix_b;
    auto mul_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mul_duration = mul_end - mul_start;
    
    // Output results
    std::cout << "Eigen matrix generation time: " << gen_duration.count() << " seconds" << std::endl;
    std::cout << "Eigen matrix addition time: " << add_duration.count() << " seconds" << std::endl;
    std::cout << "Eigen matrix multiplication time: " << mul_duration.count() << " seconds" << std::endl << std::endl;
    
}


// Execute Python code block and return success status
bool run_python_code(const char* code) {
    int result = PyRun_SimpleString(code);
    if (result != 0) {
        std::cerr << "Python code execution failed!" << std::endl;
        PyErr_Print();
        return false;
    }
    return true;
}

int numpy_matrix_operations(int size) {
    std::cout << "\n================= NumPy Matrix Operations =================" << std::endl;
    
    // 1. Import necessary modules (one-time operation)
    const char* import_code = R"(
import numpy as np
import time

# Declare global variables for data sharing across code blocks
global matrix_a, matrix_b, add_result, mul_result
)";
    if (!run_python_code(import_code)) {
        Py_Finalize();
        return 1;
    }
    
    // 2. Matrix generation stage
    // 2.1 Python internal timing - matrix generation
    std::string py_gen_code = std::format(R"(
gen_start_py = time.time()
matrix_a = np.random.rand({}, {})
matrix_b = np.random.rand({}, {})
gen_end_py = time.time()
gen_time_py = gen_end_py - gen_start_py
print(f"Matrix generation time: {{gen_time_py:.6f}} seconds", end='  ', flush=True)
)", size, size, size, size);
    
    // 2.2 C++ external timing - matrix generation
    auto gen_start_cpp = std::chrono::high_resolution_clock::now();
    if (!run_python_code(py_gen_code.c_str())) {
        Py_Finalize();
        return 1;
    }
    auto gen_end_cpp = std::chrono::high_resolution_clock::now();
    double gen_time_cpp = std::chrono::duration<double>(gen_end_cpp - gen_start_cpp).count();
    std::cout << "[C++ external: " << gen_time_cpp << " seconds]" << std::endl;
    
    // 3. Matrix addition stage
    // 3.1 Python internal timing - matrix addition
    const char* py_add_code = R"(
add_start_py = time.time()
add_result = matrix_a + matrix_b
add_end_py = time.time()
add_time_py = add_end_py - add_start_py
print(f"Matrix addition time: {add_time_py:.6f} seconds", end='  ', flush=True)
)";
    
    // 3.2 C++ external timing - matrix addition
    auto add_start_cpp = std::chrono::high_resolution_clock::now();
    if (!run_python_code(py_add_code)) {
        Py_Finalize();
        return 1;
    }
    auto add_end_cpp = std::chrono::high_resolution_clock::now();
    double add_time_cpp = std::chrono::duration<double>(add_end_cpp - add_start_cpp).count();
    std::cout << "[C++ external: " << add_time_cpp << " seconds]" << std::endl;
    
    // 4. Matrix multiplication stage
    // 4.1 Python internal timing - matrix multiplication
    const char* py_mul_code = R"(
mul_start_py = time.time()
mul_result = np.dot(matrix_a, matrix_b)
mul_end_py = time.time()
mul_time_py = mul_end_py - mul_start_py
print(f"Matrix multiplication time: {mul_time_py:.6f} seconds", end='  ', flush=True)
)";
    
    // 4.2 C++ external timing - matrix multiplication
    auto mul_start_cpp = std::chrono::high_resolution_clock::now();
    if (!run_python_code(py_mul_code)) {
        Py_Finalize();
        return 1;
    }
    auto mul_end_cpp = std::chrono::high_resolution_clock::now();
    double mul_time_cpp = std::chrono::duration<double>(mul_end_cpp - mul_start_cpp).count();
    std::cout << "[C++ external: " << mul_time_cpp << " seconds]" << std::endl << std::endl;

    return 0;
}



int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <matrix_size>" << std::endl;
        return 1;
    }

    int matrix_size = std::stoi(argv[1]);

    if (matrix_size <= 0) {
        std::cerr << "Error: Matrix size must be a positive integer!" << std::endl;
        return 1;
    }


    std::cout << "\n\n================= Matrix Operation Speed Comparison =================\n" << std::endl;
    std::cout << "Matrix size: " << matrix_size << " x " << matrix_size << "\n" << std::endl;

    // Execute Eigen matrix operations
    eigen_matrix_operations(matrix_size);
    
    // Add embedded Python directory to DLL search path
    // AddDllDirectory(L".\\python-3.13.3-embed");

    _putenv_s("PYTHONHOME", ".\\python-3.13.3-embed");

    Py_Initialize();
    // Set Python path
    const char* set_path_code = R"(
import sys
sys.path.append('./python-3.13.3-embed/Lib/site-packages')
sys.path.append('./python-3.13.3-embed/Lib')
sys.path.append('./python-3.13.3-embed')
)";
    if (!run_python_code(set_path_code)) {
        Py_Finalize();
        return 1;
    }

    // Execute NumPy matrix operations and check for errors
    if (numpy_matrix_operations(matrix_size) != 0) {
        // 函数已经调用了Py_Finalize()
        return 1;
    }
    std::cout << "\n================= Matrix Operations Completed =================\n\n" << std::endl;

    // Clean up Python environment
    Py_Finalize();
    return 0;
}