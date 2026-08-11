#include "../include/gaussian.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

void fill_random_py(sift::Image& image)
{
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(0.0f, 255.0f);

    for (float& pixel : image.pixels) {
        pixel = dist(rng);
    }
}

double benchmark_py(
    int width,
    int height,
    float sigma,
    int warmup,
    int iterations)
{
    sift::Image input(width, height);
    sift::GaussianBlur gaussBlur(sigma);
    fill_random_py(input);

    // Warmup.
    for (int i = 0; i < warmup; ++i) {
        auto result = gaussBlur.performGaussianBlur(input, sigma);
    }

    // Timed execution.
    auto start = Clock::now();

    for (int i = 0; i < iterations; ++i) {
        auto result = gaussBlur.performGaussianBlur(input, sigma);
    }

    auto end = Clock::now();

    const double total_ms =
        std::chrono::duration<double, std::milli>(
            end - start).count();

    return total_ms / iterations;
}

int main()
{

    return 0;
}
