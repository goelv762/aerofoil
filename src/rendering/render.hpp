#pragma once
#include <SDL3/SDL.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>

class Render {
	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
	
		std::vector<std::vector<SDL_Vertex>> objs;
	public:
		Render(glm::vec2 dim);
		~Render();
		
		void addObj(std::vector<glm::vec2>& obj);
		void update();
};
