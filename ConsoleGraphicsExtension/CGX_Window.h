#pragma once
#include "CGX_Core.h"
#include "CGX_Vector.h"
#include <string>

using namespace std;

namespace CGX {

    struct RenderCell {
        string character; 
        int colorID;

        bool operator!=(const RenderCell& other) const {
            return colorID != other.colorID || character != other.character;
        }
    };

    class Window {
    private:
        Vector<RenderCell> frontBuffer;
        Vector<RenderCell> backBuffer;
        Vector<std::string> paletteCache;
        string outputBuffer;

        int width, height;

        // Private helpers
        int getColorID(int r, int g, int b);
        const std::string& getAnsiColor(int id);

        // Singleton instance for global access by Images
        static Window* instance;

    public:
        Window(int w, int h);
        ~Window();

        static Window* getInstance();

        void clear(int r = 0, int g = 0, int b = 0);
        void display();

        // Low level draw used by Image/Rect classes
        void drawPoint(int x, int y, const string& symbol, int r, int g, int b);
    };
}