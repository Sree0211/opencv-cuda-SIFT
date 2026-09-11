#ifndef EXTREMA_HPP
#define EXTREMA_HPP

#include "image.hpp"
#include "DoG.hpp"
#include "keypoint.hpp"

#include <algorithm>

constexpr int MAX_ITERATIONS = 5;
namespace sift {

	struct ExtremaParams {
		int octave = 0;
		int level = 0;
		int row = 0;
		int col = 0;
		float pixelVal = 0.0f;
	};

	class Extrema {
	public:
		Extrema() = default;

		void findExtrema(const std::vector<sift::GaussianOctave>& inputDogOctaves);
		void calKeypointLocalization(const std::vector<sift::GaussianOctave>&);
		void assignKpOrientation(const std::vector<sift::GaussianOctave>&);
	private:
		std::vector<ExtremaParams> m_localExtremaCandidates;
		std::vector<sift::KeyPoint> m_localizedKeypoints;
		//std::vector<sift::GaussianOctave> m_
	};
}

#endif