#include "../include/gaussian_cuda.hpp"

#include <cuda_runtime.h>
#include <stdexcept>
#include <string>

constexpr int dim = 16;
namespace {

void check_cuda(cudaError_t status, const char* message) {
    if (status != cudaSuccess) {
        throw std::runtime_error(std::string(message) + ": " + cudaGetErrorString(status));
    }
}

__global__ void gaussian_blur_vertical(const float* input, float* output, int width, int height, const float* kernel, size_t kernelSize){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    const int r = kernelSize / 2;
    float sum = 0.0f;
    extern __shared__ float blockA[];

    int global_index = 0;
    for(int i = threadIdx.y; i< blockDim.y + kernelSize - 1; i += blockDim.y){
        int tile_start = blockIdx.y * blockDim.y - r;
        global_index = tile_start + i;
        if(global_index >= 0 && global_index < height && col < width){
            blockA[threadIdx.x * (blockDim.y + kernelSize - 1) + i] = input[global_index*width + col];
        }else{
            blockA[threadIdx.x * (blockDim.y + kernelSize - 1) + i] = 0.0f;
        }
    }
    __syncthreads();

    for(int j = 0; j < kernelSize; j++){
        sum += blockA[threadIdx.x * (blockDim.y + kernelSize - 1) + threadIdx.y+j]*kernel[j];
    }
    if(row < height && col < width){
        output[row * width + col] = sum;
    }

}

__global__ void gaussian_blur_horizontal(const float* input, float* output, int width, int height, const float* kernel, size_t kernelSize) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    const int r = kernelSize / 2;
    float sum = 0.0f;
    extern __shared__ float blockA[];

    int global_index = 0;
    
    for(int i = threadIdx.x; i< blockDim.x + kernelSize - 1; i += blockDim.x){
        int tile_start = blockIdx.x * blockDim.x - r;
        global_index = tile_start + i;
        if(global_index >= 0 && global_index < width && row < height){
            blockA[threadIdx.y * (blockDim.x + kernelSize - 1)+ i] = input[row*width + global_index];
        }else{
            blockA[threadIdx.y * (blockDim.x + kernelSize - 1) + i] = 0.0f;
        }
    }
    __syncthreads();
    
    for(size_t j = 0; j < kernelSize; j++){
        sum += blockA[threadIdx.y * (blockDim.x + kernelSize - 1) + (threadIdx.x+j)]*kernel[j];
    }
    if(row < height && col < width){
        output[row * width + col] = sum;
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

	for (int i = -radius; i <=radius; i++) {
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
    std::vector<float> kernel = makeGaussianKernel(sigma);
    size_t kernelSize = static_cast<size_t>(kernel.size());

    float* device_input = nullptr;
    float* device_output = nullptr;
    float* device_temp = nullptr;
    float* device_gaussKernel = nullptr;

    check_cuda(cudaMalloc(&device_input, input.pixels.size() * sizeof(float)), "cudaMalloc input");
    check_cuda(cudaMalloc(&device_temp, result.pixels.size() * sizeof(float)), "cudaMalloc output");
    check_cuda(cudaMalloc(&device_output, result.pixels.size() * sizeof(float)), "cudaMalloc output");
    check_cuda(cudaMalloc(&device_gaussKernel, kernel.size() * sizeof(float)), "cudaMalloc Gaussian Kernel");

    check_cuda(cudaMemcpy(device_input, input.pixels.data(), input.pixels.size() * sizeof(float), cudaMemcpyHostToDevice),
               "cudaMemcpy input to device");
    check_cuda(cudaMemcpy(device_gaussKernel, kernel.data(), kernel.size() * sizeof(float), cudaMemcpyHostToDevice),
               "cudaMemcpy input to device");

    dim3 threadsPerBlock(dim,dim);
    dim3 blocksPerGrid((input.height + dim - 1)/dim, (input.width + dim - 1)/dim);
    gaussian_blur_horizontal<<<blocksPerGrid, threadsPerBlock, (threadsPerBlock.y)*(threadsPerBlock.x + kernelSize - 1)*sizeof(float)>>>(device_input, device_temp, input.width, input.height, device_gaussKernel, kernelSize);

    check_cuda(cudaGetLastError(), "launch copy kernel");
    check_cuda(cudaDeviceSynchronize(), "kernel synchronization");

    gaussian_blur_vertical<<<blocksPerGrid, threadsPerBlock, (threadsPerBlock.x)*(threadsPerBlock.y + kernelSize - 1)*sizeof(float)>>>(device_temp, device_output, input.width, input.height, device_gaussKernel, kernelSize);
    check_cuda(cudaGetLastError(), "launch copy kernel");
    check_cuda(cudaDeviceSynchronize(), "kernel synchronization");
    check_cuda(cudaMemcpy(result.pixels.data(), device_output, result.pixels.size() * sizeof(float), cudaMemcpyDeviceToHost),
               "cudaMemcpy output to host");

    cudaFree(device_input);
    cudaFree(device_output);
    cudaFree(device_temp);
    cudaFree(device_gaussKernel);

    return result;
}

bool run_gaussian_smoke_test() {
    Image input(2,2);
    input.at(0,0) = 1.0f;
    input.at(0,1) = 5.0f;
    input.at(1,0) = 2.6f;
    input.at(1,1) = 3.7f;
    auto result = gaussian_blur(input, 1.6f);

    return result.width == 2 && result.height == 2 && result.pixels.size() == 4;
}

}  // namespace sift::cuda
