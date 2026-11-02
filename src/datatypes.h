#ifndef DATATYPES_H
#define DATATYPES_H

struct Vec3 {
	float x,y,z;
};

struct Vec3i {
	int x,y,z;
};

struct Vec2 {
	float x,y;

    Vec2 operator-(const Vec2& other) const {
        return Vec2{this->x - other.x, this->y - other.y};
    }
};

struct Vec2i {
	int x,y;
};

struct Grid{
	int columns, rows;
};

struct Rect {
	float x, y, w, h;
};

struct Color {
	int r, g, b, a;
};

inline constexpr Color BLACK       = {  0,   0,   0, 255};
inline constexpr Color GREY        = {127, 127, 127, 255};
inline constexpr Color WHITE       = {255, 255, 255, 255};
inline constexpr Color CLEAR       = {  0,   0,   0,   0};
inline constexpr Color RED         = {255,   0,   0, 255};
inline constexpr Color GREEN       = {  0, 255,   0, 255};
inline constexpr Color BLUE        = {  0,   0, 255, 255};
inline constexpr Color YELLOW      = {255, 255,   0, 255};
inline constexpr Color CYAN        = {  0, 255, 255, 255};
inline constexpr Color MAGENTA     = {255,   0, 255, 255};

inline constexpr Color DARK_GREY   = { 45,  45,  45, 255};
inline constexpr Color LIGHT_GREY  = {200, 200, 200, 200};
inline constexpr Color ORANGE      = {255, 140,   0, 255}; 
inline constexpr Color PURPLE      = {128,   0, 128, 255};
inline constexpr Color PINK        = {255, 192, 203, 255};
inline constexpr Color BROWN       = {139,  69,  19, 255};

inline bool PointInRect(Rect r, Vec2 p) {
    return (p.x >= r.x && p.x < (r.x + r.w) &&
            p.y >= r.y && p.y < (r.y + r.h));
}

#endif
