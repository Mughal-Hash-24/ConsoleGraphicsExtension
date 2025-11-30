#pragma once
#include "CGX_Image.h"
#include <string>

namespace CGX {
    class Animation {
    private:
        Image** frames; // Array of pointers to Image objects
        int numFrames;
        bool loaded;
        int currentFrame;

        void clearMemory();
        void resetPlaybackState();

    public:
        Animation(); // Default constructor added for safety
        Animation(const std::string& filepath);
        ~Animation();

        bool loadFromFile(const std::string& filepath);
        void update();
        void draw(int startX, int startY) const;
        void reset();

        bool isLoaded() const;
        int getFrameCount() const;
        int getCurrentFrameIndex() const;
    };
}