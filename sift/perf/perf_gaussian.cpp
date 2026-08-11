#include "../include/gaussian.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

void fill_random(sift::Image& image)
{
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(0.0f, 255.0f);

    for (float& pixel : image.pixels) {
        pixel = dist(rng);
    }
}

double benchmark(
    int width,
    int height,
    float sigma,
    int warmup,
    int iterations)
{
    sift::Image input(width, height);
    sift::GaussianBlur gaussBlur(sigma);
    fill_random(input);

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
  constexpr float sigma = 1.6f;

    constexpr int warmup = 5;
    constexpr int iterations = 20;

    const std::vector<int> width = {
        8,
        16,
        32,
        128,
        256,
        512,
        1024,
        2048,
        4096
    };

    const std::vector<int> height = {
        10,
        20,
        40,
        130,
        260,
        520,
        1030,
        2050,
        4100
    };

    std::cout
        << std::left
        << std::setw(12) << "Size"
        << std::setw(16) << "Pixels"
        << std::setw(16) << "Time (ms)"
        << std::setw(16) << "MPix/s"
        << '\n';

    std::cout << std::string(60, '-') << '\n';

    for (int i = 0; i < width.size(); ++i) {
        
        std::cout
        << "\nRunning "
        << width[i]
        << " x "
        << height[i]
        << std::endl;

        const double time_ms =
            benchmark(
                width[i],
                height[i],
                sigma,
                warmup,
                iterations);

        const double pixels =
            static_cast<double>(width[i]) * width[i];

        const double megapixels_per_second =
            pixels / (time_ms * 1000.0);

        std::cout
            << std::left
            << width[i]<<" x "<<height[i]  
            << std::setw(16) << static_cast<long long>(pixels)
            << std::setw(16) << std::fixed
            << std::setprecision(3) << time_ms
            << std::setw(16) << std::setprecision(2)
            << megapixels_per_second
            << '\n';
    }

    return 0;
}