#include "../INCLUDE/CGX_Text.h"
#include "../INCLUDE/CGX_Window.h"
#include "../INCLUDE/CGX_FontData.h"

namespace CGX {

    Text::Text() : x(0), y(0), r(255), g(255), b(255), scale(1), fontType(0) {}

    Text::Text(const std::string& str, int x, int y)
        : content(str), x(x), y(y), r(255), g(255), b(255), scale(1), fontType(0) {
    }

    void Text::setString(const std::string& str) { content = str; }
    void Text::setPosition(int newX, int newY) { x = newX; y = newY; }
    void Text::setColor(int red, int green, int blue) { r = red; g = green; b = blue; }
    void Text::setScale(int s) { if (s > 0) scale = s; }
    void Text::setFont(int type) { fontType = type; }

    std::string Text::getString() const { return content; }
    int Text::getX() const { return x; }
    int Text::getY() const { return y; }

    void Text::draw() const {
        Window* win = Window::getInstance();
        if (!win) return;

        int cursorX = x;
        int cursorY = y;

        // Loop through every character in the string
        for (char c : content) {

            // Handle Newlines manually
            if (c == '\n') {
                cursorX = x;
                cursorY += (8 * scale) + (2 * scale); // Line height + padding
                continue;
            }

            // Ensure character is within our font array bounds
            unsigned char charIndex = (unsigned char)c;
            if (charIndex > 127) charIndex = '?';

            // Loop through the 8 rows of the character
            for (int row = 0; row < 8; row++) {
                unsigned char line = (fontType == 1) ? FONT_CYBER[charIndex][row] : FONT5x8[charIndex][row];

                // Loop through the 5 columns (bits) of the character
                for (int col = 0; col < 5; col++) {
                    // Check if the bit at this column is set
                    // We check from Left (Bit 4) to Right (Bit 0)
                    if (line & (1 << (4 - col))) {

                        // If scale is 1, draw 1 pixel. If scale > 1, draw a block.
                        for (int sy = 0; sy < scale; sy++) {
                            for (int sx = 0; sx < scale; sx++) {
                                win->drawPoint(
                                    cursorX + (col * scale) + sx,
                                    cursorY + (row * scale) + sy,
                                    "  ", // Use solid block for text
                                    r, g, b
                                );
                            }
                        }
                    }
                }
            }

            // Move cursor forward for next character
            // 5 pixels wide + 1 pixel spacing = 6 * scale
            cursorX += (6 * scale);
        }
    }

    Rect Text::getGlobalBounds() const {
        int maxW = 0;
        int curW = 0;
        int rows = 1;
        for (char c : content) {
            if (c == '\n') {
                if (curW > maxW) maxW = curW;
                curW = 0;
                rows++;
            }
            else {
                // Each char is 5px wide + 1px spacng = 6 * scale
                curW += (6 * scale);
            }
        }
        if (curW > maxW) maxW = curW;
        // Height = (8px char + 2px line gap) * rows * scale
        int h = (rows * 10 * scale);

        // Width correction: subtract last spacing (1 * scale) for exact pixel perfectness
        if (maxW > 0) maxW -= (1 * scale);
        return Rect(x, y, maxW, h);
    }
}