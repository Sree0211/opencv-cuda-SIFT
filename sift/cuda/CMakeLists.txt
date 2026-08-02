add_library(sift_cuda STATIC
    src/gaussian_cuda.cu
)

target_include_directories(sift_cuda PUBLIC
    include
)

target_compile_features(sift_cuda PUBLIC cxx_std_17)

add_executable(sift_cuda_smoke_test
    tests/gaussian_cuda_smoke.cpp
)

target_link_libraries(sift_cuda_smoke_test PRIVATE sift_cuda)
