#pragma once

#include <cstddef>
#include <vector>

namespace sift::cuda {

struct GaussianResult {
    std::vector<float> data;
    std::size_t width{};
    std::size_t height{};
};

GaussianResult gaussian_blur(const std::vector<float>& input,
                             std::size_t width,
                             std::size_t height,
                             float sigma);

bool run_gaussian_smoke_test();

}  // namespace sift::cuda
