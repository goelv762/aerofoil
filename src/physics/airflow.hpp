#pragma once

#include <glm/glm.hpp>
#include <vector>

std::vector<glm::vec2> streamlineSPM(std::vector<Panel>& panels, glm::vec2 point);
glm::vec2 resolveVelocity(std::vector<glm::vec2> influence, Vec panelStrengths, double Vinf, double AoA);
