cmake_minimum_required(VERSION 3.12)
project(parseint)

set(CMAKE_CXX_STANDARD 20)

set(pybind11_DIR "C:/Vcpkg/installed/x64-windows/share/pybind11")

find_package(pybind11 REQUIRED)

pybind11_add_module(parseint "./src/parseint_py.cpp")

target_link_libraries(parseint PRIVATE pybind11::module)
