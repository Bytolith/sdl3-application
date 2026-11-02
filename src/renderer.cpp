#include "renderer.h"
#include <SDL3/SDL_rect.h>

Renderer::~Renderer(){
	Destroy();
}

int Renderer::Initialize(){
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return -1;
	}

	window = SDL_CreateWindow("SDL3 Application", WIDTH, HEIGHT, 0);
	if (!window) {
		println(stderr, "Failed to create window! SDL_Error: ", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		println(stderr, "Failed to create renderer! SDL_Error: ", SDL_GetError());
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
		return -1;
	}

	return 0;
}

void Renderer::Destroy(){
	UnloadAllTextures();

	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
    if(RENDERER_DEBUG) println("[RENDERER] Resources Freed");
}

void Renderer::SetDrawColor(Color color){
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

//TODO Expand support everything SDL3 Supports | rn Supports PNG
void Renderer::LoadTexture(const string& img_path){
	SDL_Surface* surface = SDL_LoadPNG(img_path.c_str());
	if (!surface) {
		println(stderr, "Failed to load PNG! SDL_Error: ", SDL_GetError());
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture){
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	}
	SDL_DestroySurface(surface);

	//Get Filename
	string filename = "";
	size_t lastSlash = img_path.find_last_of("/\\");
	if (lastSlash != std::string::npos) {
		filename = img_path.substr(lastSlash + 1);
	}

	//If Exists
	if(textures.count(filename) > 0){
		println("{} is already loaded", filename);
		return;
	}
	
	auto result = textures.insert_or_assign(filename, texture);
	if(result.second){
		if(RENDERER_DEBUG) println("[RENDERER] Loading Texture: {}", filename);
	}
	else{
		SDL_DestroyTexture(texture);
	}
}
void Renderer::UnloadTexture(const string& key){
	if(RENDERER_DEBUG) println("[RENDERER] Unloading Texture: {}" , key);
	SDL_DestroyTexture(textures.at(key));
	textures.erase(key);
}

void Renderer::UnloadAllTextures(){
	if(RENDERER_DEBUG) println("[RENDERER] Unloading All Textures: ");
	for(const auto& [key, value] : textures) {
		if(RENDERER_DEBUG) println("[RENDERER] Unloading Texture: {}" , key);
		SDL_DestroyTexture(textures.at(key));
    }
	textures.clear();
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

void Renderer::RenderTexture(Rect r, const string& key){
	SDL_FRect sdl_rect = {r.x,r.y,r.w,r.h};
	SDL_RenderTexture(renderer, textures.at(key), nullptr, &sdl_rect);
}
	
void Renderer::RenderTextureTile(Rect r, Grid grid, int index, const string& key){
	auto result = textures.find(key);
	if(result == textures.end() || !result->second){
		if(RENDERER_DEBUG) println(stderr, "[RENDERER] Texture not loaded: s", key); 
		return;
	}
	SDL_Texture* texture = result->second;

	const int cols = grid.columns;
	const int rows = grid.rows;

	if(cols <= 0 || rows <= 0){
		if(RENDERER_DEBUG) std::println(stderr, "[RENDERER] Invalid Grid {}x{}", cols, rows);
		return;
	}
	if(index < 0 || index >= cols * rows){
		if(RENDERER_DEBUG) println(stderr, "[RENDERER] Index {} out of bounds for {} tiles ({}x{})", index,cols*rows,cols,rows); 
		return;
	}
	
	float fw, fh;
	if(!SDL_GetTextureSize(texture, &fw, &fh)){
		println(stderr, "Failed to get texure size SDL_Error: ", SDL_GetError());
		return;
	}

	const int col = index % cols;
	const int row = index / cols;

	Vec2 tileSize = {fw/cols, fh/rows};

	SDL_FRect main_rect = {r.x,r.y,r.w,r.h};
	SDL_FRect cuttout_rect = {col * tileSize.x, row * tileSize.y, tileSize.x, tileSize.y};

	SDL_RenderTexture(renderer, texture, &cuttout_rect, &main_rect);
}

void Renderer::RenderDebugText(float x, float y, const string& str, Color color){
	SetDrawColor(color);
	SDL_RenderDebugText(renderer, x, y, str.c_str()); 
}

void Renderer::Present(){
	SDL_RenderPresent(renderer);
}
