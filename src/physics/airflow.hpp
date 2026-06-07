#pragma once

#include "aerofoil.hpp"
#include <glm/ext/vector_float2.hpp>

glm::vec2 resolveVelocity(std::vector<Panel>& panels, Vec& panelStrengths, double Vinf, double AoA, glm::vec2 point);
