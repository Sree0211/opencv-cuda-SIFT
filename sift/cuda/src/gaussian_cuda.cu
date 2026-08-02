#include "../include/gaussian_cuda.hpp"

#include <cuda_runtime.h>
#include <stdexcept>
#include <string>

namespace gaussian_kernel{

void check_cuda(cudaError_t status, const char* message) {
    if (status != cudaSuccess) {
        throw std::runtime_error(std::string(message) + ": " + cudaGetErrorString(status));
    }
}

__global__ void copy_kernel(const float* input, float* output, int count) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < count) {
        output[idx] = input[idx];
    }
}

}  // namespace gaussian_kernel

namespace sift::cuda {

GaussianResult gaussian_blur(const std::vector<float>& input,
                             std::size_t width,
                             std::size_t height,
                             float sigma) {
    (void)sigma;

    if (input.size() != width * height) {
        throw std::invalid_argument("Input size does not match width and height");
    }

    GaussianResult result;
    result.width = width;
    result.height = height;
    result.data.resize(input.size());

    float* device_input = nullptr;
    float* device_output = nullptr;

    check_cuda(cudaMalloc(&device_input, input.size() * sizeof(float)), "cudaMalloc input");
    check_cuda(cudaMalloc(&device_output, result.data.size() * sizeof(float)), "cudaMalloc output");

    check_cuda(cudaMemcpy(device_input, input.data(), input.size() * sizeof(float), cudaMemcpyHostToDevice),
               "cudaMemcpy input to device");

    int thread_count = 256;
    int block_count = (static_cast<int>(input.size()) + thread_count - 1) / thread_count;
    copy_kernel<<<block_count, thread_count>>>(device_input, device_output, static_cast<int>(input.size()));

    check_cuda(cudaGetLastError(), "launch copy kernel");
    check_cuda(cudaDeviceSynchronize(), "kernel synchronization");
    check_cuda(cudaMemcpy(result.data.data(), device_output, result.data.size() * sizeof(float), cudaMemcpyDeviceToHost),
               "cudaMemcpy output to host");

    cudaFree(device_input);
    cudaFree(device_output);

    return result;
}

bool run_gaussian_smoke_test() {
    std::vector<float> input{1.0f, 2.0f, 3.0f, 4.0f};
    auto result = gaussian_blur(input, 2, 2, 1.0f);

    return result.width == 2 && result.height == 2 && result.data.size() == 4 &&
           result.data[0] == 1.0f && result.data[3] == 4.0f;
}

}  // namespace sift::cuda
