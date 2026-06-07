#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <limits>
#include <ostream>
#include <sys/types.h>
#include <vector>

#include "aerofoil.hpp"

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

double normaliseAngle(double angle) {
	while (angle >= 2.0 * M_PI) angle -= 2.0 * M_PI;
	while (angle <  0.0)        angle += 2.0 * M_PI;

	return angle;
}

std::vector<glm::vec2> generateAerofoilPoints(double m, double p, double t, uint16_t n) {
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
	
		glm::vec2 u = {
			x - yt * sinTheta,
			yc + yt * cosTheta
		};

		glm::vec2 l = {
			x + yt * sinTheta,
			yc - yt * cosTheta
		};

		upper.push_back(u);
		lower.push_back(l);
	}

	// to ensure same side when rendering
	std::vector<glm::vec2> aerofoil;

	for (auto& point : upper) {
		aerofoil.push_back(point);
	}
	
	std::reverse(lower.begin(), lower.end());
	for (auto& point : lower) {
		aerofoil.push_back(point);
	}

	return aerofoil;
}

std::vector<Panel> generateAerofoilPanels(std::vector<glm::vec2> points, double AoA) {
	// first and last point of aerofoil is the same
	// as a result length of the panels vector will be one less than points
	std::vector<Panel> panels(points.size() - 1);
	
	for (size_t i = 0; i < panels.size(); i++) {
		panels[i].p1 = points[i];
		panels[i].p2 = points[i + 1];

		panels[i].pc = (points[i] + points[i + 1]) / 2.0f;
		panels[i].dydx = points[i + 1] - points[i];
		panels[i].s = glm::length(panels[i].dydx);

		panels[i].phi = std::atan2(panels[i].dydx.y, panels[i].dydx.x);
		// rotate to positive angles if neg (dont know if i should have this ??)
		if (panels[i].phi < 0.0) {
			panels[i].phi += 2.0 * M_PI;
		}

		panels[i].delta = panels[i].phi + M_PI / 2.0;
		panels[i].delta = normaliseAngle(panels[i].delta);

		panels[i].beta = panels[i].delta - AoA;
		panels[i].beta = normaliseAngle(panels[i].beta);
	}

	return panels;
}

std::pair<Matrix, Matrix> computeIJ(std::vector<Panel>& panels) {
	// for normal velocity influence
	Matrix I(panels.size(), panels.size());
	// for tangential velocity influence
	Matrix J(panels.size(), panels.size());

	size_t nPanels = panels.size();
	for (size_t i = 0; i < nPanels; i++) {
		for (size_t j = 0; j < nPanels; j++) {
			if (i != j) {
				Panel pi = panels[i];
				Panel pj = panels[j];
				
				// intermediate values
				double A = -(pi.pc.x - pj.p1.x) * std::cos(pj.phi) - (pi.pc.y - pj.p1.y) * std::sin(pj.phi);
				double B = std::pow(pi.pc.x - pj.p1.x, 2) + std::pow(pi.pc.y - pj.p1.y, 2);
				double Cn = std::sin(pi.phi - pj.phi);
				double Dn = -(pi.pc.x - pj.p1.x) * std::sin(pi.phi) + (pi.pc.y - pj.p1.y) * std::cos(pi.phi);
				double Ct = -std::cos(pi.phi - pj.phi);
				double Dt = (pi.pc.x - pj.p1.x) * std::cos(pi.phi) + (pi.pc.y - pj.p1.y) * std::sin(pi.phi);
				double E = std::sqrt(B - std::pow(A, 2));

				if (std::abs(E) < std::numeric_limits<double>::epsilon() || std::isnan(E)) {
					I[i][j] = 0;
					J[i][j] = 0;
				} else {
					// compute I
					double i1 = 0.5 * Cn * std::log((pow(pj.s, 2) + 2.0 * A * pj.s + B) / B);
					double i2 = ((Dn - A * Cn) / E) * (std::atan2(pj.s + A, E) - atan2(A, E));
					I[i][j] = i1 + i2;

					double j1 = 0.5 * Ct * std::log((pow(pj.s, 2) + 2.0 * A * pj.s + B) / B);
					double j2 = ((Dt - A * Ct) / E) * (std::atan2(pj.s + A, E) - atan2(A, E));
					J[i][j] = j1 + j2;
				}

				if (std::isnan(I[i][j])) {
					I[i][j] = 0;
				}

				if (std::isnan(J[i][j])) {
					J[i][j] = 0;
				}
			}
			
			// initialise diagonal
			if (i == j) {
				I[i][j] = M_PI;
				J[i][j] = 0.0;
			}
		}
	}
	
	return {I, J};
}

Vec computeB(std::vector<Panel>& panels, double Vinf) {
	Vec b(panels.size());
	for (size_t i = 0; i < panels.size(); i++) {
		b[i] = -Vinf * 2 * M_PI * cos(panels[i].beta);
	}

	return b;
}
