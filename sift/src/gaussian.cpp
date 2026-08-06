#include "gaussian.hpp"

GaussianBlur::GaussianBlur(float sigma_) : sigma(m_sigma)
{
}

Image GaussianBlur::performGaussianBlur(const Image& input, float sigma)
{
	const auto kernel = makeGaussianKernel(sigma);

	Image tempOut;
	tempOut = convolveHorizontal(input, kernel);
	return convolveVertical(tempOut, kernel);
}

std::vector<float> GaussianBlur::makeGaussianKernel(float sigma)
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

Image GaussianBlur::convolveHorizontal(const Image& input, const std::vector<float>& kernel)
{
	Image output(input.width, input.height);

	const float radius = static_cast<int>(kernel.size() / 2);

	for (int y = 0; y < input.width; ++y) {
		for (int x = 0; x < input.height; ++x) {
			float sum = 0.0f;
			for (int k = -radius; k < radius; ++k) {
				const int SampleX = std::clamp(x + k, 0, input.width - 1);
				sum += input.at(SampleX, y)*kernel[k + radius];
			}
			output.at(x, y) = sum;
		}
	}
	return output;
}

Image GaussianBlur::convolveVertical(const Image& input, const std::vector<float>& kernel)
{
	Image output(input.width, input.height);

	const float radius = static_cast<int>(kernel.size() / 2);

	for (int y = 0; y < input.width; ++y) {
		for (int x = 0; x < input.height; ++x) {
			float sum = 0.0f;
			for (int k = -radius; k < radius; ++k) {
				const int SampleY = std::clamp(y + k, 0, input.height - 1);
				sum += input.at(x, SampleY) * kernel[k + radius];
			}
			output.at(x, y) = sum;
		}
	}
	return output;
}
