#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <cstdint>
#include <vector>
#include <algorithm>

#include "image.hpp"

constexpr float pi = 3.14159265358979323846f;

class GaussianBlur {

public:
	GaussianBlur() = default;
	GaussianBlur(float sigma_);

	Image performGaussianBlur(const Image& input, float sigma);

private:
	std::vector<float> makeGaussianKernel(float sigma);
	Image convolveHorizontal(const Image& input, const std::vector<float>& kernel);
	Image convolveVertical(const Image& input, const std::vector<float>& kernel);

	float sigma = 1.6f;
};

#endif