#include "CGX_Window.h"
#include "CGX_Utils.h"
#include "CGX_Image.h"
#include <conio.h> 
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace CGX;

// --- GAME CONSTANTS ---
const int WIN_WIDTH = 200;
const int WIN_HEIGHT = 150;

// Game Settings
const int PADDLE_WIDTH = 36;
const int PADDLE_HEIGHT = 4;
const int PADDLE_SPEED = 6;
const int BALL_SIZE = 4;
const float BALL_SPEED_BASE = 3.0f;

// Brick Layout
const int BRICK_ROWS = 6;
const int BRICK_COLS = 10;
const int BRICK_GAP = 2;
const int BRICK_WIDTH = (WIN_WIDTH - (BRICK_COLS + 1) * BRICK_GAP) / BRICK_COLS;
const int BRICK_HEIGHT = 6;
const int BRICK_START_Y = 20;

// --- HELPER: AABB COLLISION ---
bool checkCollision(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2) {
    return x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2;
}

// --- GAME OBJECTS ---

struct Ball {
    float x, y;
    float vx, vy;
    Image* sprite;
    bool active;

    Ball() {
        sprite = new Image(BALL_SIZE, BALL_SIZE);
        for (int i = 0; i < BALL_SIZE * BALL_SIZE; i++)
            sprite->setPixel(i % BALL_SIZE, i / BALL_SIZE, 255, 255, 255);
        reset();
    }

    ~Ball() { delete sprite; }

    void reset() {
        x = WIN_WIDTH / 2.0f;
        y = WIN_HEIGHT - 40.0f;
        vx = BALL_SPEED_BASE * (rand() % 2 == 0 ? 1 : -1);
        vy = -BALL_SPEED_BASE;
        active = false; // Waits for player to launch
    }

    void launch() {
        active = true;
    }

    void update() {
        if (!active) return;
        x += vx;
        y += vy;

        // Wall Collisions
        if (x <= 0) { x = 0; vx *= -1; }
        if (x + BALL_SIZE >= WIN_WIDTH) { x = WIN_WIDTH - BALL_SIZE; vx *= -1; }
        if (y <= 0) { y = 0; vy *= -1; }
    }

    void draw() {
        sprite->draw((int)x, (int)y);
    }
};

struct Paddle {
    float x, y;
    Image* sprite;

    Paddle() {
        x = (WIN_WIDTH - PADDLE_WIDTH) / 2.0f;
        y = WIN_HEIGHT - 20.0f;
        sprite = new Image(PADDLE_WIDTH, PADDLE_HEIGHT);
        // Create a nice gradient blue paddle
        for (int py = 0; py < PADDLE_HEIGHT; py++) {
            for (int px = 0; px < PADDLE_WIDTH; px++) {
                int blue = 150 + (px * 100 / PADDLE_WIDTH);
                sprite->setPixel(px, py, 50, 50, blue);
            }
        }
    }

    ~Paddle() { delete sprite; }

    void move(int dir) {
        x += dir * PADDLE_SPEED;
        if (x < 0) x = 0;
        if (x + PADDLE_WIDTH > WIN_WIDTH) x = WIN_WIDTH - PADDLE_WIDTH;
    }

    void draw() {
        sprite->draw((int)x, (int)y);
    }
};

struct Brick {
    float x, y;
    bool active;
    Image* sprite;

    Brick(int col, int row) {
        active = true;
        x = (float)(BRICK_GAP + col * (BRICK_WIDTH + BRICK_GAP));
        y = (float)(BRICK_START_Y + row * (BRICK_HEIGHT + BRICK_GAP));

        sprite = new Image(BRICK_WIDTH, BRICK_HEIGHT);

        // Rainbow Rows
        int r, g, b;
        if (row == 0) { r = 255; g = 0; b = 0; }        // Red
        else if (row == 1) { r = 255; g = 127; b = 0; } // Orange
        else if (row == 2) { r = 255; g = 255; b = 0; } // Yellow
        else if (row == 3) { r = 0; g = 255; b = 0; }   // Green
        else if (row == 4) { r = 0; g = 255; b = 255; } // Cyan
        else if (row == 5) { r = 0; g = 0; b = 255; }   // Blue
        else { r = 128; g = 0; b = 255; }               // Purple

        for (int py = 0; py < BRICK_HEIGHT; py++)
            for (int px = 0; px < BRICK_WIDTH; px++)
                sprite->setPixel(px, py, r, g, b);
    }

