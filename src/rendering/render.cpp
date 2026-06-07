#include "render.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <glm/ext/vector_float2.hpp>
#include <vector>

constexpr float padding = 3.0f;

constexpr SDL_FColor defaultColour = {200, 200, 200, 255};
constexpr SDL_FColor renderColour  = {30, 30, 30, 255};
constexpr SDL_FColor accentColour  = {137, 207, 240, 255};

constexpr float textSize = 8;

Render::Render(glm::vec2 dim, double s) : viewport(dim), scale(s) {
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Log("Init failed: %s", SDL_GetError());
    }

    window = SDL_CreateWindow("simulation", dim.x, dim.y, 0);
    if (!window) {
        SDL_Log("Window failed: %s", SDL_GetError());
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

void Render::addObj(std::vector<glm::vec2>& obj) {
    std::vector<SDL_FPoint> sdlPoints;

    for (glm::vec2 point : obj) {
		
		// world to screen
        point = (point * scale) + offset;

        // flip y
        point.y = viewport.y - point.y;

        sdlPoints.push_back({point.x, point.y});
    }

    objs.push_back(sdlPoints);
}

void Render::setOffset(std::vector<glm::vec2>& obj) {
    glm::vec2 avg = {0.0f, 0.0f};

    for (glm::vec2& point : obj) {
        avg += point;
    }

    avg /= obj.size();

    offset = viewport * 0.5f - avg * scale;
}

void Render::addLine(Text& line) {
	text.push_back(line);
}

Render::~Render() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Render::update() {
	SDL_SetRenderDrawColor(renderer, renderColour.r, renderColour.g, renderColour.b, renderColour.a);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, defaultColour.r, defaultColour.g, defaultColour.b, defaultColour.a);
	
	for (auto obj : objs) {
		SDL_RenderLines(renderer, obj.data(), obj.size());

		// debugging
		// SDL_RenderPoints(renderer, points.data(), points.size());
	}
	
	for (auto line : text) {
		line.pos.y = viewport.y - line.pos.y;
		SDL_FRect rect = {
			line.pos.x - padding,
			line.pos.y - padding,
			line.text.size() * textSize + 2.0f * padding,
			textSize + 2.0f * padding
		};

		SDL_SetRenderDrawColor(renderer, renderColour.r, renderColour.g, renderColour.b, renderColour.a);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, accentColour.r, accentColour.g, accentColour.b, accentColour.a);
		SDL_RenderRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, defaultColour.r, defaultColour.g, defaultColour.b, defaultColour.a);
		SDL_RenderDebugText(renderer, line.pos.x, line.pos.y, line.text.c_str());
	}


	// Present to screen
	SDL_RenderPresent(renderer);
}

void Render::exportScreen() {
    SDL_Rect viewport;
    SDL_GetRenderViewport(renderer, &viewport);

    // create a texture to render into
    SDL_Texture* target = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        viewport.w, viewport.h
    );

    SDL_SetRenderTarget(renderer, target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    update();

    // read pixels from the texture target
    SDL_Surface* surface = SDL_RenderReadPixels(renderer, nullptr);

    // Restore default render target
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_DestroyTexture(target);

	IMG_SavePNG(surface, "scr.png");

	SDL_DestroySurface(surface);
}


