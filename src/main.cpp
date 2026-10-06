#include <SDL3/SDL.h>
#include <string>

#include "renderer.h"
#include "datatypes.h"

using namespace std;

struct TextBox{
	Rect rect;
	string text;
	int margin = 10;

	TextBox(float x, float y, float w, float h, const string& t) {
		rect = {x,y,w,h};
		text = t;
	}

	void render(Renderer &renderer){
		renderer.RenderRect(rect, GREY);
		renderer.RenderDebugText(rect.x + margin, rect.y + margin, text, WHITE);
	}
};

int main(/*int argc, char* argv[]*/) {
	Renderer r;
	if (r.Initialize() != 0) {
		return 1;
	}

    bool running = true;

    SDL_Event event;
	Vec2 dir = {0,0};
	Vec2 pos = {100, 150};

	Uint64 last_time = SDL_GetTicksNS();
	float delta_time = 0.0f;

	TextBox tb = TextBox(50,50,100,100,"Hello");
    while (running) {
		Uint64 current_time = SDL_GetTicksNS();
		delta_time = (float)(current_time-last_time) / 1000000000.0f;
		last_time = current_time;

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
		// SDL_FRect destinationRect;
		// destinationRect.x = pos.x;
		// destinationRect.y = pos.y;
		// destinationRect.w = 16.0f * scale;
		// destinationRect.h = 18.0f * scale;

		// SDL_SetRenderDrawColor(renderer, WHITE);

		for(int x = 75; x < width-100; x+=60){
			for(int y = 75; y < height-100; y+=60){
				r.RenderRect(x,y,50,50,GREY);
			}
		}

		tb.render(r);
		int scale = 2;
		r.RenderTexture({pos.x,pos.y,16.0f * scale, 18.0f * scale}, "assets/Green-Cap-Character-16x18.png");
		r.RenderDebugText(20,20,"Delta Time (ms): " + to_string(delta_time * 1000.0f));

		r.Present();

    }
	r.Destroy();
    SDL_Quit();

    return 0;
}
