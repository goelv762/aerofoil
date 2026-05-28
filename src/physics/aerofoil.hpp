#include <glm/glm.hpp>
#include <vector>

// mc -> max camber
// mcp -> max camber position
// t -> thickness
// n -> number of points
std::vector<glm::vec2> generateAerofoil(double m, double p, double t, uint16_t n, double scale, glm::vec2 offset);
