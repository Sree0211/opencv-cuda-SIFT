#ifndef GAUSSIAN_PYRAMID_H
#define GAUSSIAN_PYRAMID_H

#include "keypoint.hpp"

class GaussianPyramid{
public:
    explicit GaussianPyramid(const Mat& base, std::vector<Mat>& pyr, int octaves);


};

#endif