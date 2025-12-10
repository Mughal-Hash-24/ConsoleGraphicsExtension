#pragma once
#include "CGX_Core.h"
#include <windows.h>
#include <string>
#include <cstdint> // Required for uint32_t

namespace CGX {

    class Window {
    private:
        HANDLE hConsole;

        // --- BUFFERS ---
        uint32_t* backBuffer;   // Next Frame (Colors)
        uint32_t* frontBuffer;  // Current Screen State (Colors)

        // --- RAW OUTPUT MEMORY ---
        char* outputBuffer;     // The raw pile of bytes we send to Windows
        size_t outputCapacity;  // Size of the allocated block

        int width, height;
        static Window* instance;

    public:
        Window(int w, int h);
        ~Window();

        static Window* getInstance();

        void clear(int r = 0, int g = 0, int b = 0);
        void display();

        // Optimized to set colors in the buffer
        void drawPixel(int x, int y, int r, int g, int b, int a = 255);

        // Compatibility wrapper
        void drawPoint(int x, int y, const std::string& symbol, int r, int g, int b);
    };
}