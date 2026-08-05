#include "../include/gaussian_cuda.hpp"

#include <cuda_runtime.h>
#include <stdexcept>
#include <string>

constexpr int dim = 16;
namespace gaussian_kernel{

void check_cuda(cudaError_t status, const char* message) {
    if (status != cudaSuccess) {
        throw std::runtime_error(std::string(message) + ": " + cudaGetErrorString(status));
    }
}

__device__ void gaussian_blur_vertical(){

}

__global__ void gaussian_blur_hor(const float* input, float* output, int width, int height, const float* kernel) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    const int radius = static_cast<int>(kernel.size()) / 2;
    __shared__ float blockA[dim + radius];

    if(row < height & col < width){

    }
    
}

}  // namespace gaussian_kernel

namespace sift::cuda {

std::vector<float> makeGaussianKernel(float sigma)
{
	const int radius = static_cast<int>(ceil((3.0f * sigma)));
	const int size = 2 * radius + 1;

	// Calculate Gaussians
	std::vector<float> kernel(size);
	float sum = 0.0f;

	for (int i = -radius; i < radius; i++) {
		const float x = static_cast<float>(i);
		const float value = exp(-1 * (x * x) / (2.0f * sigma * sigma));

		kernel[i + radius] = value;
		sum += value;
	}

	for (float& val : kernel) {
		val /= sum;
	}

	return kernel;
}

Image gaussian_blur(const Image& input, float sigma) {
    (void)sigma;

    Image result;
    result.width = input.width;
    result.height = input.height;

    // Perform kernel calculation
    float sigma = 1.6f;
    std::vector<float> kernel = makeGaussianKernel(sigma);

    float* device_input = nullptr;
    float* device_output = nullptr;
    float* device_gaussKernel = nullptr;

    check_cuda(cudaMalloc(&device_input, input.pixels.size() * sizeof(float)), "cudaMalloc input");
    check_cuda(cudaMalloc(&device_output, result.pixels.size() * sizeof(float)), "cudaMalloc output");
    check_cuda(cudaMalloc(&device_gaussKernel, kernel.size() * sizeof(float)), "cudaMalloc Gaussian Kernel");

    check_cuda(cudaMemcpy(device_input, input.pixels.data(), input.pixels.size() * sizeof(float), cudaMemcpyHostToDevice),
               "cudaMemcpy input to device");
    check_cuda(cudaMemcpy(device_gaussKernel, kernel.data(), kernel.size() * sizeof(float), cudaMemcpyHostToDevice),
               "cudaMemcpy input to device");

    dim3 threadsPerBlock(dim,dim);
    dim3 blocksPerGrid((input.height + dim - 1)/dim, (input.width + dim - 1)/dim);
    gaussian_blur_kernel<<<blocksPerGrid, threadsPerBlock>>>(device_input, device_output, input.width, input.height, device_gaussKernel);

    check_cuda(cudaGetLastError(), "launch copy kernel");
    check_cuda(cudaDeviceSynchronize(), "kernel synchronization");
    check_cuda(cudaMemcpy(result.pixels.data(), device_output, result.pixels.size() * sizeof(float), cudaMemcpyDeviceToHost),
               "cudaMemcpy output to host");

    cudaFree(device_input);
    cudaFree(device_output);
    cudaFree(device_gaussKernel);

    return result;
}

bool run_gaussian_smoke_test() {
    std::vector<float> input{1.0f, 2.0f, 3.0f, 4.0f};
    auto result = gaussian_blur(input, 2, 2, 1.0f);

    return result.width == 2 && result.height == 2 && result.data.size() == 4 &&
           result.data[0] == 1.0f && result.data[3] == 4.0f;
}

}  // namespace sift::cuda
