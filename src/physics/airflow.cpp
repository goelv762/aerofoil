#include <cmath>
#include <vector>

#include "airflow.hpp"

glm::vec2 resolveVelocity(std::vector<Panel>& panels, Vec& panelStrengths, double Vinf, double AoA, glm::vec2 point) {
	std::vector<glm::vec2> M(panels.size());

	for (size_t i = 0; i < panels.size(); i++) {
		Panel pi = panels[i];

		// intermediate values
		double A = -(point.x - pi.p1.x) * std::cos(pi.phi) - (point.y - pi.p1.y) * std::sin(pi.phi);
		double B = std::pow(point.x - pi.p1.x, 2) + std::pow(point.y - pi.p1.y, 2);

		double Cx = -std::cos(pi.phi);
		double Dx = point.x - pi.p1.x;
		double Cy = -std::sin(pi.phi);
		double Dy = point.y - pi.p1.y;

		double E = std::sqrt(B - std::pow(A, 2));
		if (std::isnan(E)) {
			M[i] = {0.0f, 0.0f};
		}

		else {
			double x1 = 0.5 * Cx * log((std::pow(pi.s, 2) + 2 * A * pi.s + B) / B);
			double x2 = ((Dx - A * Cx) / E) * (std::atan2(pi.s + A, E) - std::atan2(A, E));

			M[i].x = x1 + x2;

			double y1 = 0.5 * Cy * log((std::pow(pi.s, 2) + 2 * A * pi.s + B) / B);
			double y2 = ((Dy - A * Cy) / E) * (std::atan2(pi.s + A, E) - std::atan2(A, E));

			M[i].y = y1 + y2;
		}

		if (std::isnan(M[i].x)) {
			M[i].x = 0.0f;
		}

		if (std::isnan(M[i].y)) {
			M[i].y = 0.0f;
		}
	}

	glm::vec2 velocity;

	glm::vec2 sum = {0.0, 0.0};
	for (size_t i = 0; i < panelStrengths.size(); i++) {
		sum.x += panelStrengths[i] * M[i].x / (2.0 * M_PI);
		sum.y += panelStrengths[i] * M[i].y / (2.0 * M_PI);
	}

	velocity.x = Vinf * std::cos(AoA) + sum.x;
	velocity.y = Vinf * std::sin(AoA) + sum.y;

	return velocity;
}
