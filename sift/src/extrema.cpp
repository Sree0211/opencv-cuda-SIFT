#include "extrema.hpp"

void sift::Extrema::findExtrema(const std::vector<sift::GaussianOctave>& inputDogOctaves)
{
	m_localExtremaCandidates.clear();
	for (int octaveIdx = 0; octaveIdx < inputDogOctaves.size(); ++octaveIdx) {
		const auto& currentOctave = inputDogOctaves[octaveIdx];
		const auto& levels = currentOctave.levels;
		if (levels.size() < 3) { continue; }

		for (int levelIdx = 1; levelIdx < levels.size() - 1; ++levelIdx) {
			const auto& previousLevel = levels[levelIdx - 1];
			const auto& currentLevel = levels[levelIdx];
			const auto& nextLevel = levels[levelIdx + 1];

			for (int row = 1; row < currentLevel.img.height - 1; ++row) {
				for (int col = 1; col < currentLevel.img.width - 1; ++col) {
					
					const float candidate =	currentLevel.img.at(row, col);

					float minVal = candidate;
					float maxVal = candidate;

					// Previous scale
					for (int i = -1; i <= 1; ++i) {
						for (int j = -1; j <= 1; ++j) {
							const float value = previousLevel.img.at(row + i, col + j);

							minVal = std::min(minVal, value);
							maxVal = std::max(maxVal, value);
						}
					}

					// Current scale
					for (int i = -1; i <= 1; ++i) {
						for (int j = -1; j <= 1; ++j) {
							if (i == 0 && j == 0) {
								continue; // Current element is neglected for comparison
							}

							const float value = currentLevel.img.at(row + i, col + j);

							minVal = std::min(minVal, value);
							maxVal = std::max(maxVal, value);
						}
					}

					// Next scale
					for (int i = -1; i <= 1; ++i) {
						for (int j = -1; j <= 1; ++j) {
							const float value = nextLevel.img.at(row + i, col + j);

							minVal = std::min(minVal, value);
							maxVal = std::max(maxVal, value);
						}
					}

					if (candidate > maxVal || candidate < minVal) {
						// Store the them as local extrema - Potential keypoint
						sift::ExtremaParams extremaParams;
						extremaParams.octave = currentLevel.octave;
						extremaParams.level = currentLevel.level;
						extremaParams.row = row;
						extremaParams.col = col;
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

	m_localizedKeypoints.clear();

	// Perform Taylor expansion
	for (auto& candidate : m_localExtremaCandidates) {
		
		const int octaveIdx = candidate.octave;
		int levelIdx = candidate.level;
		int row = candidate.row;
		int col = candidate.col;

		// Validate octave
		if (octaveIdx < 0 ||
			octaveIdx >= static_cast<int>(dogOctaves.size()))
		{
			continue;
		}

		const auto& octave = dogOctaves[octaveIdx];
		const auto& levels = octave.levels;

		// Verify if the values converge to the local extremum
		bool converged = false;
		float Delta[3]{ 0.0f, 0.0f, 0.0f }; // Offset values

		for (int iter = 0; iter < MAX_ITERATIONS; iter++) {

			if (levelIdx < 1 || levelIdx >= levels.size() - 1 ||
				row < 1 || row >= levels[levelIdx].img.height - 1 ||
				col < 1 || col >= levels[levelIdx].img.width - 1) 
			{
				break; 
			}

			const auto& previousLevel = levels[levelIdx - 1];
			const auto& currentLevel = levels[levelIdx];
			const auto& nextLevel = levels[levelIdx + 1];

			const float D = currentLevel.img.at(row, col);

			// Gradient in x, y and level(scale)
			const float Dx = (currentLevel.img.at(row, col + 1) -
				currentLevel.img.at(row, col - 1)) / 2.0f;
			const float Dy = (currentLevel.img.at(row + 1, col) -
				currentLevel.img.at(row - 1, col)) / 2.0f;
			const float Ds = (nextLevel.img.at(row, col) -
				previousLevel.img.at(row, col)) / 2.0f;
			const float neg_g[3] = {
				-Dx,
				-Dy,
				-Ds
			};

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
				currentLevel.img.at(row - 1, col - 1)) / 4.0f;
			const float Dxs = (nextLevel.img.at(row, col + 1) -
				nextLevel.img.at(row, col - 1) -
				previousLevel.img.at(row, col + 1) +
				previousLevel.img.at(row, col - 1)) / 4.0f;
			const float Dys = (nextLevel.img.at(row + 1, col) -
				nextLevel.img.at(row - 1, col) -
				previousLevel.img.at(row + 1, col) +
				previousLevel.img.at(row - 1, col)) / 4.0f;

			// Smooth function hence Dxy = Dyx, similar for others
			// H = H^T
			const float H[3][3] = {
				{Dxx, Dxy, Dxs},
				{Dxy, Dyy, Dys},
				{Dxs, Dys, Dss}
			};

			// Taylor expansion
			// Calculate offset by making use of algorithms to solve linear systems of equation
			// This is done to preserve numerical stability and reduce roundoff errors.
			// We make use of LDL^T solver to get the offsets - since H is a symmetric matrix

			// U Matrix
			const float U_Dz = ((Dxx * Dss - Dxs * Dxs) / Dxx) -
				((Dxx * Dys - Dxy * Dxs) * (Dys * Dxx - Dxs * Dxy)) / (Dxx * (Dyy * Dxx - Dxy * Dxy));

			const float U[3][3] = {
				{Dxx, Dxy, Dxs},
				{0, (Dyy * Dxx - Dxy * Dxy) / Dxx, (Dys * Dxx - Dxs * Dxy) / Dxx},
				{0, 0, U_Dz}
			};
			const float Diag[3] = {
				U[0][0],
				U[1][1],
				U[2][2]
			};

			if (std::abs(Diag[0]) < 1e-6f || std::abs(Diag[1]) < 1e-6f
				|| std::abs(Diag[2]) < 1e-6f) {
				continue;
			}

			// L Matrix multipliers
			const float L_10 = Dxy / Diag[0];
			const float L_20 = Dxs / Diag[0];
			const float L_21 = ((Dys * Dxx - Dxs * Dxy) / Dxx) / Diag[1];

			// Solve for delta
			// Forward subsittution (Ax = b) to Ly = b, where y = DL^Tx
			float y[3];
			y[0] = neg_g[0];
			y[1] = neg_g[1] - L_10 * y[0];
			y[2] = neg_g[2] - L_20 * y[0] - L_21 * y[1];

			// Dz = y, where z = L^Tx
			float z[3];
			z[0] = y[0] / Diag[0];
			z[1] = y[1] / Diag[1];
			z[2] = y[2] / Diag[2];

			// Backward substitution
			Delta[2] = z[2];
			Delta[1] = z[1] - L_21 * Delta[2];
			Delta[0] = z[0] - L_10 * Delta[1] - L_20 * Delta[2];

			// Threshold
			if (std::abs(Delta[0]) <= 0.5f && std::abs(Delta[1]) <= 0.5f && 
				std::abs(Delta[2]) <= 0.5f) {
				converged = true;
				break;
			}

			row += static_cast<int>(std::round(Delta[1]));
			col += static_cast<int>(std::round(Delta[0]));
			levelIdx += static_cast<int>(std::round(Delta[2]));
		}

		if (converged) {
			// If its converged, calculate the localized DoG contrast
			// Performing final Taylor expansion.

			// D_final = D + 0.5 * gradient^T * Delta
			const float Dx = (levels[levelIdx].img.at(row, col + 1) -
					levels[levelIdx].img.at(row, col - 1)) / 2.0f;
			const float Dy = (levels[levelIdx].img.at(row + 1, col) -
				levels[levelIdx].img.at(row - 1, col)) / 2.0f;
			const float Ds = (levels[levelIdx + 1].img.at(row, col) -
				levels[levelIdx - 1].img.at(row, col)) / 2.0f;
			const float Dxx = levels[levelIdx].img.at(row, col + 1) -
				2.0f * levels[levelIdx].img.at(row,col) +
				levels[levelIdx].img.at(row, col - 1);
			const float Dyy = levels[levelIdx].img.at(row + 1, col) -
				2.0f * levels[levelIdx].img.at(row, col) +
				levels[levelIdx].img.at(row - 1, col);
			const float Dss = levels[levelIdx+1].img.at(row, col) -
				2.0f * levels[levelIdx].img.at(row, col) +
				levels[levelIdx-1].img.at(row, col);
			const float Dxy = (levels[levelIdx].img.at(row + 1, col + 1) -
				levels[levelIdx].img.at(row + 1, col - 1) -
				levels[levelIdx].img.at(row - 1, col + 1) +
				levels[levelIdx].img.at(row - 1, col - 1)) / 4.0f;

			float D_final = levels[levelIdx].img.at(row, col) +
				(0.5 * (Dx * Delta[0] + Dy * Delta[1] + Ds * Delta[2]));

			// Low contrast rejection
			//if (std::abs(D_final) < 0.03f) { continue; } // TODO: need to verify the threashold value

			// Edge response rejection
			const float trace_H = Dxx + Dyy;
			const float det_H = Dxx * Dyy - Dxy * Dxy;

			// For spatial extremum the eigenvalues should have the same sign
			// i.e. the eigenvalues of the
			// extremum is streched along the eigenvectors in the same direction
			if (det_H <= 0.0f) {
				continue; 
			}

			// Eigenvalues should be closer to one another if not its a case of edge/corner
			// i.e. stark difference between the values
			// Threshold value considered from SIFT paper, r = 10, where r = eigenvalue1/eigenvalue2
			if ((trace_H * trace_H / det_H) > 12.1f) {
				continue; 
			}

			// Include Keypoints
			sift::KeyPoint kp;
			kp.pt.x = static_cast<float>(col) + Delta[0];
			kp.pt.y = static_cast<float>(row) + Delta[1];
			kp.level = static_cast<float>(levelIdx) + Delta[2];
			kp.octave = levels[levelIdx].octave;
			kp.response = D_final;

			m_localizedKeypoints.push_back(kp);
		}
	}
}

void sift::Extrema::assignKpOrientation(const std::vector<sift::GaussianOctave>& gaussianPyramid)
{
	if (m_localizedKeypoints.empty()) { return; }

	for (auto& localKp : m_localizedKeypoints) {
		const int kpOctave = localKp.octave;
		const int kpLevel = static_cast<int>(std::round(localKp.level));
		const int kpRow = static_cast<int>(std::round(localKp.pt.y));
		const int kpCol = static_cast<int>(std::round(localKp.pt.x));

		for (const auto& gPLevel : gaussianPyramid[kpOctave].levels)
		{
			if (gPLevel.level != kpLevel) {
				continue;
			}
		
			const float sigmaW = 1.5 * gPLevel.sigma; // From SIFT Paper
			const int radius = std::ceil(3 * sigmaW);

			// histogram bins
			// Create 36 bins and initialize it
			std::vector<float> histBins(36, 0.0f); 

			for (int i = -radius; i <= radius; ++i) {
				for (int j = -radius; j <= radius; ++j) {
					int row = kpRow + i;
					int col = kpCol + j;

					if ((row > gPLevel.img.height - 1 || row < 0) ||
						(col > gPLevel.img.width - 1 || col < 0))  {
						continue;
					}

					// Calculate Magnitude and Angle from the neighbouring pixels
					const float Lx = (gPLevel.img.at(row, col + 1) -
						gPLevel.img.at(row, col - 1)) / 2.0f;
					const float Ly = (gPLevel.img.at(row + 1, col) -
						gPLevel.img.at(row - 1, col)) / 2.0f;

					const float mxy = std::sqrt(Lx * Lx + Ly * Ly);
					const float theta = std::atan2f(Ly, Lx);
					const float thetaxy = theta < 0.0f
						? theta + 2.0f * static_cast<float>(pi)
						: theta;

					// Weighted contributing factor
					// This decides based on the scale to give a weight to the neighbouring pixels
					const float wxy = std::exp(-1 * (i * i + j * j) / (2 * sigmaW * sigmaW));
					const float cxy = mxy * wxy;

					// Histogram
					const int N = 36;
					int bin = static_cast<int>(std::floor(thetaxy * N / (2 * pi)));
					
					assert(bin >= 0 && bin < 35);
					if (bin < 0 || bin > 35) { break; }

					if (bin == N) {
						bin = 0;
					}

					histBins[bin] += cxy;
				}
			}

			float bestOriVal = *std::max_element(histBins.cbegin(), histBins.cend());

			std::vector<float> cpyHistBins;
			cpyHistBins.reserve(35);
			for (const auto& val : histBins) {
				if (val != bestOriVal) {
					cpyHistBins.push_back(val);
				}
			}
			int secondBestOriVal = *std::max_element(cpyHistBins.cbegin(), cpyHistBins.cend());
			if (secondBestOriVal > 0.8f * bestOriVal) {
				// We create 2 KPs at the same location
				sift::KeyPoint cpyKp;
				cpyKp = localKp;
				//cpyKp.angle = 
			}
		}
	}
}
