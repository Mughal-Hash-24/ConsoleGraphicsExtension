#include "../INCLUDE/CGX_Animation.h"
#include <iostream>

namespace CGX {

    Animation::Animation() : loaded(false), currentFrame(0) {}

    Animation::Animation(const std::string& filepath) : Animation() {
        loadFromFile(filepath);
    }

    Animation::~Animation() {
        // Vector automatically cleans up frames
    }

    bool Animation::loadFromFile(const std::string& filepath) {
        frames.clear();

        // MOCK IMPLEMENTATION:
        // Since we cannot implement a full file parser without heavy code or libraries,
        // we will generate a procedural animation (e.g., a pulsing square) to demonstrate functionality.
        // In a real scenario, this would read a folder or spritesheet.

        int numFrames = 5;

        for (int i = 0; i < numFrames; ++i) {
            Image img(10, 10);

            // Generate a color pattern based on frame index
            int r = (i * 50) % 255;
            int b = 255 - r;

            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    img.setPixel(x, y, r, 50, b);
                }
            }
            frames.push_back(img);
        }

        loaded = true;
        currentFrame = 0;
        return true;
    }

    void Animation::update() {
        if (!loaded || frames.size() == 0) return;
        currentFrame = (currentFrame + 1) % frames.size();
    }

    void Animation::draw(int startX, int startY) const {
        if (!loaded || frames.size() == 0) return;

        frames[currentFrame].draw(startX, startY);
    }

    void Animation::reset() {
        currentFrame = 0;
    }

    bool Animation::isLoaded() const { return loaded; }
    int Animation::getFrameCount() const { return frames.size(); }
    int Animation::getCurrentFrameIndex() const { return currentFrame; }
}