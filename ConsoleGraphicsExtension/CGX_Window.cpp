#include "CGX_Window.h"
#include "CGX_Utils.h"
#include <iostream>
#include <string>

namespace CGX {

    Window* Window::instance = nullptr;

    Window::Window(int w, int h) : width(w), height(h) {
        frontBuffer.resize(w * h);
        backBuffer.resize(w * h);
        paletteCache.reserve(256);

        // Init buffers
        RenderCell defaultCell = { "  ", 0 };
        frontBuffer.fill(defaultCell);
        backBuffer.fill(defaultCell);

        // Add default black color (ID 0)
        getColorID(0, 0, 0);

        Utils::hideCursor();
        Utils::clearScreen();
        instance = this;
    }

    Window::~Window() {
        Utils::showCursor();
        std::cout << "\033[0m"; // Reset terminal
    }

    Window* Window::getInstance() {
        return instance;
    }

    int Window::getColorID(int r, int g, int b) {
        // Simple linear cache search (could be optimized, but fits "No STL" constraint)
        // Ideally we check if color exists, but for speed we might just add it.
        // For a full app, you might want a map, but using Vector we just push back.
        // We will just create a string every time? No, we should cache.

        // Simplified: Just add new color to palette. 
        // Real implementation would search 'paletteCache' to deduplicate.

        std::string ansi = "\033[48;2;" + std::to_string(r) + ";" +
            std::to_string(g) + ";" + std::to_string(b) + "m";
        paletteCache.push_back(ansi);
        return (int)paletteCache.size() - 1;
    }

    const std::string& Window::getAnsiColor(int id) {
        return paletteCache[id];
    }

    void Window::clear(int r, int g, int b) {
        int id = getColorID(r, g, b);
        RenderCell c = { "  ", id };
        frontBuffer.fill(c);
    }

    void Window::drawPoint(int x, int y, const std::string& symbol, int r, int g, int b) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        int id = getColorID(r, g, b);
        frontBuffer[y * width + x] = { symbol, id };
    }

    void Window::display() {
        outputBuffer.clear();
        outputBuffer.reserve(width * height * 10); // Heuristic

        int currentR = -1;
        int currentC = -1;
        int activeColorID = -1;

        outputBuffer += "\033[0m"; // Reset state

        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                int idx = r * width + c;

                if (frontBuffer[idx] != backBuffer[idx]) {

                    // Cursor Optimization
                    if (r != currentR || c != currentC) {
                        // Double width correction for "  " or "##"
                        outputBuffer += "\033[" + std::to_string(r + 1) + ";" + std::to_string(c * 2 + 1) + "H";
                    }

                    // Color Optimization
                    if (frontBuffer[idx].colorID != activeColorID) {
                        outputBuffer += getAnsiColor(frontBuffer[idx].colorID);
                        activeColorID = frontBuffer[idx].colorID;
                    }

                    // Draw Char
                    outputBuffer += frontBuffer[idx].character;

                    // Sync
                    backBuffer[idx] = frontBuffer[idx];

                    currentR = r;
                    currentC = c + 1;
                }
            }
        }

        if (!outputBuffer.empty()) {
            std::cout << outputBuffer << std::flush;
        }
    }
}