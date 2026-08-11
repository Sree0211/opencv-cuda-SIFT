#include "DoG.hpp"

void sift::DoG::build(const std::vector<sift::GaussianOctave>& inputPyramid, 
    std::vector<sift::GaussianOctave>& outputDoG)
{
    int octave_level = 0;
    sift::GaussianOctave dog;
    while(octave_level < inputPyramid.size()){
        for(int level = 1; level < inputPyramid[octave_level].levels.size(); ++level){
            
        }
        
        octave_level++;
    }
}