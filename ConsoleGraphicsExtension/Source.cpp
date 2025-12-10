#include "INCLUDE/CGX_Window.h"
#include "INCLUDE/CGX_Geometry.h"
#include "INCLUDE/CGX_Utils.h"
#include "INCLUDE/CGX_Text.h"
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace CGX;

// --- Benchmark Configuration ---
const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 450;
const int OBJECT_COUNT = 500;

// --- Simple Shape Structure for Benchmark ---
struct BenchShape {
    float x, y;
    float dx, dy;
    int w, h;
    int r, g, b;
    int type; // 0 = Filled Rect, 1 = Outline Rect, 2 = Pixel/Point
    bool colorCycler;
};

int main() {
    // 1. Initialize Window (and buffers)
    // Note: Ensure your console font size is small enough (e.g., Raster 4x6 or Consolas 5)
    // to actually fit 900x450 characters on your monitor!

    _getch();

    Window* window = new Window(WIN_WIDTH, WIN_HEIGHT);

    // 2. Setup FPS Text
    // Positioned at Top-Right (Width - approx text width, 1)
    Text fpsText("FPS: 00", WIN_WIDTH-200, 1);
    fpsText.setColor(0, 255, 0); // Green
    fpsText.setScale(2);         // Large text

    // 3. Initialize Shapes
    srand(time(0));
    vector<BenchShape> shapes;
    shapes.reserve(OBJECT_COUNT);

    for (int i = 0; i < OBJECT_COUNT; i++) {
        BenchShape s;
        s.x = rand() % WIN_WIDTH;
        s.y = rand() % WIN_HEIGHT;

        // Random velocity (-2 to 2)
        s.dx = (rand() % 40 - 20) / 10.0f;
        s.dy = (rand() % 40 - 20) / 10.0f;
        if (s.dx == 0) s.dx = 1;
        if (s.dy == 0) s.dy = 1;

        // Random Size (Small for performance in high quantity)
        s.w = rand() % 10 + 2;
        s.h = rand() % 10 + 2;

        // Random Colors
        s.r = rand() % 255;
        s.g = rand() % 255;
        s.b = rand() % 255;

        // Types: 50% Pixels, 25% Filled Rects, 25% Outlines
        int roll = rand() % 100;
        if (roll < 50) s.type = 2;       // Pixel
        else if (roll < 75) s.type = 0;  // Filled
        else s.type = 1;                 // Outline

        // 10% of shapes change color dynamically
        s.colorCycler = (rand() % 100) < 10;

        shapes.push_back(s);
    }

    // 4. Time Management
    auto lastTime = chrono::high_resolution_clock::now();
    int frameCount = 0;
    float timer = 0.0f;

    // --- MAIN LOOP ---
    while (true) {
        // A. Clear Screen (Black background)
        window->clear(0, 0, 0);

        // B. Calculate Delta Time
        auto currentTime = chrono::high_resolution_clock::now();
        chrono::duration<float> diff = currentTime - lastTime;
        float deltaTime = diff.count();
        lastTime = currentTime;

        // C. Update Shapes
        for (auto& s : shapes) {
            // Move
            s.x += s.dx;
            s.y += s.dy;

            // Bounce Logic
            if (s.x <= 0 || s.x + s.w >= WIN_WIDTH) s.dx *= -1;
            if (s.y <= 0 || s.y + s.h >= WIN_HEIGHT) s.dy *= -1;

            // Clamp (prevent getting stuck outside)
            if (s.x < 0) s.x = 0;
            if (s.y < 0) s.y = 0;

            // Color Cycling
            if (s.colorCycler) {
                s.r = (s.r + 5) % 255;
                s.b = (s.b + 3) % 255;
            }

            // Draw
            Rect r = { (int)s.x, (int)s.y, s.w, s.h };
            if (s.type == 0) {
                CGX::drawRect(r, s.r, s.g, s.b, true); // Filled
            }
            else if (s.type == 1) {
                CGX::drawRect(r, s.r, s.g, s.b, false); // Outline
            }
            else {
                window->drawPixel((int)s.x, (int)s.y, s.r, s.g, s.b); // Single Pixel
            }
        }

        // D. FPS Calculation (Every 1 second)
        timer += deltaTime;
        frameCount++;
        if (timer >= 1.0f) {
            string fpsStr = "FPS: " + to_string(frameCount);
            fpsText.setString(fpsStr);

            // Debug print to console title as well (in case rendering is too slow to read)
            string titleCmd = "title CGX Benchmark - " + fpsStr;
            system(titleCmd.c_str());

            frameCount = 0;
            timer = 0;
        }

        // E. Draw FPS UI
        fpsText.draw();

        // F. Display Frame
        window->display();
    }

    delete window;
    return 0;
}