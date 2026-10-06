#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include "datatypes.h"

using namespace std;

inline constexpr int width = 800;
inline constexpr int height = 600;

class Renderer {
	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
	public:
		Renderer() = default;
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		int Initialize();
		void Destroy();

		void RenderBackground(Color color);
		void RenderRect(float x, float y, float w, float h, Color color);
		void RenderRect(Rect r, Color color);
		void RenderTexture(Rect r, const string& img_path);

		void RenderDebugText(float x, float y, const string& str, Color color = WHITE);

		void Present();

		void SetDrawColor(Color color);
};

#endif
