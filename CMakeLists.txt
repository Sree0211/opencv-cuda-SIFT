cmake_minimum_required(VERSION 3.22)

project(
    opencv_cuda_sift
    VERSION 0.1.0
    LANGUAGES CXX CUDA
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SIFT_BUILD_TESTS "Build tests" ON)
option(SIFT_BUILD_BENCHMARKS "Build benchmarks" ON)

add_subdirectory(sift)