#ifndef EXTREMA_HPP
#define EXTREMA_HPP

#include "image.hpp"
#include "DoG.hpp"

#include <algorithm>

namespace sift {

	struct ExtremaParams {
		int octave = 0;
		int level = 0;
		int row = 0;
		int col = 0;
		float pixelVal = 0.0f;
		//std::vector < sift::GaussianLevel> comparingLevels;
	};

	class Extrema {
	public:
		Extrema() = default;

		void findExtrema(const std::vector<sift::GaussianOctave>& inputDogOctaves);
		void calKeypointLocalization(const std::vector<sift::GaussianOctave>& dogOctaves);
	private:
		std::vector<ExtremaParams> m_localExtremaCandidates;
	};
}

#endif