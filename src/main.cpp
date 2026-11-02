#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <cstdlib>
#include <string>
#include <print>

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

struct MovablePanel{
	Rect rect = {0,0,0,0};
	bool is_grabbed = false;
	bool is_dragging = false;
	Vec2 drag_offset = {0,0};
	
	MovablePanel(float x,float y,float w,float h){
		rect = {x,y,w,h};
	}

	void set_position(Vec2 new_position){
		rect.x = new_position.x;
		rect.y = new_position.y;
	}

	void render(Renderer &renderer){
		renderer.RenderRect(rect, ORANGE);
	}
};


int main(/*int argc, char* argv[]*/) {
	Renderer r;
	if (r.Initialize() != 0) return 1;

    bool running = true;

    SDL_Event event;
	Vec2 dir = {0,0};
	Vec2 pos = {100, 150};
	int frame = 0;

	bool key_w = false, key_s = false, key_a = false, key_d = false;
	
	bool left_click = false;
	Vec2 click_pos = {0,0};
	Vec2 mouse_pos = {0,0};

	Uint64 last_time = SDL_GetTicksNS();
	float delta_time = 0.0f;

	TextBox tb = TextBox(50,50,100,100,"Hello");
	MovablePanel mp = MovablePanel(300,300,100,100);

	r.LoadTexture("assets/Green-Cap-Character-16x18.png");
    while (running) {
		Uint64 current_time = SDL_GetTicksNS();
		delta_time = (float)(current_time-last_time) / 1000000000.0f;
		last_time = current_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
				click_pos.x = event.button.x;
				click_pos.y = event.button.y;

				switch (event.button.button) {
					case SDL_BUTTON_LEFT: left_click = true; break;
				}
			}
			if (event.type == SDL_EVENT_MOUSE_BUTTON_UP){
				switch (event.button.button) {
					case SDL_BUTTON_LEFT: left_click = false; break;
				}
			}
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				switch(event.key.key){
					case SDLK_ESCAPE: running = false; break;
					case SDLK_W: key_w = true; break;
					case SDLK_S: key_s = true; break;
					case SDLK_A: key_a = true; break;
					case SDLK_D: key_d = true; break;
				}
			}
			if (event.type == SDL_EVENT_KEY_UP){
				switch(event.key.key){
					case SDLK_W: key_w = false; break;
					case SDLK_S: key_s = false; break;
					case SDLK_A: key_a = false; break;
					case SDLK_D: key_d = false; break;
				}
			}
			if (event.type == SDL_EVENT_MOUSE_MOTION){
				mouse_pos.x = event.motion.x;
				mouse_pos.y = event.motion.y;
			}
        }

		dir.x = (key_d ? 1.0f : 0.0f) - (key_a ? 1.0f : 0.0f);
		dir.y = (key_s ? 1.0f : 0.0f) - (key_w ? 1.0f : 0.0f);

		pos.x += dir.x * 100 * delta_time;
		pos.y += dir.y * 100 * delta_time;

		if(abs(dir.x) > abs(dir.y)){
			(dir.x <= 0) ? frame=6 : frame=9;
		}
		else{
			(dir.y <= 0) ? frame=3 : frame=0;
		}

		r.RenderBackground(BLACK);

		for(int x = 75; x < WIDTH-100; x+=60){
			for(int y = 75; y < HEIGHT-100; y+=60){
				r.RenderRect(x,y,50,50,GREY);
			}
		}
		tb.render(r);


		// println("{}x{}y",mouse_pos.x, mouse_pos.y);
		// println("{}x{}y",mp.rect.x, mp.rect.y);


		if(left_click ){
			if (!mp.is_dragging && PointInRect(mp.rect, click_pos)){
				mp.is_dragging = true;
				mp.is_grabbed = true;

				mp.drag_offset.x = mouse_pos.x - mp.rect.x;
				mp.drag_offset.y = mouse_pos.y - mp.rect.y;
			}
			if(mp.is_dragging){
				Vec2 offset = {mouse_pos.x - mp.drag_offset.x, mouse_pos.y - mp.drag_offset.y};
				mp.set_position(offset);
			}
		}
		else{
			mp.is_dragging = false;
			mp.is_grabbed = false;
		}
		mp.render(r);

		int scale = 2;
		Rect destRect = {pos.x, pos.y, 16.0f * scale, 18.0f * scale};
		r.RenderTextureTile(destRect, {3,4}, frame, "Green-Cap-Character-16x18.png");

		// destRect.x += 16;

		//Render Atlas By Tiles
		// int ii = 0;
		// for (int y = 0; y < 4; y++) {
		// 	for (int x = 0; x < 3; x++){
		// 		r.RenderTextureTile(destRect, {3,4}, ii, "Green-Cap-Character-16x18.png");
		// 		r.RenderDebugText(destRect.x, destRect.y, to_string(ii));
		// 		destRect.x += 16 * scale;
		// 		ii++;
		// 	}
		// 	destRect.x = pos.x;
		// 	destRect.y += 18 * scale;
		// }

		r.RenderDebugText(20,20,"Delta Time (ms): " + to_string(delta_time * 1000.0f));

		r.Present();
    }
    SDL_Quit();
    return 0;
}
