#include "gaussian_pyramid.hpp"

sift::GaussianPyramid::GaussianPyramid(const sift::Image &base, int scales)
    : m_inputImg(base), m_scale(scales)
{
    K = std::pow(2, static_cast<float>(1/m_scale));
    m_gaussianOctave.clear();
}

std::vector<sift::GaussianLevel> sift::GaussianPyramid::buildGaussianOctave(const sift::Image& img, int octaveIdx, float sigma0)
{
    // Create (S + 3)  Gauss kernels
    const int numberOfLevels = m_scale + 3;

    m_gaussianOctave.resize(numberOfLevels);
    
    for(int i = 0; i < numberOfLevels; i++){
        const float sigma = std::pow(K, i) * sigma0;
        m_gaussianOctave[i].img = img;
        m_gaussianOctave[i].octaves = octaveIdx;
        m_gaussianOctave[i].level = i;
        m_gaussianOctave[i].sigma = sigma;
    }

    return m_gaussianOctave;
}
