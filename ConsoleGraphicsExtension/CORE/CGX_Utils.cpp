#include "../INCLUDE/CGX_Utils.h"
#include <iostream>
#include <windows.h> 
#include <chrono> 
#include <thread>
#include <cmath>
#include <vector> // Added for buffering keys

using namespace std;

namespace CGX {
    namespace Utils {

        static int targetFPS = 30;
        static std::chrono::steady_clock::time_point lastTime;

        void sleep_ms(int milliseconds) {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }

        void gotoXY(int x, int y) {
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
            lastTime = std::chrono::steady_clock::now();
        }

        int getFrameDelayMs() {
            if (targetFPS <= 0) return 0;
            return 1000 / targetFPS;
        }

        void sleepForFrame() {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();
            int wait = getFrameDelayMs() - (int)duration;

            if (wait > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(wait));
            }
            lastTime = std::chrono::steady_clock::now();
        }

        // UPDATED: Native Event Method for Mouse Position
        // This reads directly from the Windows Input Buffer, avoiding all font/zoom issues.
        void getMousePos(int& x, int& y) {
            static bool initialized = false;
            static int cachedX = 0;
            static int cachedY = 0;

            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

            // 1. One-time Setup: Enable Mouse Input
            if (!initialized) {
                DWORD prevMode;
                GetConsoleMode(hIn, &prevMode);
                SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
                initialized = true;
            }

            // 2. Check for pending events
            DWORD numEvents = 0;
            GetNumberOfConsoleInputEvents(hIn, &numEvents);

            if (numEvents > 0) {
                const int BUFFER_SIZE = 128;
                INPUT_RECORD ir[BUFFER_SIZE];
                DWORD readCount;

                // 3. Read events (This removes them from the buffer!)
                if (ReadConsoleInput(hIn, ir, BUFFER_SIZE, &readCount)) {
                    std::vector<INPUT_RECORD> keysToReturn;

                    for (DWORD i = 0; i < readCount; i++) {
                        if (ir[i].EventType == MOUSE_EVENT) {
                            // Update our cached position
                            cachedX = ir[i].Event.MouseEvent.dwMousePosition.X;
                            cachedY = ir[i].Event.MouseEvent.dwMousePosition.Y;
                        }
                        else {
                            // SAFETY: Determine if this is a key event we shouldn't have eaten.
                            // If we don't put these back, _kbhit() and cin will stop working.
                            keysToReturn.push_back(ir[i]);
                        }
                    }

                    // 4. Return non-mouse events to the buffer
                    if (!keysToReturn.empty()) {
                        DWORD written;
                        WriteConsoleInput(hIn, keysToReturn.data(), (DWORD)keysToReturn.size(), &written);
                    }
                }
            }

            // 5. Apply Logic scaling (2 chars = 1 logical pixel)
            x = cachedX / 2;
            y = cachedY;
        }

        bool isMousePressed() {
            // Check if Left Mouse Button (VK_LBUTTON) is currently down
            // 0x8000 checks the high-order bit (key is down)
            return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        }
    }
}