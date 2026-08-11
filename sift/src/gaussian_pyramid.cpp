#include "gaussian_pyramid.hpp"
#include <cassert>

sift::GaussianPyramid::GaussianPyramid(const sift::Image &base, int scales, const float sigma0)
    : m_inputImg(base), m_scale(scales), m_sigma0(sigma0)
{
    assert(m_scale > 0);
    m_K = std::pow(2, 1/static_cast<float>(m_scale));
}

sift::GaussianOctave sift::GaussianPyramid::buildOctave(const sift::Image& base, int octaveIdx) const
{
    // Create (S + 3)  Gauss kernels
    const int numberOfLevels = m_scale + 3;

    sift::GaussianBlur gaussBlur(m_sigma0);

    sift::GaussianOctave octave;
    octave.index = octaveIdx;
    octave.levels.resize(numberOfLevels);

    octave.levels[0].img = base;
    octave.levels[0].octave = octaveIdx;
    octave.levels[0].level = 0;
    octave.levels[0].sigma = m_sigma0;
    
    for(unsigned int level = 1; level < numberOfLevels; level++){

        GaussianLevel& current = octave.levels[level];

        const float sigmaPrevious = m_sigma0 * std::pow(m_K, level - 1);
        const float sigmaCurrent = m_sigma0 * std::pow(m_K, level);
        const float sigmaIncremental = std::sqrt(
                sigmaCurrent * sigmaCurrent -
                sigmaPrevious * sigmaPrevious
            );

        // Gauss Blur
        octave.levels[level].img = gaussBlur.performGaussianBlur(
            octave.levels[level-1].img,
            sigmaIncremental
        );

        octave.levels[level].octave = octaveIdx;
        octave.levels[level].level = level;
        octave.levels[level].sigma = sigmaCurrent;
    }

    return octave;
}

std::vector<sift::GaussianOctave> sift::GaussianPyramid::build()
{
    std::vector<GaussianOctave> pyramid;

    sift::Image octaveBase = m_inputImg;
    int octaveIndex = 0;

    while ( (octaveBase.width >= minDownsampleImgSize) &&
        (octaveBase.height >= minDownsampleImgSize)) {

        sift::GaussianOctave octave = buildOctave(octaveBase, octaveIndex);

        // Downsampling needs to be done for the Gausslevel when sigma is 2*sigma0
        assert(m_scale < octave.levels.size());

        const Image& nextBase = octave.levels[m_scale].img;
        octaveBase = sift::Image::downsampleBy2(nextBase);
        pyramid.push_back(std::move(octave));
        ++octaveIndex;
    }

    return pyramid;
}