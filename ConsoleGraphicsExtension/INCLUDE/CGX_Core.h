#pragma once
#include <cstdint> // Required for uint8_t, uint32_t

namespace CGX {
    // 32-bit Packed Pixel (4 Bytes Total)
    union Pixel {
        uint32_t val; // Process whole pixel at once

        struct {
            uint8_t b, g, r, a; // BGRA layout is standard for Windows/Hardware
        };

        // Constructors for convenience
        Pixel() : val(0) {}
        Pixel(int _r, int _g, int _b, int _a = 255)
            : r((uint8_t)_r), g((uint8_t)_g), b((uint8_t)_b), a((uint8_t)_a) {
        }
    };

    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 450;
}