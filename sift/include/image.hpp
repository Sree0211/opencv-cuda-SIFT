#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>

struct Image
{
    int width = 0;
    int height = 0;

    // Row major contiguous pixel data
    std::vector<float> pixels;

    Image() = default;
    Image(int width_, int height_)
        : width(width_),
        height(height_),
        pixels(static_cast<size_t>(width_)* height_, 0.0f) {
    };

    float& at(int x, int y){
        return pixels[static_cast<size_t>(y) * width + x];
    }

    float at(int x, int y) const{
        return pixels[static_cast<size_t>(y) * width + x];
    }
    
};

#endif