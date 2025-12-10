#include "../INCLUDE/CGX_Window.h"
#include "../INCLUDE/CGX_Utils.h"
#include <iostream>
#include <algorithm>

namespace CGX {

    Window* Window::instance = nullptr;

    // --- High-Performance Pointer Helpers ---
    // Writes int to buffer and advances pointer
    inline void fastInt(char*& dst, int val) {
        if (val == 0) { *dst++ = '0'; return; }

        // Optimized for 0-255 RGB values (max 3 chars)
        // or screen coords (max 4 chars usually)
        char temp[5];
        int p = 0;
        while (val > 0) {
            temp[p++] = (val % 10) + '0';
            val /= 10;
        }
        while (p > 0) {
            *dst++ = temp[--p];
        }
    }

    // Copies string literal to buffer and advances pointer
    inline void fastStr(char*& dst, const char* src) {
        while (*src) *dst++ = *src++;
    }

    Window::Window(int w, int h) : width(w), height(h) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        DWORD dwMode = 0;
        GetConsoleMode(hConsole, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, dwMode);

        // 1. Allocate Color Buffers
        backBuffer = new uint32_t[width * height];
        frontBuffer = new uint32_t[width * height];

        // 2. Allocate Raw Output Buffer
        // Calculation: Width * Height * (Max Bytes Per Pixel)
        // Max Bytes = ~35 (Cursor Move + Color Change + 2 Spaces)
        // 32MB is plenty safe for 900x450
        outputCapacity = 32 * 1024 * 1024;
        outputBuffer = new char[outputCapacity];

        // Init Buffers
        uint32_t black = 0; // 0x00000000
        std::fill(backBuffer, backBuffer + (width * height), black);
        std::fill(frontBuffer, frontBuffer + (width * height), black);

        Utils::hideCursor();
        Utils::clearScreen();
        instance = this;
    }

    Window::~Window() {
        delete[] backBuffer;
        delete[] frontBuffer;
        delete[] outputBuffer; // Clean up raw memory
        std::cout << "\033[0m";
    }

    Window* Window::getInstance() { return instance; }

    void Window::clear(int r, int g, int b) {
        uint32_t val = (r << 16) | (g << 8) | b;
        std::fill(backBuffer, backBuffer + (width * height), val);
    }

    void Window::drawPixel(int x, int y, int r, int g, int b, int a) {
        // Bounds check
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        if (a < 10) return;

        backBuffer[y * width + x] = (r << 16) | (g << 8) | b;
    }

    void Window::drawPoint(int x, int y, const std::string& symbol, int r, int g, int b) {
        // We ignore the symbol to enforce 1 Pixel = 2 Spaces aspect ratio
        drawPixel(x, y, r, g, b);
    }

    void Window::display() {
        // Pointer to the start of our raw buffer
        char* ptr = outputBuffer;

        uint32_t lastColor = 0xFFFFFFFF; // Impossible color
        int totalPixels = width * height;
        int idx = 0;

        while (idx < totalPixels) {

            // 1. Skip unchanged pixels
            if (backBuffer[idx] == frontBuffer[idx]) {
                idx++;
                continue;
            }

            // 2. Identify the Run (Sequence of changes)
            int runStart = idx;
            int runEnd = idx;

            // Gap Bridging: Skip over small gaps of unchanged pixels 
            // to avoid expensive cursor jumps.
            int gap = 0;
            int search = idx + 1;
            while (search < totalPixels) {
                if (backBuffer[search] != frontBuffer[search]) {
                    runEnd = search;
                    gap = 0;
                }
                else {
                    gap++;
                    // Lookahead limit: 4 pixels. 
                    // Logic: 4 pixels * 2 chars = 8 bytes.
                    // Cursor jump sequence = ~8-10 bytes.
                    // If gap < 5, it's cheaper to overwrite than to jump.
                    if (gap > 4) break;
                }
                search++;
            }

            // 3. Move Cursor (Hard Jump)
            int r = runStart / width;
            int c = runStart % width;

            // ASPECT RATIO FIX: 
            // Logical X=0 -> Console Col 1
            // Logical X=1 -> Console Col 3
            // Formula: (c * 2) + 1
            int consoleCol = (c * 2) + 1;

            // ANSI: \033[Row;ColH
            fastStr(ptr, "\033[");
            fastInt(ptr, r + 1);
            *ptr++ = ';';
            fastInt(ptr, consoleCol);
            *ptr++ = 'H';

            // 4. Render the Run
            for (int k = runStart; k <= runEnd; ++k) {
                uint32_t col = backBuffer[k];

                // Check Color Change
                if (col != lastColor) {
                    int rr = (col >> 16) & 0xFF;
                    int gg = (col >> 8) & 0xFF;
                    int bb = col & 0xFF;

                    // ANSI Background: \033[48;2;R;G;Bm
                    fastStr(ptr, "\033[48;2;");
                    fastInt(ptr, rr);
                    *ptr++ = ';';
                    fastInt(ptr, gg);
                    *ptr++ = ';';
                    fastInt(ptr, bb);
                    *ptr++ = 'm';

                    lastColor = col;
                }

                // ASPECT RATIO FIX: 1 Pixel = 2 Spaces
                *ptr++ = ' ';
                *ptr++ = ' ';

                // Sync Front Buffer
                frontBuffer[k] = col;
            }

            // Advance
            idx = runEnd + 1;
        }

        // 5. System Call
        // Calculate exact bytes used
        DWORD bytesToWrite = (DWORD)(ptr - outputBuffer);

        if (bytesToWrite > 0) {
            DWORD written;
            WriteConsoleA(hConsole, outputBuffer, bytesToWrite, &written, NULL);
        }
    }
}