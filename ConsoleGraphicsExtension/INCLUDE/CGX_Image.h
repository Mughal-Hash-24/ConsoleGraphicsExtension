#pragma once
#include "CGX_Core.h"
#include "CGX_Vector.h"
#include <string> // Added for filename string

namespace CGX {
    class Image {
    private:
        // OPTIMIZATION: Use Vector instead of raw pointers
        Vector<Pixel> pixels;
        int width;
        int height;

    public:
        Image(); // Default constructor for Vector safety
        Image(int w, int h);
        Image(const std::string& textFilePath); // New constructor for loading from .txt
        ~Image();

        // In class Image public section:
        void setPixel(int x, int y, int r, int g, int b, int a = 255);
        Pixel getPixel(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        void draw(int startX, int startY) const;
        void drawFlipped(int startX, int startY) const;  // Horizontally flipped
        void drawScaled(int startX, int startY, int scale) const;  // Scaled draw
        void drawHalfScale(int startX, int startY) const;  // Half-size draw
    };
}