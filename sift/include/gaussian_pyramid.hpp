#ifndef GAUSSIAN_PYRAMID_H
#define GAUSSIAN_PYRAMID_H

#include "gaussian.hpp"

namespace sift{

    constexpr int minDownsampleImgSize = 16;

    struct GaussianLevel{
        sift::Image img;

        int octave = 0;
        int level = 0;

        float sigma = 0.0f;
    };

    struct GaussianOctave{
        int index = 0;
        std::vector<sift::GaussianLevel> levels;
    };

    class GaussianPyramid{
    public:
        explicit GaussianPyramid(const sift::Image& base, int scales = 3, const float sigma0 = 1.6f);
        std::vector<sift::GaussianOctave> build();

    private:
        sift::GaussianOctave buildOctave(const sift::Image& base,int octaveIdx) const;
    private:        
        Image m_inputImg;

        int m_scale = 3;
        float m_K = 0.0f;
        float m_sigma0 = 1.6f;
    };
}

#endif