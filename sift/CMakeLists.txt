cmake_minimum_required(VERSION 3.22)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

set(SIFT_HEADERS
    include/image.hpp
    include/gaussian.hpp
    include/gaussian_pyramid.hpp
    include/DoG.hpp
    include/keypoint.hpp
    include/orientation.hpp
    include/descriptor.hpp
)

set(SIFT_SOURCES
    src/gaussian.cpp
    src/gaussian_pyramid.cpp
    src/DoG.cpp
    src/extrema.cpp
    src/orientation.cpp
    src/descriptor.cpp
)

add_library(sift STATIC
    ${SIFT_HEADERS}
    ${SIFT_SOURCES}
)

if(SIFT_CUDA_ENV)
    add_subdirectory(cuda)
endif()

target_include_directories(sift PUBLIC
    include
)

target_link_libraries(sift PUBLIC
    Threads::Threads
    ${OpenCV_LIBS}
)

# Executable
add_executable(sift_app 
    app/main.cpp)

target_link_libraries(sift_app PRIVATE sift)

# Tests
add_executable(test_gaussian 
    tests/test_gaussian.cpp)
target_link_libraries(test_gaussian PRIVATE sift)

# Benchmarks
add_executable(perf_gaussian
    perf/perf_gaussian.cpp
)
target_link_libraries(perf_gaussian PRIVATE sift)