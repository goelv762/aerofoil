#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "rendering/render.hpp"
#include "physics/aerofoil.hpp"

constexpr glm::vec2 screenDim = {800, 500};

constexpr glm::uint16_t foilPoints = 50;

double getDT() {
    static uint64_t lastTime = SDL_GetPerformanceCounter();
    uint64_t currentTime = SDL_GetPerformanceCounter();

    double dt = (double)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    return dt;
}

int main() {
	Render render(screenDim);

	std::string NACA4;
	std::cout << "Enter 4 digits for NACA airfoil: ";
	std::cin >> NACA4;

	double m = (NACA4[0] - '0') / 100.0f;
	double p = (NACA4[1] - '0') / 10.0f;
	double t = std::stoi(NACA4.substr(NACA4.length() - 2)) / 100.0f;

	std::vector<glm::vec2> points = generateAerofoil(m, p, t, foilPoints, 300.0f, {250, 230});
	
	Text name = {
		.text = "NACA " + NACA4 + " AIRFOIL",
		.pos = {10, 10}
	};
	
	render.addObj(points);
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
