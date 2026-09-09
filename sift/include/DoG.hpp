#ifndef DOG_H
#define DOG_H

#include "gaussian_pyramid.hpp"

namespace sift {
    class DoG {
    public:
        DoG() = default;

        std::vector<sift::GaussianOctave>& build(const std::vector<sift::GaussianOctave>&);
        std::vector<sift::GaussianOctave> getDoG();
    private:
        std::vector<sift::GaussianOctave> m_DogOctaves;
    };
}

#endif