cmake_minimum_required(VERSION 3.22)

project(
    opencv_cuda_sift
    VERSION 0.1.0
    LANGUAGES CXX )

option(SIFT_BUILD_TESTS "Build tests" ON)
option(SIFT_BUILD_BENCHMARKS "Build benchmarks" ON)
option(SIFT_CUDA_ENV "CUDA development" Off)

include(CheckLanguage)

check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(SIFT_CUDA_ENV ON)
else()
    set(SIFT_CUDA_ENV OFF)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_subdirectory(sift)