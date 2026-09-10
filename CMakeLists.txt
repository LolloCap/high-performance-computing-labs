cmake_minimum_required(VERSION 3.20)

project(high_performance_computing_labs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HPC_BUILD_OPENMP "Build the OpenMP benchmark" ON)
option(HPC_BUILD_MPI "Build the MPI benchmark" ON)

if(HPC_BUILD_OPENMP)
    find_package(OpenMP REQUIRED COMPONENTS CXX)
    add_executable(benchmark_openmp openmp/src/benchmark_openmp.cpp)
    target_link_libraries(benchmark_openmp PRIVATE OpenMP::OpenMP_CXX)
endif()

if(HPC_BUILD_MPI)
    find_package(MPI REQUIRED COMPONENTS CXX)
    add_executable(benchmark_mpi mpi/src/benchmark_mpi.cpp)
    target_link_libraries(benchmark_mpi PRIVATE MPI::MPI_CXX)
endif()
