#include "../include/gaussian_cuda.hpp"

#include <iostream>

int main() {
    if (sift::cuda::run_gaussian_smoke_test()) {
        std::cout << "CUDA smoke test passed" << std::endl;
        return 0;
    }

    std::cerr << "CUDA smoke test failed" << std::endl;
    return 1;
}
