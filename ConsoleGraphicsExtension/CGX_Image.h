#pragma once
#include "CGX_Core.h"

namespace CGX {
    class Image {
    private:
        Pixel* pixels;
        int width;
        int height;

    public:
        Image(int w, int h);
        ~Image();

        void setPixel(int x, int y, int r, int g, int b);
        Pixel getPixel(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        void draw(int startX, int startY) const;
    };
}