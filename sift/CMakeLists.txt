cmake_minimum_required(VERSION 3.22)

find_package(Threads REQUIRED)
include(CTest)
option(SIFT_USE_OPENCV "Build with OpenCV support" OFF)

if(SIFT_USE_OPENCV)
    find_package(OpenCV REQUIRED)
endif()

set(SIFT_HEADERS
    include/image.hpp
    include/gaussian.hpp
    include/gaussian_pyramid.hpp
    include/DoG.hpp
    include/keypoint.hpp
    include/orientation.hpp
    include/descriptor.hpp
    include/extrema.hpp
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
)

if(SIFT_USE_OPENCV)
    target_link_libraries(sift PUBLIC ${OpenCV_LIBS})
    target_include_directories(sift
        PUBLIC
            ${OpenCV_INCLUDE_DIRS}
    )
endif()

# Executable
add_executable(sift_app 
    app/main.cpp)

target_link_libraries(sift_app PRIVATE sift)

# Tests
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Benchmarks
add_executable(perf_gaussian
    perf/perf_gaussian.cpp 
)
target_link_libraries(perf_gaussian PRIVATE sift)

add_executable(perf_gaussian_pyramid
    perf/perf_gaussian_pyramid.cpp 
)
target_link_libraries(perf_gaussian_pyramid PRIVATE sift)