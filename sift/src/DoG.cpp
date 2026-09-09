#include "DoG.hpp"

std::vector<sift::GaussianOctave>& sift::DoG::build(const std::vector<sift::GaussianOctave>& inputPyramid)
{
    int octave_level = 0;
    sift::GaussianOctave dog;
    while(octave_level < inputPyramid.size()){
        dog.index = octave_level;
        dog.levels.clear();
        dog.levels.reserve(inputPyramid[octave_level].levels.size() - 1);
        for(unsigned int level = 1; level < inputPyramid[octave_level].levels.size(); ++level){
            
            dog.levels[level - 1].octave = octave_level;
            dog.levels[level - 1].level = level - 1;
            dog.levels[level - 1].sigma = inputPyramid[octave_level].levels[level].sigma;
            dog.levels[level - 1].img = inputPyramid[octave_level].levels[level].img - inputPyramid[octave_level].levels[level-1].img;
        }
        m_DogOctaves.push_back(dog);
        octave_level++;
    }
    return m_DogOctaves;
}

std::vector<sift::GaussianOctave> sift::DoG::getDoG()
{
    return m_DogOctaves;
}
