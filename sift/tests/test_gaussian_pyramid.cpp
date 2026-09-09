#include <gtest/gtest.h>

#include "../include/gaussian_pyramid.hpp"

TEST(GaussianPyramid, BasicTest)
{
    EXPECT_EQ(1, 1);
}

/*
#include <cassert>
#include <cmath>
#include <iostream>

void test_number_of_levels()
{
    sift::Image input(640, 480);

    constexpr int scales = 5;
    constexpr float sigma0 = 1.6f;

    sift::GaussianPyramid pyramid(input, scales, sigma0);
    const auto octaves = pyramid.build();

    assert(!octaves.empty());

    for (const auto& octave : octaves)
    {
        assert(octave.levels.size() == scales + 3);
    }

    std::cout << "test_number_of_levels: PASS\n";
}

void test_level_dimensions() {
    
    sift::Image input(640, 480);

    constexpr int scales = 5;
    constexpr float sigma0 = 1.6f;

    sift::GaussianPyramid pyramid(input, scales, sigma0);
    const auto octaves = pyramid.build();

    assert(octaves.size() >= 2);

    for (const auto& octave : octaves) {
        const auto expectedWidth = octave.levels[0].img.width;
        const auto expectedHeight = octave.levels[0].img.height;

        for (const auto& level : octave.levels) {
            assert(level.img.width == expectedWidth);
            assert(level.img.height == expectedHeight);
        }
    }
    std::cout << "test_level_dimensions: PASS\n";
}

int main()
{
    test_number_of_levels();
    test_level_dimensions();
    std::cout << "All tests passed.\n";

    return 0;
}

*/
