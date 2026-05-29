#include "aerofoil.hpp"
#include <algorithm>
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <sys/types.h>
#include <vector>

double computeYT(double t, double x, bool isOpen) {
	// constants are derived somewhere else,
	// better to accept them and move on with my life ~~
	// http://airfoiltools.com/airfoil/naca4digit
	
	double a4 = isOpen ? -0.1015 : -0.1036;

	return 5.0 * t * 
		  (0.2969 * std::sqrt(x) - 
		   0.1260 * x -
		   0.3516 * std::pow(x, 2) + 
		   0.2843 * std::pow(x, 3) +
		   a4     * std::pow(x, 4));
}

std::vector<glm::vec2> generateAerofoil(double m, double p, double t, uint16_t n, double scale) {
	std::vector<glm::vec2> upper;
	std::vector<glm::vec2> lower;
	
	// do math !! 
	// https://www.youtube.com/watch?v=pcPJf7f3VOQ

	double deltaPhi = M_PI / (n - 1);

	for (size_t i = 0; i < n; i++) {
		double phi = deltaPhi * i;

		double x = 0.5 * (1.0 - std::cos(phi));
		double yt = computeYT(t, x, false);
		double yc, dyc;
		if (x < p) {
			yc  = (m / pow(p, 2)) * (2 * p * x - pow(x, 2));
			dyc = (2 * m / pow(p, 2)) * (p - x);
		} else {
			yc  = (m / pow(1 - p, 2)) * (1 - 2 * p + 2 * p * x - pow(x, 2));
			dyc = (2 * m / pow(1 - p, 2)) * (p - x);
		}

		double theta = std::atan(dyc);

		double cosTheta = std::cos(theta);
		double sinTheta = std::sin(theta);
	
		// negitave y coords as the y axis goes in opp dir
		glm::vec2 u = {
			x - yt * sinTheta,
			-(yc + yt * cosTheta)
		};

		u *= scale;

		glm::vec2 l = {
			x + yt * sinTheta,
			-(yc - yt * cosTheta)
		};

		l *= scale;

		upper.push_back(u);
		lower.push_back(l);
	}

	// to ensure same side when rendering
	std::reverse(upper.begin(), upper.end());

	std::vector<glm::vec2> aerofoil;
	for (auto& point : upper) {
		aerofoil.push_back(point);
	}

	for (auto& point : lower) {
		aerofoil.push_back(point);
	}


	return aerofoil;
}
