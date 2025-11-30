#pragma once

namespace CGX {
    namespace Utils {
        void sleep_ms(int milliseconds);
        void gotoXY(int x, int y);
        void hideCursor();
        void showCursor();
        void clearScreen();

        // FPS Control
        void setFrameRate(int fps);
        int getFrameDelayMs();
        void sleepForFrame();
    }
}