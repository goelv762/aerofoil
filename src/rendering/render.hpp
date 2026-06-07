#pragma once
#include <SDL3/SDL.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <string>
#include <vector>

struct Text {
	std::string text;
	glm::vec2 pos;
};

class Render {
	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
	
		std::vector<std::vector<SDL_FPoint>> objs;
		std::vector<Text> text;

		glm::vec2 viewport;
		float scale;
		glm::vec2 offset;
	public:
		Render(glm::vec2 dim, double s);
		~Render();
		
		void addObj(std::vector<glm::vec2>& obj);
		void setOffset(std::vector<glm::vec2>& obj);
		void addLine(Text& line);

		void update();
		void exportScreen();
};
