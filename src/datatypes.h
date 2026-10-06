#ifndef DATATYPES_H
#define DATATYPES_H

struct Vec3 {
	float x,y,z;
};

struct Vec2 {
	float x,y;
};

struct Rect {
	float x, y, w, h;
};

struct Color {
	int r, g, b, a;
};
inline constexpr Color BLACK = {  0,   0,   0, 255};
inline constexpr Color GREY  = {127, 127, 127, 255};
inline constexpr Color WHITE = {255, 255, 255, 255};

#endif
