#include "renderer.h"

Renderer::~Renderer(){
	Destroy();
}

int Renderer::Initialize(){
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return -1;
	}

	window = SDL_CreateWindow("SDL3 Application", width, height, 0);
	if (!window) {
		std::cerr << "Failed to create window! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		std::cerr << "Failed to create renderer! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
		return -1;
	}

	return 0;
}

void Renderer::Destroy(){
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

void Renderer::SetDrawColor(Color color){
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::RenderBackground(Color color){
	SetDrawColor(color);
	SDL_RenderClear(renderer);
}

void Renderer::RenderRect(float x, float y, float w, float h, Color color){
	SetDrawColor(color);
	SDL_FRect rect = {x, y, w, h};
	SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderRect(Rect r, Color color){
	SetDrawColor(color);
	SDL_FRect rect = {r.x, r.y, r.w, r.h};
	SDL_RenderFillRect(renderer, &rect);
}

//TODO This creates a surface and blit's it to a texture then passes to GPU every update.
//		FIX: It would be better if I cashed textures then used (hashmap maybe)
//			 to find and display them
//TODO Expand support everything SDL3 Supports | rn Supports PNG
void Renderer::RenderTexture(Rect r, const string& img_path){
	SDL_Surface* surface = SDL_LoadPNG(img_path.c_str());
	if (!surface) {
		std::cerr << "Failed to load PNG! SDL_Error: " << SDL_GetError() << std::endl;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture){
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	}
	SDL_DestroySurface(surface);

	SDL_FRect sdl_rect = {r.x,r.y,r.w,r.h};
	SDL_RenderTexture(renderer, texture, nullptr, &sdl_rect);
	SDL_DestroyTexture(texture);
}
	
void Renderer::RenderDebugText(float x, float y, const string& str, Color color){
	SetDrawColor(color);
	SDL_RenderDebugText(renderer, x, y, str.c_str());
}

void Renderer::Present(){
	SDL_RenderPresent(renderer);
}
