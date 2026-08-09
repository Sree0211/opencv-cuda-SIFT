#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <cmath>
#include <cstdint>
#include <vector>
#include <algorithm>

#include "image.hpp"

constexpr float pi = 3.14159265358979323846f;

namespace sift{

  class GaussianBlur {

  public:
    GaussianBlur() = default;
    GaussianBlur(float sigma_);

    Image performGaussianBlur(const Image& input, float sigma);

  private:
    std::vector<float> makeGaussianKernel(float sigma);
    Image convolveHorizontal(const Image& input, const std::vector<float>& kernel);
    Image convolveVertical(const Image& input, const std::vector<float>& kernel);
    
    float m_sigma = 0.0f;
  };

}
#endif