#include <SDL3/SDL.h>
#include <iostream>
#include <string>

using namespace std;

const int width = 800;
const int height = 600;

struct Color{
	int r,g,b,a;
};

const Color BLACK = {  0,  0,  0,255};
const Color GREY  = {127,127,127,255};
const Color WHITE = {255,255,255,255};

struct Rect{
	float x,y,w,h;
};

// Renderer
struct Renderer{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;

		void SetDrawColor(Color color){
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		}
	public:
		int Initialize(){
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
				SDL_Quit();
				return -1;
			}	

			return 0;
		}

		void Destroy(){
    		SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
		}

		void RenderBackground(Color color){
			SetDrawColor(color);
			SDL_RenderClear(renderer);
		}

		void RenderRect(float x, float y, float w, float h, Color color){
			SetDrawColor(color);
		    SDL_FRect rect = {x,y,w,h};
			SDL_RenderFillRect(renderer, &rect);
		}
		void RenderRect(Rect r, Color color){
			SetDrawColor(color);
		    SDL_FRect rect = {r.x,r.y,r.w,r.h}; 
 			SDL_RenderFillRect(renderer, &rect);
		}


		void Present() { SDL_RenderPresent(renderer); }
};


struct Display {
	int width, height;
};

struct vec2 {
	float x, y;
};

struct TextBox{
	SDL_FRect rect;
	string text;
	int margin = 10;

	TextBox(float x, float y, float w, float h, const string& t) {
		rect = {x,y,w,h};
		text = t;
	}

	// void render(SDL_Renderer *renderer){
	// 	SDL_SetRenderDrawColor(renderer, GREY);
	// 	SDL_RenderFillRect(renderer, &rect);
	//
	// 	SDL_SetRenderDrawColor(renderer, WHITE);
	// 	SDL_RenderDebugText(renderer, rect.x + margin, rect.y + margin, text.c_str());
	// }
};

const Display d = {800,600};

int main(int argc, char* argv[]) {
	Renderer r;
	r.Initialize();

	// SDL_Surface* surface = SDL_LoadPNG("assets/Green-Cap-Character-16x18.png");
	// if (!surface) {
	//        std::cerr << "Failed to load PNG! SDL_Error: " << SDL_GetError() << std::endl;
	//    }
	// SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	// if (texture){
	// 	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	// }
	// SDL_DestroySurface(surface);

    bool running = true;

    SDL_Event event;
	vec2 dir = {0,0};
	vec2 pos = {100, 150};

	Uint64 last_time = SDL_GetTicksNS();
	float delta_time = 0.0f;

	// TextBox tb = TextBox(50,50,100,100,"Hello");
    while (running) {
		Uint64 current_time = SDL_GetTicksNS();

		Rect rect = {50,50,50,50};

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				switch(event.key.key){
					case SDLK_ESCAPE: running = false; break;
					case SDLK_W:      dir.y = -1.0f;   break;
					case SDLK_S:      dir.y =  1.0f;   break;
					case SDLK_A:      dir.x = -1.0f;   break;
					case SDLK_D:      dir.x =  1.0f;   break;
				}
			}
			if (event.type == SDL_EVENT_KEY_UP){
				switch(event.key.key){
					case SDLK_W: if (dir.y < 0) dir.y = 0.0f; break;
					case SDLK_S: if (dir.y > 0) dir.y = 0.0f; break;
					case SDLK_A: if (dir.x < 0) dir.x = 0.0f; break;
					case SDLK_D: if (dir.x > 0) dir.x = 0.0f; break;
				}
			}
        }
		pos.x += dir.x * 100 * delta_time;
		pos.y += dir.y * 100 * delta_time;

		r.RenderBackground(BLACK);

		// SDL_FRect sourceRect;
		// sourceRect.x = 0.0f;    
		// sourceRect.y = 0.0f;   
		// sourceRect.w = 16.0f; 
		// sourceRect.h = 18.0f; 
		//
		// int scale = 2;
		// SDL_FRect destinationRect;
		// destinationRect.x = pos.x; 
		// destinationRect.y = pos.y;
		// destinationRect.w = 16.0f * scale; 
		// destinationRect.h = 18.0f * scale;
		
		// SDL_SetRenderDrawColor(renderer, WHITE);
		// SDL_RenderDebugText(renderer, 10.0f, 10.0f, std::to_string(delta_time).c_str());

		// tb.render(renderer);
		
		for(int x = 75; x < width-100; x+=60){
			for(int y = 75; y < height-100; y+=60){
				r.RenderRect(x,y,50,50,GREY);
			}
		}
		// if(texture) SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);

		r.Present();
		delta_time = (float)(current_time-last_time) / 1000000000.0f;
		last_time = current_time;

    }
	r.Destroy();
    SDL_Quit();

    return 0;
}
