#include "extrema.hpp"

void sift::Extrema::findExtrema(const std::vector<sift::GaussianOctave>& inputDogOctaves)
{
	m_localExtremaCandidates.clear();
	for (int octaveIdx = 0; octaveIdx < inputDogOctaves.size(); ++octaveIdx) {
		const auto& currentOctave = inputDogOctaves[octaveIdx];
		const auto& levels = currentOctave.levels;
		if (levels.size() < 3) { continue; }

		for (int levelIdx = 1; levelIdx < levels.size() - 1; ++levelIdx) {
			const auto& previousLevel = currentOctave[levelIdx - 1];
			const auto& currentLevel = currentOctave[levelIdx];
			const auto& nextLevel = currentOctave[levelIdx + 1];

			for (int row = 1; row < currentLevel.img.height - 1; ++row) {
				for (int col = 1; col < currentLevel.img.width - 1; ++col) {
					
					const float candidate =	currentLevel.img.at(row, col);

					float minVal = candidate;
					float maxVal = candidate;

					std::vector<float> minMaxComparePixels;
					minMaxComparePixels.insert(
						minMaxComparePixels.end(), {
							previousLevel.img.at(i - 1, j - 1),
							previousLevel.img.at(i - 1, j),
							previousLevel.img.at(i - 1, j + 1),
							previousLevel.img.at(i, j - 1),
							previousLevel.img.at(i, j),
							previousLevel.img.at(i, j + 1),
							previousLevel.img.at(i + 1, j - 1),
							previousLevel.img.at(i + 1, j),
							previousLevel.img.at(i + 1, j + 1),

							currentLevel.img.at(i - 1, j - 1),
							currentLevel.img.at(i - 1, j),
							currentLevel.img.at(i - 1, j + 1),
							currentLevel.img.at(i, j - 1),
							currentLevel.img.at(i, j + 1),
							currentLevel.img.at(i + 1, j - 1),
							currentLevel.img.at(i + 1, j),
							currentLevel.img.at(i + 1, j + 1),

							nextLevel.img.at(i - 1, j - 1),
							nextLevel.img.at(i - 1, j),
							nextLevel.img.at(i - 1, j + 1),
							nextLevel.img.at(i, j - 1),
							nextLevel.img.at(i, j),
							nextLevel.img.at(i, j + 1),
							nextLevel.img.at(i + 1, j - 1),
							nextLevel.img.at(i + 1, j),
							nextLevel.img.at(i + 1, j + 1)
						}
						);

					assert(minMaxComparePixels.size() > 0);

					minVal = *std::min_element(minMaxComparePixels.begin(), minMaxComparePixels.end());
					maxVal = *std::max_element(minMaxComparePixels.begin(), minMaxComparePixels.end());

					if (candidate > maxVal || candidate < minVal) {
						// Store the them as local extrema - Potential keypoint
						sift::ExtremaParams extremaParams;
						extremaParams.octave = currentLevel.octave;
						extremaParams.level = currentLevel.level;
						extremaParams.row = i;
						extremaParams.col = j;
						extremaParams.pixelVal = candidate;

						m_localExtremaCandidates.push_back(extremaParams);
					}
				}
			}
		}
	}
}

void sift::Extrema::calKeypointLocalization(const std::vector<sift::GaussianOctave>& dogOctaves)
{
	if (m_localExtremaCandidates.empty()) { return; }

	// Perform Taylor expansion
	for (auto& candidate : m_localExtremaCandidates) {
		
		const int octaveIdx = candidate.octave;
		const int levelIdx = candidate.level;
		const int row = candidate.row;
		const int col = candidate.col;

		// Validate octave
		if (octaveIdx < 0 ||
			octaveIdx >= static_cast<int>(dogOctaves.size()))
		{
			continue;
		}

		const auto& octave = dogOctaves[octaveIdx];
		const auto& levels = octave.levels;

		if (levelIdx < 1 || levelIdx >= levels.size() - 1) { continue; }

		// Gradient in x, y and level(scale)
		const auto& previousLevel = levels[levelIdx - 1];
		const auto& currentLevel = levels[levelIdx];
		const auto& nextLevel = levels[levelIdx + 1];

		const float D = currentLevel.img.at(row, col);

		const float Dx = (currentLevel.img.at(row, col + 1) -
			currentLevel.img.at(row, col - 1)) / 2.0f;
		const float Dy = (currentLevel.img.at(row + 1, col) -
			currentLevel.img.at(row - 1, col)) / 2.0f;
		const float Ds = (nextLevel.img.at(row, col) - 
			previousLevel.img.at(row, col))/ 2.0f;

		// Hessian 
		const float Dxx = currentLevel.img.at(row, col + 1) -
			2.0f * D +
			currentLevel.img.at(row, col - 1);
		const float Dyy = currentLevel.img.at(row + 1, col) -
			2.0f * D +
			currentLevel.img.at(row - 1, col);
		const float Dss = nextLevel.img.at(row, col) -
			2.0f * D +
			previousLevel.img.at(row, col);
		const float Dxy = (currentLevel.img.at(row + 1, col + 1) -
			currentLevel.img.at(row + 1, col - 1) -
			currentLevel.img.at(row - 1, col + 1) +
			currentLevel.img.at(row - 1, col - 1)) / 4;
		const float Dxs = (nextLevel.img.at(row, col + 1) -
			nextLevel.img.at(row, col + 1) -
			previousLevel.img.at(row, col - 1) +
			previousLevel.img.at(row, col - 1)) / 4;
		const float Dys = (nextLevel.img.at(row + 1, col) -
			nextLevel.img.at(row - 1, col) -
			previousLevel.img.at(row + 1, col) +
			previousLevel.img.at(row - 1, col)) / 4;

	}
}
