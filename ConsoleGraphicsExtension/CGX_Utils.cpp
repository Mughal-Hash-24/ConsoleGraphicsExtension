#include "CGX_Utils.h"
#include <iostream>
#include <windows.h> 

using namespace std;

namespace CGX {
    namespace Utils {

        static int targetFPS = 30;
        static unsigned long lastTime = 0;

        void sleep_ms(int milliseconds) {
            Sleep(milliseconds);
        }

        void gotoXY(int x, int y) {
            // ANSI Escape code for positioning
            cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
        }

        void hideCursor() {
            cout << "\033[?25l";
        }

        void showCursor() {
            cout << "\033[?25h";
        }

        void clearScreen() {
            cout << "\033[2J";
        }

        void setFrameRate(int fps) {
            targetFPS = fps;
            lastTime = (unsigned long)GetTickCount();
        }

        int getFrameDelayMs() {
            if (targetFPS <= 0) return 0;
            return 1000 / targetFPS;
        }

        void sleepForFrame() {
            unsigned long now = (unsigned long)GetTickCount();

            // Calculate elapsed time
            int elapsed_ms = (int)(now - lastTime);

            int wait = getFrameDelayMs() - elapsed_ms;
            if (wait > 0) {
                sleep_ms(wait);
            }
            // Update lastTime to current time after sleep
            lastTime = (unsigned long)GetTickCount();
        }
    }
}