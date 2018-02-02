#include "colors.h"

const Color Colors::red = Color(1.0f, 0.0f, 0.0f);
const Color Colors::blue = Color(0.0f, 0.0f, 1.0f);
const Color Colors::white = Color(1.0f, 1.0f, 1.0f);
const Color Colors::black = Color(0.0f, 0.0f, 0.0f);
const Color Colors::grey = Color(0.5f, 0.5f, 0.5f);

// Ensure RGB values are constrained within [0, 1]
Color Colors::clamp_color(const Color& c)
{
    float r = std::fminf(c[0], 1.0f);
    float g = std::fminf(c[1], 1.0f);
    float b = std::fminf(c[2], 1.0f);
    return Color(r, g, b);
}
