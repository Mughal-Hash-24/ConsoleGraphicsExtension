#include "../INCLUDE/CGX_Geometry.h"
#include "../INCLUDE/CGX_Window.h"
#include <cstdlib> // abs

namespace CGX {

    // Bresenham's Line Algorithm
    void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b) {
        Window* win = Window::getInstance();
        if (!win) return;

        int dx = std::abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -std::abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;
        int e2;

        while (true) {
            // Draw a small dot for the line segment
            win->drawPoint(x0, y0, "  ", r, g, b);

            if (x0 == x1 && y0 == y1) break;

            e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    void drawRect(Rect rect, int r, int g, int b, bool filled) {
        Window* win = Window::getInstance();
        if (!win) return;

        if (filled) {
            for (int i = 0; i < rect.h; i++) {
                for (int j = 0; j < rect.w; j++) {
                    win->drawPoint(rect.x + j, rect.y + i, "  ", r, g, b);
                }
            }
        }
        else {
            // Top & Bottom
            for (int i = 0; i < rect.w; i++) {
                win->drawPoint(rect.x + i, rect.y, "  ", r, g, b);
                win->drawPoint(rect.x + i, rect.y + rect.h - 1, "  ", r, g, b);
            }
            // Left & Right
            for (int i = 0; i < rect.h; i++) {
                win->drawPoint(rect.x, rect.y + i, "  ", r, g, b);
                win->drawPoint(rect.x + rect.w - 1, rect.y + i, "  ", r, g, b);
            }
        }
    }
}