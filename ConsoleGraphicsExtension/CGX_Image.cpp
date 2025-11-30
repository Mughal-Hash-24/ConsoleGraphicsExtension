#include "CGX_Image.h"
#include "CGX_Window.h"

namespace CGX {

    Image::Image(int w, int h) : width(w), height(h) {
        pixels = new Pixel[w * h];
        for (int i = 0; i < w * h; ++i) {
            pixels[i] = { 0, 0, 0 };
        }
    }

    Image::~Image() {
        if (pixels) delete[] pixels;
    }

    void Image::setPixel(int x, int y, int r, int g, int b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = { r, g, b };
        }
    }

    Pixel Image::getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y * width + x];
        }
        return { 0, 0, 0 };
    }

    int Image::getWidth() const { return width; }
    int Image::getHeight() const { return height; }

    void Image::draw(int startX, int startY) const {
        Window* win = Window::getInstance();
        if (!win) return;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = pixels[y * width + x];
                // Check for basic transparency (Magic Pink)
                if (p.r == 255 && p.g == 0 && p.b == 255) continue;

                win->drawPoint(startX + x, startY + y, "  ", p.r, p.g, p.b);
            }
        }
    }
}