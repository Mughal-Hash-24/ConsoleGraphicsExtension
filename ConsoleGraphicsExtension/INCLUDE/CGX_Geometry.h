#pragma once
#include <cmath>

namespace CGX {

    // --- VECTOR 2 (Integer for Screen Coords) ---
    struct Vec2 {
        int x, y;

        Vec2() : x(0), y(0) {}
        Vec2(int _x, int _y) : x(_x), y(_y) {}

        // Operator Overloads for easy math
        Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
        Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
        Vec2 operator*(float scalar) const { return Vec2((int)(x * scalar), (int)(y * scalar)); }

        bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }

        float magnitude() const { return std::sqrt(x * x + y * y); }

        // Distance static helper
        static float dist(Vec2 a, Vec2 b) {
            return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
        }
    };

    // --- RECTANGLE (For UI and Collision) ---
    struct Rect {
        int x, y, w, h;

        // FIXED: Added Default Constructor
        Rect() : x(0), y(0), w(0), h(0) {}

        Rect(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}

        bool contains(Vec2 p) const {
            return (p.x >= x && p.x < x + w && p.y >= y && p.y < y + h);
        }

        bool intersects(const Rect& other) const {
            return (x < other.x + other.w && x + w > other.x &&
                y < other.y + other.h && y + h > other.y);
        }
    };

    // --- GEOMETRY FUNCTIONS ---
    void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b);
    void drawRect(Rect r, int red, int green, int blue, bool filled);
}