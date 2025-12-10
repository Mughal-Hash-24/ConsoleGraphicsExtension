#pragma once
#include <string>
#include "CGX_Core.h"
#include "CGX_Geometry.h"

namespace CGX {
    class Text {
    private:
        std::string content;
        int x, y;
        int r, g, b;
        int scale; // 1 = 5x8, 2 = 10x16, etc.
        int fontType; // 0 = Standard, 1 = Cyberpunk

    public:
        // Default constructor
        Text();

        // Convenience constructor
        Text(const std::string& str, int x, int y);

        // Setters
        void setString(const std::string& str);
        void setPosition(int x, int y);
        void setColor(int r, int g, int b);
        void setScale(int s);
        void setFont(int type); // 0 = Standard, 1 = Cyberpunk

        // Getters
        std::string getString() const;
        int getX() const;
        int getY() const;

        // Render to window
        void draw() const;

        Rect getGlobalBounds() const;
    };
}