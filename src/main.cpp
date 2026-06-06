#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstddef>
#include <cstdlib>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "math/linAlg.hpp"
#include "rendering/render.hpp"
#include "physics/aerofoil.hpp"

constexpr glm::vec2 screenDim = {800, 500};

constexpr glm::uint16_t foilPoints = 200;
constexpr double scale = 300.0;

// freestream velocity
constexpr double Vinf = 1.0;

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

	std::vector<glm::vec2> boundryPoints = generateAerofoilPoints(m, p, t, foilPoints);
	std::vector<Panel> panels = generateAerofoilPanels(boundryPoints, 0.0);
	// I and J matrices (note: stupid notation ??? I is not the identity matrix here)
 	auto [I, J] = computeIJ(panels);
	// b vector
	Vec b = computeB(panels, Vinf);

	// solve the matrix equation Ix = b
	Vec panelStrengths = GaussianElimination(I, b);
	double avg = 0.0f;
	for (size_t i = 0; i < panels.size(); i++) {
		avg += panelStrengths[i] * panels[i].s;
	}
	std::cout << avg << std::endl;
	
	Text name = {
		.text = "NACA " + NACA4 + " AIRFOIL",
		.pos = {3, 10}
	};

	render.addObj(boundryPoints);
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
