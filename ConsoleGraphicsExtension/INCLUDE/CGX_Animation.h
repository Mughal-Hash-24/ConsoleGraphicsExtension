#pragma once
#include "CGX_Image.h"
#include "CGX_Vector.h"
#include <string>

namespace CGX {
    class Animation {
    private:
        // OPTIMIZATION: Store Images by value to ensure contiguous memory
        Vector<Image> frames;
        bool loaded;
        int currentFrame;

    public:
        Animation();
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