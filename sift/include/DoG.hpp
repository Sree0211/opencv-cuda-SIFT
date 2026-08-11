#ifndef DOG_H
#define DOG_H

#include "gaussian_pyramid.hpp"

namespace sift {

    class DoG {
        public:
            DoG() = default;

            void build(const std::vector<sift::GaussianOctave>& inputPyramid, std::vector<sift::GaussianOctave>&);

    };
}

#endif