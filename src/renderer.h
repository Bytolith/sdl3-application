#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>

#include <SDL3/SDL.h>

#include "datatypes.h"

using namespace std;

inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 600;
inline constexpr bool RENDERER_DEBUG = true;

class Renderer {
	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		unordered_map<string, SDL_Texture*> textures; 

	public:
		Renderer() = default;
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		int Initialize();
		void Destroy();
        
		//Texture Ops
		void LoadTexture(const string& img_path);
		void UnloadTexture(const string& key);
		
		void RenderBackground(Color color);
		void RenderRect(float x, float y, float w, float h, Color color);
		void RenderRect(Rect r, Color color);
		void RenderTexture(Rect r, const string& key);
		void RenderTextureTile(Rect r, Grid grid, int index, const string& key);

		void RenderDebugText(float x, float y, const string& str, Color color = WHITE);

		void Present();

		void SetDrawColor(Color color);
};

#endif
