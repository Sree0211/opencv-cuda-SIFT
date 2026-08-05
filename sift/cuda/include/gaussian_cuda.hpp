#ifndef GAUSSIANCUDA_H
#define GAUSSIANCUDA_H

#include <cstddef>
#include <vector>

#include "../include/image.hpp"

namespace sift::cuda {

Image gaussian_blur(const Image& input, float sigma);

bool run_gaussian_smoke_test();

}  // namespace sift::cuda

#endif