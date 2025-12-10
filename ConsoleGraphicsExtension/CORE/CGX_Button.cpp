#include "../INCLUDE/CGX_Button.h"
#include "../INCLUDE/CGX_Geometry.h"
#include "../INCLUDE/CGX_Utils.h"

namespace CGX {
    Button::Button(int x, int y, int w, int h, std::string text) {
        bounds = { x, y, w, h };
        label = Text(text, x, y);

        // Center text initially
        Rect tBounds = label.getGlobalBounds();
        int tx = x + (w - tBounds.w) / 2;
        int ty = y + (h - tBounds.h) / 2;
        label.setPosition(tx, ty);

        // Default Colors (Gray theme)
        idleR = 100; idleG = 100; idleB = 100;
        hoverR = 150; hoverG = 150; hoverB = 150;
        clickR = 50;  clickG = 50;  clickB = 50;

        // Default Border
        borderThickness = 0;
        borderR = 200; borderG = 200; borderB = 200;

        // Default Padding
        padding = 5;

        isHovered = false;
        isClicked = false;
    }

    void Button::setIdleColor(int r, int g, int b) { idleR = r; idleG = g; idleB = b; }
    void Button::setHoverColor(int r, int g, int b) { hoverR = r; hoverG = g; hoverB = b; }
    void Button::setClickColor(int r, int g, int b) { clickR = r; clickG = g; clickB = b; }

    void Button::setTextColor(int r, int g, int b) { label.setColor(r, g, b); }

    void Button::setScale(int s) {
        label.setScale(s);
        // Re-center
        Rect tBounds = label.getGlobalBounds();
        int tx = bounds.x + (bounds.w - tBounds.w) / 2;
        int ty = bounds.y + (bounds.h - tBounds.h) / 2;
        label.setPosition(tx, ty);
    }

    void Button::setString(std::string text) {
        label.setString(text);
        // Re-center whenever text changes
        Rect tBounds = label.getGlobalBounds();
        int tx = bounds.x + (bounds.w - tBounds.w) / 2;
        int ty = bounds.y + (bounds.h - tBounds.h) / 2;
        label.setPosition(tx, ty);
    }

    // --- Added Missing Functions ---
    void Button::setBorder(int thickness, int r, int g, int b) {
        borderThickness = thickness;
        borderR = r;
        borderG = g;
        borderB = b;
    }

    void Button::setPadding(int p) {
        padding = p;
        // Padding generally affects text area, but since we center text, 
        // we might not need to recalculate position unless we supported alignment.
    }
    // -------------------------------

    void Button::update() {
        int mx, my;
        Utils::getMousePos(mx, my);

        isHovered = (mx >= bounds.x && mx <= bounds.x + bounds.w &&
            my >= bounds.y && my <= bounds.y + bounds.h);

        if (isHovered && Utils::isMousePressed()) {
            isClicked = true;
        }
        else {
            isClicked = false;
        }
    }

    void Button::draw() {
        // Background
        if (isClicked) CGX::drawRect(bounds, clickR, clickG, clickB, true);
        else if (isHovered) CGX::drawRect(bounds, hoverR, hoverG, hoverB, true);
        else CGX::drawRect(bounds, idleR, idleG, idleB, true);

        // Border
        if (borderThickness > 0) {
            // Draw border rect(s) based on thickness
            for (int i = 0; i < borderThickness; i++) {
                Rect r = { bounds.x + i, bounds.y + i, bounds.w - i * 2, bounds.h - i * 2 };
                CGX::drawRect(r, borderR, borderG, borderB, false);
            }
        }

        // Text
        label.draw();
    }

    bool Button::isPressed() const {
        return isClicked;
    }

    bool Button::Hovered() const {
        return isHovered;
    }

    Text Button::getLabel() const { return label; }
    Rect Button::getGlobalBounds() const { return bounds; }
}