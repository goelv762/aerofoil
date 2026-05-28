#include <SDL3/SDL.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "rendering/render.hpp"
#include "physics/aerofoil.hpp"

constexpr glm::vec2 screenDim = {800, 500};

double getDT() {
    static uint64_t lastTime = SDL_GetPerformanceCounter();
    uint64_t currentTime = SDL_GetPerformanceCounter();

    double dt = (double)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    return dt;
}

int main() {
	Render render(screenDim);

	std::vector<glm::vec2> points = generateAerofoil(0.04, 0.4, 0.15, 50, 300.0f, {250, 220});

	render.addObj(points);

    bool running = true;
    SDL_Event event;

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
