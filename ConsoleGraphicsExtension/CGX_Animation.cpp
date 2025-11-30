#include "CGX_Animation.h"
#include <iostream>

namespace CGX {

    Animation::Animation() : frames(nullptr), numFrames(0), loaded(false), currentFrame(0) {}

    Animation::Animation(const std::string& filepath) : Animation() {
        loadFromFile(filepath);
    }

    Animation::~Animation() {
        clearMemory();
    }

    void Animation::clearMemory() {
        if (frames) {
            for (int i = 0; i < numFrames; ++i) {
                if (frames[i]) delete frames[i];
            }
            delete[] frames;
            frames = nullptr;
        }
        numFrames = 0;
        loaded = false;
    }

    void Animation::resetPlaybackState() {
        currentFrame = 0;
    }

    bool Animation::loadFromFile(const std::string& filepath) {
        clearMemory();

        // MOCK IMPLEMENTATION:
        // Since we cannot implement a full file parser without heavy code or libraries,
        // we will generate a procedural animation (e.g., a pulsing square) to demonstrate functionality.
        // In a real scenario, this would read a folder or spritesheet.

        numFrames = 5;
        frames = new Image * [numFrames];

        for (int i = 0; i < numFrames; ++i) {
            frames[i] = new Image(10, 10);

            // Generate a color pattern based on frame index
            int r = (i * 50) % 255;
            int b = 255 - r;

            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    frames[i]->setPixel(x, y, r, 50, b);
                }
            }
        }

        loaded = true;
        return true;
    }

    void Animation::update() {
        if (!loaded || numFrames == 0) return;
        currentFrame = (currentFrame + 1) % numFrames;
    }

    void Animation::draw(int startX, int startY) const {
        if (!loaded || !frames) return;
        if (frames[currentFrame]) {
            frames[currentFrame]->draw(startX, startY);
        }
    }

    void Animation::reset() {
        resetPlaybackState();
    }

    bool Animation::isLoaded() const { return loaded; }
    int Animation::getFrameCount() const { return numFrames; }
    int Animation::getCurrentFrameIndex() const { return currentFrame; }
}