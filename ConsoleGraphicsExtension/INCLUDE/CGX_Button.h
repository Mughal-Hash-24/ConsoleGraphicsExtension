#pragma once
#include <string>
#include "CGX_Geometry.h"
#include "CGX_Text.h"
#include "CGX_Utils.h"

namespace CGX {
    class Button {
    private:
        Rect bounds;
        Text label;

        // Colors
        int idleR, idleG, idleB;
        int hoverR, hoverG, hoverB;
        int clickR, clickG, clickB;
        int borderR, borderG, borderB;

        int padding;
        int borderThickness;
        bool isHovered;
        bool isClicked;

    public:
        // Constructor
        Button(int x, int y, int w, int h, std::string text);

        // Styling
        void setIdleColor(int r, int g, int b);
        void setHoverColor(int r, int g, int b);
        void setClickColor(int r, int g, int b);
        void setTextColor(int r, int g, int b);
        void setScale(int s);

        // NEW: Allow updating text dynamically
        void setString(std::string text);

        // Logic
        void update();
        void draw();

        // State Check
        bool isPressed() const;
        bool Hovered() const;

        Text getLabel() const;
        Rect getGlobalBounds() const;

        void setBorder(int thickness, int r, int g, int b);
        void setPadding(int p);
    };
}