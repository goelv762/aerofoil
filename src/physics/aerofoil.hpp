#include "../math/linAlg.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <vector>

struct Panel { 
	// first and second point (cw order)
	glm::vec2 p1, p2;
	 // control point
	glm::vec2 pc;
	// difference between p1 and p2
	glm::vec2 dydx;
	// length of panel
	double s;
	// panel orientation angle
	double phi;
	// panel normal angle
	double delta;
	// angle between freestream and panel normal
	double beta;

	// source strength
	double sigma;
};

// mc -> max camber
// mcp -> max camber position
// t -> thickness
// n -> number of points
std::vector<glm::vec2> generateAerofoilPoints(double m, double p, double t, uint16_t n);
// AoA -> angle of attack
std::vector<Panel> generateAerofoilPanels(std::vector<glm::vec2> points, double AoA);

std::pair<Matrix, Matrix> computeIJ(std::vector<Panel>& panels);
// Vinf -> free stream velocity
Vec computeB(std::vector<Panel>& panels, double Vinf);

