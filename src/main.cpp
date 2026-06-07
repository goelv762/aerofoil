#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "math/linAlg.hpp"
#include "rendering/render.hpp"
#include "physics/airflow.hpp"

constexpr glm::vec2 screenDim = {800, 500};
constexpr double scale = 500.0;

constexpr uint16_t foilPoints = 100;
// NOTE: not all will be visible, will also have to adjust according to scale
constexpr uint16_t streamLines = 100;

constexpr float streamLineStart = -1.0f;
constexpr float streamLineEnd = 2.0f;
constexpr float streamLineStride = 0.01f; 

// freestream velocity
constexpr double Vinf = 1.0;
// angle of attack (of freestream), rad
constexpr double AoA = 0.05;

double getDT() {
    static uint64_t lastTime = SDL_GetPerformanceCounter();
    uint64_t currentTime = SDL_GetPerformanceCounter();

    double dt = (double)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    return dt;
}

int main() {
	Render render(screenDim, scale);

	std::string NACA4;
	std::cout << "Enter 4 digits for NACA airfoil: ";
	std::cin >> NACA4;

	double m = (NACA4[0] - '0') / 100.0f;
	double p = (NACA4[1] - '0') / 10.0f;
	double t = std::stoi(NACA4.substr(NACA4.length() - 2)) / 100.0f;

	std::vector<glm::vec2> boundaryPoints = generateAerofoilPoints(m, p, t, foilPoints);
	std::vector<Panel> panels = generateAerofoilPanels(boundaryPoints, AoA);
	// I and J matrices (note: stupid notation ??? I is not the identity matrix here)
 	auto [I, J] = computeIJ(panels);
	// b vector
	Vec b = computeB(panels, Vinf);

	// solve the matrix equation Ix = b
	Vec panelStrengths = GaussianElimination(I, b);

	// double s = 0.0f;
	// for (size_t i = 0; i < panels.size(); i++) {
	// 	s += panelStrengths[i] * panels[i].s;
	// }
	// // s should be 0
	// std::cout << s << std::endl;

	render.setOffset(boundaryPoints);

	for (size_t i = 0; i < streamLines; i++) {
		std::vector<glm::vec2> line;
		// normalise between -1 and 1
		glm::vec2 pos = {streamLineStart, 2 * i / (float)streamLines - 1};
		
		while (pos.x < streamLineEnd) {
			line.push_back(pos);
			glm::vec2 vel = resolveVelocity(panels, panelStrengths, Vinf, AoA, pos);

			pos += vel * streamLineStride;
		}

		render.addObj(line);
	}
	
	
	Text name = {
		.text = "NACA " + NACA4 + " AIRFOIL, " + "AoA: " + std::to_string(AoA * 180.0 / M_PI) + " deg",
		.pos = {5, 15}
	};


	render.addObj(boundaryPoints);

	render.addLine(name);

    bool running = true;
    SDL_Event event;
	
	render.exportScreen();

    while (running) {
		double dt = getDT();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

		render.update();
    }
	
    return 0;
}