    // We don't delete sprite in destructor here because we might manage memory differently 
    // in a real engine, but for this simple struct we will handle cleanup manually or leak slightly (acceptable for main loop)
};

int main() {
    Window window(WIN_WIDTH, WIN_HEIGHT);
    Utils::setFrameRate(60);

    char ch = _getch();

    // Init Objects
    Paddle paddle;
    Ball ball;

    // Init Bricks
    vector<Brick*> bricks;
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            bricks.push_back(new Brick(c, r));
        }
    }

    bool running = true;
    int score = 0;
    int lives = 3;

    while (running && lives > 0) {
        // --- INPUT ---
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a') paddle.move(-1);
            if (key == 'd') paddle.move(1);
            if (key == ' ') {
                if (!ball.active) ball.launch();
            }
            if (key == 'q') running = false;
        }

        // --- UPDATE ---
        if (!ball.active) {
            // Ball sticks to paddle
            ball.x = paddle.x + (PADDLE_WIDTH / 2) - (BALL_SIZE / 2);
            ball.y = paddle.y - BALL_SIZE - 1;
        }
        else {
            ball.update();

            // Paddle Collision
            if (checkCollision(ball.x, ball.y, BALL_SIZE, BALL_SIZE, paddle.x, paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT)) {
                ball.vy *= -1;
                ball.y = paddle.y - BALL_SIZE - 1; // Unstick

                // Add English (Angle control based on where it hit paddle)
                float centerPaddle = paddle.x + PADDLE_WIDTH / 2;
                float centerBall = ball.x + BALL_SIZE / 2;
                float offset = (centerBall - centerPaddle) / (PADDLE_WIDTH / 2);
                ball.vx = offset * 6.0f; // Max horizontal speed
            }

            // Brick Collision
            for (auto brick : bricks) {
                if (brick->active) {
                    if (checkCollision(ball.x, ball.y, BALL_SIZE, BALL_SIZE, brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT)) {
                        brick->active = false;
                        ball.vy *= -1; // Simple bounce
                        score += 10;
                        break; // Only hit one brick at a time
                    }
                }
            }

            // Death Logic
            if (ball.y > WIN_HEIGHT) {
                lives--;
                ball.reset();
            }
        }

        // Win Condition Check (All bricks gone)
        bool allGone = true;
        for (auto b : bricks) if (b->active) allGone = false;
        if (allGone) {
            ball.reset();
            // Reset bricks
            for (auto b : bricks) b->active = true;
        }


        // --- RENDER ---
        window.clear(20, 20, 30); // Dark background

        // Draw Bricks
        for (auto brick : bricks) {
            if (brick->active) brick->sprite->draw((int)brick->x, (int)brick->y);
        }

        paddle.draw();
        ball.draw();

        // UI: Draw Lives (Red Dots bottom left)
        for (int i = 0; i < lives; i++) {
            for (int h = 0; h < 4; h++)
                for (int w = 0; w < 4; w++)
                    window.drawPoint(10 + i * 10 + w, WIN_HEIGHT - 10 + h, "#", 255, 50, 50);
        }

        // UI: Score Bar (Green bar at bottom grows)
        int barWidth = (score * 2) % (WIN_WIDTH - 50);
        for (int i = 0; i < barWidth; i++) {
            window.drawPoint(50 + i, WIN_HEIGHT - 8, "=", 0, 255, 0);
        }

        window.display();
        Utils::sleepForFrame();
    }

    Utils::clearScreen();
    cout << "GAME OVER - FINAL SCORE: " << score << endl;

    // Cleanup
    for (auto b : bricks) { delete b->sprite; delete b; }

    return 0;
}