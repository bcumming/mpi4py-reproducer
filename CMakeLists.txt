cmake_minimum_required(VERSION 3.6)
project(example)
enable_language(CXX)

# required to use in pthon .so files
set(CMAKE_CXX_FLAGS "-fPIC")

find_package(MPI REQUIRED CXX)

# Set up pybind.
# Add support for pybind by adding the directory of pybind11, instead of using
# find_package().
set(PYBIND11_CPP_STANDARD -std=c++11)
add_subdirectory(pybind11)

pybind11_add_module(mpitest mpitest.cpp)
target_link_libraries(mpitest LINK_PUBLIC MPI::MPI_CXX)

