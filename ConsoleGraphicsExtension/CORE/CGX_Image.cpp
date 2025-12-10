#include "../INCLUDE/CGX_Image.h"
#include "../INCLUDE/CGX_Window.h"
#include <fstream> // Added for file input
#include <iostream>

using namespace std;

namespace CGX {

    Image::Image() : width(0), height(0) {}

    Image::Image(int w, int h) : width(w), height(h) {
    pixels.resize(w * h);
    
    // OLD: Pixel black = { 0, 0, 0 }; 
    // NEW: Use the constructor
    Pixel black(0, 0, 0, 255); 
    
    pixels.fill(black);
}

    // Implementation of the new text-file loading constructor
    Image::Image(const std::string& textFilePath) : width(0), height(0) {
        std::ifstream file(textFilePath);

        // 1. Check if file is open
        if (!file.is_open()) {
            std::cout << textFilePath << endl;
            return;
        }
        else {
            cout << "LOADED" << endl;
        }

        // 2. Read the Header (Width and Height)
        if (file >> width >> height) {

            // 3. Resize pixel storage
            pixels.resize(width * height);

            // 4. Read the single row of RGB data
            int r, g, b, a; // Keep these as plain ints for reading from file
            for (int i = 0; i < width * height; ++i) {
                if (file >> r >> g >> b >> a) {
                    // The new Pixel constructor handles the casting to uint8_t automatically
                    pixels[i] = Pixel(r, g, b, a);
                }
                else {
                    pixels[i] = Pixel(0, 0, 0, 255);
                }
            }
        }
        else {
            // Failed to read header
            width = 0;
            height = 0;
        }
    }

    Image::~Image() {
        // Vector destructor handles memory automatically
    }

    void Image::setPixel(int x, int y, int r, int g, int b, int a) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            // New constructor handles the packing
            pixels[y * width + x] = Pixel(r, g, b, a);
        }
    }

    Pixel Image::getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y * width + x];
        }
        return { 1, 1, 1 };
    }

    int Image::getWidth() const { return width; }
    int Image::getHeight() const { return height; }

    void Image::draw(int startX, int startY) const {
        Window* win = Window::getInstance();
        if (!win) return;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = pixels[y * width + x];

                // NEW: Check Alpha instead of Magic Pink
                if (p.a == 0) continue;

                // Pass alpha to drawPixel (see step 4)
                win->drawPixel(startX + x, startY + y, p.r, p.g, p.b, p.a);
            }
        }
    }

    void Image::drawFlipped(int startX, int startY) const {
        Window* win = Window::getInstance();
        if (!win) return;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = pixels[y * width + x];

                // Check Alpha
                if (p.a == 0) continue;

                // Draw in reverse x order (flipped horizontally)
                win->drawPixel(startX + (width - 1 - x), startY + y, p.r, p.g, p.b, p.a);
            }
        }
    }

    void Image::drawScaled(int startX, int startY, int scale) const {
        Window* win = Window::getInstance();
        if (!win || scale < 1) return;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = pixels[y * width + x];
                if (p.a == 0) continue;

                // Draw scaled pixel (scale x scale block)
                for (int sy = 0; sy < scale; ++sy) {
                    for (int sx = 0; sx < scale; ++sx) {
                        win->drawPixel(startX + x * scale + sx, startY + y * scale + sy, p.r, p.g, p.b, p.a);
                    }
                }
            }
        }
    }

    void Image::drawHalfScale(int startX, int startY) const {
        Window* win = Window::getInstance();
        if (!win) return;

        // Draw at half size by sampling every other pixel
        for (int y = 0; y < height; y += 2) {
            for (int x = 0; x < width; x += 2) {
                Pixel p = pixels[y * width + x];
                if (p.a == 0) continue;
                win->drawPixel(startX + x / 2, startY + y / 2, p.r, p.g, p.b, p.a);
            }
        }
    }
}