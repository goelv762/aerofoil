#include "aerofoil.hpp"
#include <algorithm>
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <ostream>
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

double normaliseAngle(double angle) {
	while (angle >= 2.0 * M_PI) angle -= 2.0 * M_PI;
	while (angle <  0.0)        angle += 2.0 * M_PI;

	return angle;
}

std::vector<glm::vec2> generateAerofoilPoints(double m, double p, double t, uint16_t n, double scale) {
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

		u *= scale;

		glm::vec2 l = {
			x + yt * sinTheta,
			yc - yt * cosTheta
		};

		l *= scale;

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
