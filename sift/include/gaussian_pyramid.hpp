#ifndef GAUSSIAN_PYRAMID_H
#define GAUSSIAN_PYRAMID_H

#include "gaussian.hpp"

namespace sift{

    struct GaussianLevel{
        sift::Image img;
        int octaves = 0;
        float sigma = 0.0f;
        int level = 0;
    };

    class GaussianPyramid{
    public:
        explicit GaussianPyramid(const sift::Image& base, int scales = 3);
        std::vector<sift::GaussianLevel> buildGaussianOctave(const sift::Image& img,int octaveIdx, float sigma0);

    private:
        sift::Image m_inputImg;
        std::vector<sift::GaussianLevel> m_gaussianOctave;
        int m_scale;
        float K;
    };
}

#endif