#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>
#include <cassert>

namespace sift{

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
          assert(width_ >= 0);
          assert(height_ >= 0);
      };

      Image operator-(const Image& src) const{
          assert(src.width == this->width &&
              src.height == this->height);

          Image img;
          img.width = src.width;
          img.height = src.height;
          
          for (int i = 0; i < src.width; ++i) {
            for (int j = 0; j < src.height; ++j) {
                img.at(i, j) = this->at(i, j) - src.at(i, j);
            }
          }

          return img;
      }

      float& at(int x, int y){
          return pixels[static_cast<size_t>(y) * width + x];
      }

      float at(int x, int y) const{
          return pixels[static_cast<size_t>(y) * width + x];
      }

      static Image downsampleBy2(const Image& input) {
          Image output(input.width / 2, input.height / 2);
          for (int y = 0; y < output.height; ++y) {
              for (int x = 0; x < output.width; ++x) {
                  output.at(x, y) = input.at(2 * x, 2 * y);
              }
          }
          return output;
      }
      
  };
}
#endif